#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "GameArea.h"
#include "Item.h"
#include "CardDeck.h"



struct PlayerStats {
	int starting_armor = 0;
	int poison_res = 0;
	int fire_res = 0;
	int bleed_res = 0;
	int health;
	int max_health;

	int mana;
	int max_mana;

	int armor;

	int gold = 10;

	int manaRecharge = 5;
	int startingDraw = 5;
	int redraw = 3;
	int healthRecharge = 3;
	int parry = 0;

	int bleed_stacks = 0;
	int poison_stacks = 0;
	int fire_stacks = 0;
};

class Player
{
public:
	int addedArmor = 0;
	int healedHealth = 0;
	Player();
	std::vector <Item> allItems;
	std::vector <Item> currentItems;
	PlayerStats stats;
	CardDeck current_deck;
	CardDeck all_cards;
	CardDeck locked_deck;
	CardDeck discarded_deck;
	CardDeck draw_deck;
	GameArea* enemy_board;

	void createAllCards();
	void drawCard(int count);
	void discardCard(Card *card, int index);
	void unlockNewCard(float chance);
	int checkNewCardDeck(std::string rarity);
	void getNewCard(int index);
	void draw();
	void drawInfo();

};

