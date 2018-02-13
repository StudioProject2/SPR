#include "Monster.h"
#include <cstdlib>

//CHANGE THEO2

Monster::Monster()
{
	float spawnPtX = 300;
	float spawnPtZ = 100;

	spawnPtX = (rand() % 1400 - 700) + 1.0;
	spawnPtZ = (rand() % 1400 - 700) + 1.0;

	pos = Vector3(spawnPtX, 0, spawnPtZ);
	dirChanger = 0;
	moveRight = true;
	monsterDirTime = 0.0;
}


Monster::~Monster()
{
}

void Monster::move(Vector3 camView)
{
	Vector3 temp;
	target = camView;
	temp = (camView - pos).Normalize();
	pos.x = pos.x + temp.x;
	pos.z = pos.z + temp.z;
}

void Monster::moveRand(Vector3 camView, double elaspeTime)
{
	//Vector for movement
	Vector3 up = Vector3(0, 1, 0);
	Vector3 view = (target - pos).Normalized();
	Vector3 right = view.Cross(up);

	//Calculations
	Vector3 temp;
	target = camView;
	temp = (camView - pos).Normalize();

	if (moveRight == true)
	{
		pos = pos + right;
		pos.x = pos.x + temp.x;
		pos.z = pos.z + temp.z;
	}
	if (moveRight == false)
	{
		pos = pos - right;
		pos.x = pos.x + temp.x;
		pos.z = pos.z + temp.z;
	}

	if (elaspeTime > monsterDirTime)
	{
		if (moveRight == true)
			moveRight = false;
		else if (moveRight == false)
			moveRight = true;

		monsterDirTime = elaspeTime + 2.0;
	}
}

