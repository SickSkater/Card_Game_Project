#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "CardDeck.h"
#include "rewardScreen.h"
#include "EnemyHandler.h"
#include "GameArea.h"
#include "Player.h"
#include "GameMap.h"
#include "CardCollection.h"



class Game
{
public:
	ItemHandler itemHandler;
	ItemActionStats itemActRem;
	int round = -1;
	Player player;
	CardDeck cards;
	EnemyHandler enemyHandler;
	std::vector <GameArea> areas;
	Rectangle round_btn;
	GameMap map;
	rewardScreen rwdScreen;
	CardCollection collection;
	bool isMapActive = true;

	void roundManager();
	void createFight();
	void playerRound();
	void create();
	void update();
	void draw();

};

