#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>




class Enemy;
class Player;
class Card;

class CardActions
{
public:
	bool boosted = false;
	int boostedAmount = 0;
	int amount;
	int id;
	
	std::string action_type;
	std::vector<int> range_left;
	std::vector<int> range_right;

	void castSpells(std::vector<Enemy>* enemies, Card* card, Player* player, int round);
	/*
	int dealDamage(Enemy* target, bool ignore_armor, Player *player);
	void heal(Player* player);
	void addArmor(Player* player);
	void freeze(Enemy* target, int round);
	void poison(Enemy* target);
	void boostDamage(Card* card);
	void addMana(Player* player);
	void lifeSteal(Player* player, Enemy* target);
	void parry(Player* player);
	void fire(Enemy* target);
	void bleed(Enemy* target);
	*/



};

