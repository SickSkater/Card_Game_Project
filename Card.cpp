#include "Card.h"
#include "Player.h"

using json = nlohmann::json;
std::fstream mainCardsFile("cardDatabase.json");
json cardData = json::parse(mainCardsFile);



enum Shapes1 stringToEnum(std::string str) {
	if (str == "SQR") {
		return SQUARE;
	}
	else if (str == "L") {
		return SHAPE_L;
	}
	else if (str == "LR") {
		return SHAPE_LR;
	}
	else if (str == "PNS") {
		return SHAPE_PNS;
	}
	else if (str == "Z") {
		return SHAPE_Z;
	}
	else if (str == "ZR") {
		return SHAPE_ZR;
	}
	else if (str == "BIGI") {
		return SHAPE_BIGI;
	}
	else if (str == "KRISKROS") {
		return SHAPE_KRISKROS;
	}
	else if (str == "ZIGZAG") {
		return SHAPE_ZIGZAG;
	}
	else {
		std::cout << "ERROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR";
	}
}


Card::Card(float x, float y, float width, float height, int cardDataIndex) { // gets data from json file
	mainCardsFile.close();
	this->cost = cardData.at(cardDataIndex).at("Cost");
	this->name = cardData.at(cardDataIndex).at("Name");
	this->shapeName = cardData.at(cardDataIndex).at("Shape");
	this->dtbLocked = cardData.at(cardDataIndex).at("Locked");
	this->dtbID = cardData.at(cardDataIndex).at("ID");
	this->rarity = cardData.at(cardDataIndex).at("Rarity");
	this->unlocked_count = cardData.at(cardDataIndex).at("Unlocked_Count");
	this->dtbMaxCount = cardData.at(cardDataIndex).at("Count");
	for (int i = 0; i < 4; i++) {
		if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "DMG") {
			this->actions[i].action_type = "DMG";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();

		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "DMG_IGN") {
			this->actions[i].action_type = "DMG_IGN";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "FREEZE") {
			this->actions[i].action_type = "FREEZE";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "POISON") {
			this->actions[i].action_type = "POISON";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "FIRE") {
			this->actions[i].action_type = "FIRE";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "LIFE_STEAL") {
			this->actions[i].action_type = "LIFE_STEAL";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "PARRY") {
			this->actions[i].action_type = "PARRY";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "BLEED") {
			this->actions[i].action_type = "BLEED";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "MANA") {
			this->actions[i].action_type = "MANA";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "HEAL") {
			this->actions[i].action_type = "HEAL";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "ARMOR") {
			this->actions[i].action_type = "ARMOR";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
		}
		else if (cardData.at(cardDataIndex).at("Actions").at(i).at("Type") == "BOOST") {
			this->actions[i].action_type = "BOOST";
			this->actions[i].amount = cardData.at(cardDataIndex).at("Actions").at(i).at("Amount");
			this->actions[i].range_left = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
			this->actions[i].range_right = cardData.at(cardDataIndex).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
		}
		else {
			this->actions[i].action_type = "Empty";

		}
	}
	this->shape.createShape(stringToEnum(cardData.at(cardDataIndex).at("Shape")));
	card_body = { x,y,width,height };
	paid = false;
	active = false;

}

bool Card::isClicked(Player *player) {
	if (!active) { // prevents to create multiple active shapes from one card
		if (CheckCollisionPointRec(GetMousePosition(), card_body) && player->stats.mana >= cost && !paid) { // selects card, pays the price, activates cardshape
			this->selected = true;
			this->paid = true;
			player->stats.mana -= cost;
			return true;
		}
		this->selected = false;
		if (paid) { // refunds cost, deselects card
			this->paid = false;
			player->stats.mana += cost;
		}

		return false;
	}
}

void Card::followMouse() {
	if (selected) {
		shape.moveShape();
	}
}

void Card::setPosition(Segments* segment) { // sets card body to current deck of player
	this->card_body = { segment->body.x + segment->body.width / 2 - this->card_body.width / 2, segment->body.y + segment->body.height / 2 - this->card_body.height / 2, this->card_body.width, this->card_body.height }; // sets card to middle of segment
	segment_index = segment->id;
	segment->isOccupied = true;
}


Rarity Card::rarityToEnum(std::string rarity) {
	if (rarity == "Common") {
		return COMMON;
	}
	else if (rarity == "Uncommon") {
		return UNCOMMON;
	}
	else if (rarity == "Rare") {
		return RARE;
	}
	else if (rarity == "Epic") {
		return EPIC;
	}
	else {
		return LEGENDARY;
	}
}

Color Card::rarityColor(std::string rarity) {
	switch (rarityToEnum(rarity)) {
	case COMMON:
		return GRAY;
		break;
	case UNCOMMON:
		return GREEN;
		break;
	case RARE:
		return BLUE;
		break;
	case EPIC:
		return PURPLE;
		break;
	case LEGENDARY:
		return ORANGE;
		break;

	}
}
void Card::draw() {
	if (active || selected) { // hides card, shows card shape
		shape.draw();
		DrawText(TextFormat("%s, C: %d, S: %s",this->name.c_str(), this->cost, this->shapeName.c_str()), shape.shape_body.at(0).shapeBody.x + 2, shape.shape_body.at(0).shapeBody.y + 2, 6, WHITE);
		for (int i = 0; i < 4; i++) {
			DrawText(TextFormat("R: %d: %s : %d\n%s", i + 1, this->actions[i].action_type.c_str(), this->actions[i].amount, rangeToString(this->actions[i].range_left, this->actions[i].range_right).c_str()), shape.shape_body.at(0).shapeBody.x + 2, shape.shape_body.at(0).shapeBody.y + 15 + 30 * i, 10, WHITE);

		}
	}
	else {
		DrawRectangleLinesEx(card_body, 3, rarityColor(this->rarity));
		DrawRectangleRec(card_body, BROWN);
		

	}
	DrawText(TextFormat("%s, C: %d, S: %s", this->name.c_str(), this->cost, this->shapeName.c_str()), card_body.x + 2, card_body.y+ 2, 6, WHITE);
	for (int i = 0; i < 4; i++) {
		DrawText(TextFormat("R: %d: %s : %d\n%s", i+1, this->actions[i].action_type.c_str(), this->actions[i].amount, rangeToString(this->actions[i].range_left, this->actions[i].range_right).c_str()), card_body.x + 2, card_body.y + 15 + 30 * i, 10, WHITE);

	}

}

std::string Card::rangeToString(std::vector<int> range_left, std::vector<int> range_right) { // visualization of range for actions containing it
	std::string rem = "< < < < < ^ > > > > >";
	for (int i = 0; i < range_left.size(); i++) {
		rem[10 - (range_left.at(i) * 2)] = '*';
	}
	for (int i = 0; i < range_right.size(); i++) {
		rem[10 + (range_right.at(i) * 2)] = '*';
	}
	return rem;
}


void Card::update(Player *player) {

	if (!this->locked) { // prevents interaction with alerady casted cards
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { // input handler 
			if (shape.placed && shape.isClicked()) { // selecting alerady placed shape
				shape.selectShape(this);
				return;
			}
			if (selected && CheckCollisionPointRec(GetMousePosition(), action_area->body)) { // placing active shape on the gameboard
				if (CheckCollisionPointRec(GetMousePosition(), action_area->getHoveredSegment().getSelectedCollisionRectangle())) {
					shape.setShapePosition(action_area->getHoveredSegment().getSelectedCollisionRectangle(), action_area->getHoveredSegment());
					if (shape.placed) {
						this->selected = false;
						this->active = true;
					}
					return;
				}
				else if (CheckCollisionPointRec(GetMousePosition(), action_area->body)) { // handles missclicks within range of playboard, else removes card from selection
					this->selected = true;
					this->active = true;
					return;
				}
			}

			isClicked(player);
		}
	}

}

void CardShape::createShape(enum Shapes1 shape_type) { // creates shape of active card from preset
	Body main_part;
	Body secondary_part;
	Body secondary_part2;
	switch (shape_type) {
	case 0: // SQUARE
		main_part.mods.push_back({-size / 2, -size / 2, size * 2, size * 2});
		shape_body.push_back(main_part);
		break;
	case 1: // L shape

		main_part.mods.push_back({0,0, size, size * 3});
		secondary_part.mods.push_back({size, size, size, size});

		main_part.mods.push_back({ 0,0, size * 3, size});
		secondary_part.mods.push_back({ size, -size, size, size });
		
		main_part.mods.push_back({ 0,0, size, size * 3 });
		secondary_part.mods.push_back({ -size, -size, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ -size, size, size, size });

		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		break;
	case 2: // L REVERSED

		main_part.mods.push_back({ 0,0, size, size * 3 });
		secondary_part.mods.push_back({ -size, size, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ size, size, size, size });

		main_part.mods.push_back({ 0,0, size, size * 3 });
		secondary_part.mods.push_back({ size, -size, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ -size, -size, size, size });
		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		break;
	case 3: // PENIS shape

		main_part.mods.push_back({ 0,0, size * 3, size});
		secondary_part.mods.push_back({ 0, -size, size, size });

		main_part.mods.push_back({ 0,0, size, size * 3});
		secondary_part.mods.push_back({ -size, 0, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ 0, size, size, size });

main_part.mods.push_back({ 0,0, size, size * 3 });
secondary_part.mods.push_back({ size, 0, size, size });

shape_body.push_back(main_part);
shape_body.push_back(secondary_part);
break;
	case 4: // Z shape

		main_part.mods.push_back({ 0,0, size, size * 3 });
		secondary_part.mods.push_back({ -size, -size, size, size });
		secondary_part2.mods.push_back({ size, size, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ -size, size, size, size });
		secondary_part2.mods.push_back({ size, -size, size, size });

		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		shape_body.push_back(secondary_part2);
		break;
	case 5: // Z REVERSED shape

		main_part.mods.push_back({ 0,0, size, size * 3 });
		secondary_part.mods.push_back({ size, -size, size, size });
		secondary_part2.mods.push_back({ -size, size, size, size });

		main_part.mods.push_back({ 0,0, size * 3, size });
		secondary_part.mods.push_back({ -size, -size, size, size });
		secondary_part2.mods.push_back({ size, size, size, size });

		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		shape_body.push_back(secondary_part2);
		break;
	case 6: // BIG I

		main_part.mods.push_back({ 0,0, size, size * 4 });

		main_part.mods.push_back({ 0,0, size * 4, size });

		shape_body.push_back(main_part);
		break;
	case 7: //  shape : /

		main_part.mods.push_back({ 0,0, size, size });
		secondary_part.mods.push_back({ size, -size, size, size });
		secondary_part2.mods.push_back({ -size, size, size, size });

		main_part.mods.push_back({ 0,0, size, size });
		secondary_part.mods.push_back({ -size, -size, size, size });
		secondary_part2.mods.push_back({ size, size, size, size });

		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		shape_body.push_back(secondary_part2);
		break;
	case 8: //  shape : ^ 

		main_part.mods.push_back({ 0,-size, size, size });
		secondary_part.mods.push_back({ -size, 0, size, size });
		secondary_part2.mods.push_back({ size, 0, size, size });

		main_part.mods.push_back({ 0,-size, size, size });
		secondary_part.mods.push_back({ -size, 0, size, size });
		secondary_part2.mods.push_back({ 0, size, size, size });

		main_part.mods.push_back({ 0,size, size, size });
		secondary_part.mods.push_back({ -size, 0, size, size });
		secondary_part2.mods.push_back({ size, 0, size, size });

		main_part.mods.push_back({ 0,-size, size, size });
		secondary_part.mods.push_back({ size, 0, size, size });
		secondary_part2.mods.push_back({ 0, size, size, size });

		shape_body.push_back(main_part);
		shape_body.push_back(secondary_part);
		shape_body.push_back(secondary_part2);
		break;
	}

}

void CardShape::moveShape() { // moves shape to mouse cursor 
	// Changing Rect. body props using modifiers, affecting offset and size
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) { // input handler
		if (state < shape_body.at(0).mods.size() - 1) { // check against overflow, incrementation of state -> change of mods
			this->state++;
		}
		else { // reset to original state
			this->state = 0;
		}
	}

	for (int i = 0; i < shape_body.size(); i++) { // follow mouse, add mod values to original body to change apperance
		shape_body.at(i).move(state);
	}
}

void CardShape::draw() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), shape_body.at(0).shapeBody)) {
		std::cout << "INDEX: " << this->shape_segment_index << "\n";
	}
	for (int i = 0; i < shape_body.size(); i++) {
		shape_body.at(i).draw();
	}
}

void CardShape::setShapePosition(Rectangle rec, Segments segBody) { // checks points inside segments within certain area in segment, offsets the shape to "middle"
	for (int i = 0; i < shape_body.size(); i++) {
		shape_body.at(i).setPosition(state, rec);
	}
	for (int i = 0; i < shape_body.size(); i++) {
		if (GetCollisionRec(segBody.body, shape_body.at(i).shapeBody).width + GetCollisionRec(segBody.body, shape_body.at(i).shapeBody).height < ((shape_body.at(i).shapeBody.width + shape_body.at(i).shapeBody.height) / 100) * 90) { // sets shape near point if shape passes checks
			return;
		}
	}
	shape_segment_index = segBody.id;
	placed = true;

}

bool CardShape::isClicked() { 
	for (int i = 0; i < shape_body.size(); i++) {
		if (shape_body.at(i).isClicked()) {
			//std::cout << "Clicked shape\n";
			return true;
		}
	}
	return false;
} // collision check with mouse

void CardShape::selectShape(Card *card) { // allows to move alerady placed shape
	//std::cout << "Selected shape\n";
	this->placed = false;
	card->selected = true;
	card->active = false;
} 

bool CardShape::checkCollision(std::vector <Body> shape) { // checks collision with other shapes in same segment 
	for (int i = 0; i < this->shape_body.size(); i++) {
		for (int j = 0; j < shape.size(); j++) {
			if (this->shape_body.at(i).checkCollision(shape.at(j).shapeBody)) {
				std::cout << "|	CARD SHAPE	|: "  << "colliding\n";
				return true;
			}
		}
	}
	//std::cout << "allright babe\n";
	return false;
}

