#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>


class Card;
struct Button2
{
	Rectangle body = { 1000, 800, 400, 200 };
	void setStats(float x, float y, float width, float height) {
		this->body = { x,y, width, height };
	}
	void draw(std::string text, Color color) {

		DrawRectangleRec(body, color);
		DrawRectangleLinesEx(body, 2, BLACK);
		if (!text.empty()) {
			for (int i = 0; i < text.length(); i++) {
				DrawText(TextFormat("%c", text.at(i)), body.x + body.width / 2 - 10, body.y + body.height / 6 + (body.height / 6) * i, 50, BLACK);
			}
		}
	}
	bool isClicked() {
		return (CheckCollisionPointRec(GetMousePosition(), body) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
	};
};

struct DeckGrid {
	Rectangle body;
	bool occupied;
	int index;
};

class CardWindow {
public:
	Card *card;
	int maxCount;
	int currentCount;
	Rectangle body;
	int index;
	CardWindow(Card *cardp, int count, Rectangle seg);
	~CardWindow();
	bool isClicked() {
		return CheckCollisionPointRec(GetMousePosition(), body);
	};
	void draw();

};

class CardWindowActive {
public:
	CardWindow* origin;
	Card* card;
	Rectangle body = {0,0, 150, 110};
	int count;
	int index;
	bool isClicked() {
		return CheckCollisionPointRec(GetMousePosition(), body);

	};
	void setPosition(Rectangle seg ){
		body = { seg.x +10, seg.y + 10, body.width, body.height };
	};
	void remove() {
		if (isClicked() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && count > 0) {
			count--;
			origin->currentCount++;
		}
	};
	void draw(Card* card);
	CardWindowActive (CardWindow* CW, Rectangle rec, int i) {
		origin = CW;
		card = CW->card;
		count = 1;
		index = i;
		setPosition(rec);
	}
};



class CardCollection
{
public:
	int activeCards = 0;
	CardCollection();
	std::vector<Rectangle> grid;
	std::vector<CardWindow> cards;
	std::vector<DeckGrid> deckGrid;
	std::vector<CardWindowActive> deck;
	std::vector <int> occupiedGridIndex;
	Button2 btn;
	Button2 nextPage;
	Button2 previousPage;
	bool end = false;
	std::vector<Card> cardDeck;


	void draw();
	void drawCardInfo(Rectangle rec, Card* card, int count);
	void addToCollection(Card *card, int count);
	void addToDeck(CardWindow* cardWindow);
	void createPreset();
	void removeFromDeck();
	void endSelection();
	std::vector<Card> getDeck();
};

