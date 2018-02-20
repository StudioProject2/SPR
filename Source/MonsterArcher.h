#ifndef MONSTER_ARCHER_H
#define MONSTER_ARCHER_H

#include "Vector3.h"
#include "Monster.h"
#include "Box.h"

class MonsterArcher : public Monster
{
public:
	MonsterArcher();
	~MonsterArcher();

	int health;
	void move(Vector3 view);
	void moveRand(Vector3 view, double elaspeTime);
	int direction;
};

#endif