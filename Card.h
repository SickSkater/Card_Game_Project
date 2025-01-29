#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "GameArea.h"
#include "CardActions.h"
#include "Actions.h"
#include <nlohmann/json.hpp>

class Player;

enum Rarity {
	COMMON,
	UNCOMMON,
	RARE,
	EPIC,
	LEGENDARY
};

enum Shapes1 {
	SQUARE,
	SHAPE_L,
	SHAPE_LR,
	SHAPE_PNS,
	SHAPE_Z,
	SHAPE_ZR,
	SHAPE_BIGI,
	SHAPE_KRISKROS,
	SHAPE_ZIGZAG

};

struct Modifiers { // offset for cursor interaction, draws shape in middle of cursor
	float modX, modY, modW, modH;
}; 

struct Body {
	Rectangle shapeBody;
	std::vector<Modifiers> mods;
	bool checkCollision(Rectangle rec) {
		Rectangle rem = GetCollisionRec(this->shapeBody, rec);
		if ((rem.width + rem.height) >= 50) return true;
		//Rectangle rem = { rec.x, rec.y, rec.width - 20 , rec.height - 20};
		return false;
	};
	void draw() {
		DrawRectangleRec(shapeBody, BLUE);


	};

	bool isClicked() {
		return CheckCollisionPointRec(GetMousePosition(), shapeBody);
	};

	void move(int state) {
		this->shapeBody.x = GetMousePosition().x + mods[state].modX - mods[state].modW / 2;
		this->shapeBody.y = GetMousePosition().y + mods[state].modY - mods[state].modH / 2;
		this->shapeBody.width = mods[state].modW;
		this->shapeBody.height = mods[state].modH;
	};
	void setPosition(int state, Rectangle collRectBody) {
		this->shapeBody.x = collRectBody.x + mods[state].modX - mods[state].modW / 2 + collRectBody.width / 2;
		this->shapeBody.y = collRectBody.y + mods[state].modY - mods[state].modH / 2 + collRectBody.height / 2;

	}


};

class Card;

class CardShape {
public:
	bool placed = false;
	int state = 0;
	int shape_segment_index;
	float size = 50;
	std::vector<Body> shape_body;
	void createShape(enum Shapes1 shape_type);
	void moveShape();
	bool isClicked();
	bool checkCollision(std::vector <Body> shape);
	void selectShape(Card *card);
	void setShapePosition(Rectangle collRectBody, Segments segBody);
	void draw();


};

class Card
{
public:
	Actions doAction;
	int currently_drawn = 0;
	int max_drawn = 0;
	int dtbID;
	int state = 0;
	int cost;
	bool active;
	bool selected;
	bool paid;
	bool locked = false;
	bool dtbLocked = true;
	int unlocked_count = 0;
	int dtbMaxCount;
	std::string rarity;
	CardActions actions[4];
	std::string name;
	std::string shapeName;
	int segment_index;
	Rectangle card_body;
	CardShape shape;
	GameArea *action_area;


	Card(float x, float y, float width, float height, int cardDataIndexParam);
	void setPosition(Segments* segment);
	void followMouse();
	bool isClicked(Player *player);
	void draw();
	void isPositionEmpty();
	void update(Player *player);
	std::string rangeToString(std::vector<int> range_left, std::vector<int> range_right);
	Color rarityColor(std::string rarity);
	Rarity rarityToEnum(std::string rarity);



};

