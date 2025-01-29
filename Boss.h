#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Enemy.h"


class Boss:public Enemy
{
public:
	int a;
	Boss();
	void createBoss();
};

