#ifndef MONSTERBULLET_H
#define MONSTERBULLET_H

#include "Vector3.h"
#include "Monster.h"
#include "Box.h"

class monsterBullet
{
public:
	monsterBullet();
	monsterBullet(Monster*, Vector3);
	~monsterBullet();

	void move();
	bool isBulletInBox(Box box);
	bool bulletCollide();

	Vector3 pos;
	Vector3 target;
};

#endif 