#include "GameMap.h"
GameMap::GameMap() {
	countOfRooms = 0;
	countOfSegments = 0;
	createSegments(8, 16);
	getNextRoomPos();
}

void GameMap::createType() {
	int enemyChance = 75;

	int shopChance = 30;
	int encounterChance = 30;
	int lootChance = 10;

	if (GetRandomValue(1, 100) < enemyChance) { // sets type to fight room
		room.getType(ENEMY);
		cofEnemyRooms++;
	}
	else { // sets type of other rooms based on percentual chance
		int chance = GetRandomValue(1, (shopChance + encounterChance + lootChance));
		if (chance <= shopChance) {
			room.getType(SHOP);
			cofShopRooms++;
		}
		else if (chance - shopChance <= encounterChance) {
			room.getType(ENCOUNTER);
			cofEncounterRooms++;

		}
		else {
			room.getType(LOOT);
			cofLootRooms++;

		}
	}




}

void GameMap::createRooms(int min, int max, int segmentIndex) { // creates x rooms in range 
	int size = GetRandomValue(min, max);
	countOfRooms += size;
	roomCells.push_back(size);
	float x = (windowWidth - 50) / countOfSegments;
	float xStart = round(x / 2);
	float y = windowHeight / (size * 2);
	for (int i = 0; i < size; i++) {
		if (segmentIndex == 0 || segmentIndex == (countOfSegments - 1)) { // creates starting and ending rooms
			;
			room.body = { xStart + (x * segmentIndex), 400, 100, 100 };
			room.index = index;
			room.pos = { (room.body.x + (room.body.width / 2)), (room.body.y + (room.body.height / 2)) };
			if (i == 0) { room.getType(START); room.current = true; }
			else room.getType(END);
			//std::cout << "room: " << room.index << " size: " << room.height << " " << room.width << " XY: " << room.body.x << " " << room.body.y << "\n";
			rooms.push_back(room);
			index++;
			break;
		}
		else { // math that i forgot, created by some trial and error, dont touch the cordinates
			createType();
			room.current = false;
			room.body = { xStart + (x * segmentIndex), y + (y * (2 * i)), 50, 50 };
			room.index = index;
			room.pos = { (room.body.x + (room.body.width / 2)), (room.body.y + (room.body.height / 2)) };
			rooms.push_back(room);
			index++;
		}

		//std::cout << "room: " << room.index << " size: " << room.height << " " << room.width << " XY: " << room.body.x << " " << room.body.y << "\n";
	}
}

void GameMap::createSegments(int min, int max) {
	countOfSegments = GetRandomValue(min, max);
	for (int i = 0; i < countOfSegments; i++) {
		segmentss.push_back({ ((windowWidth) / countOfSegments) * i, 0, ((windowWidth) / countOfSegments), 900 });

		if (i == 0 || i == countOfSegments - 1) { // starting and ending room
			createRooms(1, 1, i);
		}
		else { // all other rooms, contains 1 to 4 blocks
			createRooms(1, 4, i);
		}
	}
}
int skibidi = 0;
Rectangle tempButton = { 1600, 150, 150, 100 };
void GameMap::drawRect() { // drawing and logic that should be in update func

	// button to acces collection 
	DrawRectangleRec(tempButton, ORANGE);
	DrawText("Show Collections", tempButton.x - tempButton.width - 10, tempButton.y + 10, 50, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), tempButton)) {
		this->type = COLLECTION;
	}

	for (int i = 0; i < rooms.size(); i++) { // draws all rooms

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			//rooms.at(i).current = rooms.at(i).isHovered();
			if (rooms.at(i).current) { // checks for current active room
				for (int j = 0; j < rooms.at(i).nextRec.size(); j++) { // gets all other neighbours
					for (int k = i; k < rooms.size() && k - i < 8; k++) { // checks max 8 rooms because max count of eighbours is 7 + self
						if (rooms.at(k).isClicked().x + rooms.at(k).isClicked().y == rooms.at(i).nextRec.at(j).x + rooms.at(i).nextRec.at(j).y) { // check if x,y clicked room is withing range of neighbours to current active room
							rooms.at(i).current = false;
							rooms.at(k).current = true;
							if (rooms.at(k).getType() == "ENEMY") { // sets type to x,y,z, takes effect in game.cpp
								this->type = ENEMY;
							}
							else if (rooms.at(k).getType() == "LOOT") {
								this->type = LOOT;
								this->loot.createRoom();
							}
							else if (rooms.at(k).getType() == "SHOP") {
								this->type = SHOP;
								this->shop.createShopRoom();
							}
							else if (rooms.at(k).getType() == "ENCOUNTER") {
								this->type = ENCOUNTER;
							}
							goto drawingos;
						}
					}
				}
			}
		}
		drawingos:
		DrawRectangleRec(rooms.at(i).body, rooms.at(i).color);
		DrawText(TextFormat("Ind: %d\nX: %1.f / Y: %1.f", rooms.at(i).index, rooms.at(i).body.x, rooms.at(i).body.y), rooms.at(i).body.x, rooms.at(i).body.y, 1, GREEN);
		for (Vector2 next : rooms.at(i).nextPos) {
			if (rooms.at(i).current) { // display accessible rooms 
				DrawLineEx(rooms.at(i).pos, next, 6, DARKGREEN);
			}
			else {
				DrawLineEx(rooms.at(i).pos, next, 3, BLACK);
			}
		}

		if (rooms.at(i).current) {
			DrawRectangleLinesEx(rooms.at(i).body, 5, BLACK);
		}
	}
	for (Rectangle rect : segmentss) {
		DrawRectangleLinesEx(rect, 1, DARKGRAY);
		DrawText(TextFormat("Index: %d", skibidi), rect.x, rect.y + 10, 20, BLACK);
		skibidi++;
	}
	skibidi = 0;
}

void GameMap::getNextRoomPos() { // pretty much  up "math" logic, creates paths based on count of current and upcoming rooms, next and current size is stored in roomCells, based on size and some pseudorandom numbers creates paths
	int room, nextRoom, index = 0;
	for (int i = 0; i < roomCells.size() - 1; i++) {
		room = roomCells.at(i);
		nextRoom = roomCells.at(i + 1);

		if (room == 1) { // logic for starting room
			//std::cout << "room == 1 START" << "\n";

			for (int j = 1; j <= nextRoom; j++) {
				rooms.at(index).nextPos.push_back(rooms.at(index + j).pos);
				rooms.at(index).nextRec.push_back(rooms.at(index + j).body);

				//std::cout << rooms.at(index).index << " --> " << rooms.at(index + j).index << "\n";
			}
			//std::cout << "room == 1 END" << "\n";

			index++;
		}
		else if (room == nextRoom) {
			//std::cout << "room == nextRoom START" << "\n";
			for (int j = 0; j < room; j++) {
				rooms.at(index).nextPos.push_back(rooms.at(index + room).pos);
				rooms.at(index).nextRec.push_back(rooms.at(index + room).body);
				//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room).index << "\n";
				index++;
			}
			//std::cout << "room == nextRoom END" << "\n";
		}
		else if (nextRoom == 1) {
			//std::cout << "room NEXT == 1 START" << "\n";

			for (int j = 0; j < room; j++) {
				rooms.at(index).nextPos.push_back(rooms.at(index + room - j).pos);
				rooms.at(index).nextRec.push_back(rooms.at(index + room - j).body);
				//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room - j).index << "\n";
				index++;
			}
			//std::cout << "room NEXT == 1 END" << "\n";

		}
		else if (room < nextRoom) {
			//std::cout << "room < nextRoom START" << "\n";
			for (int j = 0; j < room; j++) {
				rooms.at(index).nextPos.push_back(rooms.at(index + room).pos);
				rooms.at(index).nextPos.push_back(rooms.at(index + nextRoom).pos);
				rooms.at(index).nextRec.push_back(rooms.at(index + room).body);
				rooms.at(index).nextRec.push_back(rooms.at(index + nextRoom).body);
				//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room).index << " || ";
				//std::cout << rooms.at(index).index << " --> " << rooms.at(index + nextRoom).index << "\n";

				index++;
			}
			//std::cout << "room < nextRoom END" << "\n";

		}
		else {
			for (int j = 0; j < room; j++) {
				if ((nextRoom % 2) == 0)
				{
					if (j < (room / 2)) {
						rooms.at(index).nextPos.push_back(rooms.at(index + room).pos);
						rooms.at(index).nextRec.push_back(rooms.at(index + room).body);
						//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room).index << "\n";
					}
					else {
						rooms.at(index).nextPos.push_back(rooms.at(index + nextRoom).pos);
						rooms.at(index).nextRec.push_back(rooms.at(index + nextRoom).body);
						//std::cout << rooms.at(index).index << " --> " << rooms.at(index + nextRoom).index << "\n";

					}
					index++;
					/*if (GetRandomValue(1, 10) < 3) {
						rooms.at(index - 1).nextPos.push_back(rooms.at(index).pos);
						//std::cout << rooms.at(index - 1).index << " --> " << rooms.at(index).index << "\n";
					}
					if (GetRandomValue(1, 10) < 3) {
						rooms.at(index - room).nextPos.push_back(rooms.at(index + 1).pos);
						//std::cout << rooms.at(index - room).index << " --> " << rooms.at(index + 1).index << "\n";
					}*/

				}
				else
				{
					//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room).index << "\n";
					if (j < 2) {
						rooms.at(index).nextPos.push_back(rooms.at(index + room).pos);
						rooms.at(index).nextRec.push_back(rooms.at(index + room).body);
						if (GetRandomValue(1, 10) < 3) {
							rooms.at(index).nextPos.push_back(rooms.at(index + room + 1).pos);
							rooms.at(index).nextRec.push_back(rooms.at(index + room + 1).body);
							//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room + 1).index << "\n";
						}
					}
					else {
						rooms.at(index).nextPos.push_back(rooms.at(index + nextRoom).pos);
						rooms.at(index).nextRec.push_back(rooms.at(index + nextRoom).body);
						//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room).index << "\n";
						/*if (GetRandomValue(1, 10) < 3) {
							rooms.at(index).nextPos.push_back(rooms.at(index + room - j).pos);
							//std::cout << rooms.at(index).index << " --> " << rooms.at(index + room - j).index << "\n";
						}*/
					}
					index++;
				}
			}
		}
	}
}
