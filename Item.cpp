#include "Item.h"
#include "Player.h"
#include "Enemy.h"

using json = nlohmann::json;
std::fstream itemFile("itemDatabase.json");
json itemData = json::parse(itemFile);

std::vector<ItemActionStats> ItemActionStats::getStats(json data, int dataID) { // gets stats from json file
	std::vector<ItemActionStats> typeRem;
	if (data.at(dataID).contains("Stats")) {
		for (int i = 0; i < data.at(dataID).at("Stats").size(); i++) {
			//std::cout << itemData.at(dataID).at("Stats").at(i) << " NORMAL \n";
			typeRem.push_back({action, itemData.at(dataID).at("Stats").at(i).at("Amount"), itemData.at(dataID).at("Stats").at(i).at("Type"), 0, false, false });
		}
	}
	if (data.at(dataID).contains("Exchange")) {
		for (int i = 0; i < data.at(dataID).at("Exchange").size(); i++) {
			//std::cout << itemData.at(dataID).at("Exchange").at(i) << " EXCHANGE \n";
			typeRem.push_back({ action, itemData.at(dataID).at("Exchange").at(i).at("Amount"), itemData.at(dataID).at("Exchange").at(i).at("Type"),0, true, false });

		}
	}
	if (data.at(dataID).contains("Roll")) {
		for (int i = 0; i < data.at(dataID).at("Roll").size(); i++) {
			//std::cout << itemData.at(dataID).at("Roll").at(i) << " ROLL \n";
			typeRem.push_back({ action, itemData.at(dataID).at("Roll").at(i).at("Amount"), itemData.at(dataID).at("Roll").at(i).at("Type"),itemData.at(dataID).at("Roll").at(i).at("Chance"), false, true });

		}
	}
	return typeRem;
}

std::vector<Item> Item::createAllItems() { // creates all items from json database

	for (int dataID = 0; dataID < itemData.size(); dataID++) {
		Item placeholderItem;
		ItemActionStats placeholderItemAction;
		placeholderItem.name = itemData.at(dataID).at("Name");
		placeholderItem.id = itemData.at(dataID).at("ID");
		placeholderItem.rarity = itemData.at(dataID).at("Rarity");
		placeholderItem.cooldown = itemData.at(dataID).at("Cooldown");
		placeholderItem.target = itemData.at(dataID).at("Target");
		/*if (itemData.at(dataID).at("Stats").contains("Armor")) {

		}
		if (itemData.at(dataID).at("Stats").contains("Armor")) {

		}
		if (itemData.at(dataID).at("Stats").contains("Health")) {

		}
		if (itemData.at(dataID).at("Stats").contains("Damage")) {

		}
		if (itemData.at(dataID).at("Stats").contains("Health_max")) {

		}
		if (itemData.at(dataID).at("Stats").contains("Exchange")) {

		}
		*/

		placeholderItem.actions = placeholderItemAction.getStats(itemData, dataID);
		this->allItems.push_back(placeholderItem);
	}
	/*for (Item item : this->allItems) {
		std::cout << item.name << " " << item.rarity << " " << item.target << " " << item.cooldown << "\n";
	}
	*/
	return allItems;
	
} 

void ItemActionStats::doPassive(Player* player, std::vector<Enemy> *enemyHandler) { // does passive actions with one time effect, or checks for conditions of permanent ones
	for (int itemCounter = 0; itemCounter < player->currentItems.size(); itemCounter++) {
		if (player->currentItems.at(itemCounter).cooldown == "PASSIVE" && !player->currentItems.at(itemCounter).actions.at(0).exchange && !player->currentItems.at(itemCounter).actions.at(0).roll && player->currentItems.at(itemCounter).used == false) {
			for (int spellCounter = 0; spellCounter < player->currentItems.at(itemCounter).actions.size(); spellCounter++) {
				int amount = player->currentItems.at(itemCounter).actions.at(spellCounter).amount;
					player->currentItems.at(itemCounter).used = true;
					if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Armor") {
						player->stats.starting_armor += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Fire_ressistance") {
						player->stats.fire_res += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Poison_ressistance") {
						player->stats.poison_res += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Bleed_ressistance") {
						player->stats.bleed_res += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Drawn_cards") {
						player->stats.redraw += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Health_max") {
						player->stats.max_health += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Mana_max") {
						player->stats.max_mana += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Mana_recharge") {
						player->stats.manaRecharge += amount;
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Health_recharge") {
						player->stats.healthRecharge += amount;
					}
				}
		}
		else if (player->currentItems.at(itemCounter).actions.at(0).exchange) {
			this->doExchange(player, enemyHandler);
		}
	}
}

void ItemActionStats::doAction(Player* player, std::vector<Enemy> *enemyHandler, std::string cooldownParam) { // does action that matches cooldown param, used for non passive items
	int rNum;
	for (int itemCounter = 0; itemCounter < player->currentItems.size(); itemCounter++) {
		rNum = GetRandomValue(0, enemyHandler->size());
		if (player->currentItems.at(itemCounter).cooldown == cooldownParam && !player->currentItems.at(itemCounter).actions.at(0).exchange && !player->currentItems.at(itemCounter).actions.at(0).roll && enemyHandler->size() > 0) {
			std::cout << "|	ITEM	|: Item triggered: " << player->currentItems.at(itemCounter).name << "\n";
			for (int spellCounter = 0; spellCounter < player->currentItems.at(itemCounter).actions.size(); spellCounter++) {
				if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Damage") {
					if (player->currentItems.at(itemCounter).target == "Random") {
						if (rNum == 0) {
							std::cout << "|	ITEM	|: " << "" << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";
							this->action.dealDamage(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false, true);
						}
						else {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.dealDamage(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false, false);

						}
					}
					else if (player->currentItems.at(itemCounter).target == "Player") {
						std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";
						this->action.dealDamage(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false, true);

					}
					else if (player->currentItems.at(itemCounter).target == "Enemy") {
						std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";
						this->action.dealDamage(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false, false);

					}

				}
				else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Armor") {
					if (player->currentItems.at(itemCounter).target == "Random") {
						if (rNum == 0) {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";
							this->action.getArmor(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount);
						}
						else {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.getArmor(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

						}
					}
					else if (player->currentItems.at(itemCounter).target == "Player") {
						std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

						this->action.getArmor(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

					}
					else if (player->currentItems.at(itemCounter).target == "Enemy") {
						std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

						this->action.getArmor(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Heal") {
						if (player->currentItems.at(itemCounter).target == "Random") {
							if (rNum == 0) {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

								this->action.healTarget(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount);
							}
							else {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

								this->action.healTarget(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

							}
						}
						else if (player->currentItems.at(itemCounter).target == "Player") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

							this->action.healTarget(player, NULL, player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

						}
						else if (player->currentItems.at(itemCounter).target == "Enemy") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.healTarget(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount);

						}
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Poison") {
						if (player->currentItems.at(itemCounter).target == "Random") {
							if (rNum == 0) {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

								this->action.castPoison(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);
							}
							else {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

								this->action.castPoison(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

							}
						}
						else if (player->currentItems.at(itemCounter).target == "Player") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

							this->action.castPoison(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);

						}
						else if (player->currentItems.at(itemCounter).target == "Enemy") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.castPoison(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

						}
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Fire") {
						if (player->currentItems.at(itemCounter).target == "Random") {
							if (rNum == 0) {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

								this->action.castFire(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);
							}
							else {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

								this->action.castFire(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

							}
						}
						else if (player->currentItems.at(itemCounter).target == "Player") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

							this->action.castFire(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);

						}
						else if (player->currentItems.at(itemCounter).target == "Enemy") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.castFire(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

						}
					}
					else if (player->currentItems.at(itemCounter).actions.at(spellCounter).type == "Bleed") {
						if (player->currentItems.at(itemCounter).target == "Random") {
							if (rNum == 0) {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

								this->action.castBleed(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);
							}
							else {
								std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

								this->action.castBleed(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

							}
						}
						else if (player->currentItems.at(itemCounter).target == "Player") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted PLAYER!\n";

							this->action.castBleed(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, true);

						}
						else if (player->currentItems.at(itemCounter).target == "Enemy") {
							std::cout << "|	ITEM	|: " << player->currentItems.at(itemCounter).name << " targeted ENEMY!\n";

							this->action.castBleed(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(itemCounter).actions.at(spellCounter).amount, false);

						}
					}

				}
			}
		}
		else if (player->currentItems.at(itemCounter).actions.at(0).roll) {
					this->doRoll(player, enemyHandler);
		}
	}
}

void ItemActionStats::ExchangeArmorToHeal(Player* player) { // hardcoded func for certain item
	if (player->addedArmor >= 2) {
		std::cout << "|	ITEM	|: " << "CONVERTED: " << player->addedArmor << " to: " << player->addedArmor / 2 << " damage!\n";
		player->addedArmor = 0;
		player->stats.health += player->addedArmor / 2;
		if (player->stats.health > player->stats.max_health) {
			player->stats.health = player->stats.max_health;
		}

	}
}

void ItemActionStats::ExchangeHealToDamage(Player* player, std::vector<Enemy>* enemyHandler) {// hardcoded func for certain item
	if (player->healedHealth >= 2) {
		std::cout << "|	ITEM	|: " << "CONVERTED: " << player->healedHealth << " to: " << player->healedHealth / 2 << " armor!\n";
		enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)).stats.health -= player->healedHealth / 2;
		player->healedHealth = 0;
	}
}

void ItemActionStats::doExchange(Player* player, std::vector<Enemy>* enemyHandler) { // exchange handler
	for (int item = 0; item < player->currentItems.size(); item++) {
		if (player->currentItems.at(item).actions.at(0).exchange) {
			if (player->currentItems.at(item).actions.at(0).type == "Armor") {
				this->ExchangeArmorToHeal(player);
			}
			else {
				this->ExchangeHealToDamage(player, enemyHandler);
			}
		}
	}
}

void ItemActionStats::doRoll(Player* player, std::vector<Enemy> *enemyHandler) { // roll handler

	int rNum = GetRandomValue(0, enemyHandler->size());
	for (int item = 0; player->currentItems.size(); item++) {
		if (player->currentItems.at(item).actions.at(0).roll) {
			int act1chance = player->currentItems.at(item).actions.at(0).chance;
			int act2chance = player->currentItems.at(item).actions.at(1).chance;
			if (GetRandomValue(0, act1chance + act2chance) <= act1chance) {
				if (player->currentItems.at(item).target == "Random") {
					if (rNum == 0) {
						std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 1 targeted PLAYER!\n";

						this->action.dealDamage(player, NULL, player->currentItems.at(item).actions.at(0).amount, false, true);
					}
					else {
						std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 1 targeted ENEMY!\n";

						this->action.dealDamage(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(item).actions.at(0).amount, false, false);

					}
				}
				else if (player->currentItems.at(item).target == "Player") {
					std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 1 targeted PLAYER!\n";

					this->action.dealDamage(player, NULL, player->currentItems.at(item).actions.at(0).amount, false, true);

				}
				else if (player->currentItems.at(item).target == "Enemy") {
					std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 1 targeted ENEMY!\n";

					this->action.dealDamage(player, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(item).actions.at(0).amount, false, false);

				}
				return;
			}
			else {
				if (player->currentItems.at(item).target == "Random") {
					if (rNum == 0) {
						std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 2 targeted PLAYER!\n";

						this->action.getArmor(player, NULL, player->currentItems.at(item).actions.at(1).amount);
					}
					else {
						std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 2 targeted ENEMY!\n";

						this->action.getArmor(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(item).actions.at(1).amount);

					}
				}
				else if (player->currentItems.at(item).target == "Player") {
					std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 2 targeted PLAYER!\n";

					this->action.getArmor(player, NULL, player->currentItems.at(item).actions.at(1).amount);

				}
				else if (player->currentItems.at(item).target == "Enemy") {
					std::cout << "|	ITEM	|: " << player->currentItems.at(item).name << " rolled option 2 targeted ENEMY!\n";

					this->action.getArmor(NULL, &enemyHandler->at(GetRandomValue(0, enemyHandler->size() - 1)), player->currentItems.at(item).actions.at(1).amount);

				}
				return;
			}
		}
		
	}
}


ItemHandler::ItemHandler() { // create vectors sorted by rarity
	Item rem;
	this->all_items = rem.createAllItems();
	for (int item = 0; item < all_items.size(); item++) {
		if (all_items.at(item).rarity == "Common") {
			this->common_items.push_back(all_items.at(item));
		}
		else if (all_items.at(item).rarity == "Uncommon") {
			this->uncommon_items.push_back(all_items.at(item));
		}
		else if (all_items.at(item).rarity == "Rare") {
			this->rare_items.push_back(all_items.at(item));
		}
		else if (all_items.at(item).rarity == "Epic") {
			this->epic_items.push_back(all_items.at(item));
		}
		else {
			this->legendary_items.push_back(all_items.at(item));
		}
	}
}

Item ItemHandler::getItem(std::string rarity) { // get item by rarity or chance
	int rNum = GetRandomValue(0, this->all_items.size() - 1);
	if (rarity == "RANDOM") {
		std::cout << "|	ITEM	|: " << "Obtained: " << this->all_items.at(rNum).name << "\n";
		return this->all_items.at(rNum);
	}
	else {
		if (rarity == "Common") {
			rNum = GetRandomValue(0, this->common_items.size() - 1);
			std::cout << "|	ITEM	|: " << "Obtained: " << this->common_items.at(rNum).name << "\n";
			return this->common_items.at(rNum);
		}
		else if (rarity == "Uncommon") {
			rNum  = GetRandomValue(0, this->uncommon_items.size() - 1);
			std::cout << "|	ITEM	|: " << "Obtained: " << this->uncommon_items.at(rNum).name << "\n";
			return this->uncommon_items.at(rNum);
		}
		else if (rarity == "Rare") {
			rNum =  GetRandomValue(0, this->rare_items.size() - 1);
			std::cout << "|	ITEM	|: " << "Obtained: " << this->rare_items.at(rNum).name << "\n";
			return this->rare_items.at(rNum);
		}
		else if (rarity == "Epic") {
			rNum = GetRandomValue(0, this->epic_items.size() - 1);
			std::cout << "|	ITEM	|: " << "Obtained: " << this->epic_items.at(rNum).name << "\n";
			return this->epic_items.at(rNum);
		}
		else if (rarity == "Legendary") {
			rNum = GetRandomValue(0, this->legendary_items.size() - 1);
			std::cout << "|	ITEM	|: " << "Obtained: " << this->legendary_items.at(rNum).name << "\n";
			return this->legendary_items.at(rNum);
		}
		else {
			std::cout << "|	ITEM	|: " << "Item of such rarity NOT FOUND!\n";
		}
	}
}