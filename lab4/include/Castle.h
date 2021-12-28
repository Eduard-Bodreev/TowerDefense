///File
/*!
 *  File determinate class.
 */
#pragma once

#ifndef __CASTLE__
#define __CASTLE__

namespace Tower_Defence {

    /// Class for determinate castle.
    /*!
     * Determinate and work with castle, level, cooldown, money, health and other information.
     * This child class of Building.
     */
	class Castle : public Building {
	
	private:
		int m_level;              //< number of level
		float m_durability;       //< current health
		int m_money;              //< the amount of gold at the player's disposal at the moment

		int m_repair_and_revenue_cooldown;          //< cooldown for repair and revenue power
		std::vector<Castle_Level_Info>& m_table;    //< table of information about castle levels (loading from config)

	public: 

        /// Construct of class for determinate start state
		Castle(std::vector<Castle_Level_Info>& info, int coord = 0, int money = 0);
		~Castle(); 

		/// Get information of current building (Getter)
		int getLevel() const noexcept { return m_level; }
        /// Get information of current money (Getter)
		int getMoney() const noexcept { return m_money; }
        /// Get information of table (Getter)
		int getSpeed() const noexcept { return m_table[m_level].repair_time; }
        /// Get information of durability (Getter)
		float getDurability()    const noexcept { return m_durability; }
        /// Get information of max durability (Getter)
		float getMaxDurability() const noexcept { return m_table[m_level].max_durability; }
        /// Get information of table (Getter)
		const std::vector<Castle_Level_Info>& getTable() const noexcept { return m_table; }

		void buy_anything(int cost);  //< Throws an exception if not enough money
	
		void receive_damage(float damage, std::stringstream&);  //< Function for calculate damage to castle

		void turn(Landscape&, std::stringstream&);  //< Calculate condition after turn
		void level_up();  //< Calculate condition after level up
		int cost_upgrade() const;  //< Calculate cost of new upgrade
		

	private:
		void repair(); //< Calculate condition after repair castle
	};

}

#endif __CASTLE__
