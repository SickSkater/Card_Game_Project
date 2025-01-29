#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

class GameRoom
{
public:
	int index = 0;
	Rectangle body;
	Vector2 pos;
	Color color = RED;
	bool current = false;
	std::vector <Rectangle> nextRec; 
	std::vector <Vector2> nextPos;
	void getType(int type);
	std::string type;
	bool isHovered();
	std::string getType();
	Rectangle isClicked();
};
