#pragma once

#ifndef __LIGHT_INFANTRY__
#define __LIGHT_INFANTRY__

namespace Tower_Defence {

    class Light_Infantry : public Enemy {
    public:
        Light_Infantry(enemy_type_info& type_info, int coord = -1) : Enemy(type_info, coord) {};
        int move(Landscape&, std::stringstream&);
        const std::vector<Aura*>* getAuras() const noexcept { return nullptr; };
    };


    class Light_Infantry_Hero : public Light_Infantry, public Hero {
    public:
        Light_Infantry_Hero(enemy_type_info& type_info, std::vector<Aura*>& auras, int coord = -1) :
            Light_Infantry(type_info, coord), Hero(auras) {};
        // default destructor - destructors of base classes
        const std::vector<Aura*>* getAuras() const noexcept { return Hero::getAuras(); }
    };
}

#endif __LIGHT_INFANTRY__
