#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "LootRoom.h"


class Player;
class CardDeck;

struct Button 
{
	Rectangle body = {1000, 800, 400, 200};
	void setStats(float x, float y, float width, float height) {
		this->body = { x,y, width, height };
	}
	void draw(Color color) {
		DrawRectangleRec(body, color);
		DrawRectangleLinesEx(body, 2, BLACK);
	}
	bool isClicked() {
		return !CheckCollisionPointRec(GetMousePosition(), body);
	};
};

struct ShopItems
{
	std::string typeString;
	Rectangle body = { 25, 500, 200, 200 };
	int cost = 10;
	ShopItems(int i) {
		pos = i;
		this->body.x = (250 * i) + 50;
		switch (i) {
		case 0:
			type = HEALTH;
			cost = GetRandomValue(10, 50);
			typeString = "Health";
			break;
		case 1:
			type = MAXHEALTH;
			cost = GetRandomValue(10, 100);
			typeString = "Max Health";
			break;
		default:
			cost = GetRandomValue(20, 100);
			int rem = GetRandomValue(0, 1);
			type = rem == 0 ? ITEM : CARD;
			typeString = rem == 0 ? "Item" : "Card";
			break;
		}

	}

	TypeOfReward type;
	bool active = true;
	int pos;	
	bool isClicked() {
		return CheckCollisionPointRec(GetMousePosition(), body);
	}
	void draw() {
		DrawRectangleRec(this->body, DARKBLUE);
	}
};
class ShopRoom:Reward
{
public:

	bool active = true;
	Button btn;
	std::vector<ShopItems> items;
	void createShopRoom();
	void doAction(Player *player, CardDeck *deck);
	void draw(Player* player);
	void update(Player* player, CardDeck* deck);
};

