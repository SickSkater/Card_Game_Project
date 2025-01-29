#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

struct CollisionRectangle {
	Rectangle body;
	bool occupied;
	CollisionRectangle(float x, float y, float w, float h) {
		this->body = { x + w ,y + h , w, h};
		occupied = false;
	}

	bool isSelected() {
		return CheckCollisionPointRec(GetMousePosition(), body);
	}

	void draw() {
		DrawRectangleLinesEx(body, 1, RED);
	}
};

struct Segments {
	bool hovered = false;
	int id;
	bool isOccupied = false;
	Rectangle body;
	std::vector <CollisionRectangle> collision_Rectangles;

	Segments(int id, float x, float y, float w, float h, bool colRecs) {
		this->id = id;
		this->body = { x,y,w,h };
		if (colRecs) {
			for (int rows = 0; rows < 4; rows++) {
				for (int cols = 0; cols < 4; cols++) {
					collision_Rectangles.push_back(CollisionRectangle(x + 50 * cols, y + 50 * rows, 35, 35));
				}
			}
		}

	};

	bool check() {
		return (CheckCollisionPointRec(GetMousePosition(), this->body));
	}

	Rectangle getSelectedCollisionRectangle() {
		for (int i = 0; i < collision_Rectangles.size(); i++) {
			if (collision_Rectangles.at(i).isSelected()) {
				return collision_Rectangles.at(i).body;
			}
		}
		return { 0,0,0,0 };
	}

	Segments getSegment() {
		if (check()) return *this;
	}

	void draw() {
		if (CheckCollisionPointRec(GetMousePosition(), this->body) && !collision_Rectangles.empty()) {
			this->hovered = true;
			DrawRectangleLinesEx(body, 2, RED);
		}
		else {
			this->hovered = false;
			DrawRectangleLinesEx(body, 2, BLACK);
		}

		for (CollisionRectangle body : collision_Rectangles) {
			body.draw();
		}
	};

};

class GameArea
{
public:
	float pos_X, pos_Y, width, height;
	Rectangle body;
	Color background, outline;
	std::vector <Segments> segments;

	GameArea(float x, float y, float width, float height, Color bg, Color outline, int segments, bool colRecs);
	void CreateSegments(int segments, bool colRecs);
	Segments getHoveredSegment();
	void draw();

};

