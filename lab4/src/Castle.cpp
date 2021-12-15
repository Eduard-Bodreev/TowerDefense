#include "../include/All_Header.h"

namespace Tower_Defence {

	Castle::Castle(std::vector<Castle_Level_Info>& info, int coord, int money) :
		Building(coord),
		m_level(0),
		m_money(money),
		m_table(info) {

		m_durability = m_table[0].max_durability;
		m_repair_and_revenue_cooldown = m_table[m_level].repair_time;
	}

	Castle::~Castle() {}

	void Castle::buy_anything(int cost) {
		if (m_money < cost)
			throw std::logic_error("Not enough money!");
		m_money -= cost;
	}

	// raising the level of the castle
	void Castle::level_up() {
		buy_anything(cost_upgrade());
		
		// if the health was full, then it increases and becomes equal to the max health of the next level
		// if health is not full, it does not increase
		if (m_durability == m_table[m_level].max_durability)
			m_durability = m_table[m_level + 1].max_durability;
		m_level++;
	}

	int Castle::cost_upgrade() const {
		if (m_level + 1 == m_table.size())
			throw std::logic_error("Already max level");
		return m_table[m_level + 1].cost;
	}

	void Castle::receive_damage(float damage, std::stringstream & ss) {
		if (m_durability <= damage)
			throw std::logic_error("Game over!");
		m_durability -= damage;
		ss << "Castle receives damage " << damage << ", health left : " << m_durability << "/" << m_table[m_level].max_durability << "\n";
	}

	// restoration of the health of the castle
	void Castle::repair() {
		float add = (float) (m_table[m_level].max_durability / 100.0 * m_table[m_level].repair_percent);
		float max = m_table[m_level].max_durability;
		m_durability = (m_durability + add) < max ? (m_durability + add) : max;
	}

	// after a certain period of time, the castle restores the percentage of health and adds money
	void Castle::turn(Landscape&, std::stringstream& ss)	{
		if (--m_repair_and_revenue_cooldown == 0) {
			m_money += m_table[m_level].revenue;
			repair();
			m_repair_and_revenue_cooldown = m_table[m_level].repair_time;
			ss << "Money += " << m_table[m_level].revenue << " , health = " << m_durability << "/" << m_table[m_level].max_durability << "\n";
		}
	}
}
