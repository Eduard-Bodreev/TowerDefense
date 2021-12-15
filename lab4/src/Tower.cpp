#include "../include/All_Header.h"

namespace Tower_Defence{

	Tower::Tower(int coord, std::vector<Tower_Level_Info>& info) :
		Building(coord),
		m_level(0),
		m_table(&info),
		m_cooldown(info[0].speed){};

	void Tower::turn(Landscape& land, std::stringstream& ss) noexcept{
		if (--m_cooldown == 0) {
			for (std::list<Enemy*>::const_iterator it = land.getEnemies().begin(); it != land.getEnemies().end(); it++)
				if (land.distance(getCoord(), (*it)->getCoord()) <= (*m_table)[m_level].radius) {
					ss << "Tower (coord " << getCoord() << ") damages Enemy (coord " << (*it)->getCoord() << ")";
					try { land.shoot_enemy((*it), (*m_table)[m_level].damage, ss); }
					catch (std::exception& ex) {
						ss << ex.what();
					}
					ss << "\n";
					m_cooldown = (*m_table)[m_level].speed;
					return;
				}
			m_cooldown = (*m_table)[m_level].speed;
		}
	}

	int Tower::level_up() {
		int cost = cost_upgrade();
		m_level++;
		return cost;
	}

	int Tower::cost_upgrade() const	{
		if (m_level + 1 == (*m_table).size())
			throw std::logic_error("Already max level");
		return (*m_table)[m_level + 1].cost;
	}

}
