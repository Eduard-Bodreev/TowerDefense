#include "../include/All_Header.h"

namespace Tower_Defence {

    Config::Config() {
        load_castle_table();
        load_towers_table();
        load_walls_info();
        load_enemies_chars();
    }

    void Config::load_castle_table() {
        std::ifstream fin("config_files/castle_info.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];
        Castle_Level_Info item;
        while (fin >> buf) {
            fin >> buf >> item.max_durability >> buf >> item.repair_percent;
            fin >> buf >> item.repair_time >> buf >> item.revenue >> buf >> item.cost;
            m_castle_info.push_back(item);
        } 

        fin.close();
    }

    void Config::load_walls_info() {
        std::ifstream fin("config_files/walls_info.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];
        fin >> buf;
        fin >> buf >> m_wall_info.cost >> buf >> m_wall_info.repair_cost >> buf >> m_wall_info.max_health;
        fin.close();
    }



    void Config::load_enemies_chars() {
        std::ifstream fin("config_files/enemies_info.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];
        float fl;
        for (int i = 0; i < 6; i++) {
            fin >> buf;  // service symbol # + enemy type
            m_enemy_chars[i].first = static_cast<EnemyType>(i);
            while (fin >> buf) {
                if (buf[0] == '#') break;
                fin >> fl;
                m_enemy_chars[i].second.insert(std::pair<std::string, float>(buf, fl));
            }
        }
        fin.close();
    }

    void Config::load_towers_table() {
        std::ifstream fin("config_files/towers_info.txt");
        if (!fin.is_open())
            throw std::runtime_error("File not found!");

        char buf[50];
        Tower_Level_Info item;
        while (fin >> buf) {
            fin >> buf >> item.radius >> buf >> item.damage;
            fin >> buf >> item.speed >> buf >> item.cost;
            m_towers_info.push_back(item);
        }

        fin.close();
    }
}