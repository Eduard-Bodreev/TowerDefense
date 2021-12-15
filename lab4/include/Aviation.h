#pragma once

#ifndef __AVIATION__
#define __AVIATION__

namespace Tower_Defence {
    class Aviation : public Enemy {
    private:
        int ammunition;  // number of shoots

    public:
        Aviation(enemy_type_info& type_info, int coord = -1);
        int move(Landscape&, std::stringstream&);
        float getRadius() const noexcept { return getInfo().find("radius")->second; }
    };


    class Aviation_Hero : public Aviation, public Hero {
    public:
        Aviation_Hero(enemy_type_info& type_info, std::vector<Aura*>& auras, int coord = -1) :
            Aviation(type_info, coord), Hero(auras) {};
        // default destructor - destructors of base classes
        const std::vector<Aura*>* getAuras() const noexcept { return Hero::getAuras(); }
    };
}

#endif __AVIATION__
