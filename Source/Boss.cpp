#include "Boss.h"
#include "Application.h"
#include "monsterBullet.h"
#include <cstdlib>

bool Boss::isPointInBox(Vector3 position, Box box)
{
	return (position.x >= box.minX&&position.x <= box.maxX) &&
		(position.y >= box.minY&&position.y <= box.maxY) &&
		(position.z >= box.minZ && position.z <= box.maxZ);
}

Boss::Boss()
{
	health = 1000;

	//Vector3
	Vector3 pos = Vector3(0, 0, 100);
	Vector3 target;
	Vector3 view;

	//Movement 
	firstSpawn = true;
	dirChanger = 0;
	moveRight = false;
	bossDirTime = 0.0;
	startCharge = false;
	bossChargeDelay = 0.0;

	//for leap
	rising = false;
	chasing = false;
	dropping = false;
	leaped = false;

	//Collision for mob
	prevPosX = 0.0;
	prevPosY = 0.0;
	prevPosZ = 0.0;
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;
}

Boss::~Boss()
{
}

bool Boss::isPointXInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX - 1 && position.x <= box.minX + 1)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.maxX - 1 && position.x <= box.maxX + 1)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Boss::isPointYInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY - 1 && position.y <= box.minY + 1)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.maxY - 1 && position.y <= box.maxY + 1)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Boss::isPointZInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ - 1 && position.z <= box.minZ + 1))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.maxZ - 1 && position.z <= box.maxZ + 1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Boss::move(Vector3 camPos)
{
	prevPosX = pos.x;
	prevPosY = pos.y;
	prevPosZ = pos.z;

	Vector3 temp;
	target = camPos;
	temp = (camPos - pos).Normalize();
	pos.x = pos.x + temp.x;
	pos.z = pos.z + temp.z;

	boundsCheck();
}

void Boss::moveZigZag(Vector3 camPos, double elaspeTime)
{
	//PrevPositions For Ai
	prevPosX = pos.x;
	prevPosY = pos.y;
	prevPosZ = pos.z;

	//Vector for movement
	target = camPos;

	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = view.Cross(up);
	view = (target - pos).Normalized();


	if (moveRight == true)
	{
		pos = pos + right;
		pos.x = pos.x + view.x;
		pos.z = pos.z + view.z;
	}
	if (moveRight == false)
	{
		pos = pos - right;
		pos.x = pos.x + view.x;
		pos.z = pos.z + view.z;
	}

	if (elaspeTime > bossDirTime)
	{
		if (moveRight == true)
			moveRight = false;
		else if (moveRight == false)
			moveRight = true;

		bossDirTime = elaspeTime + 2.0;
	}

	boundsCheck();

}

void Boss::charge(Vector3 camPos, double elaspeTime)
{
	if (!firstCharge)
	{
		bossChargeDelay = elaspeTime + 1.5;
		firstCharge = true;
	}
	if (elaspeTime > bossChargeDelay)
	{
		if (!startCharge)
		{
			chargeTarget = camPos;
			view = (chargeTarget - pos).Normalize();
			bossChargeTime = elaspeTime + 1.0;
			startCharge = true;
		}
		prevPosX = pos.x;
		prevPosY = pos.y;
		prevPosZ = pos.z;

		pos.x = pos.x + view.x * 9;
		pos.z = pos.z + view.z * 9;

		boundsCheck();
		if (elaspeTime > bossChargeTime)
		{
			firstCharge = false;
			startCharge = false;
		}
	}
}

void Boss::leap(Vector3 camPos)
{
	if (!rising && !chasing && !dropping && !leaped)
	{
		leaped = true;
		rising = true;
	}
	else if (pos.y > 150)
	{
		rising = false;
		chasing = true;
		pos.y = 150;
	}
	else if (pos.y <= 0)
	{
		pos.y = 0;
		dropping = false;
	}
	else if (pos.x > camPos.x - 30 && pos.x < camPos.x + 30 && pos.z < camPos.z + 30 && pos.z > camPos.z - 30)
	{
		chasing = false;
		dropping = true;
	}

	if (rising)
	{
		pos.y = pos.y + 1;
	}
	else if (chasing)
	{
		Vector3 temp;
		target = camPos;
		temp = (camPos - pos).Normalize();
		pos.x = pos.x + temp.x * 4;
		pos.z = pos.z + temp.z * 4;
	}
	else if (dropping)
	{
		pos.y = pos.y - 3;
	}
	else if (leaped)
	{
		Vector3 temp;
		target = camPos;
		temp = (camPos - pos).Normalize();
		pos.x = pos.x + temp.x;
		pos.z = pos.z + temp.z;
	}
}

void Boss::boundsCheck()
{
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(200, -10, -200), 50);
	Box hut4 = Box(Vector3(-200, -10, 0), 50);
	Box hut5 = Box(Vector3(-200, -10, 200), 50);
	Box hut6 = Box(Vector3(-200, -10, -200), 50);

	if (isPointXInBox1(pos, hut1)
		|| isPointXInBox1(pos, hut2)
		|| isPointXInBox1(pos, hut3)
		|| isPointXInBox1(pos, hut4)
		|| isPointXInBox1(pos, hut5)
		|| isPointXInBox1(pos, hut6)
		)
	{
		_collidedX = true;
	}

	if (isPointYInBox1(pos, hut1)
		|| isPointYInBox1(pos, hut2)
		|| isPointYInBox1(pos, hut3)
		|| isPointYInBox1(pos, hut4)
		|| isPointYInBox1(pos, hut5)
		|| isPointYInBox1(pos, hut6)
		)
	{
		_collidedY = true;
	}

	if (isPointZInBox1(pos, hut1)
		|| isPointZInBox1(pos, hut2)
		|| isPointZInBox1(pos, hut3)
		|| isPointZInBox1(pos, hut4)
		|| isPointZInBox1(pos, hut5)
		|| isPointZInBox1(pos, hut6)
		)
	{
		_collidedZ = true;
	}

	Vector3 view = (target - pos).Normalized();

	if (_collidedX)
	{
		pos.x = prevPosX;
		target = pos + view;
	}

	else if (_collidedZ)
	{
		pos.z = prevPosZ;
		target = pos + view;
	}

	if (pos.x > 290)
	{
		pos.x = 290;
		target = pos + view;
	}
	if (pos.x < -290)
	{
		pos.x = -290;
		target = pos + view;
	}
	if (pos.z > 890)
	{
		pos.z = 890;
		target = pos + view;
	}
	if (pos.z < -890)
	{
		pos.z = -890;
		target = pos + view;
	}
}

int Boss::getHealth()
{
	return health;
}

Vector3 Boss::getPos()
{
	return pos;
}

void Boss::setHealth(int x)
{
	health = x;
}

void Boss::resetY()
{
	pos.y = 0;
}


