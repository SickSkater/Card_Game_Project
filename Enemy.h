#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "GameArea.h"
#include "EnemyActions.h"
#include "Player.h"

#include "nlohmann/json.hpp"

struct Stats {
	bool active = false;
	std::string name;
	int id;
	int segment_index;
	int health, max_health;
	int armor = 0;
	std::string rarity;
	std::string reward;
	int parry = 0;
	bool sneak = false;
	bool hasChangedPosition = false;

};

class Enemy
{
public:
	bool poison_imune = false;
	bool freeze_imune = false;
	bool bleed_imune = false;
	bool fire_imune = false;

	float pos_X, pos_Y;
	float width, height;
	bool attacking = false;
	Rectangle body = { pos_X, pos_Y, width, height };
	Stats stats;
	bool right = false;
	bool priority = false;
	int poison_stacks = 0;
	int fire_stacks = 0;
	int bleed_stacks = 0;
	ActionStats survive;
	ActionStats actions[4];
	Enemy(int dataID);
	void setPosition(Segments* segment);
	void drawActionStats();
	void draw();
	void printInfo();
	void setNewPosition(std::vector<Segments>* segment, bool right, Enemy* swapTarget);

};

