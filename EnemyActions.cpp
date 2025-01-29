#include "EnemyActions.h"

#include "Enemy.h"
#include "Player.h"



void ActionStats::draw(Enemy *enemy) {
	if (enemy->stats.armor > 0) {
		DrawText(TextFormat("Armor: %d", enemy->stats.armor), enemy->body.x + 5, enemy->body.y + 60, 4, BLACK);

	}
}



/*void ActionStats::addArmor(Enemy* target, int amount) {
	this->amount = amount;
	//std::cout << "\ntargets segment index: " << target->stats.segment_index << " arm: " << target->stats.armor << " + " << this->amount << "\n";
	target->stats.armor += this->amount;
	std::cout << "Armor after increase: " << target->stats.armor << " " << target->stats.name << "\n";


	
	//std::cout << target->actions.armor.armor << "  " << target->stats.name << "\n addArmor triggered";
}

void ActionStats::healTarget(Enemy* target, int amount) {
	this->amount = amount;
	std::cout << "\nHealed: " << target->stats.segment_index <<" from: " << target->stats.health;
	if (target->stats.health + this->amount >= target->stats.max_health) {
		target->stats.health = target->stats.max_health;
	}
	else {
		target->stats.health += this->amount;
	}
	std::cout << " health to: " << target->stats.health << " health\n";
}

int ActionStats::dealDamage(Player* player, bool ignArm, Enemy *self) {
	std::cout << self->stats.name << " attacking player for: " << amount << " of damage!\n";
	int tempAmount = amount;
	if (player->stats.parry > 0) {
		std::cout << "Player parried the attack!\n";
		if (tempAmount - self->stats.parry <= 0) {
			int rem = player->stats.parry;
			if (self->stats.armor > 0) {
				if (rem - self->stats.armor > 0) {
					self->stats.armor = 0;
					self->stats.health -= (rem - self->stats.armor);
				}
				else {
					self->stats.armor -= rem;
				}
			}
			else {
				self->stats.health -= rem;
			}
		}
	}

	int rem = player->stats.armor;
	tempAmount = amount;
	if (rem > 0 && ignArm == false) {
		if (rem - tempAmount > 0) {
			player->stats.armor = rem - tempAmount;
			return 0;
		}
		else {
			player->stats.armor = 0;
			rem = tempAmount - rem;
			player->stats.health -= rem;
			return rem;
		}
	}
	player->stats.health -= tempAmount;
	return tempAmount;
}

void ActionStats::lifeSteal(Player* player, Enemy* self) {
	std::cout << self->stats.name << " casts: Life_Steal on Player\n";
	int rem = dealDamage(player, false, self);
	healTarget(self, rem);

}

void ActionStats::bleed(Player* player, int amount) {
	std::cout << "Inflicting bleed on player!\n";
	int rem;
	for (int i = 0; i < amount; i++) {
		rem = GetRandomValue(1, 10);
		if (rem > 7) { // 30% chance per amount to inflict bleed
			player->stats.bleed_stacks += 1;
			std::cout << "Bleed on player inflicted!\n";
		}
		else {
			std::cout << "Bleed on player failed!\n";
		}
	}
}

void ActionStats::fire(Player* player, int amount) {
	std::cout << "Inflicting fire on player!\n";
	if (GetRandomValue(amount * 10, 100) > 50) {
		player->stats.fire_stacks += 1;
		std::cout << "Fire on player inflicted!\n";

	}
	else {
		std::cout << "Fire on player failed!\n";

	}
	if (player->stats.fire_stacks >= 3 + player->stats.health / 20) {
		system("cls");
		std::cout << "PLAYER BURNED TO DEATH HAHAHA";
		player->stats.health = -666;
	}
}

void ActionStats::parry(Enemy* self) {
	std::cout << self->stats.name << " prepparing parry!\n";

	self->stats.parry = this->amount;
}

void ActionStats::boost(Enemy* target, int amount) {
	for (int i = 0; i < 4; i++) {
		if (target->actions[i].type == "dmg" || target->actions[i].type == "dmg_ign" || target->actions[i].type == "life_steal") {
			target->actions[i].amount += amount;
			target->actions[i].boosted = true;
			target->actions[i].boostedAmount = amount;
			std::cout << target->stats.name << " Attack got boosted\n";
			return;
		}
	}
}

void ActionStats::poison(Player* player, int amount) {
	std::cout << "Inflicting poison on Player!\n";
	int rem;
	for (int i = 0; i < amount; i++) {
		rem = GetRandomValue(1, 10);
		if (rem > 7) { // 30% chance per amount to inflict poison
			player->stats.poison_stacks += 1;
			std::cout << "Poison on player inflicted!\n";
		}
		else {
			std::cout << "Poison on player failed!\n";
		}
	}
}


void ActionStats::sneak(Enemy* target) {
	std::cout << target->stats.name << " is now hidden!\n";
	target->stats.sneak = true;
}
*/
enum ActionType2 {
	ARMOR,
	DAMAGE,
	EMPTY,
	DMG_IGNORE_ARMOR,
	HEAL,
	SACRIFICE,
	LIFE_STEAL,
	FIRE,
	POISON,
	BLEED,
	PARRY,
	BOOST,
	SNEAK
};

enum ActionType2 stringToEnum(std::string str) {
	if (str == "arm") {
		return ARMOR;
	}
	else if (str == "dmg") {
		return DAMAGE;
	}
	else if (str == "dmg_ign") {
		return DMG_IGNORE_ARMOR;
	}
	else if (str == "heal") {
		return HEAL;
	}
	else if (str == "life_steal") {
		return LIFE_STEAL;
	}
	else if (str == "fire") {
		return FIRE;
	}
	else if (str == "poison") {
		return POISON;
	}
	else if (str == "bleed") {
		return BLEED;
	}
	else if (str == "parry") {
		return PARRY;
	}
	else if (str == "boost") {
		return BOOST;
	}
	else if (str == "sneak") {
		return SNEAK;
	}

	return EMPTY;
}

void ActionStats::checkForOthers(Enemy * self, std::vector <Enemy> *others, int round, Player* player) { // triggers actions of enemy affecting self, neighbours or player

	bool checkerinos = true;
	if (self->bleed_stacks > 0) {
		self->stats.health -= self->bleed_stacks * (round + 1);
		std::cout << self->stats.name << " got " << self->bleed_stacks * (round + 1) << " damage from bleeding\n";
	}
			if (self->actions[round].range_left.empty() && self->actions[round].range_right.empty()){ // action affecting only self or player, has no range effects, exits after finished
				switch (stringToEnum(self->actions[round].type)) {
				case DAMAGE:
					doAction.dealDamage(player, self, self->actions[round].amount, false, true);
					break;
				case DMG_IGNORE_ARMOR:
					doAction.dealDamage(player, self, self->actions[round].amount, true, true);
					break;
				case LIFE_STEAL:
					doAction.useLifesteal(player, self, self->actions[round].amount, true);
					break;
				case SNEAK:
					doAction.castSneak(self);
					break;
				case PARRY:
					doAction.setParry(NULL, self, self->actions[round].amount);
					break;
				case FIRE:
					doAction.castFire(player, self, self->actions[round].amount, true);
					break;
				case POISON:
					doAction.castPoison(player, self, self->actions[round].amount, true);
					break;
				case BLEED:
					doAction.castBleed(player, self, self->actions[round].amount, true);
					break;
				}

				if (self->actions[round].sacrifice) { // SACRIFICES target
					std::cout << self->stats.name << " used SACRIFICE!\n";
					self->stats.health = -666;
				}

				if (self->poison_stacks >= 1) { // has poison apllied
					self->stats.health -= self->poison_stacks;
					std::cout << "Dealt " << self->poison_stacks << " of poison damage to: " << self->stats.name << "\n";
					// 30% chance to remove one poison stack
					if (GetRandomValue(1, 10) < 3) {
						self->poison_stacks--;
						std::cout << "Poison stack removed from " << self->stats.name << "\n";
					}
				}
				return;
			}

			for (int i = 0; i < range_right.size(); i++) { // cast action to self, spells CONTAINING RANGE
				if (self->actions[round].range_right.at(i) == 0 && self->actions[round].range_left.at(i) == 0 && checkerinos) { // target in range and checker to not cast effect on self multiple times, casts on self once, then continues to other targets 
					switch (stringToEnum(self->actions[round].type)) {
					case ARMOR:
						doAction.getArmor(NULL, self, self->actions[round].amount);
						break;
					case HEAL:
						doAction.healTarget(NULL, self, self->actions[round].amount);
						break;
					case BOOST:
						doAction.boostTarget(NULL, self, self, NULL, self->actions[round].amount, round);
						break;
					}
					checkerinos = false;
					break;
				}
				else {
					for (int j = 0; j < others->size(); j++) { // casting effect on other targets if given conditions are ok
						if (self->actions[round].range_right.at(i) != 0 && self->stats.segment_index + self->actions[round].range_right.at(i) == others->at(j).stats.segment_index) {
							switch (stringToEnum(self->actions[round].type)) {
							case ARMOR:
								doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
								break;
							case HEAL:
								doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
								break;
							case BOOST:
								doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
								break;
							}
							//std::cout << "Caster seg index: " << self->stats.segment_index << " Range right: " << self->actions[round].range_right.at(i) << " Target index: " << others->at(j).stats.segment_index << "\n";

						}
					}

				}
				
			}

			for (int i = 0; i < range_left.size(); i++) { // same as example upfront
				for (int j = 0; j < others->size(); j++) {
					if (self->actions[round].range_right.at(i) == 0 && self->actions[round].range_left.at(i) == 0 && checkerinos) { // duplicita bo skippovaní indexu 0/0
						switch (stringToEnum(self->actions[round].type)) {
						case ARMOR:
							doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
							break;
						case HEAL:
							doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
							break;
						case BOOST:
							doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
							break;
						}
						checkerinos = false;
						break;
					}
					else {
						if (self->actions[round].range_left.at(i) != 0 && self->stats.segment_index - self->actions[round].range_left.at(i) == others->at(j).stats.segment_index) {
							//std::cout << "Caster seg index: " << self->stats.segment_index << " Range left: " << self->actions[round].range_left.at(i) << " Target index: " << others->at(j).stats.segment_index << "\n";
							switch (stringToEnum(self->actions[round].type)) {
							case ARMOR:
								doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
								break;
							case HEAL:
								doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
								break;
							case BOOST:
								doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
								break;
							}
						}
					}
					
				}
			}

			if (self->poison_stacks >= 1) { // has poison apllied
				self->stats.health -= self->poison_stacks;
				std::cout << "Dealt " << self->poison_stacks << " of poison damage to: " << self->stats.name << "\n";
				// 30% chance to remove one poison stack
				if (GetRandomValue(1, 10) < 3) {
					self->poison_stacks--;
					std::cout << "Poison stack removed from " << self->stats.name << "\n";
				}
			}

			if (self->actions[round].sacrifice) { // sacrifice action
				self->stats.health = -666;
			}
	}

void ActionStats::surviveAction(Enemy *self, std::vector <Enemy>* others, int round,  Player* player) { // penalization for player if enemy survived, (in case that enemy has function)
	std::cout << self->stats.name << " is casting SURVIVE ACTION\n";
		bool checkerinos = true;
		if (self->bleed_stacks > 0) {
			self->stats.health -= self->bleed_stacks * (round + 1);
			std::cout << self->stats.name << " got " << self->bleed_stacks * (round + 1) << " damage from bleeding\n";
		}
		if (self->survive.range_left.empty() && self->survive.range_right.empty()) { // action affecting only self or player, has no range effects, exits after finished
			switch (stringToEnum(self->survive.type)) {
			case DAMAGE:
				doAction.dealDamage(player, self, self->actions[round].amount, false, true);
				break;
			case DMG_IGNORE_ARMOR:
				doAction.dealDamage(player, self, self->actions[round].amount, true, true);
				break;
			case LIFE_STEAL:
				doAction.useLifesteal(player, self, self->actions[round].amount, true);
				break;
			case SNEAK:
				doAction.castSneak(self);
				break;
			case PARRY:
				doAction.setParry(NULL, self, self->actions[round].amount);
				break;
			case FIRE:
				doAction.castFire(player, self, self->actions[round].amount, true);
				break;
			case POISON:
				doAction.castPoison(player, self, self->actions[round].amount, true);
				break;
			case BLEED:
				doAction.castBleed(player, self, self->actions[round].amount, true);
				break;
			}

			if (self->survive.sacrifice) { // SACRIFICES target
				std::cout << self->stats.name << " used SACRIFICE!\n";
				self->stats.health = -666;
			}

			if (self->poison_stacks >= 1) { // has poison apllied
				self->stats.health -= self->poison_stacks;
				std::cout << "Dealt " << self->poison_stacks << " of poison damage to: " << self->stats.name << "\n";
				// 30% chance to remove one poison stack
				if (GetRandomValue(1, 10) < 3) {
					self->poison_stacks--;
					std::cout << "Poison stack removed from " << self->stats.name << "\n";
				}
			}
			return;
		}

		for (int i = 0; i < range_right.size(); i++) { // cast action to self, spells CONTAINING RANGE
			if (self->survive.range_right.at(i) == 0 && self->survive.range_left.at(i) == 0 && checkerinos) { // target in range and checker to not cast effect on self multiple times, casts on self once, then continues to other targets 
				switch (stringToEnum(self->survive.type)) {
				case ARMOR:
					doAction.getArmor(NULL, &others->at(i), self->actions[round].amount);
					break;
				case HEAL:
					doAction.healTarget(NULL, &others->at(i), self->actions[round].amount);
					break;
				case BOOST:
					doAction.boostTarget(NULL, self, &others->at(i), NULL, self->actions[round].amount, round);
					break;
				
				}
				checkerinos = false;
				break;
			}
			else {
				for (int j = 0; j < others->size(); j++) { // casting effect on other targets if given conditions are ok
					if (self->survive.range_right.at(i) != 0 && self->stats.segment_index + self->survive.range_right.at(i) == others->at(j).stats.segment_index) {
						switch (stringToEnum(self->survive.type)) {
						case ARMOR:
							doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
							break;
						case HEAL:
							doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
							break;
						case BOOST:
							doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
							break;
						
						}
						//std::cout << "Caster seg index: " << self->stats.segment_index << " Range right: " << self->actions[round].range_right.at(i) << " Target index: " << others->at(j).stats.segment_index << "\n";

					}
				}

			}

		}

		for (int i = 0; i < range_left.size(); i++) { // same as example upfront
			for (int j = 0; j < others->size(); j++) {
				if (self->survive.range_right.at(i) == 0 && self->survive.range_left.at(i) == 0 && checkerinos) { // duplicita bo skippovaní indexu 0/0
					switch (stringToEnum(self->survive.type)) {
					case ARMOR:
						doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
						break;
					case HEAL:
						doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
						break;
					case BOOST:
						doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
						break;
					}
					checkerinos = false;
					break;
				}
				else {
					if (self->survive.range_left.at(i) != 0 && self->stats.segment_index - self->survive.range_left.at(i) == others->at(j).stats.segment_index) {
						//std::cout << "Caster seg index: " << self->stats.segment_index << " Range left: " << self->survive.range_left.at(i) << " Target index: " << others->at(j).stats.segment_index << "\n";
						switch (stringToEnum(self->survive.type)) {
						case ARMOR:
							doAction.getArmor(NULL, &others->at(j), self->actions[round].amount);
							break;
						case HEAL:
							doAction.healTarget(NULL, &others->at(j), self->actions[round].amount);
							break;
						case BOOST:
							doAction.boostTarget(NULL, self, &others->at(j), NULL, self->actions[round].amount, round);
							break;
						}
					}
				}

			}
		}

		if (self->poison_stacks >= 1) { // has poison apllied
			self->stats.health -= self->poison_stacks;
			std::cout << "Dealt " << self->poison_stacks << " of poison damage to: " << self->stats.name << "\n";
			// 30% chance to remove one poison stack
			if (GetRandomValue(1, 10) < 3) {
				self->poison_stacks--;
				std::cout << "Poison stack removed from " << self->stats.name << "\n";
			}
		}

		if (self->survive.sacrifice) {
			self->stats.health = -666;
		}
	}


