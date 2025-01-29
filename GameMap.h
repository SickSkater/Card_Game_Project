#pragma once
#include <vector>
#include <raylib.h>
#include <iostream>
#include <math.h>
#include "GameRoom.h"
#include "LootRoom.h"
#include "ShopRoom.h"
#include "EncounterRoom.h"

/*enum ROOMTYPE
{
	ENEMY,
	LOOT,
	ENCOUNTER,
	SHOP,
	BOSS,
	START,
	END
};

struct room {
	ROOMTYPE type;
	Color color;
	Rectangle rec;
	int index;
	Vector2 pos;
	std::vector <Vector2> lines;
	int nextIndex[4] = { 0,0,0,0};
	int sector;


};
class GameMap
{
public:
	GameMap();
	~GameMap();
	void draw();
	void createRoom(int max, int min);
	void getVector(int index);
	int mapSector;
	std::vector <Rectangle> sector;
	std::vector <room> rooms;

};

*/

class GameMap
{
public:
	enum Type
	{
		START = 1, END = 6, ENEMY = 2, LOOT = 3, ENCOUNTER = 4, SHOP = 5, COLLECTION = 7, GAMEOVER = 8
	};
	float windowHeight = 900;
	float windowWidth = 1800;
	int index = 0;
	int countOfSegments;
	int countOfRooms;
	int cofEnemyRooms;
	int cofLootRooms;
	int cofEncounterRooms;
	int cofShopRooms;
	int maxLootRooms = 3;
	int maxEncounterRooms = 5;
	int maxShopRooms = 2;

	LootRoom loot;
	ShopRoom shop;
	EncounterRoom encounter;

	Type type;
	bool createdRoom = false;

	GameRoom room;
	std::vector <GameRoom> rooms;
	std::vector <Rectangle> segmentss;
	std::vector <int> roomCells; // contains count of rooms per segment

	GameMap();
	void createSegments(int min, int max);
	void createRooms(int min, int max, int segmentIndex);
	void drawRect();
	void getNextRoomPos();
	void createType();
};