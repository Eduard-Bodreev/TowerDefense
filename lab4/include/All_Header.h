#pragma once

#ifndef __ALL_HEADER__
#define __ALL_HEADER__

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../../vector_template/mvector.h"


namespace Tower_Defence {

	struct Tower_Level_Info;
	struct Castle_Level_Info;
	struct WallInfo;
	struct Config;

	class Building;
	class Tower;
	class Wall;
	class Castle;

	class Aura;
	class Hero;
	class Enemy;
	class Light_Infantry;
	class Heavy_Infantry;
	class Aviation;

	class Den;
	struct Cell;
	class Landscape;

	

	enum class Color;
	typedef enum class AuraType  AT;
	typedef enum class LandType  LT;
	typedef enum class EnemyType ET;
	typedef std::pair<EnemyType, std::map<std::string, float>> enemy_type_info;
}

#include "Config.h"
#include "Aura.h"
#include "Hero.h"

#include "Building.h"
#include "Tower.h"
#include "Wall.h"
#include "Castle.h"
#include "Den.h"

#include "Enemy.h"
#include "Light_Infantry.h"
#include "Heavy_Infantry.h"
#include "Aviation.h"

#include "Landscape.h"

#endif __ALL_HEADER__