#include "Boss.h"


Boss::Boss() : Enemy(NULL){
	this->stats.health = 100;
	this->stats.max_health = 100;
	this->fire_imune = true;
	this->freeze_imune = true;
	this->actions[0];
}
