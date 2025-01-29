#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "Actions.h"

class Player;
class Enemy;
class ActionStats {
public:
	Actions doAction;
	std::string type;
	bool boosted = false;
	int boostedAmount;
	bool sacrifice = false;
	int amount = 0;
	int amount_Max = this->amount;
	std::vector<int> range_left;
	std::vector<int> range_right;


	void checkForOthers(Enemy *self, std::vector <Enemy> *others, int round, Player* player); // searches for ally in range, casts actions
	void surviveAction(Enemy* self, std::vector <Enemy>* others, int round,  Player* player);

	
	void draw(Enemy* enemy);


	bool ignore_armor;
	/*
	void healTarget(Enemy* target, int amount);
	void addArmor(Enemy* target, int amount);
	int dealDamage(Player* player, bool ignArm, Enemy* enemy);
	void lifeSteal(Player* player, Enemy* self);
	void poison(Player* player, int amount);
	void fire(Player* player, int amount);
	void bleed(Player* player, int amount);
	void boost(Enemy* target, int amount);
	void parry(Enemy* target);
	void sneak(Enemy* target);
	*/



};



