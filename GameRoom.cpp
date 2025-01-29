#include "GameRoom.h"

void GameRoom::getType(int type) {
	switch (type) {
	case 1: // start
		color = ORANGE;
		this->type = "START";
		break;
	case 2: // enemy
		color = RED;
		this->type = "ENEMY";
		break;
	case 3: // loot
		color = YELLOW;
		this->type = "LOOT";
		break;
	case 4: // encounter
		color = SKYBLUE;
		this->type = "ENCOUNTER";
		break;
	case 5: // shop
		color = PURPLE;
		this->type = "SHOP";
		break;
	case 6: // end
		color = ORANGE;
		this->type = "END";
		break;
	}
}

bool GameRoom::isHovered() {
		if (CheckCollisionPointRec(GetMousePosition(), body)) {
			return true;
		}
	return false;
}

std::string GameRoom::getType() {
	return this->type;
}

Rectangle GameRoom::isClicked() {
	if (CheckCollisionPointRec(GetMousePosition(), body)) {
		return body;
	}
	return { 0,0,0,0 };

}