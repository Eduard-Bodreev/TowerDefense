#pragma once

#ifndef __WALL__
#define __WALL__

namespace Tower_Defence {

	class Wall : public Building {
	private:
		WallInfo& m_table;
		float m_health; 

	public:
		Wall(int coord, WallInfo& info) : Building(coord), m_table(info), m_health(info.max_health) {};
		~Wall() {};

		void damage(float damage);  
		int repair();
		int cost_repair() const;
		void turn(Landscape&, std::stringstream&) {};

		float getHealth()    const noexcept { return m_health; }
		float getMaxHealth() const noexcept { return m_table.max_health; }
	};
}

#endif __WALL__

