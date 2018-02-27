#ifndef BOSS_H
#define BOSS_H

#include "Vector3.h"
#include "Box.h"
#include "monsterBullet.h"

class Boss
{
private:
	enum direction
	{
		LEFT,
		RIGHT
	};

	int health;

	//for leap
	bool rising;
	bool chasing;
	bool dropping;
	bool leaped;

	//Vector3
	Vector3 pos;
	Vector3 target;
	Vector3 view;

	//Movement 
	bool firstCharge;
	bool startCharge;
	bool moveRight;
	double bossDirTime;
	double bossChargeDelay;
	double bossChargeTime;

	//Collision for mob
	float prevPosX;
	float prevPosY;
	float prevPosZ;
	bool _collidedX;
	bool _collidedY;
	bool _collidedZ;

	//Function Memebers for collision
	bool isPointInBox(Vector3 position, Box box);
	bool isPointXInBox1(Vector3 position, Box box);
	bool isPointYInBox1(Vector3 position, Box box);
	bool isPointZInBox1(Vector3 position, Box box);
	void boundsCheck();
public:
	Boss();
	~Boss();

	//Moveset
	void move(Vector3 camPos);
	void moveZigZag(Vector3 camPos, double elaspeTime);
	void charge(Vector3 camPos, double elaspeTime);
	void leap(Vector3 camPos);
	int getHealth();
	Vector3 getPos();
	void setHealth(int x);
	void resetY();
	void resetVariables();
};

#endif 
