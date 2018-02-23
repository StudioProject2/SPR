#ifndef MONSTER_H
#define MONSTER_H

#include "Vector3.h"
#include "Box.h"
class Monster
{
public:
	Monster();
	Monster(Vector3);
	~Monster();
	enum direction
	{
		LEFT,
		RIGHT
	};
	int health;

	virtual void move(Vector3 view);
	virtual void moveRand(Vector3 view, double elaspeTime);

	//Vector3
	Vector3 pos;
	Vector3 target;
	Vector3 view;

	//Movement 
	bool firstSpawn;
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
	//Function Memebers for collision
	bool isPointInBox(Vector3 position, Box box);
	bool isPointXInBox1(Vector3 position, Box box);
	bool isPointYInBox1(Vector3 position, Box box);
	bool isPointZInBox1(Vector3 position, Box box);
	void boundsCheckStage1();
	void boundsCheckStage2();
	void boundsCheckStage3();
	void boundsCheckBoss();
};

#endif 