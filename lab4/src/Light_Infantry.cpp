#include "../include/All_Header.h" 

namespace Tower_Defence {
    int Light_Infantry::move(Landscape& land, std::stringstream& ss) {
        heal();
        int speed = getSpeed();
        for (int i = 0; i < speed; i++) {
            int coord = this->getCoord();
            int directions[4] = { land.right(coord), land.left(coord), land.up(coord), land.down(coord) };

            if (land.getDistLight(coord) == INF) {
                ss << "You've trapped LIGHT_INF (coord " << coord << ")!\n";
                return 1;
            }

            int next = coord;
            for (auto dir : directions)
                if (dir != -1 && land.getDistLight(dir) < land.getDistLight(next))
                    next = dir;
            this->setCoord(land, next);

            // made a move. If you came to the castle, then we inflict damage and self-destruct
            if (land.getDistLight(next) == 0) {
                float damage = getDamage();
                ss << "Light (coord " << next << ") damages Castle - " << damage << "\n";
                land.castle_receive_damage(damage, ss); // if the castle died, an exception is thrown here
                return 0;
            }
        }
        return 1;
    }

}
