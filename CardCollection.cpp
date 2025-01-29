#include "CardCollection.h"
#include "Card.h";
using json = nlohmann::json;
std::fstream currentDeckFile1("currentDeckDatabase.json");
json cardDeckData1 = json::parse(currentDeckFile1);

CardWindow::CardWindow(Card* cardp, int count, Rectangle seg) {
	this->card = cardp;
	maxCount = count;
	currentCount = count;
	body = { seg.x + 5,seg.y + 10,150,110 };
}

CardWindow::~CardWindow() {};


void CardWindow::draw() {
	// currentcount -> currently nonselected cards

	for (int i = maxCount; i > 0; i--) {
		if (card->unlocked_count <= maxCount - currentCount) {
			if (i > currentCount) { // draws remenants of picked cards
				DrawRectangleLines(body.x + i * 3, body.y + i * 3, body.width, body.height, BLACK);
			}
			else { // draws locked cards
				DrawRectangle(body.x + i * 3, body.y + i * 3, body.width, body.height, RED);
				DrawRectangleLines(body.x + i * 3, body.y + i * 3, body.width, body.height, card->rarityColor(card->rarity));
			}
		}
		else {
			if (i > currentCount) { // remenants
				DrawRectangleLines(body.x + i * 3, body.y + i * 3, body.width, body.height, BLACK);
			}
			else { // partly or fully unlocked cards, ready to be choosen
				DrawRectangle(body.x + i * 3, body.y + i * 3, body.width, body.height, ORANGE);
				DrawRectangleLines(body.x + i * 3, body.y + i * 3, body.width, body.height, card->rarityColor(card->rarity));
			}
		}

	}
}
float posX, posY;
int offset = 0;
CardCollection::CardCollection() {
	previousPage.setStats(910, 800, 50, 70);
	nextPage.setStats(968, 800, 50, 70);
	btn.setStats(912.5, 400, 100, 200);
	int rem = 0;
	for (int i = 0; i < 120; i++) { // creates grid for card selection
		offset++;
		rem += i % 5 == 0 && i != 0 ? 1 : 0;
		if (i % 30 == 0) {
			offset = 0;
			rem = 0;
		}
		posX = (25 + 175 * (offset - 5 * rem));
		posY = (50 + 150 * rem);
		grid.push_back({ posX, posY, 175, 150 });
	}
	rem = 0;
	offset = 0;
	for (int i = 0; i < 120; i++) { // creates grid for selected cards
		offset++;
		rem += i % 5 == 0 && i != 0 ? 1 : 0;
		if (i % 30 == 0) {
			offset = 0;
		}
		posX = (1025 + 175 * (offset - 5 * rem));
		posY = (50 + 150 * rem);
		this->deckGrid.push_back({ {posX, posY, 175, 150 }, false, i });
	}
}
int rem2 = 0;
void CardCollection::createPreset() { // creates and updates json by removing or adding picked cards
	for (int i = 0; i < cardDeckData1.size(); i++) {
		for (int j = 0; j < cards.size(); j++) {
			if (cards.at(j).card->name == cardDeckData1.at(i).at("Name")) {
				if (cardDeckData1.at(i).at("Count") == 0) {
					cardDeckData1.erase(i);
					i--;
				}
				else {
					for (int k = 0; k < cardDeckData1.at(i).at("Count"); k++) {
						addToDeck(&cards.at(j));
					}
				}

			}
		}
	}
	currentDeckFile1 << std::setw(4) << cardDeckData1 << std::endl; // file rewriting
}
void CardCollection::addToCollection(Card *card, int count) { // fills this collection with cards from player / game
	this->cards.push_back(CardWindow(card, count, grid.at(rem2)));
	this->cards.at(rem2).index = rem2;
	rem2++;
}

void CardWindowActive::draw(Card* card) {
		for (int i = 0; i < count; i++) {
			DrawRectangle(body.x + i * 3, body.y + i * 3, body.width, body.height, BROWN);
			DrawRectangleLinesEx({ body.x + i * 3, body.y + i * 3, body.width, body.height }, 2, card->rarityColor(card->rarity));
		}	
}

int pageCounter = 0;
void CardCollection::draw() { // shows cards based on page number
	btn.draw("Save", RAYWHITE);
	nextPage.draw(">", RAYWHITE);
	previousPage.draw("<", RAYWHITE);
	DrawText("Availabe Cards", 100, 5, 40, BLACK);
	DrawText(TextFormat("Player cards: %d / 30", activeCards), 1050, 5, 40, BLACK);
	for (int i = 0 + (30 * pageCounter); i < 30 + (30 * pageCounter); i++) {
		DrawRectangleLinesEx(grid.at(i), 2, BLACK);
		if (i < cards.size()) {
			cards.at(i).draw();
			drawCardInfo(grid.at(i), cards.at(i).card, cards.at(i).maxCount);
			if (cards.at(i).isClicked() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cards.at(i).currentCount > 0 && this->activeCards < 30) {
					addToDeck(&cards.at(i));

				
			}
		}
	}
	for (int i = 0; i < 30; i++) {
		DrawRectangleLinesEx(deckGrid.at(i).body, 2, BLACK);

	}
	for (int j = 0; j < this->deck.size(); j++) {
			this->deck.at(j).draw(this->deck.at(j).card);
			this->drawCardInfo(this->deck.at(j).body, this->deck.at(j).card, this->deck.at(j).count);
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { // should be in update func but i got no time
		this->removeFromDeck();
	}
	if (nextPage.isClicked()) {
		if (pageCounter == 0) {
			pageCounter++;
		}
	}
	if (previousPage.isClicked()) {
		if (pageCounter == 1) {
			pageCounter--;
		}
	}
	endSelection();
}

void CardCollection::drawCardInfo(Rectangle rec, Card* card, int count) {
	if (card->dtbLocked) {
		DrawText(TextFormat("%s", card->name.c_str()), rec.x + 20 + 3 * (count - 1), rec.y + 10 + 3 * (count - 1), 2, BLACK);
		DrawText(TextFormat("%s : %d", card->shapeName.c_str(), card->cost), rec.x + 10 + 3 * (count - 1), rec.y + 20 + 3 * (count - 1), 2, BLACK);

		for (int i = 0; i < 4; i++) {
			if (card->actions[i].range_left.size() < 0 || card->actions[i].action_type == "empty") {
				DrawText(TextFormat("%s: %d", card->actions[i].action_type.c_str(), card->actions[i].amount), rec.x + 20 + 3 * (count - 1), rec.y + 30 + 20 * i + 3 * (count - 1), 2, BLACK);

			}
			else {
				DrawText(TextFormat("%s: %d", card->actions[i].action_type.c_str(), card->actions[i].amount), rec.x + 20 + 3 * (count - 1), rec.y + 30 + 20 * i + 3 * (count - 1), 2, BLACK);
				DrawText(TextFormat("%s", card->rangeToString(card->actions[i].range_left, card->actions[i].range_right).c_str()), rec.x + 10 + 3 * (count - 1), rec.y + 40 + 20 * i + 3 * (count - 1), 2, BLACK);

			}
		}
	}
	else {
		DrawText(TextFormat("%s", card->name.c_str()), rec.x + 20 + 3 * (count - 1), rec.y + 10 + 3 * (count - 1), 2, BLACK);
		DrawText(TextFormat("%s : %d", card->shapeName.c_str(), card->cost), rec.x + 10 + 3 * (count - 1), rec.y + 20 + 3 * (count - 1), 2, BLACK);

		for (int i = 0; i < 4; i++) {
			if (card->actions[i].range_left.size() < 0 || card->actions[i].action_type == "empty") {
				DrawText(TextFormat("%s: %d", card->actions[i].action_type.c_str(), card->actions[i].amount), rec.x + 20 + 3 * (count - 1), rec.y + 30 + 20 * i + 3 * (count - 1), 2, BLACK);

			}
			else {
				DrawText(TextFormat("%s: %d", card->actions[i].action_type.c_str(), card->actions[i].amount), rec.x + 20 + 3 * (count - 1), rec.y + 30 + 20 * i + 3 * (count - 1), 2, BLACK);
				DrawText(TextFormat("%s", card->rangeToString(card->actions[i].range_left, card->actions[i].range_right).c_str()), rec.x + 10 + 3 * (count - 1), rec.y + 40 + 20 * i + 3 * (count - 1), 2, BLACK);

			}
		}
	}

}

void CardCollection::addToDeck(CardWindow *CW) { // adds selected card to player current deck, makes this card appear in game, saves it in preset
	if (CW->card->unlocked_count > CW->maxCount - CW->currentCount) {
		CW->currentCount--;
		for (int i = 0; i < deckGrid.size(); i++) {
			if (this->deckGrid.at(i).occupied) {
				for (int j = 0; j < this->deck.size(); j++) {
					if (this->deck.at(j).origin->card == CW->card) {
						this->deck.at(j).count++;
						activeCards++;
						return;
					}
				}
			}
			else if (!this->deckGrid.at(i).occupied) {
				occupiedGridIndex.push_back(i);
				this->deck.push_back(CardWindowActive(CW, deckGrid.at(i).body, i));
				deckGrid.at(i).occupied = true;
				activeCards++;
				return;
			}
		}
	}
	else {
		std::cout << "CARD LOCKED!\n";
	}
	
}

void CardCollection::removeFromDeck() { // same as above, but removes alerady selected ones
	for (int i = 0; i < this->deck.size(); i++) {
		if (deck.at(i).isClicked()) {
			deck.at(i).count--;
			this->activeCards--;
			deck.at(i).origin->currentCount++;
			if (deck.at(i).count <= 0) {
				this->deckGrid.at(deck.at(i).index).occupied = false;
				occupiedGridIndex.erase(std::remove(occupiedGridIndex.begin(), occupiedGridIndex.end(), i), occupiedGridIndex.end());
				break;
			}
		}
	}
	for (auto it = deck.begin(); it != deck.end();) {
		if (it->count <= 0) {
			it = deck.erase(it);
		}
		else {
			it++;
		}
	}

}

void to_json(const Card& card, int count) { // save preset to json
	std::ofstream modifiedFile("currentDeckDatabase.json");
	for (int i = 0; i < cardDeckData1.size(); i++) {
		if (card.name == cardDeckData1.at(i).at("Name")) {
			cardDeckData1.at(i).at("Count") = count;
			modifiedFile << std::setw(4) << cardDeckData1 << std::endl;
			modifiedFile.close();
			return;
		}

	}
	std::string rem = card.name;
	cardDeckData1.push_back({{"Name", rem}, {"Count", count}});
	modifiedFile << std::setw(4) << cardDeckData1 << std::endl;
	modifiedFile.close();

}

std::vector<Card> CardCollection::getDeck() { // returns deck of selected cards


	for (int i = 0; i < deck.size(); i++) {
		//std::cout << deck.at(i).origin->card->name << " - INDEX: " << deck.at(i).index << " Count: " << deck.at(i).count << "\n";
		to_json(*deck.at(i).origin->card, deck.at(i).count);
		for (int j = 0; j < this->deck.at(i).count; j++) {
			cardDeck.push_back(*deck.at(i).card);
		}
	}
	return cardDeck;
}

void CardCollection::endSelection() { // quits selection, shows map
	if (btn.isClicked()) {
		if (this->activeCards >= 10) {
			
			this->end = true;

		}
	}
}