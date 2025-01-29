#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "Card.h"

class Player;

class CardDeck
{
public:
	std::vector <Card> card_deck;

	void draw(int round);
	void update(Player *player);
	std::vector<Card> getActiveCards();
	void createCard(int cardDataIndexParam);
	void setCardPosition(std::vector <Segments>* segments, GameArea* area);


};

