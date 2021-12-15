#include "../include/All_Header.h" 

namespace Tower_Defence {

    Enemy::Enemy(enemy_type_info& info, int coord) :
        all_info(info.second),
        type(info.first),
        health(info.second.find("max_health")->second),
        heal_cooldown((int)info.second.find("heal_speed")->second),
        coord(coord)
    {};


    int Enemy::getSpeed() const noexcept {
        int speed = (int)all_info.find("speed")->second;
        for (auto aura : exerted_auras)
            if (aura->getType() == AT::SPEED)
                speed *= (int)aura->getMult();
        return speed;
    }

    float Enemy::getDamage() const noexcept {
        float damage = all_info.find("damage")->second;
        for (auto aura : exerted_auras)
            if (aura->getType() == AT::DAMAGE)
                damage *= aura->getMult();
        return damage;
    }

    const std::vector<Aura*>* Enemy::getExertedAuras() const noexcept { 
        if (exerted_auras.size()) return &exerted_auras; 
        else return nullptr; 
    }

    void Enemy::receive_damage(float damage) {
        if (health <= damage)
            throw std::out_of_range("Enemy killed");
        health -= damage;
    }


    void Enemy::setCoord(Landscape& land, int coord_) {
   //     if (coord_ < 0 || coord_ >= land.getSize())
     //       throw std::out_of_range("This coordinate doesn't belong to map");
        coord = coord_;
    }

    void Enemy::heal() noexcept {
        if (--heal_cooldown == 0) {
            float percent = all_info.find("heal_percent")->second;
            for (auto aura : exerted_auras)
                if (aura->getType() == AT::HEAL)
                    percent += aura->getMult(); 
            float max = all_info.find("max_health")->second;
            health += max / 100 * percent;
            if (health > max) health = max;
            heal_cooldown = (int)all_info.find("heal_speed")->second;
        }

    };
}