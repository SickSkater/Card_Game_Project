#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Actions.h"

using json = nlohmann::json;
class Player;
class Enemy;
class Card;

enum ItemActionType {
	MAX_HEALTH_ITEM,
	MAX_MANA_ITEM,
	HEALTH_ITEM,
	HEAL_ITEM,
	ARMOR_ITEM,
	DAMAGE_ITEM,
	POISON_ITEM,
	FIRE_ITEM,
	BLEED_ITEM,
	DRAWN_CARDS_ITEM,
	POISON_RES_ITEM,
	FIRE_RES_ITEM,
	BLEED_RES_ITEM,
	DMG_BOOST_ITEM,
	MANA_RECHARGE_ITEM,
	HEALTH_RECHARGE_ITEM

};

enum ItemCooldown {
	SPELL_ITEM,
	ROUND_ITEM,
	MATCH_ITEM,
	PASSIVE_ITEM
};

enum ItemTarget {
	PLAYER_ITEM,
	ENEMY_ITEM,
	RANDOM_ITEM
};

struct ItemActionStats {
	Actions action;
	int amount;
	std::string type;
	int chance = 0;
	bool exchange = false;
	bool roll = false;
	std::vector<ItemActionStats> getStats(json data, int dataID);
	void doExchange(Player* player, std::vector<Enemy> *enemyHandler);
	void doRoll(Player* player, std::vector<Enemy> *enemyHandler);
	void doPassive(Player *player, std::vector<Enemy> *enemyHandler);
	void doAction(Player* player, std::vector<Enemy> *enemyHandler, std::string cooldownParam);
	void ExchangeArmorToHeal(Player* player);
	void ExchangeHealToDamage(Player* player, std::vector<Enemy> *enemyHandler);
};


class Item
{
public:
	std::vector<Item> createAllItems();
	std::vector<Item> allItems;
	std::vector<ItemActionStats> actions;
	std::string name;
	std::string rarity;
	bool used = false;
	std::string cooldown;
	std::string target;
	int id;



};

class ItemHandler {
public:
	ItemHandler();
	std::vector <Item> all_items;
	std::vector <Item> common_items;
	std::vector <Item> uncommon_items;
	std::vector <Item> rare_items;
	std::vector <Item> epic_items;
	std::vector <Item> legendary_items;

	Item getItem(std::string rarity);
};

