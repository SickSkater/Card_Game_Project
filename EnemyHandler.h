#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "Enemy.h"

struct EnemyCollection {
	float c_chance = 80;
	float u_chance = 50;
	float r_chance = 10;
	float e_chance = 5;
	float l_chance = 2;
	void fillCollection(Enemy enemy) {
		if (enemy.stats.rarity == "Common") {
			common.push_back(enemy);
		}
		else if (enemy.stats.rarity == "Uncommon") {
			uncommon.push_back(enemy);
		}
		else if (enemy.stats.rarity == "Rare") {
			rare.push_back(enemy);
		}
		else if (enemy.stats.rarity == "Epic") {
			epic.push_back(enemy);
		}
		else {
			legendary.push_back(enemy);
		}
	}

	std::vector<Enemy> common;
	std::vector<Enemy> uncommon;
	std::vector<Enemy> rare;
	std::vector<Enemy> epic;
	std::vector<Enemy> legendary;


};

class EnemyHandler
{
public:
	EnemyCollection enemyCollection;
	std::vector<Enemy> enemies;
	void pickEnemyFromCollection(Segments* segment);
	void draw(int round);

};

