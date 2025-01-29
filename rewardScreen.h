#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

class Card;
class rewardScreen
{
public:
	bool end = false;
	int gold;
	std::vector<Card*> cards;
	Rectangle screen = { 1920 / 2 - 600, 1080 / 2 - 400, 600, 400 };
	Rectangle btn = { screen.x, screen.y, 100, 50 };

	void draw();
	void claimReward();
};

