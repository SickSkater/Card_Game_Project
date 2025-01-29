#include "Game.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
std::ifstream myEnemiesFile2("enemyDatabase.json");
std::fstream mainCardsFile2("cardDatabase.json");
json enemyData2 = json::parse(myEnemiesFile2);
json cardData2 = json::parse(mainCardsFile2);


// define a timer
typedef struct Timer
{
	float Lifetime = 0;
};

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float lifetime)
{
	if (timer != NULL)
		timer->Lifetime = lifetime;
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
	// subtract this frame from the timer if it's not allready expired
	if (timer != NULL && timer->Lifetime > 0)
		timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
	if (timer != NULL && timer->Lifetime < 0) {
		timer->Lifetime = 0;
		return true;
	}
	return false;
}


void Game::create() { // creates all key instances needed to play
	GameMap map;
	GameArea player_board(160, 775, 1600, 200, GRAY, BLACK, 10, false);
	GameArea game_board(320, 20, 1250, 250, GRAY, BLACK, 5, false);
	GameArea action_board(320, 240, 1250, 250, GRAY, BLACK, 5, true);
	EnemyHandler enemyHandler;
	for (int i = 0; i < enemyData2.size(); i++) {
		enemyHandler.enemyCollection.fillCollection(Enemy(i));
	}
	CardDeck cards;
	Player player;
	CardCollection collection;
	ItemHandler itemhandler;
	round_btn = { 1300, 550, 100, 40 };
	this->areas.push_back(player_board); 	this->areas.push_back(game_board);	this->areas.push_back(action_board);
	for (int i = 0; i < cardData2.size(); i++) {
		if (cardData2.at(i).contains("Locked") && (cardData2.at(i).at("Locked") == true)) { // create card deck of not yet obtained cards
			player.locked_deck.createCard(i);

		}
		player.all_cards.createCard(i); // deck of all cards
	}
	this->player = player;
	this->collection = collection;
	for (int i = 0; i < this->player.all_cards.card_deck.size(); i++) { // creates deck of cards to be picked 
		this->collection.addToCollection(&this->player.all_cards.card_deck.at(i), (int)cardData2.at(i).at("Count"));
	}
	mainCardsFile2.close();
	this->collection.createPreset();
	this->player.enemy_board = &areas.at(1);
	this->map = map;
	this->enemyHandler = enemyHandler;
	this->map.type = map.COLLECTION;
	this->player.stats.gold += 100;
	this->itemHandler = itemhandler;
	this->player.currentItems.push_back(itemHandler.getItem("RANDOM"));

}


void Game::createFight() {
	this->player.current_deck.card_deck.clear(); // removes leftover cards from fights before
	this->enemyHandler.enemies.clear(); // same but enemy
	map.createdRoom = true;
	for (int i = 0; i < GetRandomValue(2, 5); i++) { // creates x enemies from random percentual selection
		this->enemyHandler.pickEnemyFromCollection(&areas.at(1).segments[i]);

	}
	this->player.drawCard(player.stats.startingDraw); // draw x cards 
	this->player.current_deck.setCardPosition(&areas.at(0).segments, &areas.at(2)); // places cards on playerboard
	this->itemActRem.doPassive(&player, &enemyHandler.enemies);

}

void Game::update() { // changes gamestate and whats drawn on screen
	if (map.type == 2) { // enemy
		player.current_deck.update(&player);
		roundManager();
		if (player.stats.health <= 0) {
			map.type = map.GAMEOVER;
		}
	}
	else if (map.type == 3) { // loot
		map.loot.active = true;
		map.loot.update(&player);
		if (!map.loot.active) {
			map.type = map.START;
		}
	}
	else if (map.type == 4) { // encounter
		map.encounter.getResult(&player);
		if (map.encounter.fight) {
			std::cout << "|	GAME	|: "  << "TRIGGERED FIGHT!\n";
			map.encounter.fight = false;
			map.type = map.ENEMY;
		}
		else {
			map.type = map.START;
		}
	}
	else if (map.type == 5) { // shop
		map.shop.active = true;
		map.shop.update(&player, &this->player.all_cards);
		if (!map.shop.active) {
			map.type = map.START;
		}
	}
	else if (map.type == 7) { // collection
		if (this->collection.end) {
			this->player.draw_deck.card_deck = this->collection.getDeck();
			map.type = map.START;
			this->collection.end = false;
		}
	}



}
Timer t;
Timer r;
Timer bugPrevention;
bool check = false;
std::vector <std::string> rarityCardHolder;
std::vector <std::string> rarityItemHolder;




void Game::playerRound() { // triggers all active cards, does its effects
	player.stats.health -= player.stats.bleed_stacks * (round + 1);
	itemActRem.doPassive(&player, &enemyHandler.enemies);
	for (int i = 0; i < player.current_deck.getActiveCards().size(); i++) {
		itemActRem.doAction(&player, &enemyHandler.enemies, "SPELL");
		player.current_deck.getActiveCards().at(i).actions[round].castSpells(&enemyHandler.enemies, &player.current_deck.getActiveCards().at(i),&player, round);
	}
	// poison
	if (player.stats.poison_stacks >= 1) { // has poison apllied
		player.stats.health -= player.stats.poison_stacks;
		std::cout << "|	GAME	|: "  << "Dealt " << player.stats.poison_stacks << " of poison damage to player" << "\n";
		// 30% chance to remove one poison stack
		if (GetRandomValue(1, 10) < 3) {
			player.stats.poison_stacks--;
			std::cout << "|	GAME	|: "  << "Poison stack removed from player\n";
		}
	}
}

float getRarityDropMult(std::string rarity) {
	if (rarity == "Legendary") {
		return 3;
	}
	else if (rarity == "Epic") {
		return 2.5;
	}
	else if (rarity == "Rare") {
		return 2;
	}
	else if (rarity == "Uncommon") {
		return 1.5;
	}
	else {
		return 1;
	}
}

int counter = 0;
bool ignoreSwapped = false;
bool movedRight = false;
bool ignoreSwappedChecker = false;
int ignoredIndex;
bool endRound = false;
bool enableNewRound = true;
bool multiplecastPrevention = true;
int lastMovedEnemy = 0;
void Game::roundManager() { // does round logic, moves enemy as part of visualization of enemy attack, does player round
	if (map.createdRoom == false) { // creates fight room
		createFight();
		itemActRem.doAction(&player, &enemyHandler.enemies, "MATCH");
		player.stats.armor = player.stats.starting_armor;

	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), round_btn) && enableNewRound) { // starts battle
		itemActRem.doAction(&player, &enemyHandler.enemies, "ROUND");
		itemActRem.doAction(&player, &enemyHandler.enemies, "PASSIVE");

		StartTimer(&bugPrevention, 5.0f);
		if (TimerDone(&bugPrevention)) {
			enableNewRound = true;
		}
		player.stats.parry = 0;
		multiplecastPrevention = true;
		enableNewRound = false;
		if (enemyHandler.enemies.size() == 0) { // check to end fight, resets states of player and map
			map.type = map.END;
			std::cout << "|	GAME	|: "  << "ALL ENEMIES ARE DEAD\n";
			//map.type = map.START; // sets screen to show map
			map.createdRoom = false;
			player.stats.bleed_stacks = 0;
			player.stats.health += player.stats.healthRecharge;
			if (player.stats.health >= player.stats.max_health) {
				player.stats.health = player.stats.max_health;
			}

				player.stats.mana = player.stats.max_mana;
			
			player.stats.poison_stacks = 0;
			player.stats.fire_stacks = 0;
			/*for (int i = 0; i < player.current_deck.card_deck.size(); i++) {
				player.discardCard(&player.current_deck.card_deck.at(i), i);
			}*/
			player.discarded_deck.card_deck.clear(); // clearing all decks
			player.draw_deck.card_deck.clear();
			player.current_deck.card_deck.clear();
			player.draw_deck.card_deck = this->collection.getDeck(); // generating new draw deck to mitigate multiple copies of left cards
			counter = 5;
			this->round = -1;
			endRound = false;
			enableNewRound = true;
			multiplecastPrevention = true;
			for (int crd = 0; crd < rarityCardHolder.size(); crd++) {
				std::cout << "|	GAME	|: " << "TRYING TO CREATE CARD\n";
				this->player.checkNewCardDeck(rarityCardHolder.at(crd));
			}
			for (int item = 0; item < rarityItemHolder.size(); item++) {
				std::cout << "|	GAME	|: "  << "TRYING TO GET ITEM\n";
				this->player.currentItems.push_back(this->itemHandler.getItem(rarityItemHolder.at(item)));
			}
			return;
		}


		this->round = this->round < 3 ? this->round + 1 : 0;
		playerRound();
		for (int i = 0; i < enemyHandler.enemies.size(); i++) {
			if (enemyHandler.enemies.at(i).stats.sneak) {
				enemyHandler.enemies.at(i).stats.sneak = false;
			}
			enemyHandler.enemies.at(i).stats.parry = 0;

		}

		counter = 0;
		StartTimer(&t, 0.1f);
	}
	if (round != -1) { // timer updating, update actions
				if (counter > 4) {
					if (r.Lifetime <= 0) {
						enableNewRound = true;
					}
				}


		for (int i = 0; i < enemyHandler.enemies.size(); i++) {
			if (enemyHandler.enemies.at(i).stats.health <= 0) { // kills enemy if health <= 0
				//rwdScreen.gold += GetRandomValue(1, 10);

				if (enemyHandler.enemies.at(i).stats.reward == "GOLD") {
					int rem = GetRandomValue(1, 10) * getRarityDropMult(enemyHandler.enemies.at(i).stats.rarity);
					rwdScreen.gold += rem;
					std::cout << "|	GAME	|: "  << "Obtained: " << rem << " gold from killing: " << enemyHandler.enemies.at(i).stats.name << "!\n";
				}
				else if (enemyHandler.enemies.at(i).stats.reward == "CARD") {
					std::cout << "|	GAME	|: "  << "Obtained CARD from killing: " << enemyHandler.enemies.at(i).stats.name << "!\n";
					rarityCardHolder.push_back(enemyHandler.enemies.at(i).stats.rarity);
				}
				else if (enemyHandler.enemies.at(i).stats.reward == "ITEM") {

				}
				player.enemy_board->segments.at(enemyHandler.enemies.at(i).stats.segment_index).isOccupied = false;
				enemyHandler.enemies.erase(enemyHandler.enemies.begin() + i);
				i = 0;
			}
		}

		UpdateTimer(&t);
		UpdateTimer(&r);
		UpdateTimer(&bugPrevention);



			if (TimerDone(&r)) { // resets last active enemy to original position
				movedRight = false;
				if (ignoreSwapped) {
					if (!ignoreSwappedChecker) {
						ignoreSwappedChecker = true;
					}
					else {
						ignoreSwapped = false;
						ignoreSwappedChecker = false;
					}
				}
				for (int i = 0; i < enemyHandler.enemies.size(); i++) {
					if (enemyHandler.enemies.at(i).stats.segment_index == lastMovedEnemy) {
						enemyHandler.enemies.at(i).body.y -= 100;
					}					
				}

			}
			if (TimerDone(&t)) { // moves active enemy to front, casts actions, starts timer to move enemy back to original position
					for (int enem = 0; enem < enemyHandler.enemies.size(); enem++) {
						if (enemyHandler.enemies.at(enem).stats.segment_index == areas.at(1).segments.at(counter).id) { // left to right check for move
							if (enemyHandler.enemies.at(enem).right) { // try to swap right
								int segRem = enemyHandler.enemies.at(enem).stats.segment_index + 1; // array overflow prevention
									if ((segRem < 5 && enem+1 < enemyHandler.enemies.size()) && !enemyHandler.enemies.at(enem+1).right) {
										for (int i = 0; i < enemyHandler.enemies.size(); i++) {
											if (enemyHandler.enemies.at(i).stats.segment_index == segRem && !ignoreSwapped) {
												this->enemyHandler.enemies.at(enem).setNewPosition(&player.enemy_board->segments, this->enemyHandler.enemies.at(enem).right, &this->enemyHandler.enemies.at(i)); // swap 2 instances of enemy if higher pos is not right
												ignoreSwapped = true;
											}
				
										}

										
									}
									else {
										if (!movedRight) {
											
											this->enemyHandler.enemies.at(enem).setNewPosition(&player.enemy_board->segments, this->enemyHandler.enemies.at(enem).right, NULL); // try to MOVE RIGHT
											movedRight = true;
										}

									}
							}
							else { // try to move left
								int rem = enemyHandler.enemies.at(enem).stats.segment_index - 1;
								this->enemyHandler.enemies.at(enem).setNewPosition(&player.enemy_board->segments, false, NULL);
								if (rem == this->enemyHandler.enemies.at(enem).stats.segment_index ) {
									counter--;
								}
							}
							
						}
						if (enemyHandler.enemies.at(enem).stats.segment_index == areas.at(1).segments.at(counter).id) { // left to right check for current enemy playing
							
								enemyHandler.enemies.at(enem).body.y += 100;
								lastMovedEnemy = enemyHandler.enemies.at(enem).stats.segment_index;
								//std::cout << "round started" << enem << "\n";
								enemyHandler.enemies.at(enem).actions[round].checkForOthers(&enemyHandler.enemies.at(enem), &enemyHandler.enemies, round, &player); // cast actions on self, allies and player
							
							
							StartTimer(&r, 0.1f);
						}
					}
					counter++;
				if (counter <= 4) { // continues to start timer until all enemies had their turn
					StartTimer(&t, 0.25f);
				}
				if (this->round == 3 && multiplecastPrevention && this->enemyHandler.enemies.size() > 0) { // Discards cards to discard deck, draws new cards, replenishes some mana
					multiplecastPrevention = false;
					player.stats.mana += player.stats.manaRecharge;
					if (player.stats.mana > player.stats.max_mana) player.stats.mana = player.stats.max_mana;

					for (int i = 0; i < enemyHandler.enemies.size(); i++) {
						enemyHandler.enemies.at(i).survive.surviveAction(&enemyHandler.enemies.at(i), &enemyHandler.enemies, round, &player);
					}

					for (int i = 0; i < player.current_deck.card_deck.size(); i++) { // check current deck 
						if (player.current_deck.card_deck.at(i).locked) { // if card was played, discard it
							player.discardCard(&player.current_deck.card_deck.at(i), i);
							i--;
						}
					}

					player.drawCard(player.stats.redraw); // draw cards
					player.stats.health += player.stats.healthRecharge;
					if (player.stats.health > player.stats.max_health) {
						player.stats.health = player.stats.max_health;
					}
					player.current_deck.setCardPosition(&areas.at(0).segments, &areas.at(2)); // fill cards to availabe positions
				}
			}
		}
	
}

void Game::draw() {
	if (map.type == 2) { // fight
		ClearBackground(BLANK);
		areas[0].draw();
		areas[1].draw();
		areas[2].draw();
		enemyHandler.draw(round);
		player.current_deck.draw(round);
		player.draw();

		DrawRectangleRec(round_btn, LIGHTGRAY);
		DrawText(TextFormat("Round: %d", this->round + 1), round_btn.x + 10, round_btn.y + round_btn.height / 2 - 5, 10, BLACK);
	}
	else if (map.type == 3) { // loot
		map.loot.draw(&player);
	}
	else if (map.type == 5) { // shop
		ClearBackground(GRAY);
		map.shop.draw(&player);
	}
	else if (map.type == 7){ // collection
		ClearBackground(GRAY);
		collection.draw();
	}
	else if (map.type == map.END) { // rewardScreen cast
		ClearBackground(WHITE);
		rwdScreen.draw();
		if (rwdScreen.end) {
			player.stats.gold += rwdScreen.gold;
			rwdScreen.gold = 0;
			map.type = map.START;
		}
	}
	else if (map.type == 8) { // game over
		ClearBackground(BLACK);
		DrawText("GAME OVER!", 50, 400, 200, RED);
	}
	else { // map
		ClearBackground(GRAY);
		map.drawRect();
	}

}

