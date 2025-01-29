#include "EncounterRoom.h"
#include "Player.h"

std::string getRandomRarity2() {
	int rNum = GetRandomValue(1, 100);
	if (rNum >= 95) {
		return "Legendary";
	}
	else if (rNum >= 80) {
		return "Epic";

	}
	else if (rNum >= 75) {
		return "Rare";

	}
	else if (rNum >= 60) {
		return "Uncommon";

	}
	else {
		return "Common";

	}
}

void EncounterRoom::looseGold(Player* player, int amount) {
	player->stats.gold -= amount;
	if (player->stats.gold < 0) {
		player->stats.gold = 0;
	}
	std::cout << "LOST " << amount << " of gold!\n";
}

void EncounterRoom::looseHealth(Player* player, int amount) {
	player->stats.health -= amount;
	std::cout << "LOST " << amount << " of health!\n";

}

void EncounterRoom::looseMaxHealth(Player* player, int amount) {
	player->stats.max_health -= amount;
	if (player->stats.health > player->stats.max_health) {
		player->stats.health = player->stats.max_health;
	}
	std::cout << "LOST " << amount << " of maximum health!\n";

}

void EncounterRoom::looseItem(Player* player) { // removes permanent effect from items with PASSIVE condition, removes randomly selected item
	int rNum = GetRandomValue(0, player->currentItems.size() - 1);
	if (player->currentItems.size() != 0) {
		if (player->currentItems.at(rNum).cooldown == "PASSIVE") {
			for (int spellCounter = 0; spellCounter < player->currentItems.at(rNum).actions.size(); spellCounter++) {
				int amount = player->currentItems.at(rNum).actions.at(spellCounter).amount;
				if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Armor") {
					player->stats.starting_armor -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Fire_ressistance") {
					player->stats.fire_res -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Poison_ressistance") {
					player->stats.poison_res -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Bleed_ressistance") {
					player->stats.bleed_res -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Drawn_cards") {
					player->stats.redraw -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Health_max") {
					player->stats.max_health -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Mana_max") {
					player->stats.max_mana -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Mana_recharge") {
					player->stats.manaRecharge -= amount;
				}
				else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Health_recharge") {
					player->stats.healthRecharge -= amount;
				}
			}
		}
		player->currentItems.erase(player->currentItems.begin() + rNum);
	}
}

void EncounterRoom::upgradeItem(Player* player) { // removes permanent effect from items with PASSIVE condition, upgrades to higher rarity randomly selected item
	if (player->currentItems.size() == 0) {
		this->getItem(player, getRandomRarity2());

	}
	else {
		int rNum = GetRandomValue(0, player->currentItems.size() - 1);
		if (player->currentItems.at(rNum).rarity == "Legendary") {
			this->getItem(player, getRandomRarity2());
		}
		else if (player->currentItems.at(rNum).rarity == "Epic") {
			this->getItem(player, "Legendary");
		}
		else if (player->currentItems.at(rNum).rarity == "Rare") {
			this->getItem(player, "Epic");
		}
		else if (player->currentItems.at(rNum).rarity == "Uncommon") {
			this->getItem(player, "Rare");
		}
		else if (player->currentItems.at(rNum).rarity == "Common") {
			this->getItem(player, "Uncommon");
		}
		else {
			std::cout << "Upgrade item issue\n";
		}
		if (player->currentItems.at(rNum).cooldown == "PASSIVE") {
				for (int spellCounter = 0; spellCounter < player->currentItems.at(rNum).actions.size(); spellCounter++) {
					int amount = player->currentItems.at(rNum).actions.at(spellCounter).amount;
					if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Armor") {
						player->stats.starting_armor -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Fire_ressistance") {
						player->stats.fire_res -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Poison_ressistance") {
						player->stats.poison_res -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Bleed_ressistance") {
						player->stats.bleed_res -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Drawn_cards") {
						player->stats.redraw -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Health_max") {
						player->stats.max_health -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Mana_max") {
						player->stats.max_mana -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Mana_recharge") {
						player->stats.manaRecharge -= amount;
					}
					else if (player->currentItems.at(rNum).actions.at(spellCounter).type == "Health_recharge") {
						player->stats.healthRecharge -= amount;
					}
				}
			}
		player->currentItems.erase(player->currentItems.begin() + rNum);

	}

}



void EncounterRoom::getRandomGood(Player* player, int count ) { // get x functions with positive outcome
	for (int i = 0; i < count; i++) {
		switch (GetRandomValue(0, 6)) {
		case 0:
			this->getGold(player, GetRandomValue(1, 100));
			break;
		case 1:
			this->getMaxHealth(player, GetRandomValue(5, 20));
			break;
		case 2:
			this->heal(player, GetRandomValue(10, 50));
			break;
		case 3:
			this->upgradeItem(player);
			break;
		case 4:
			this->getCard(player, getRandomRarity2());
			break;
		case 5:
			this->getItem(player, getRandomRarity2());
			break;
		case 6:
			i--;
			break;
		}

	}
}

void EncounterRoom::getRandomBad(Player* player, int count) { // get x functions with negative outcome
	for (int i = 0; i < count; i++) {
		switch (GetRandomValue(0, 4)) {
		case 0:
			this->looseGold(player, GetRandomValue(1, 100));
			break;
		case 1:
			this->looseHealth(player, GetRandomValue(5, 10));
			break;
		case 2:
			this->looseMaxHealth(player, GetRandomValue(5, 10));
			break;
		case 3:
			this->looseItem(player);
			break;
		case 4:
			i--;
			break;
		}

	}
}

void EncounterRoom::getResult(Player *player) { // triggers P/N functions by random result
	int type = GetRandomValue(0, 14);
	if (type == 0) { // good
		this->getRandomGood(player, 3);
	}
	else if (type >= 1 && type <= 3) { // neutral
		this->getRandomGood(player, 1);
	}
	else if (type >= 4 && type <= 9) { // none
		std::cout << "NOTHING HAPPENED!\n";
	}
	else if (type >= 10 && type <= 13) { // trade off
		this->getRandomGood(player, 1);
		this->getRandomBad(player, GetRandomValue(1, 2));
	}
	else { // negativ
		this->getRandomBad(player, 2);
		this->fight = true;
	}
}