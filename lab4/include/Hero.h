#pragma once

#ifndef __HERO__
#define __HERO__

namespace Tower_Defence {

	class Hero {
    private:
        std::vector<Aura*> auras;

    public:
        Hero(std::vector<Aura*>& aur) : auras(aur) {};
        ~Hero() { for (auto aura : auras) if (aura) delete aura; };
        const std::vector<Aura*>* getAuras() const noexcept { return &auras; }
	};

}

#endif __HERO__
