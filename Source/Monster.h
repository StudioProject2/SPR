#ifndef MONSTER_H
#define MONSTER_H

#include "Vector3.h"
#include "Box.h"
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
	void boundsCheck();

	Vector3 pos;
	Vector3 target;

	bool isAlive;
	int dirChanger;
	bool moveRight;
	double monsterDirTime;
	
	//Collision for mob
	double prevPosX;
	double prevPosY;
	double prevPosZ;
	bool _collidedX;
	bool _collidedY;
	bool _collidedZ;

};

#endif 