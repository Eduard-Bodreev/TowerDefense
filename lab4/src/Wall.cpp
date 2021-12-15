#include "../include/All_Header.h"

namespace Tower_Defence {

    void Wall::damage(float damage) {
        if (damage >= m_health)
            throw std::invalid_argument("Wall destroyed");
        else m_health -= damage;
    }

    int Wall::repair() {
        int cost = cost_repair(); // if health is max, an exception will be thrown here
        m_health = m_table.max_health;
        return cost;
    }

    int Wall::cost_repair() const {
        if (m_health == m_table.max_health)
            throw std::logic_error("Health is already max");
        return m_table.repair_cost;
    }

}
