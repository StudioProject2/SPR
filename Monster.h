#ifndef MONSTER_H
#define MONSTER_H

#include "Vector3.h"
class Monster
{
public:
	Monster();
	~Monster();
	enum direction
	{
		LEFT,
		RIGHT
	};

	void move(Vector3 view);
	void moveRand(Vector3 view, double elaspeTime);

	Vector3 pos;
	Vector3 target;
	bool isAlive;
	int dirChanger;
	bool moveRight;
	double monsterDirTime;
	
};

#endif 