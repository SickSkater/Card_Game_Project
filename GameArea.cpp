#include "GameArea.h"

GameArea::GameArea(float x, float y, float widthP, float heightP, Color bg, Color outlineP, int segments, bool colRecs) { // creates map filled with various rooms by set of rules
	pos_X = x, pos_Y = y, width = widthP, height = heightP, background = bg, outline = outlineP;
	body = { pos_X, pos_Y, width, height };
	if (segments > 0 ) { CreateSegments(segments, colRecs); }
}

void GameArea::CreateSegments(int segments, bool colRecs) { // splits map into segments for better visualization
	for (int i = 0; i < segments; i++) {
		this->segments.push_back(Segments(i, pos_X + (i * (width / segments)), pos_Y, width / segments, height, colRecs));
	}
}

void GameArea::draw() {
	DrawRectangleRec(body, background);
	for (Segments segment : segments) {
		segment.draw();
	}
}

Segments GameArea::getHoveredSegment() { // gets segment data 
	for (int i = 0; i < segments.size(); i++) {
		if (segments.at(i).check()) {
			return segments.at(i).getSegment();
		}
	}
}

