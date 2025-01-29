#include "CardDeck.h"

#include "Player.h"
void CardDeck::createCard(int cardDataIndexParam) {
	Card card(0, 0, 125, 150, cardDataIndexParam);
	this->card_deck.push_back(card);
}

void CardDeck::setCardPosition(std::vector <Segments>* segments, GameArea* area) {
	for (int i = 0; i < this->card_deck.size(); i++) {
		if (segments->size() >= i) {
			this->card_deck.at(i).action_area = area;
			this->card_deck.at(i).setPosition(&segments->at(i));

		}
	}
}

void CardDeck::draw(int round) {
	for (int i = 0; i < card_deck.size(); i++) {
		card_deck.at(i).draw();
	}
}

std::vector<Card> CardDeck::getActiveCards() { // returns vector of active cards
	std::vector<Card> rem;
	for (int i = 0; i < this->card_deck.size(); i++) { 
		if (this->card_deck.at(i).active) {
			this->card_deck.at(i).locked = true;
			rem.push_back(card_deck.at(i));
		}
	}
	return rem;
}

void CardDeck::update(Player *player) {
	for (int i = 0; i < card_deck.size(); i++) {
		/*if (CheckCollisionPointRec(GetMousePosition(), this->card_deck.at(i).card_body)) {
			std::cout << this->card_deck.at(i).name << "\n";
			std::cout << this->card_deck.at(i).active << "\n";
			std::cout << this->card_deck.at(i).cost << "\n";
			std::cout << this->card_deck.at(i).unlocked_count << "\n";
			std::cout << this->card_deck.at(i).card_body.x << " / " << this->card_deck.at(i).card_body.y << "\n";



		}*/
		card_deck.at(i).followMouse();
		if (card_deck.at(i).selected) { // prevents shape from being placed on alerady placed shape
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				for (int j = 0; j < card_deck.size(); j++) {
					if ((card_deck.at(j).shape.placed && card_deck.at(i).shape.checkCollision(card_deck.at(j).shape.shape_body)) || !card_deck.at(i).paid) { // selects card if ok 
						card_deck.at(i).active = true;
						card_deck.at(i).shape.placed = false;
						card_deck.at(i).selected = true;
						return;
					}
				}

			}
		}
		card_deck.at(i).update(player);		
	}
}