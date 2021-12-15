#include "../include/dialog.h"

namespace Tower_Defence {

	int (*fptr[])(Landscape&) = { quit, build_tower, build_wall, level_up_tower, level_up_castle, repair_wall, next_turn };

	const char* menu[]{
			"0. Quit the game",
			"1. Build new tower",
			"2. Build new wall",
			"3. Upgrade tower",
			"4. Upgrade castle",
			"5. Repair wall",
			"6. Continue"
	};

	const int NMsgs = sizeof(menu) / sizeof(menu[0]);


	int dialog(Landscape& land) {
		clear();
		print_map_in_console(land);

		const char* errmsg = "";
		int input;
		do {
			std::cout << errmsg << std::endl;
			errmsg = "You are wrong. Repeate, please!";
			for (int i = 0; i < NMsgs; ++i)
				std::cout << menu[i] << std::endl;
			if (!getNum(input, "\nMake your choice: --> ")) return 0;
		} while (input < 0 || input >= NMsgs);
		return input;
	}

	bool get_yes_no_answer() {
		while (!std::cin.eof()) {
			std::string s;
			std::getline(std::cin, s);
			if (s == "y" || s == "yes" || s == "Y" || s == "Yes")
				return true;
			if (s == "n" || s == "no" || s == "N" || s == "No")
				return false;
			std::cout << "Wrong answer. Try again --> ";
		}
		return false;
	}

	void clear() {
		std::cout << std::endl << std::endl;
		std::cout << "\x1B[2J\x1B[H";
	}

	void print_money(Landscape& land) {
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 11));
		std::cout << "Money: " << land.getMoney() << std::endl << std::endl; 
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	}

	void click_enter() {
		std::cout << std::endl << "Click Enter to proceed" << std::endl;
		while (std::cin.get() != '\n') {}
		return;
	}


	void game_over() {
		std::cout << std::endl;
		std::cout << "                      ---------------------" << std::endl;
		std::cout << "                      |     GAME OVER!    |" << std::endl;
		std::cout << "                      ---------------------" << std::endl;
		// and in general there are several options why the game closed - a breakdown inside the program / won / lost / left without finishing the level
	}

	void game_won() {
		std::cout << std::endl;
		std::cout << "                      ---------------------" << std::endl;
		std::cout << "                      |      CONGRATS!    |" << std::endl;
		std::cout << "                      ---------------------" << std::endl;
	}

	
	int next_turn(Landscape& land) {
		std::stringstream ss;  // output logs in console
		try {
			if (land.make_turn(ss)) {
				std::cout << ss.str();
				game_won();
				return 0;
			}
		}
		catch (std::exception&) {  // only one exception - the castle ran out of health
			std::cout << ss.str();
			game_over();
			return 0;
		}
		std::cout << ss.str();
		return 1;
	}

	int build_tower(Landscape& land) {
		try {
			int cost;
			cost = land.cost_new_tower(); // thrown out only if there is not enough money

			std::cout << "Input coordinates, where to build a new tower" << std::endl;
			int i, j;
			if (!getNum(i, "") || !getNum(j, "")) return 0;

			land.build_tower(i, j);  // thrown if blocking the path for infantry or this cell is not a plain
			return 1;
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
			click_enter();
			return 1;
		}
	}

	int build_wall(Landscape& land) {
		try {
			int cost;
			cost = land.cost_new_wall(); // thrown out only if there is not enough money

			std::cout << "Input coordinates, where to build a new wall" << std::endl;
			int i, j;
			if (!getNum(i, "") || !getNum(j, "")) return 0;

			land.build_wall(i, j);  // thrown if blocking the path for infantry or this cell is not a plain
			return 1;
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
			click_enter();
			return 1;
		}
	}

	int level_up_tower(Landscape& land) {
		// go through the array of towers, highlight each one in turn, display information and ask if you can upgrade it
		for (mvector<Tower>::ConstIterator it = land.getTowers().cbegin(); it != land.getTowers().cend(); it++) {
			clear();
			print_map_in_console(land, it->getCoord());  // re-display the map, highlighting this tower
			std::cout << "Tower info: level: " << it->getLevel() << std::endl;
			try {
				int cost = land.cost_upgrade_tower(*it);   // if there is not enough money or max level, then excl.
				std::cout << "Do you want to upgrade this tower? (" << cost << " gold)" << std::endl;
				if (get_yes_no_answer()) {
					land.level_up_tower(it->getCoord());
					std::cout << "Tower upgraded successfully" << std::endl;
				}
			}
			catch (std::exception& ex) {
				std::cout << ex.what() << std::endl;
			}
			click_enter();
		}
		return 1;
	}

	int level_up_castle(Landscape& land) {
		// first display the current characteristics of the castle
		const Castle* c = land.getCastle();

		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 11));

		std::cout << std::endl << "Your castle info: Level " << c->getLevel() << ", money: " << land.getMoney() << std::endl;
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
		std::cout << "Durability: " << c->getDurability() << "/" << c->getMaxDurability() << std::endl;
		std::cout << "Repair and revenue period: " << c->getSpeed() << std::endl; 

		int cost;
		try { cost = land.cost_upgrade_castle(); }
		catch (std::exception& ex) {   // if there is not enough money and the level is already max
			std::cout << ex.what() << std::endl;
			click_enter();
			return 1;
		}

		std::cout << "Do you want to level up your castle? (" << cost << " gold)"<< std::endl;
		if (get_yes_no_answer()) {
			land.level_up_castle();
			std::cout << "Castle upgraded successfully" << std::endl;
		}
		return 1;
	}

	int repair_wall(Landscape& land) {
		// go through the array of walls, highlight each one in turn, display information and ask if you can fix it
		for (std::list<Wall>::const_iterator it = land.getWalls().begin(); it != land.getWalls().end(); it++) {
			clear();
			print_map_in_console(land, it->getCoord());  // re-display the map, highlighting this wall
			std::cout << "Wall info: health: " << it->getHealth() << "/" << it->getMaxHealth() << std::endl;
			try {
				int cost = land.cost_repair_wall(*it);
				std::cout << "Do you want to repair this wall? (" << cost << " gold)" << std::endl;
				if (get_yes_no_answer()) {
					land.repair_wall(it->getCoord());
					std::cout << "Wall repaired successfully" << std::endl;
				}
			}
			catch (std::exception& ex) {
				std::cout << ex.what() << std::endl;
			}
			// if health is full, we do nothing
			click_enter();
		}
		return 1;
	}

	int quit(Landscape& land) {
		std::cout << "Are you sure?" << std::endl;
		if (get_yes_no_answer()) {
			std::cout << "That's all, bye! :)" << std::endl;
			return 0;
		}
		return 1;
	}
	
	void print_map_in_console(Landscape& land, int coord) {
		print_money(land);
		// highlight the selected cell in yellow
		std::pair<char, Color>* field = land.make_colored_field(coord, Color::Green);
		int size = land.getSize();
		for (int i = 0; i < size; i++) {
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | (int)field[i].second));
			std::cout << field[i].first;
			if (i % land.getWidth() == land.getWidth() - 1)
				std::cout << std::endl;
		}
		std::cout << std::endl << std::endl;
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	}

}