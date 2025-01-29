#include "rewardScreen.h"
#include "Card.h"

void rewardScreen::draw() {
	claimReward();
	DrawRectangleRec(this->screen, GRAY);
	DrawRectangleRec(this->btn, RED);
	DrawText(TextFormat("Gold: %d\n\n\nCards: %d", gold, this->cards.size()), screen.x + screen.width / 3, screen.y + screen.height / 3, 40, BLACK);
	DrawText("Continue", btn.x +btn.width / 2, btn.y + btn.height / 2, 10, WHITE);
}

void rewardScreen::claimReward() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), btn)) {
		this->end = true;
		return;
	}
	this->end = false;
}