#ifndef MONSTER_FODDER_H
#define MONSTER_FODDER_H

#include "Vector3.h"
#include "Monster.h"
#include "Box.h"

class MonsterFodder : public Monster
{
public:
	MonsterFodder();
	~MonsterFodder();

	int health;
	void move(Vector3 view);
	void moveRand(Vector3 view, double elaspeTime);

};

#endif 