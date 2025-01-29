#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "LootRoom.h"

class EncounterRoom :Reward
{
public:
	bool fight = false;
	void getRandomGood(Player* player, int count);
	void getRandomBad(Player* player, int count);
	void looseHealth(Player *player, int amount);
	void looseMaxHealth(Player* player, int amount);
	void looseGold(Player* player, int amount);
	void looseItem(Player* player);
	void upgradeItem(Player* player);


	void getResult(Player *player);





};

