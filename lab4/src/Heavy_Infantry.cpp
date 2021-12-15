#include "../include/All_Header.h" 
#include "../include/Heavy_Infantry.h"

namespace Tower_Defence {

    float Heavy_Infantry::getWallDamage() const noexcept {
        float damage = getInfo().find("walls_damage")->second;
        const std::vector<Aura*>* auras = getExertedAuras();
        if (auras) for (auto aura : *auras)
            if (aura->getType() == AT::DAMAGE)
                damage *= aura->getMult();
        return damage;
    };

    int Heavy_Infantry::move(Landscape& land, std::stringstream& ss) {
        heal();
        int speed = getSpeed();
        for (int i = 0; i < speed; i++) {
            int coord = this->getCoord();
            // before we walk we shoot at the walls
            for (std::list<Wall>::const_iterator it = land.getWalls().begin(); it != land.getWalls().end(); it++) {
                if (land.distance(coord, it->getCoord()) <= getRadius()) {
                    float damage = getWallDamage();
                    ss << "Heavy (coord " << coord << ") damages Wall (coord " << it->getCoord() << ") - " << damage;
                    land.wall_receive_damage(it->getCoord(), damage, ss);
                    return 1;
                }
            }

            int directions[4] = { land.right(coord), land.left(coord), land.up(coord), land.down(coord) };
            int next = coord;
            for (auto dir : directions)
                if (dir != -1 && land.getDistHeavy(dir) < land.getDistHeavy(next) && 
                    (land.getMap()[dir].type == LT::PLAIN || land.getMap()[dir].type == LT::CASTLE))
                    next = dir;
            this->setCoord(land, next);

            // made a move. If you came to the castle, then we inflict damage and self-destruct
            if (land.getDistHeavy(getCoord()) == 0) {
                float damage = getDamage();
                ss << "Heavy (coord " << next << ") damages Castle - " << damage << "\n";
                land.castle_receive_damage(damage, ss); // if the castle died, exception is thrown out here
                return 0;
            }
        }
        return 1;
    }

}