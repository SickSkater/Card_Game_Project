#include "CardActions.h"

#include "Enemy.h"
#include "Player.h"
#include "Card.h"
#include "Boss.h"



void CardActions::castSpells(std::vector<Enemy>* enemies, Card* card, Player* player, int round) { // triggers actions of caster, affecting caster or target if conditions are fullfiled
	bool check = false;
	//std::cout << "|	CARD	|: " << "CAST SPELLS TRIGGERED" << "\n";
	if (card->actions[round].range_left.empty() && card->actions[round].range_right.empty() || card->actions[round].action_type == "BOOST" || card->actions[round].action_type == "PARRY") { // casting on self, boost temporarz until implemented
		std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << "\n";
		if (card->actions[round].action_type == "HEAL") {
			card->doAction.healTarget(player, NULL, card->actions[round].amount);
		}
		else if (card->actions[round].action_type == "BOOST") { // později předávát karty v dosahu
			card->doAction.boostTarget(player, NULL, NULL, card, card->actions[round].amount, round);
		}
		else if (card->actions[round].action_type == "ARMOR") {
			card->doAction.getArmor(player, NULL, card->actions[round].amount);
		}
		else if (card->actions[round].action_type == "MANA") {
			card->doAction.addMana(player, card->actions[round].amount);
		}
		else if (card->actions[round].action_type == "HEAL") {
			card->doAction.healTarget(player, NULL, card->actions[round].amount);
		}
		else if (card->actions[round].action_type == "PARRY") {
			card->doAction.setParry(player, NULL, card->actions[round].amount);
		}

	}
	else {
		if (card->actions[round].range_left.at(0) == 0 && card->actions[round].range_right.at(0) == 0 && !check) { // casting on enemy infront, check to cast only once
			for (int enem = 0; enem < enemies->size(); enem++) {
				if (enemies->at(enem).stats.segment_index == card->shape.shape_segment_index && !enemies->at(enem).stats.sneak) { // enemy in range of card shape, ignoring sneaking enemies
					std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << " at: " << enemies->at(enem).stats.name << "\n";
					if (card->actions[round].action_type == "DMG") {
						card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, false, false);
					}
					else if (card->actions[round].action_type == "DMG_IGN") {
						card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, true, false);
					}
					else if (card->actions[round].action_type == "POISON") {
						card->doAction.castPoison(player, &enemies->at(enem), card->actions[round].amount, false);
					}
					else if (card->actions[round].action_type == "FREEZE") {
						card->doAction.castFreeze(&enemies->at(enem), card->actions[round].amount);
					}
					else if (card->actions[round].action_type == "BOOST") {
						card->doAction.boostTarget(player, NULL, NULL, card, card->actions[round].amount, round);
					}
					else if (card->actions[round].action_type == "FIRE") {
						card->doAction.castFire(player, &enemies->at(enem), card->actions[round].amount, false);
					}
					else if (card->actions[round].action_type == "PARRY") {
						card->doAction.setParry(player, NULL, card->actions[round].amount);
					}
					else if (card->actions[round].action_type == "LIFE_STEAL") {
						card->doAction.useLifesteal(player, &enemies->at(enem), card->actions[round].amount, false);
					}
					else if (card->actions[round].action_type == "BLEED") {
						card->doAction.castBleed(player, &enemies->at(enem), card->actions[round].amount, false);
					}
				}
			}
			check = true;
		}
			for (int i = 0; i < card->actions[round].range_left.size(); i++) { // cast spells if target on left is present, ignores 0 range value
				for (int enem = 0; enem < enemies->size(); enem++) {
					//std::cout << "|	CARD	|: " << "Shape index:" << card->shape.shape_segment_index << " range left: " << card->actions[round].range_left.at(i) << " Enemy index: " << enemies->at(enem).stats.segment_index << "\n";
					if (enemies->at(enem).stats.segment_index == card->shape.shape_segment_index - card->actions[round].range_left.at(i) && card->actions[round].range_left.at(i) != 0 && !enemies->at(enem).stats.sneak) { // enemy in range of card shape
						std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << " at: " << enemies->at(enem).stats.name << "\n";

						if (card->actions[round].action_type == "DMG") {
							card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, false, false);
						}
						else if (card->actions[round].action_type == "DMG_IGN") {
							card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, true, false);
						}
						else if (card->actions[round].action_type == "POISON") {
							card->doAction.castPoison(player, &enemies->at(enem), card->actions[round].amount, false);

						}
						else if (card->actions[round].action_type == "FREEZE") {
							card->doAction.castFreeze(&enemies->at(enem), card->actions[round].amount);
						}
						else if (card->actions[round].action_type == "BOOST") {
							card->doAction.boostTarget(player, NULL, NULL, card, card->actions[round].amount, round);
						}
						else if (card->actions[round].action_type == "FIRE") {
							card->doAction.castFire(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						else if (card->actions[round].action_type == "PARRY") {
							card->doAction.setParry(player, NULL, card->actions[round].amount);
						}
						else if (card->actions[round].action_type == "LIFE_STEAL") {
							card->doAction.useLifesteal(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						else if (card->actions[round].action_type == "BLEED") {
							card->doAction.castBleed(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						//std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << "\n";

					}
				}
			}
			for (int i = 0; i < card->actions[round].range_right.size(); i++) { // same as above but right
				for (int enem = 0; enem < enemies->size(); enem++) {
					//std::cout << "|	CARD	|: " << "Shape index:" << card->shape.shape_segment_index << " range right: " << card->actions[round].range_right.at(i) << " Enemy index: " << enemies->at(enem).stats.segment_index << "\n";
					if (enemies->at(enem).stats.segment_index == card->shape.shape_segment_index + card->actions[round].range_right.at(i) && card->actions[round].range_right.at(i) != 0 && !enemies->at(enem).stats.sneak) {
						std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << " at: " << enemies->at(enem).stats.name << "\n";
						if (card->actions[round].action_type == "DMG") {
							card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, false, false);
						}
						else if (card->actions[round].action_type == "DMG_IGN") {
							card->doAction.dealDamage(player, &enemies->at(enem), card->actions[round].amount, true, false);
						}
						else if (card->actions[round].action_type == "POISON") {
							card->doAction.castPoison(player, &enemies->at(enem), card->actions[round].amount, false);

						}
						else if (card->actions[round].action_type == "FREEZE") {
							card->doAction.castFreeze(&enemies->at(enem), card->actions[round].amount);
						}
						else if (card->actions[round].action_type == "BOOST") {
							card->doAction.boostTarget(player, NULL, NULL, card, card->actions[round].amount, round);
						}
						else if (card->actions[round].action_type == "FIRE") {
							card->doAction.castFire(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						else if (card->actions[round].action_type == "PARRY") {
							card->doAction.setParry(player, NULL, card->actions[round].amount);
						}
						else if (card->actions[round].action_type == "LIFE_STEAL") {
							card->doAction.useLifesteal(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						else if (card->actions[round].action_type == "BLEED") {
							card->doAction.castBleed(player, &enemies->at(enem), card->actions[round].amount, false);
						}
						//std::cout << "|	CARD	|: " << "Card: " << card->name << " Casting: " << card->actions[round].action_type << "\n";

					}
				}
			}
	}
	

}


/*
int CardActions::dealDamage(Enemy* target, bool ignore, Player* player) {
	std::cout << "|	CARD	|: " << "Inflicted " << amount << " of damage to: " << target->stats.name << "\n";
	int tempAmount = amount;
	if (target->stats.parry > 0) {
		if (tempAmount - target->stats.parry <= 0) {
			int rem = target->stats.parry;
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

	if (ignore) {
		target->stats.health -= amount;
		return 0;

	}
	else {
		int rem = amount;
		if (target->stats.armor > 0) {
			//std::cout << "|	CARD	|: " << "Has Armor\n";
			if (rem - target->stats.armor > 0) {
				//std::cout << "|	CARD	|: " << "Armor breached\n";

				target->stats.armor = 0;
				target->stats.health -= (rem - target->stats.armor);
				return (rem - target->stats.armor);
			}
			else {
				//std::cout << "|	CARD	|: " << "Armor lowered\n";
				target->stats.armor -= rem;
				return 0;
			}
		}
		else {
			//std::cout << "|	CARD	|: " << "No Armor\n";
			target->stats.health -= amount;
			return rem;
		}
	}
}

void CardActions::addArmor(Player* player) {
	player->stats.armor += amount;
	std::cout << "|	CARD	|: " << "Added " << amount << " armor to player\n";
}

void CardActions::heal(Player* player) {
	if (player->stats.health + amount >= player->stats.max_health) {
		player->stats.health = player->stats.max_health;
	}
	else {
		player->stats.health += amount;
	}
	std::cout << "|	CARD	|: " << "Replenished health\n";
}

void CardActions::addMana(Player* player) {
	if (player->stats.mana + amount >= player->stats.max_mana) {
		player->stats.mana = player->stats.max_mana;
	}
	else {
		player->stats.mana += amount;
	}
	std::cout << "|	CARD	|: " << "Replenished mana\n";
}

void CardActions::boostDamage(Card *card) {
	for (int i = 0; i < 4; i++) {
		if (card->actions[i].action_type == "DMG" || card->actions[i].action_type == "DMG_IGN") {
			card->actions[i].amount += this->amount;
			card->actions[i].boosted = true;
			card->actions[i].boostedAmount = amount;
			return;
		}
	}
}

void CardActions::freeze(Enemy* enemy, int round) { // prozatim nastavení na EMPTY, později vytvořit vlastní metodu
	enemy->actions[round].type = "act_empty";
	std::cout << "|	CARD	|: " << "Freeze on " << enemy->stats.name << " - action on round: " << round << " inflicted!\n";
}

void CardActions::poison(Enemy* enemy) {
	enemy->poison_stacks += 1;
	std::cout << "|	CARD	|: " << "Poison on " << enemy->stats.name << " inflicted!\n";

}

void CardActions::lifeSteal(Player* player, Enemy* target) { // dmg + heal z dmg
	int rem = dealDamage(target, false, player);
	if (player->stats.health + rem >= player->stats.max_health) {
		player->stats.health = player->stats.max_health;
	}
	else {
		player->stats.health += rem;
		std::cout << "|	CARD	|: " << "Lifesteal for: " << rem << " health\n";

	}
}

void CardActions::fire(Enemy* enemy) {
	if (GetRandomValue(amount * 10, 100) > 50) {
		enemy->fire_stacks += 1;
		std::cout << "|	CARD	|: " << "Fire on enemy" << enemy->stats.name << " inflicted!\n";

	}
	else {
		std::cout << "|	CARD	|: " << "Fire on enemy" << enemy->stats.name << " failed!\n";

	}
	if (enemy->fire_stacks >= 3 + enemy->stats.max_health / 20) {
		enemy->stats.health = -666;
	}

}

void CardActions::parry(Player *player) {
	std::cout << "|	CARD	|: " << "Prepparing parry!\n";

	player->stats.parry = this->amount;
}

void CardActions::bleed(Enemy* enemy) {
	int rem;
	for (int i = 0; i < amount; i++) {
		rem = GetRandomValue(1, 10);
		if (rem > 7) { // 30% chance per amount to inflict bleed
			enemy->bleed_stacks += 1;
			std::cout << "|	CARD	|: " << "Bleed on enemy" << enemy->stats.name << " inflicted!\n";
		}
		else {
			std::cout << "|	CARD	|: " << "Bleed on enemy" << enemy->stats.name << " failed!\n";
		}
	}
}
*/