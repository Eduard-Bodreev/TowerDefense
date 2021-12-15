#pragma once

#ifndef __CONFIG__
#define __CONFIG__

namespace Tower_Defence {

	struct WallInfo {
		int cost;
		int repair_cost;
		float max_health;
	};

	struct Castle_Level_Info {
		float max_durability; // max health at this level
		float repair_percent; // the percentage by which health is restored
		int repair_time;      // repair rate (time for which 10% of maximum durability is restored)
		int revenue;	      // profitability (the period of profitability is equal to the period of recovery of strength)
		int cost;             // current level cost
	};

	struct Tower_Level_Info {
		float radius;
		float damage;
		float speed;
		int cost; // current level cost
	};

	struct Config {
	public:
		enemy_type_info m_enemy_chars[6];  // info about enemies gets from here
		WallInfo m_wall_info;              // info about walls
		std::vector<Castle_Level_Info> m_castle_info;        // castle levels
		std::vector<Tower_Level_Info>  m_towers_info;        // towers levels

	public:
		Config();
		~Config() {};

	private:
		void load_castle_table();
		void load_walls_info();
		void load_enemies_chars();
		void load_towers_table();
	};

}

#endif __CONFIG__
