#include "../include/All_Header.h"

namespace Tower_Defence {

	Cell::Cell(LandType b_type) :
		type(b_type),
		light_dist(INF),
		heavy_dist(INF),
        avia_dist (INF) {}


    Cell::Cell(LandType b_type, int light, int heavy, int avia) :
        type(b_type), light_dist(light), heavy_dist(heavy), avia_dist(avia) {};


    Landscape::Landscape() {
        m_config = new Config();
        // reading level map from config
        load_map();  

        // reading log tables (characteristics of enemies have already been read)
        load_dens();

        // calculating paths for all kinds of enemies
        path_for_light();
        path_for_heavy();
        path_for_avia();
    }

    Landscape::~Landscape() {
        if (m_castle) delete m_castle; 
        if (m_config) delete m_config;
        // towers, walls and lairs are removed by themselves
        //for (std::list<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
        //    delete (*it);
    }


    void Landscape::level_up_castle() {
        m_castle->level_up();
    }

    void Landscape::add_enemy(Enemy* en) {
        if (static_cast<int>(en->getType()) > 2)
            m_enemies.push_front(en);
        else
            m_enemies.push_back(en);
    }



    int Landscape::cost_new_wall() const {
        int cost = m_config->m_wall_info.cost;
        if (cost > getMoney())
            throw std::logic_error("Cannot build new wall, not enough money!");
        return cost;
    }

    int Landscape::cost_repair_wall(const Wall& wall) const {
        int cost = wall.cost_repair();
        if (cost > getMoney())
            throw std::logic_error("Cannot repair anny wall, not enough money!");
        return cost;
    }

    int Landscape::cost_upgrade_castle() const {
        int cost = m_castle->cost_upgrade();
        if (cost > getMoney())
            throw std::logic_error("Not enout money to upgrade castle!");
        return cost;
    }

    int Landscape::cost_new_tower() const {
        int cost = m_config->m_towers_info[0].cost;
        if (cost > getMoney())
            throw std::logic_error("Cannot build new wall, not enough money!");
        return cost;
    }

    int Landscape::cost_upgrade_tower(const Tower& tower) const {
        int cost = tower.cost_upgrade();
        if (cost > getMoney()) 
            throw std::logic_error("Not enout money to upgrade tower!");
        return cost;
    }

    // returns the coordinate in a one-dimensional array, if it is possible to build (otherwise an exception)
    int Landscape::can_build_smth(int i, int j) const {
        // checking that the coordinates are inside the map at all
        if (i < 0 || j < 0 || i >= m_width || j >= m_height)
            throw std::out_of_range("This coordinates don't belog to map!");
        int coord = index(i, j);
        if (m_map[coord].type != LT::PLAIN)
            throw std::logic_error("You cannot build anything in this cell!");
        
        for (std::list<Enemy*>::const_iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
            if((*it)->getCoord() == coord && (*it)->getType() != ET::AVIATION)
                throw std::logic_error("You cannot build anything in the cell where an enemy stands!");
        return coord;
    }


    void Landscape::load_map() {
        std::ifstream fin("config_files/level_2.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];  // assume that the level is not more than 50 cells wide

        fin >> m_width;
        fin >> m_height;
        
        int start_money;
        fin >> buf >> start_money;

        for (int i = 0; i < m_height; i++) {
            fin >> buf;
            for (int j = 0; j < m_width; j++) {
                switch (buf[j]) {
                case '.':
                    m_map.push_back(Cell(LT::PLAIN));
                    break;
                case '^':
                    m_map.push_back(Cell(LT::HILL));
                    break;
                case '~':
                    m_map.push_back(Cell(LT::WATER));
                    break;
                case '@':
                    m_map.push_back(Cell(LT::CASTLE));
                    m_castle = new Castle(m_config->m_castle_info, index(i, j), start_money);
                    break;
                case '*':
                    m_map.push_back(Cell(LT::DEN));
                    break;
                default:
                    fin.close();
                    throw std::invalid_argument("Wrong symbol in config file!");
                }
            }
        }
        fin.close();
    }

    void Landscape::load_dens() {
        std::ifstream fin("config_files/dens_info.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];
        int i, j, type, time;
        while (!fin.eof()) {
            fin >> buf;   // #DEN_1
            if (buf[0] == '#') {
                fin >> buf >> i >> j;   // coordinates: 0 0
                int coord = index(i, j);
                m_dens.push_back(Den(coord));  // creating a new den

                while (fin >> buf) {
                    if (buf[0] == '#') break;
                    fin >> type >> time;
                    Enemy* enemy;
                    std::vector<Aura*> auras;
                    switch (type) {
                    case 0:
                        enemy = new Light_Infantry(m_config->m_enemy_chars[type], coord);
                        break;
                    case 1:
                        enemy = new Heavy_Infantry(m_config->m_enemy_chars[type], coord);
                        break;
                    case 2:
                        enemy = new Aviation(m_config->m_enemy_chars[type], coord);
                        break;
                    case 3:
                        read_auras(fin, auras);
                        enemy = new Light_Infantry_Hero(m_config->m_enemy_chars[type], auras, coord);
                        break;
                    case 4:
                        read_auras(fin, auras);
                        enemy = new Heavy_Infantry_Hero(m_config->m_enemy_chars[type], auras, coord);
                        break;
                    case 5:
                        read_auras(fin, auras);
                        enemy = new Aviation_Hero(m_config->m_enemy_chars[type], auras, coord);
                        break;
                    }
                    m_dens[m_dens.size() - 1].add_enemy(std::pair<int, Enemy*>(time, enemy));  // add each enemy
                }
            }
        }
        fin.close();
    }

    void Landscape::read_auras(std::ifstream& fin, std::vector<Aura*>& vec) {
        char buf[50];
        fin >> buf;  // service symbol, then the first type of aura
        while (!fin.eof()) {
            fin >> buf;
            if (buf[0] == '}') break;
            float radius, mult;
            fin >> radius >> mult;
            if      ((std::string)buf == "speed")  vec.push_back(new Aura(AT::SPEED,  radius, mult));
            else if ((std::string)buf == "damage") vec.push_back(new Aura(AT::DAMAGE, radius, mult));
            else if ((std::string)buf == "heal")   vec.push_back(new Aura(AT::HEAL,   radius, mult));
        }
    }


    int Landscape::right(int coord) const noexcept{
        if (coord % m_width == m_width - 1) 
            return -1;
        return coord + 1;
    }

    int Landscape::left(int coord) const noexcept {
        if (coord % m_width == 0) 
            return -1;
        return coord - 1;
    }

    int Landscape::up(int coord) const noexcept {
        if (coord / m_width == 0) 
            return -1;
        return coord - m_width;
    }

    int Landscape::down(int coord) const noexcept {
        if (coord / m_width == m_height - 1) 
            return -1;
        return coord + m_width;
    }

    void Landscape::castle_receive_damage(float damage, std::stringstream& ss) {
        m_castle->receive_damage(damage, ss);  // if the health of the castle is over, an exception is thrown
    }

    void Landscape::wall_receive_damage(int coord, float damage, std::stringstream& ss) {
        for (std::list<Wall>::iterator it = m_walls.begin(); it != m_walls.end(); it++)
            if (it->getCoord() == coord) {
                try { it->damage(damage); }
                catch (std::exception& ex) { 
                    ss << ". " << ex.what() << "\n";
                    it = m_walls.erase(it);
                    m_map[coord].type = LT::PLAIN;
                }
                return;
            }
        ss << "\n";
    }

    void Landscape::shoot_enemy(Enemy* en, float damage, std::stringstream& ss) {
        for (std::list<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
            if ((*it) == en) {
                try { (*it)->receive_damage(damage); }
                catch (std::exception& ex) {
                    ss << ". " << ex.what()<< "\n";
                    it = m_enemies.erase(it);
                }
                return;
            }
        ss << "\n";
    }



    void Landscape::path_for_light() {
        // update the data
        for (int i = 0; i < m_width * m_height; i++)
            m_map[i].light_dist = INF;
        m_map[m_castle->getCoord()].light_dist = 0;

        std::queue<int> q;
        q.push(m_castle->getCoord());
        int cur;

        while (!q.empty()) {
            cur = q.front();
            q.pop();

            // add all the following directions if it is a plain
            std::vector<int> directions = { right(cur), left(cur), up(cur), down(cur) };
            for (auto direct : directions) {
                if (direct != -1 && m_map[direct].type == LT::PLAIN && m_map[direct].light_dist > m_map[cur].light_dist + 1) {
                    m_map[direct].light_dist = m_map[cur].light_dist + 1;
                    q.push(direct);
                }
                else if (direct != -1 && m_map[direct].type == LT::DEN)
                    m_map[direct].light_dist = m_map[cur].light_dist + 1;
            }
        }        

        // walked all over the map, if some of the logs dens out of reach, throw an exception
        // if the lair is within reach, but the player drove the infantry into a trap, this is not prohibited by the rules :)
        for (mvector<Den>::ConstIterator it = m_dens.cbegin(); it != m_dens.cend(); it++)
            if (!it->is_empty() && m_map[it->getCoord()].light_dist == INF)
                throw std::logic_error("Error! No way for light infantry!");
    }

    void Landscape::path_for_heavy() {
        for (int i = 0; i < m_width * m_height; i++)
            m_map[i].heavy_dist = INF;
        m_map[m_castle->getCoord()].heavy_dist = 0;

        std::queue<int> q;
        q.push(m_castle->getCoord());
        int cur;

        while (!q.empty()) {
            cur = q.front();
            q.pop();

            // add all the following directions if it is a plain or a wall
            std::vector<int> directions = { right(cur), left(cur), up(cur), down(cur) };
            for (auto direct : directions) {
                if (direct != -1 && (m_map[direct].type == LT::PLAIN || m_map[direct].type == LT::WALL) 
                    && m_map[direct].heavy_dist > m_map[cur].heavy_dist + 1) {

                    m_map[direct].heavy_dist = m_map[cur].heavy_dist + 1;
                    q.push(direct);
                }
                else if (direct != -1 && m_map[direct].type == LT::DEN)
                    m_map[direct].heavy_dist = m_map[cur].heavy_dist + 1;
            }
        }
        // exceptions are not thrown
    }

    void Landscape::path_for_avia() {
        for (int i = 0; i < m_width * m_height; i++)
            m_map[i].avia_dist = INF;
        m_map[m_castle->getCoord()].avia_dist = 0;

        std::queue<int> q;
        q.push(m_castle->getCoord());
        int cur;

        while (!q.empty()) {
            cur = q.front();
            q.pop();

            // add all the following directions if it is a plain or a wall
            std::vector<int> directions = { right(cur), left(cur), up(cur), down(cur) };
            for (auto direct : directions) {
                if (direct != -1 && m_map[direct].type != LT::HILL && m_map[direct].avia_dist > m_map[cur].avia_dist + 1) {
                    m_map[direct].avia_dist = m_map[cur].avia_dist + 1;
                    q.push(direct);
                }
                else if (direct != -1 && m_map[direct].type == LT::DEN)
                    m_map[direct].avia_dist = m_map[cur].avia_dist + 1;
            }
        }
    }


    bool Landscape::check_dens_empty() const noexcept {
        for (mvector<Den>::ConstIterator it = m_dens.cbegin(); it != m_dens.cend(); it++)
            if (!it->is_empty()) return false;
        return true;
    }


    // checked beforehand that this coordinate corresponds to the wall
    void Landscape::repair_wall(int coord) {
        if (m_map[coord].type != LT::WALL)
            throw std::invalid_argument("This is not the wall!");

        // find this wall
        for (std::list<Wall>::iterator it = m_walls.begin(); it != m_walls.end(); it++)
            if (it->getCoord() == coord) {
                m_castle->buy_anything(it->repair());  // repair returns the cost
                return;
            }
            
    }

    // checking that the coordinate belongs to the map and is a plain must already be done before
    // the function throws an exception only if there is no path for the infantry during construction
    void Landscape::build_wall(int i, int j) {
        int coord = can_build_smth(i, j);                            // exception if not plain
        m_map[coord].type = LT::WALL;
        try { 
            path_for_light();                                        // exception if blocks the road
            m_castle->buy_anything(m_config->m_wall_info.cost);      // exception if there is not enough money
            m_walls.push_back(Wall(coord, m_config->m_wall_info));  
        }
        catch (std::exception& ex) {
            m_map[coord].type = LT::PLAIN;
            path_for_light();
            throw ex;
        }
    }

    void Landscape::build_tower(int i, int j) {
        int coord = can_build_smth(i, j);                                // exception if not plain
        m_map[coord].type = LT::TOWER;
        try {
            path_for_light();                                            // exception if blocks the road
            m_castle->buy_anything(m_config->m_towers_info[0].cost);     // exception if there is not enough money
            m_towers.push_back(Tower(coord, m_config->m_towers_info));   
            path_for_heavy();  // re-count, as it blocks the road
        }
        catch (std::exception& ex) {
            m_map[coord].type = LT::PLAIN;
            path_for_light();
            throw ex;
        }
    }

    void Landscape::level_up_tower(int coord) {
        if (m_map[coord].type != LT::TOWER)
            throw std::invalid_argument("This is not a tower :(");
         
        // find this tower
        for (mvector<Tower>::Iterator it = m_towers.begin(); it != m_towers.end(); it++)
            if (it->getCoord() == coord) {
                m_castle->buy_anything(it->level_up());
                return;
            }
    }

    float Landscape::distance(int a, int b) const noexcept {
        return (float) (abs(a / m_width - b / m_width) + abs(a % m_width - b % m_width));
    }

   
    std::pair<char, Color>* Landscape::make_colored_field(int coord, Color color_) const noexcept {
        int size = m_width * m_height;

        std::pair<char, Color>* field = new std::pair<char, Color>[size];
        for (int i = 0; i < size; i++)
            field[i] = { '-', Color::White };

        Color color;
        // first we go over the array of enemies, mark them on the map. Then we mark all free places as landscape / buildings
        for (std::list<Enemy*>::const_reverse_iterator it = m_enemies.rbegin(); it != m_enemies.rend(); it++) {
            color = Color::White;
            // all auras
            const std::vector<Aura*>* ex_aura = (*it)->getExertedAuras();
            if (ex_aura) switch (static_cast<int>((*ex_aura)[0]->getType())) {
            case 0: color = Color::Blue;  break;  //speed
            case 1: color = Color::Magenta; break;  // damage
            case 2: color = Color::Cyan;  break;   //heal
            }

            // own auras
            const std::vector<Aura*>* aura = (*it)->getAuras();
            if (aura) switch (static_cast<int>((*aura)[0]->getType())) {
            case 0: color = Color::LightBlue;  break;  //speed
            case 1: color = Color::LightMagenta; break;  // damage
            case 2: color = Color::LightCyan;  break; //heal
            }

            switch ((*it)->getType()) {
            case ET::LIGHT:
                field[(*it)->getCoord()] = { 'a', color }; break;
            case ET::HEAVY:
                field[(*it)->getCoord()] = { 'b', color }; break;
            case ET::AVIATION:
                field[(*it)->getCoord()] = { 'c', color }; break;
            case ET::LIGHT_HERO:
                field[(*it)->getCoord()] = { 'A', color }; break;
            case ET::HEAVY_HERO:
                field[(*it)->getCoord()] = { 'B', color }; break;
            case ET::AVIATION_HERO:
                field[(*it)->getCoord()] = { 'C', color }; break;
            }
        }

        // then we go through the array of towers, because they can be of different levels, and the level of the tower is not stored in the Cell class
        for (mvector<Tower>::ConstIterator it = m_towers.cbegin(); it != m_towers.cend(); it++)
            field[it->getCoord()] = { it->getLevel() + '0', Color::Yellow };
        
       
        for (int i = 0; i < size; i++)
            if (field[i].first == '-')
                switch (m_map[i].type) {
                case LT::PLAIN:
                    field[i] = { '.', Color::White };     break;
                case LT::WATER:
                    field[i] = { '~', Color::White };      break;
                case LT::HILL:
                    field[i] = { '^', Color::White };     break;
                case LT::CASTLE:
                    field[i] = { '@', Color::Green };   break;
                case LT::WALL:
                    field[i] = { '#', Color::LightGray }; break;
                case LT::DEN:
                    field[i] = { '*', Color::Red };       break;
                case LT::TOWER:
                    // towers are already marked
                    break;
                }

        // change the color of the selected cell (tower or wall when the player chooses which one to upgrade)
        if (coord != -1)
            field[coord].second = color_;

        return field;
    }

    // returns true if the level is passed; false - the level continues; excl - level trimmed
    bool Landscape::make_turn(std::stringstream& ss) {
        m_time++;

        ss << "Turn #" << m_time << "\n";

        // check if we won the game
        if (check_dens_empty() && !m_enemies.size())
            return true;

        // the castle is healed and brings money
        m_castle->turn(*this, ss);


        // dens release enemies
        for (mvector<Den>::Iterator it = m_dens.begin(); it != m_dens.end(); it++)
            if (!it->is_empty())
                it->turn(*this, ss);

         // enemies are freed from auras
        for (std::list<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
            (*it)->release_from_auras();

        // influence of auras
        for (std::list<Enemy*>::const_iterator it_1 = m_enemies.begin(); it_1 != m_enemies.end(); it_1++) {
            const std::vector<Aura*>* auras = (*it_1)->getAuras();
            if (!auras) break;
            for (std::list<Enemy*>::const_iterator it_2 = m_enemies.begin(); it_2 != m_enemies.end(); it_2++) {
                for (size_t i = 0; i < auras->size(); i++) {
                    float dist = distance((*it_1)->getCoord(), (*it_2)->getCoord());
                    if (distance((*it_1)->getCoord(), (*it_2)->getCoord()) <= (*auras)[i]->getRadius())
                        (*it_2)->undegro_aura((*auras)[i]);
                }
            }
        }

        //enemies move
        for (std::list<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end();)
            if (!(*it)->move(*this, ss)) {
                delete (*it);
                it = m_enemies.erase(it);
            }
            else it++;

        // towers shoot
        for (mvector<Tower>::Iterator it = m_towers.begin(); it != m_towers.end(); it++)
            it->turn(*this, ss);


        return false;
    }

}