#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "Item.h";

class CardDeck;
class Player;
enum TypeOfReward {
	MONEY,
	CARD,
	ITEM,
	HEALTH,
	MAXHEALTH

};



struct Reward
{
	ItemHandler item;
	int amount;
	Rectangle body;
	TypeOfReward type;

	bool isClicked() {
		return CheckCollisionPointRec(GetMousePosition(), body);
	};
	void getCard(Player *player, std::string rarity);
	void getGold(Player* player, int amount);
	void heal(Player* player, int amount);
	void getMaxHealth(Player* player, int amount);
	void getItem(Player *player, std::string rarity);
};

class LootRoom 
{
public:
	ItemHandler item;
	bool active = true;
	std::vector <Reward> options;
	void update(Player *player);
	void draw(Player *player);
	void createRoom();

};

