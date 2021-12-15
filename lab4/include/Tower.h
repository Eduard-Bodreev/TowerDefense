/*!
\file
\brief Tower class description header file
This file contains the definitions of the main
tower methods
*/

#pragma once

#ifndef __TOWER__
#define __TOWER__

namespace Tower_Defence {

	class Tower : public Building {
	private:
		int m_level;                             // Current level of the tower
		float m_cooldown;                        // How many moves left before next shot
		std::vector<Tower_Level_Info>* m_table;  // Table with initial params of towers of all levels (uploaded from config file)

	public:
		Tower(): Building(0), m_level(0), m_cooldown(0), m_table(nullptr) {};
		Tower(int coord, std::vector<Tower_Level_Info>& info);
		~Tower() {};

		/*! Getter of the current level of the tower. */
		int getLevel() const noexcept { return m_level; }
		/*! Getter of table with initial params */
		const std::vector<Tower_Level_Info>* getInfo() const noexcept { return m_table; }
		/*!
		Shooting an enemy. If cooldown = 0, the tower damages the first enemy in an enemy array, which is within the firing radius. 
		\param land Map of the game, that includes coordinates of all enemies on map.
		\param ss Stringsteam for logging, there is written all information about damaged and killed enemies during this turn.
		\throw This method doesn't hrow any exceptions.
		*/
		void turn(Landscape&, std::stringstream&) noexcept;
		/*!
		Upgrading the tower by one level.
		\return Returns price paid for the upgrading.
		\throw Throws an exception, if the level is already max.
		*/
		int level_up();
		/*!
		Getting cost of upgrading tower to next level.
		\return Cost of the next level.
		\throw std::logic_error If the level is already max. 
		*/
		int cost_upgrade() const;
	};
}

#endif __TOWER__