#pragma once

#ifndef __HEAVY_INFANTRY__
#define __HEAVY_INFANTRY__

namespace Tower_Defence {

	class Heavy_Infantry : public Enemy {

	public:
		Heavy_Infantry(enemy_type_info& type_info, int coord = -1) : Enemy(type_info, coord) {};
		int move(Landscape&, std::stringstream&);
		float getRadius() const noexcept { return getInfo().find("radius")->second; }
		float getWallDamage() const noexcept;

	};

	class Heavy_Infantry_Hero : public Heavy_Infantry, public Hero {
	public:
		Heavy_Infantry_Hero(enemy_type_info& type_info, std::vector<Aura*>& auras, int coord = -1) :
			Heavy_Infantry(type_info, coord), Hero(auras) {};
		//default destructor - destructors of base classes
		const std::vector<Aura*>* getAuras() const noexcept { return Hero::getAuras(); }
	};

}


#endif __HEAVY_INFANTRY__