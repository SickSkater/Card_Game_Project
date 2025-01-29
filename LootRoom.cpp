#include "LootRoom.h"
#include "CardDeck.h"
#include "Player.h"

TypeOfReward intToEnum(int x) {
	switch (x) {
	case 0:
		return MONEY;
		break;
	case 1:
		return CARD;
		break;
	case 2:
		return ITEM;
		break;
	case 3:
		return HEALTH;
		break;
	}
}

std::string enumToString(TypeOfReward x) {
	switch (x) {
	case 0:
		return "MONEY";
		break;
	case 1:
		return "CARD";
		break;
	case 2:
		return "ITEM";
		break;
	case 3:
		return "HEALTH";
		break;
	}
}

void LootRoom::createRoom() {
	for (int i = 0; i < 4; i++) {
		float rem = 500 + (250 * i);
		this->options.push_back({ item, 5, {rem,500, 100, 200}, intToEnum(i) });
	}
}

void Reward::getCard(Player *player, std::string rarity) {
	if (rarity == "Common") {
		player->getNewCard(player->checkNewCardDeck("Common"));

	}
	else if (rarity == "Uncommon") {
		player->getNewCard(player->checkNewCardDeck("Uncommon"));

	}
	else if (rarity == "Rare") {
		player->getNewCard(player->checkNewCardDeck("Rare"));

	}
	else if (rarity == "Epic") {
		player->getNewCard(player->checkNewCardDeck("Epic"));

	}
	else if (rarity == "Legendary") {
		player->getNewCard(player->checkNewCardDeck("Legendary"));

	}
	else {
		std::cout << "GETCARD ERROR! RARITY NOT FOUND\n";
	}
}

void Reward::getGold(Player *player, int amount) {
	player->stats.gold += amount;
	std::cout << "RECIVED " << amount << " of gold!\n";

}

void Reward::getItem(Player *player, std::string rarity) {
	if (rarity == "Legendary") {
		player->currentItems.push_back(item.getItem("Legendary"));
	}
	else if (rarity == "Epic") {
		player->currentItems.push_back(item.getItem("Epic"));
	}
	else if (rarity == "Rare") {
		player->currentItems.push_back(item.getItem("Rare"));
	}
	else if (rarity == "Uncommon") {
		player->currentItems.push_back(item.getItem("Uncommon"));
	}
	else {
		player->currentItems.push_back(item.getItem("Common"));
	}

}

void Reward::getMaxHealth(Player* player, int amount) {
	player->stats.max_health += amount;
	player->stats.health += amount;
	std::cout << "RECIVED " << amount << " of maximum health!\n";

}

void Reward::heal(Player* player, int amount) {
	player->stats.health += amount;
	if (player->stats.health > player->stats.max_health) {
		player->stats.health = player->stats.max_health;
	}
	std::cout << "HEALED " << amount << " HEALTH!\n";

}

std::string getRandomRarity() {
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

void LootRoom::update(Player *player) { // if reward is choosen, does action, exits to map
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (int i = 0; i < options.size(); i++) {
			if (options.at(i).isClicked()) {
				// CHECK NA KLIK, DOSTANI ODMĚNY
				switch (options.at(i).type) {
				case MONEY:
					options.at(i).getGold(player, GetRandomValue(1,99));
					std::cout << "|	LOOT ROOM	|: " << "Picked: MONEY \n";
					break;
				case CARD:
					options.at(i).getCard(player, getRandomRarity());
					std::cout << "|	LOOT ROOM	|: " << "Picked: CARD \n";
					break;
				case ITEM:
					options.at(i).getItem(player, getRandomRarity());
					std::cout << "|	LOOT ROOM	|: " << "Picked: ITEM \n";
					break;
				case HEALTH:
					options.at(i).heal(player, GetRandomValue(1, 100));
					std::cout << "|	LOOT ROOM	|: " << "Picked: HEALTH \n";

					break;
				}
				active = false;


			}
		}
	}
}


void LootRoom::draw(Player* player) {
	ClearBackground(GRAY);
	player->drawInfo();
	for (Reward opt : this->options) {
		DrawRectangleRec(opt.body, ORANGE);
		DrawText(TextFormat("%s", enumToString(opt.type).c_str()), opt.body.x + 25, opt.body.y + 20, 40, BLACK );
	}
}
