#include "ShopRoom.h"
#include "Player.h";
#include "CardDeck.h"

std::string getRandomRarity(int cost) {
	if (cost >= 95) {
		return "Legendary";
	}
	else if (cost >= 75) {
		return "Epic";

	}
	else if (cost >= 60) {
		return "Rare";

	}
	else if (cost >= 40) {
		return "Uncommon";

	}
	else {
		return "Common";

	}
}

void ShopRoom::createShopRoom() {
	this->items.clear(); // clear previous items
	for (int i = 0; i < GetRandomValue(3, 6); i++) { // generates 3-6 items to be displayed and created
		this->items.push_back(ShopItems(i));

	}
}

void ShopRoom::draw(Player* player) {
	player->drawInfo();
	for (int i = 0; i < items.size(); i++) {
		DrawRectangleRec(btn.body, ORANGE);
		if (items.at(i).active) {
			items.at(i).draw();
			if (items.at(i).type != ITEM || items.at(i).type != CARD) {
				DrawText(TextFormat("Item: %s, price: %d\n\nAmount: %d", items.at(i).typeString.c_str(), items.at(i).cost, items.at(i).cost / 2), items.at(i).body.x, items.at(i).body.y, 20, BLACK);

			}
			else {
				DrawText(TextFormat("Item: %s, price: %d\n\Rarity: %s", items.at(i).typeString.c_str(), items.at(i).cost, getRandomRarity(items.at(i).cost).c_str()), items.at(i).body.x, items.at(i).body.y, 20, BLACK);
			}

		}
		else {
			DrawRectangleLinesEx(items.at(i).body, 3, BLACK);
			DrawText(TextFormat("Sold out"), items.at(i).body.x, items.at(i).body.y, 20, BLACK);

		}
	}
}

void ShopRoom::update(Player* player, CardDeck* deck) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		active = btn.isClicked(); // check to exit shop, shows map
		doAction(player, deck);
	}
}


void ShopRoom::doAction(Player* player, CardDeck* deck) { // pays for item, does action, sets itself as sold out
	for (int i = 0; i < this->items.size(); i++) {
		if (this->items.at(i).isClicked() && player->stats.gold >= this->items.at(i).cost) {
			player->stats.gold -= this->items.at(i).cost;
			switch (items.at(i).type) {
			case HEALTH:
				heal(player, items.at(i).cost / 2);
				break;
			case MAXHEALTH:
				getMaxHealth(player, items.at(i).cost / 3);
				break;
			case CARD:
				
				getCard(player, getRandomRarity(items.at(i).cost));
				break;
			case ITEM:
				getItem(player, getRandomRarity(items.at(i).cost));
				break;
			
			}

			items.at(i).active = false;
		}
	}
}