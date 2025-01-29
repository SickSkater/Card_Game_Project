#include "EnemyHandler.h"




void EnemyHandler::pickEnemyFromCollection(Segments* segment) {
	int chance = GetRandomValue(1, 100);
	if (chance >= 100 - this->enemyCollection.l_chance) {
		this->enemies.push_back(enemyCollection.legendary.at(GetRandomValue(0, enemyCollection.legendary.size() - 1)));
	} else if (chance >= 100 - this->enemyCollection.e_chance) {
		this->enemies.push_back(enemyCollection.epic.at(GetRandomValue(0, enemyCollection.epic.size() - 1)));
	} else if (chance >= 100 - this->enemyCollection.r_chance) {
		this->enemies.push_back(enemyCollection.rare.at(GetRandomValue(0, enemyCollection.rare.size() - 1)));
	} else if (chance >= 100 - this->enemyCollection.u_chance) {
		this->enemies.push_back(enemyCollection.uncommon.at(GetRandomValue(0, enemyCollection.uncommon.size() - 1)));
	}
	else {
		this->enemies.push_back(enemyCollection.common.at(GetRandomValue(0, enemyCollection.common.size() - 1)));
	}
	this->enemies.back().setPosition(segment);
}

void EnemyHandler::draw(int round) {
	for (Enemy enemy : enemies) {
		enemy.draw();
	}
}

