#include "Enemy.h"

using json = nlohmann::json;
std::ifstream myFile("enemyDatabase.json");
json data = json::parse(myFile);

 enum ActionType {
	ARMOR,
	DAMAGE,
	EMPTY,
	DAMAGE_IGNORE_ARMOR,
	HEAL,
	LIFE_STEAL,
	FIRE,
	POISON,
	BLEED,
	PARRY,
	BOOST,
	SNEAK
};

NLOHMANN_JSON_SERIALIZE_ENUM(ActionType, {
	{ARMOR, "ARMOR"},
	{DAMAGE, "DAMAGE"},
	{EMPTY, "EMPTY"},
	{DAMAGE_IGNORE_ARMOR, "DAMAGE_IGNORE_ARMOR"},
	{HEAL, "HEAL"},
	{LIFE_STEAL, "LIFE_STEAL"},
	{FIRE, "FIRE"},
	{POISON, "POISON"},
	{BLEED, "BLEED"},
	{PARRY, "PARRY"},
	{BOOST, "BOOST"},
	{SNEAK, "SNEAK"}
	})




Enemy::Enemy(int dataID) { // creates enemy from json database
	if (dataID != NULL) {
		stats.health = data.at(dataID).at("Health");
		stats.max_health = this->stats.health + 5;
		stats.name = data.at(dataID).at("Name");
		stats.rarity = data.at(dataID).at("Rarity");
		if (data.at(dataID).contains("Reward")) {
			stats.reward = data.at(dataID).at("Reward");
		}
		if (data.at(dataID).contains("Armor")) {
			this->stats.armor = data.at(dataID).at("Armor");
		}
		if (data.at(dataID).contains("Right")) {
			this->right = true;
		}
		if (data.at(dataID).contains("Survive")) {
			if (data.at(dataID).at("Survive").contains("Sacrifice")) {
				survive.sacrifice = true;
			}
			switch (data.at(dataID).at("Survive").at("Type").get <ActionType>()) {
			case ARMOR:
				survive.type = "arm";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				survive.range_left = data.at(dataID).at("Survive").at("Range_left").get <std::vector<int>>();
				survive.range_right = data.at(dataID).at("Survive").at("Range_right").get <std::vector<int>>();
				break;
			case DAMAGE:
				survive.type = "dmg";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case DAMAGE_IGNORE_ARMOR:
				survive.type = "dmg_ign";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				survive.ignore_armor = true;
				break;
			case HEAL:
				survive.type = "heal";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				survive.range_left = data.at(dataID).at("Survive").at("Range_left").get <std::vector<int>>();
				survive.range_right = data.at(dataID).at("Survive").at("Range_right").get <std::vector<int>>();
				break;
			case LIFE_STEAL:
				survive.type = "life_steal";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case PARRY:
				survive.type = "parry";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case POISON:
				survive.type = "poison";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case BOOST:
				survive.type = "boost";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				survive.range_left = data.at(dataID).at("Survive").at("Range_left").get <std::vector<int>>();
				survive.range_right = data.at(dataID).at("Survive").at("Range_right").get <std::vector<int>>();
				break;
			case BLEED:
				survive.type = "bleed";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case FIRE:
				survive.type = "fire";
				survive.amount = data.at(dataID).at("Survive").at("Amount");
				break;
			case SNEAK:
				survive.type = "sneak";
				break;
			case EMPTY:
				survive.type = "empty";
				break;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (data.at(dataID).at("Actions").at(i).contains("Sacrifice")) {
				actions[i].sacrifice = true;
			}
			switch (data.at(dataID).at("Actions").at(i).at("Type").get <ActionType>()) {
			case ARMOR:
				actions[i].type = "arm";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				actions[i].range_left = data.at(dataID).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
				actions[i].range_right = data.at(dataID).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
				break;
			case DAMAGE:
				actions[i].type = "dmg";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case DAMAGE_IGNORE_ARMOR:
				actions[i].type = "dmg_ign";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				actions[i].ignore_armor = true;
				break;
			case HEAL:
				actions[i].type = "heal";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				actions[i].range_left = data.at(dataID).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
				actions[i].range_right = data.at(dataID).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
				break;
			case LIFE_STEAL:
				actions[i].type = "life_steal";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case PARRY:
				actions[i].type = "parry";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case POISON:
				actions[i].type = "poison";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case BOOST:
				actions[i].type = "boost";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				actions[i].range_left = data.at(dataID).at("Actions").at(i).at("Range_left").get <std::vector<int>>();
				actions[i].range_right = data.at(dataID).at("Actions").at(i).at("Range_right").get <std::vector<int>>();
				break;
			case BLEED:
				actions[i].type = "bleed";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case FIRE:
				actions[i].type = "fire";
				actions[i].amount = data.at(dataID).at("Actions").at(i).at("Amount");
				break;
			case SNEAK:
				actions[i].type = "sneak";
				break;
			case EMPTY:
				actions[i].type = "empty";
				break;
			}
		}
	}
	

}

void Enemy::printInfo() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), this->body)) {
		std::cout << "Name: " << this->stats.name << "\n";
		std::cout << "SegIndex: " << this->stats.segment_index << "\n";
		std::cout << "Before: " << this->right << "\n";
		this->right = right  ? false : true;
		std::cout << "After: " << this->right << "\n";
	}
}

void Enemy::setPosition(Segments *segment) { // sets position and width, height to empty segment on enemy board
	this->body.width = segment->body.width / 1.5;
	this->body.height = segment->body.height / 1.5;
	this->body.x = segment->body.x + (segment->body.width / 2) - (this->body.width / 2);
	this->body.y = segment->body.y + (segment->body.height / 2.5) - (this->body.height / 2);
	this->stats.segment_index = segment->id;
	segment->isOccupied = true;

}

void Enemy::setNewPosition(std::vector <Segments>* segment, bool right, Enemy *swapTarget) { // moves enemy to right / left if index at vector enemyboard is empty if given conditions are met
	this->stats.hasChangedPosition = false;
	for (int i = 0; i < segment->size(); i++) {
		if (right) { 
			int newPos = this->stats.segment_index + 1;
			if (newPos > 4) {
				newPos = 4;
				break;
			}
			if (!segment->at(newPos).isOccupied && !this->stats.hasChangedPosition) {
				this->stats.hasChangedPosition = true;
				segment->at((newPos - 1)).isOccupied = false;
				this->setPosition(&segment->at(newPos));
			}
			else if (swapTarget != NULL && segment->at(swapTarget->stats.segment_index).isOccupied && !swapTarget->right && !this->stats.hasChangedPosition) {
				this->stats.hasChangedPosition = true;
				swapTarget->setPosition(&segment->at(newPos -1));
				this->setPosition(&segment->at(newPos)); // change pos to targets pos, move target to old main pos


			}
		}
		else {
			int newPos = this->stats.segment_index - 1;
			if (newPos < 0) {
				newPos = 0;
				break;
			}
			if (!segment->at(newPos).isOccupied && !this->stats.hasChangedPosition) {
				this->stats.hasChangedPosition = true;
				this->setPosition(&segment->at(newPos));
				segment->at((newPos + 1)).isOccupied = false;
			}
		}

	}

}

std::string rangeVisualisation(std::vector<int> range_left, std::vector<int> range_right) {
	std::string rem = "< < < < < ^ > > > > >";
	for (int i = 0; i < range_left.size(); i++) {
		rem[10 - (range_left.at(i) * 2)] = '*';
	}
	for (int i = 0; i < range_right.size(); i++) {
		rem[10 + (range_right.at(i) * 2)] = '*';
	}
	return rem;
}

void Enemy::drawActionStats() {
	for (int i = 0; i < 4; i++) {
		if (actions[i].sacrifice) {
			DrawRectangle(body.x + 5, body.y + 38 + (25 * i), 15, body.width - 5, ColorAlpha(RED, 0.2f));
		}
		if (actions[i].boosted) {
			DrawRectangle(body.x + 5, body.y + 30 + (25 * i), body.width - 10, 5, ColorAlpha(BLUE, 0.2f));
		}
		DrawText(TextFormat("%d: %s: %d", (i+1), actions[i].type.c_str(), actions[i].amount), body.x + 5, body.y + 30 + (25 * i), 4, BLACK);
		if (actions[i].type == "dmg" || actions[i].type == "dmg_ign" || actions[i].type == "empty" || actions[i].type == "sneak" || actions[i].type == "bleed" || actions[i].type == "poison" || actions[i].type == "fire" || actions[i].type == "life_steal") {
		}
		else {
			DrawText(TextFormat("	%s", rangeVisualisation(actions[i].range_left, actions[i].range_right).c_str()), body.x + 5, body.y + 42 + (25 * i), 10, BLACK);
		}


	}
	DrawText(TextFormat("Survive: %s: %d", survive.type.c_str(), survive.amount), body.x + 5, body.y + 30 + (25 * 4), 4, BLACK);
	if (survive.type == "dmg" || survive.type == "ign" || survive.type == "empty" || survive.type == "sneak" || survive.type == "bleed" || survive.type == "poison" || survive.type == "fire" || survive.type == "life_steal") {
	}
	else {
		DrawText(TextFormat("	%s", rangeVisualisation(survive.range_left, survive.range_right).c_str()), body.x + 5, body.y + 42 + (25 * 4), 10, BLACK);
	}

}

void Enemy::draw() {
	
	printInfo();

	DrawRectangleRec(this->body, ORANGE);
	if (this->stats.rarity == "Common") {
		DrawRectangleLinesEx(this->body, 3, DARKGRAY);
	}
	else 	if (this->stats.rarity == "Uncommon") {
		DrawRectangleLinesEx(this->body, 3, GREEN);
	}
	else 	if (this->stats.rarity == "Rare") {
		DrawRectangleLinesEx(this->body, 3, BLUE);
	}
	else if (this->stats.rarity == "Epic") {
		DrawRectangleLinesEx(this->body, 3, PURPLE);
	}
	else {
		DrawRectangleLinesEx(this->body, 3, GOLD);
	}
	if (this->stats.sneak) {
		DrawRectangleRec(this->body, ColorAlpha(BLUE, 0.5));
	}
	if (this->stats.armor > 0) {
		DrawText(TextFormat("%s\nHealth: %d / %d	Armor: %d", stats.name.c_str(), stats.health, stats.max_health, stats.armor), body.x + 5, body.y + 5, 4, BLACK);

	}
	else {
		DrawText(TextFormat("%s\nHealth: %d / %d", stats.name.c_str(), stats.health, stats.max_health), body.x + 5, body.y + 5, 4, BLACK);
	}
	if (right) {
		DrawText(TextFormat(">"), body.x + body.width / 2, body.y + body.height + 2, 30, BLACK);
	}
	else {
		DrawText(TextFormat("<"), body.x + body.width / 2, body.y + body.height + 2, 30, BLACK);

	}
	drawActionStats();


	for (int i = 0; i < poison_stacks; i++) { // POISON VISUALIZATION
		DrawRectangleRec({ this->body.x + 6 * i, this->body.y - 6, 5, 5 }, GREEN);
	}
	for (int i = 0; i < fire_stacks; i++) { // FIRE VISUALIZATION
		DrawRectangleRec({ this->body.x + 6 * i, this->body.y - 12, 5, 5 }, ORANGE);
	}
	for (int i = 0; i < bleed_stacks; i++) { // BLEED VISUALIZATION
		DrawRectangleRec({ this->body.x + 6 * i, this->body.y - 18, 5, 5 }, RED);
	}
	if (this->stats.parry) { // PARRY VISUALIZATION
		DrawRectangleRec({ this->body.x, this->body.y - 25, 15, 5 }, BLUE);

	}
}
