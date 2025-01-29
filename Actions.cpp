#include "Actions.h"
#include "Player.h"
#include "Enemy.h"
#include "Card.h"

int Actions::dealDamage(Player* player, Enemy* enemy, int amount, bool ignoreArmor, bool targetPlayer) { // deal amount of damage to target, break, or lower armor, enemy NULL for item actions, if target has parry active, it returns part of damage to caster
	if (enemy == NULL) {
		std::cout << "|	ACTION	|: " << "PLAYER recived: " << amount << " of damage!\n";
		int tempAmount = amount;
		int rem = player->stats.armor;
		tempAmount = amount;
		if (rem > 0 && ignoreArmor == false) {
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
		else {
			player->stats.health -= tempAmount;
			return tempAmount;
		}
	}
	else {


		if (targetPlayer) {
			std::cout << "|	ACTION	|: " << enemy->stats.name << " attacking player for: " << amount << " of damage!\n";
			int tempAmount = amount;
			if (player->stats.parry > 0) { // parry efect
				if (tempAmount - enemy->stats.parry <= 0) {
					int rem = player->stats.parry;
					std::cout << "|	ACTION	|: " << "Player parried the attack! " << " Returning: " << rem << " of damage!\n";
					if (enemy->stats.armor > 0) {
						if (rem - enemy->stats.armor > 0) {
							enemy->stats.armor = 0;
							enemy->stats.health -= (rem - enemy->stats.armor);
						}
						else {
							enemy->stats.armor -= rem;
						}
					}
					else {
						enemy->stats.health -= rem;
					}
				}
			}

			int rem = player->stats.armor;
			tempAmount = amount;
			if (rem > 0 && ignoreArmor == false) {
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
			else {
				player->stats.health -= tempAmount;
				return tempAmount;
			}

			if (ignoreArmor) {
				std::cout << "|	ACTION	|: " << "Ignoring armor!\n";
				player->stats.health -= tempAmount;
				return tempAmount;

			}

		}
		else {
			std::cout << "|	ACTION	|: " << "Inflicted " << amount << " of damage to: " << enemy->stats.name << "\n";
			int tempAmount = amount;
			if (enemy->stats.parry > 0) { // parry logic
				if (tempAmount - enemy->stats.parry <= 0) {
					int rem = enemy->stats.parry;
					std::cout << enemy->stats.name << " parried the attack, returning: " << rem << " of damage!\n";
					if (player->stats.armor > 0) {
						if (rem - player->stats.armor > 0) {
							player->stats.armor = 0;
							player->stats.health -= (rem - player->stats.armor);
						}
						else {
							player->stats.armor -= rem;
						}
					}
					else {
						player->stats.health -= rem;
					}
				}
			}

			if (ignoreArmor) {
				std::cout << "|	ACTION	|: " << "Ignoring armor!\n";
				enemy->stats.health -= amount;
				return amount;
			}
			else {
				int rem = amount;
				if (enemy->stats.armor > 0) {
					//std::cout << "Has Armor\n";
					if (rem - enemy->stats.armor > 0) {
						//std::cout << "Armor breached\n";

						enemy->stats.armor = 0;
						enemy->stats.health -= (rem - enemy->stats.armor);
						return (rem - enemy->stats.armor);
					}
					else {
						//std::cout << "Armor lowered\n";
						enemy->stats.armor -= rem;
						return 0;
					}
				}
				else {
					//std::cout << "No Armor\n";
					enemy->stats.health -= amount;
					return amount;
				}
			}
		}
	}
}

void Actions::healTarget(Player* player, Enemy* enemy, int amount) { // heals target with checking for maximum cap
	if (player == NULL) { // heal enemy
		std::cout << "|	ACTION	|: " << "Healing " << enemy->stats.name << " for: " << amount << " of health!\n";
		enemy->stats.health += amount;
		if (enemy->stats.health > enemy->stats.max_health) {
			enemy->stats.health = enemy->stats.max_health;
		}

	}
	else { // heal player
		std::cout << "|	ACTION	|: " << "Healing PLAYER for: " << amount << " of health!\n";
		if (player->stats.max_health - player->stats.health >= 2) {
			player->healedHealth = amount;
		}
		player->stats.health += amount;
		if (player->stats.health > player->stats.max_health) {
			player->stats.health = player->stats.max_health;
		}
	}
}


void Actions::useLifesteal(Player* player, Enemy* enemy, int amount, bool targetPlayer) { // heals caster for damage dealt to target
	std::cout << "|	ACTION	|: " << "Casting lifesteal!\n";
	if (targetPlayer) {
		healTarget(NULL, enemy, dealDamage(player, enemy, amount, false, true));
	}
	else {
		healTarget(player, enemy, dealDamage(player, enemy, amount, false, false));
	}
}

void Actions::getArmor(Player* player, Enemy* enemy, int amount) { // add x armor to target
	if (player == NULL) { // cast on enemy
		enemy->stats.armor += amount;
		std::cout << "|	ACTION	|: " << enemy->stats.name << " recived: " << amount << " of armor!\n";
	}
	else { // cast on player
		player->addedArmor = amount;
		player->stats.armor += amount;
		std::cout << "|	ACTION	|: " << "PLAYER recived: " << amount << " of armor!\n";

	}
}

void Actions::boostTarget(Player* player, Enemy* enemy, Enemy *target, Card * castingCard, int amount, int rnd) { // boosts damaging actions for X amount, has chance to boost multiple spells of target
	if (player == NULL) { //boost enemy attack in range of caster
		for (int round = 0; round < 4; round++) {
			if (target->actions[round].type == "dmg" || target->actions[round].type == "dmg_ign" || target->actions[round].type == "life_steal") {
				target->actions[round].amount += amount;
				target->actions[round].boosted = true; // draws blue rectangle on action name to show boost
				target->actions[round].boostedAmount = amount;
				if (GetRandomValue(1, 3) != 3) { // 33% chance to continue boosting attacks if possible
					break;
				}
			}
		}
	}
	else { // boost Players active cards in range of caster
		// for (int cofActiveCards = 0; player->current_deck.getActiveCards().size(); cofActiveCards++) 
			if (castingCard->actions[rnd].range_left.at(0) == 0 && castingCard->actions[rnd].range_right.at(0) == 0) {	// casts on self and all other cards at same segment
				for (int cofActiveCards = 0; player->current_deck.getActiveCards().size(); cofActiveCards++) {
					if (castingCard->shape.shape_segment_index == player->current_deck.getActiveCards().at(cofActiveCards).shape.shape_segment_index) { // affected cards are on same index as caster
						for (int round = 0; round < 4; round++) {
							if (player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg_ign" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "life_steal") {
								std::cout << "|	ACTION	|: " << castingCard->name << " boosted: " << player->current_deck.getActiveCards().at(cofActiveCards).name << " attack on round: " << round << " for: " << amount << " of damage!\n";
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].amount += amount;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boosted = true;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boostedAmount = amount;
								if (GetRandomValue(1, 3) != 3) { // 33% chance to continue boosting attacks if possible
									break;
								}
							}
						}
					}
				}
			}
			for (int rangeLeftIndex = 0; rangeLeftIndex < castingCard->actions[rnd].range_left.size(); rangeLeftIndex++) { // casts boost on active cards left to target
				for (int cofActiveCards = 0; player->current_deck.getActiveCards().size(); cofActiveCards++) {
					if (castingCard->actions[rnd].range_left.at(rangeLeftIndex) - castingCard->shape.shape_segment_index == player->current_deck.getActiveCards().at(cofActiveCards).shape.shape_segment_index) {
						for (int round = 0; round < 4; round++) {
							if (player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg_ign" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "life_steal") {
								std::cout << "|	ACTION	|: " << castingCard->name << " boosted: " << player->current_deck.getActiveCards().at(cofActiveCards).name << " attack on round: " << round << " for: " << amount << " of damage!\n";
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].amount += amount;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boosted = true;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boostedAmount = amount;
								if (GetRandomValue(1, 3) != 3) { // 33% chance to continue boosting attacks if possible
									break;
								}
							}
						}
					}
				}
			}

			for (int rangeRightIndex = 0; rangeRightIndex < castingCard->actions[rnd].range_left.size(); rangeRightIndex++) { // casts boost on active cards right to target
				for (int cofActiveCards = 0; player->current_deck.getActiveCards().size(); cofActiveCards++) {
					if (castingCard->actions[rnd].range_left.at(rangeRightIndex) + castingCard->shape.shape_segment_index == player->current_deck.getActiveCards().at(cofActiveCards).shape.shape_segment_index) {
						for (int round = 0; round < 4; round++) {
							if (player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "dmg_ign" || player->current_deck.getActiveCards().at(cofActiveCards).actions[round].action_type == "life_steal") {
								std::cout << "|	ACTION	|: " << castingCard->name << " boosted: " << player->current_deck.getActiveCards().at(cofActiveCards).name << " attack on round: " << round << " for: " << amount << " of damage!\n";
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].amount += amount;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boosted = true;
								player->current_deck.getActiveCards().at(cofActiveCards).actions[round].boostedAmount = amount;
								if (GetRandomValue(1, 3) != 3) { // 33% chance to continue boosting attacks if possible
									break;
								}
							}
						}
					}
				}
			}

		
	}
}

void Actions::castPoison(Player* player, Enemy* enemy, int amount, bool targetPlayer) { // adds poison to target if passed, poison affects target by health per stack after casting actions
	if (targetPlayer) { // apply on player
		std::cout << "|	ACTION	|: " << "Inflicting poison on Player!\n";
		int rem;
		for (int i = 0; i < amount; i++) {
			rem = GetRandomValue(1, 10);
			if (rem > 7) { // 30% chance per amount to inflict poison
				if (GetRandomValue(0, 10) < player->stats.poison_res) {
					std::cout << "|	ACTION	|: " << "Player deflected poison!\n";

				}
				else {
					player->stats.poison_stacks += 1;
					std::cout << "|	ACTION	|: " << "Poison on player inflicted!\n";
				}

			}
			else {
				std::cout << "|	ACTION	|: " << "Poison on player failed!\n";
			}
		}
	}
	else { // apply on enemy
		enemy->poison_stacks += 1;
		std::cout << "|	ACTION	|: " << "Poison on " << enemy->stats.name << " inflicted!\n";
	}
}

void Actions::castFire(Player* player, Enemy* enemy, int amount, bool targetPlayer) { //adds fire to target if passed, if target has 3 stacks + maxHealth / x target dies 
	if (targetPlayer) { // inflict fire on player
		std::cout << "|	ACTION	|: " << enemy->stats.name << " is inflicting fire on player!\n";
		if (GetRandomValue(amount * 10, 100) > 50) { // minimal 10% chance to apply fíre stack
			if (GetRandomValue(0, 10) < player->stats.fire_res) {
				std::cout << "|	ACTION	|: " << "Player deflected fire!\n";

			}
			else {
				player->stats.fire_stacks += 1;
				std::cout << "|	ACTION	|: " << "Fire on player inflicted!\n";
			}


		}
		else {
			std::cout << "|	ACTION	|: " << "Fire on player failed!\n";

		}
		if (player->stats.fire_stacks >= 3 + player->stats.max_health / 20) {
			system("cls");
			std::cout << "|	ACTION	|: " << "PLAYER BURNED TO DEATH HAHAHA";
			player->stats.health = -666;
		}
	}
	else { // inflict fire on enemy
		if (GetRandomValue(amount * 10, 100) > 50) { // minimal 10% chance to apply fíre stack
			enemy->fire_stacks += 1;
			std::cout << "|	ACTION	|: " << "Fire on enemy" << enemy->stats.name << " inflicted!\n";

		}
		else {
			std::cout << "|	ACTION	|: " << "Fire on enemy" << enemy->stats.name << " failed!\n";

		}
		if (enemy->fire_stacks >= 2 + enemy->stats.max_health / 20) {
			std::cout << "|	ACTION	|: " << enemy->stats.name << " BURNED TO DEATH!\n";
			enemy->stats.health = -666;
		}
	}
}

void Actions::castBleed(Player* player, Enemy* enemy, int amount, bool targetPlayer) { // adds bleed to target if passed, deals damage per stack by round ( amount * round )
	if (targetPlayer) { // inflict bleed on player
		std::cout << "|	ACTION	|: " << enemy->stats.name << " is inflicting bleed on player!\n";
		int rem;
		for (int i = 0; i < amount; i++) {
			rem = GetRandomValue(1, 10);
			if (rem > 7) { // 30% chance per amount to inflict bleed
				if (GetRandomValue(0, 10) < player->stats.bleed_res) {
					std::cout << "|	ACTION	|: " << "Player deflected bleed!\n";

				}
				else {
					player->stats.bleed_stacks += 1;
					std::cout << "|	ACTION	|: " << "Bleed on player inflicted!\n";
				}

			}
			else {
				std::cout << "|	ACTION	|: " << "Bleed on player failed!\n";
			}
		}
	}
	else { // inflict bleed on enemy
		int rem;
		for (int i = 0; i < amount; i++) {
			rem = GetRandomValue(1, 10);
			if (rem > 7) { // 30% chance per amount to inflict bleed
				enemy->bleed_stacks += 1;
				std::cout << "|	ACTION	|: " << "Bleed on enemy" << enemy->stats.name << " inflicted!\n";
			}
			else {
				std::cout << "|	ACTION	|: " << "Bleed on enemy" << enemy->stats.name << " failed!\n";
			}
		}
	}
}

void Actions::castSneak(Enemy* enemy) { // sets target untargetable for actions
	enemy->stats.sneak = true;
	std::cout << "|	ACTION	|: " << enemy->stats.name << " is now HIDDEN!\n";
}

void Actions::castFreeze(Enemy* enemy, int amount) { // removes action from target if passed
	for (int i = 0; i < amount; i++) {
		if (GetRandomValue(1, 100) > 70) { // 30% to apply on random round action
			enemy->actions[GetRandomValue(0, 3)].type = "act_empty";
		}
	}
}

void Actions::setParry(Player* player, Enemy* enemy, int amount) { // sets parry to active state for 1 round ( parry returns part of damage dealt to caster
	if (player == NULL) { // cast on ENEMY
		enemy->stats.parry = amount;
	}
	if (enemy == NULL) {// cast on PLAYER 
		player->stats.parry = amount;
	}
}

void Actions::addMana(Player* player, int amount) { // replenishes mana for amount
	std::cout << "|	ACTION	|: " << "Replenished: " << amount << " of mana!\n";
	player->stats.mana += amount;
	if (player->stats.mana > player->stats.max_mana) {
		player->stats.mana = player->stats.max_mana;
	}
}