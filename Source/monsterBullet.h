#ifndef MONSTERBULLET_H
#define MONSTERBULLET_H

#include "Vector3.h"
#include "Monster.h"
#include "Box.h"

class monsterBullet
{
public:
	monsterBullet();
	monsterBullet(Vector3, Vector3);
	~monsterBullet();

	void move();
	bool isBulletInBox(Box box);
	bool bulletCollide();
	bool bulletCollideStage1();
	bool bulletCollideStage2();
	bool bulletCollideStage3();
	bool bulletCollideStage4();

	Vector3 pos;
	Vector3 target;
};

#endif 