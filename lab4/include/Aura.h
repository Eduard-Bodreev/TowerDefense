#pragma once

#ifndef __AURA__
#define __AURA__

namespace Tower_Defence {

	typedef enum class AuraType {
		SPEED, DAMAGE, HEAL
	}AT;

	class Aura {
	private:
		AuraType type;
		float radius;
		float mult;

	public:
		Aura() : type(AT::DAMAGE), radius(0), mult(1) {};
		Aura(AT type, float rad, float mult) : type(type), radius(rad), mult(mult) {};
		~Aura() {};
		float getRadius() const noexcept { return radius; }
		float getMult()   const noexcept { return mult; }
		AT getType() const noexcept { return type; }
	};
}

#endif __AURA__