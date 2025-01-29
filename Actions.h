#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

class Player;
class Enemy;
class Card;
class Actions
{
	public:
		int dealDamage(Player* player, Enemy* enemy, int amount,  bool ignoreArmor, bool targetPlayer);
		void healTarget(Player* player, Enemy* enemy, int amount);
		void useLifesteal(Player* player, Enemy* enemy, int amount, bool targetPlayer);
		void getArmor(Player* player, Enemy* enemy, int amount);
		void boostTarget(Player* player, Enemy* enemy, Enemy* target, Card* castingCard, int amount, int rnd);
		void castPoison(Player* player, Enemy* enemy, int amount, bool targetPlayer);
		void castFire(Player* player, Enemy* enemy, int amount, bool targetPlayer);
		void castBleed(Player* player, Enemy* enemy, int amount, bool targetPlayer);
		void castSneak(Enemy* enemy);
		void castFreeze(Enemy *enemy, int amount);
		void setParry(Player* player, Enemy* enemy, int amount);
		void addMana(Player* player, int amount);


};

