#include "Player.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;
std::ifstream mainCardsFile4("cardDatabase.json");
json cardDTB = json::parse(mainCardsFile4);

Player::Player() { 
	Item item;
	this->allItems = item.createAllItems();
	stats.armor = 0;
	stats.gold = 10;
	stats.health = 20;
	stats.max_health = stats.health;
	stats.mana = 10;
	stats.max_mana = stats.mana;
}

void Player::drawCard(int count) { // get card from unlocked deck, erase it from draw deck
	int rNum;
	for (int i = 0; i < count; i++) {
		if (current_deck.card_deck.size() < 10) {
			rNum = GetRandomValue(0, draw_deck.card_deck.size() - 1);
			std::cout << "|	PLAYER	|: "  << "Drawn card: " << draw_deck.card_deck.at(rNum).name << "\n";
			current_deck.card_deck.push_back(draw_deck.card_deck.at(rNum));
			discarded_deck.card_deck.push_back(draw_deck.card_deck.at(rNum));
			draw_deck.card_deck.erase(draw_deck.card_deck.begin() + rNum);
		}
		else {
			std::cout << "|	PLAYER	|: "  << "CURRENT_DECK LIMIT HAS BEEN REACHED!\n";
		}

		if (draw_deck.card_deck.size() <= 0) {
			draw_deck.card_deck.clear();
			draw_deck.card_deck = discarded_deck.card_deck;
			discarded_deck.card_deck.clear();
			std::cout << "|	PLAYER	|: "  << "RESHUFFLING DISCARDED DECK!\n";
		}



	}
}

void Player::draw() {
	for (int i = 0; i < this->stats.bleed_stacks; i++) { // bleed visual
		DrawRectangle(50 + 6*i, 545, 5,5, RED);
	}
	for (int i = 0; i < this->stats.poison_stacks; i++) { // poison visual
		DrawRectangle(50 + 6 * i, 539, 5, 5, GREEN);
	}
	for (int i = 0; i < this->stats.fire_stacks; i++) { // fire visual
		DrawRectangle(50 + 6 * i, 533, 5, 5, ORANGE);
	}
	if (this->stats.parry) { // parry visual
		DrawRectangle(10, 518, 50, 20, BLUE);
	}
	// player info
	DrawText(TextFormat("Gold: %d\n\nHealth %d / %d (+%d)\n\nMana: %d / %d (+%d)\n\nArmor: %d", stats.gold, stats.health, stats.max_health, this->stats.healthRecharge, stats.mana, stats.max_mana, this->stats.manaRecharge, stats.armor), 50, 550, 30, WHITE);
}

void Player::unlockNewCard(float chance) { // checks deck of locked cards for certain rarity, gets back such card, unlocks it forever or adds availible count for it
	std::string currentRarityLevel;
	int upgradeCheck = GetRandomValue(chance, 100);
	if (upgradeCheck >= 95) { // legendary 5%
		checkNewCardDeck("Legendary");
	}
	else if (upgradeCheck >= 85) { // epic 15%
		checkNewCardDeck("Epic");
	}
	else if (upgradeCheck >= 70) { // rare 30%
		checkNewCardDeck("Rare");
	}
	else if (upgradeCheck >= 60) { // uncommon 40%
		checkNewCardDeck("Uncommon");
	}
	else { // common %60
		checkNewCardDeck("Common");
	}
}

int Player::checkNewCardDeck(std::string rarity) { // gets vector of indexes of certain rarity 
	std::vector <int> cardIndexes;
	for (int i = 0; i < this->locked_deck.card_deck.size(); i++) {
		if (this->locked_deck.card_deck.at(i).rarity == rarity && this->locked_deck.card_deck.at(i).dtbLocked) {
			std::cout << "|	GAME	|: "  << this->locked_deck.card_deck.at(i).rarity << " " << this->locked_deck.card_deck.at(i).name << " " << this->locked_deck.card_deck.at(i).dtbLocked << "\n";
			cardIndexes.push_back(this->locked_deck.card_deck.at(i).dtbID);
		}
			//std::cout << this->locked_deck.card_deck.at(i).rarity << " " << this->locked_deck.card_deck.at(i).name << " " << this->locked_deck.card_deck.at(i).dtbLocked << "\n";

		
	}
	if (cardIndexes.size() > 0) {
		int rem = GetRandomValue(0, cardIndexes.size() - 1);
		return rem; // index of new card to be picked
	}
	else {
		return -1; // try to draw new rarity
	}
}

void Player::getNewCard(int index) { // adds new card to avilabe deck
	if (index == -1) {
		std::cout << "|	GAME	|: "  << "NEW CARD TO UNLOCK COULDNT BE FOUND\n"; // later on create system to get card for inventory, instead of error;
		return;
	}
	for (int i = 0; i < cardDTB.size(); i++) {
		if (cardDTB.at(i).at("Name") == this->locked_deck.card_deck.at(index).name) {
			std::cout << "|	GAME	|: "  << "Found card in database " << this->locked_deck.card_deck.at(index).name << "\n";
			std::cout << "|	GAME	|: "  << "before: " << cardDTB.at(i).at("Unlocked_Count") << "\n";
			cardDTB.at(i)["Unlocked_Count"] = (int)((int)this->locked_deck.card_deck.at(i).unlocked_count + 1);
			std::cout << "|	GAME	|: "  << "after: " << cardDTB.at(i).at("Unlocked_Count") << "\n";
			//this->locked_deck.card_deck.at(index).unlocked_count++;
			if (this->locked_deck.card_deck.at(index).unlocked_count >= this->locked_deck.card_deck.at(index).dtbMaxCount) {
				this->locked_deck.card_deck.at(index).dtbLocked = false;
				std::cout << "|	GAME	|: "  << "IS NOW UNLOCKED\n";
				cardDTB.at(i).at("Locked") = false; 
			}
			else {
				std::cout << "|	GAME	|: "  << "Increasing stats of: " << this->locked_deck.card_deck.at(index).name << "\n";
			}
			break;
		}
	}
	mainCardsFile4.close();
	std::ofstream modifiedDTB("cardDatabase.json", std::ofstream::trunc);

	modifiedDTB << cardDTB.dump(4);
	modifiedDTB.close();

}

void Player::drawInfo() {
		DrawText(TextFormat("Health: %d / %d | Recharge: %d", this->stats.health, this->stats.max_health, this->stats.healthRecharge), 10, 20, 40, BLACK);
		DrawText(TextFormat("MANA: %d / %d | Recharge: %d", this->stats.mana, this->stats.max_mana, this->stats.manaRecharge), 10, 60, 40, BLACK);
		DrawText(TextFormat("Gold: %d", this->stats.gold), 10, 100, 40, BLACK);


	
}

void Player::discardCard(Card *card, int index) { // removes card from deck, adds it to discarded deck
	card->active = false;
	card->paid = false;
	card->locked = false;
	card->shape.placed = false;
	card->selected = false;
	this->discarded_deck.card_deck.push_back(*card);
	this->current_deck.card_deck.erase(this->current_deck.card_deck.begin() + index);
}