#include "Monster.h"
#include "Application.h"
#include <cstdlib>


Monster::Monster()
{
	health = 100;

	//Movement 
	firstSpawn = true;
	dirChanger = 0;
	moveRight = false;
	monsterDirTime = 0.0;

	//Collision for mob
	prevPosX = 0.0;
	prevPosY = 0.0;
	prevPosZ = 0.0;
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;
	 
	
}

Monster::Monster(Vector3 x)
{
	health = 100;

	//Vector3
	pos = x;

	//Movement 
	firstSpawn = false;
	dirChanger = 0;
	moveRight = false;
	monsterDirTime = 0.0;

	//Collision for mob
	prevPosX = 0.0;
	prevPosY = 0.0;
	prevPosZ = 0.0;
	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;


}

Monster::~Monster()
{
}

bool Monster::isPointInBox(Vector3 position, Box box)
{
	return (position.x >= box.minX&&position.x <= box.maxX) &&
		(position.y >= box.minY&&position.y <= box.maxY) &&
		(position.z >= box.minZ && position.z <= box.maxZ);
}
bool Monster::isPointXInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX - 2 && position.x <= box.minX + 2)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.maxX - 2 && position.x <= box.maxX + 2)
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
bool Monster::isPointYInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY - 2 && position.y <= box.minY + 2)
		&& (position.z >= box.minZ && position.z <= box.maxZ))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.maxY - 2 && position.y <= box.maxY + 2)
			&& (position.z >= box.minZ && position.z <= box.maxZ)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Monster::isPointZInBox1(Vector3 position, Box box)
{
	if (((position.x >= box.minX && position.x <= box.maxX)
		&& (position.y >= box.minY && position.y <= box.maxY)
		&& (position.z >= box.minZ - 2 && position.z <= box.minZ + 2))
		|| ((position.x >= box.minX && position.x <= box.maxX)
			&& (position.y >= box.minY && position.y <= box.maxY)
			&& (position.z >= box.maxZ - 2 && position.z <= box.maxZ + 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Monster::move(Vector3 camPos)
{
	Vector3 temp;
	target = camPos;
	temp = (camPos - pos).Normalize();
	pos.x = pos.x + temp.x;
	pos.z = pos.z + temp.z;
}
void Monster::moveRand(Vector3 camPos, double elaspeTime)
{
	//PrevPositions For Ai
	prevPosX = pos.x;
	prevPosY = pos.y;
	prevPosZ = pos.z;

	//Vector for movement
	target = camPos;

	view = (target - pos).Normalized();
	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = view.Cross(up);

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

	if (elaspeTime > monsterDirTime)
	{
		if (moveRight == true)
			moveRight = false;
		else if (moveRight == false)
			moveRight = true;

		monsterDirTime = elaspeTime + 2.0;
	}

	//Monster Spawn
	if (Application::whatScene == Application::STAGE1)
	{
		boundsCheckStage1();
	}
	if (Application::whatScene == Application::STAGE2)
	{
		boundsCheckStage2();
	}
	if (Application::whatScene == Application::STAGE3)
	{
		boundsCheckStage3();
	}
	if (Application::whatScene == Application::STAGE4)
	{
		boundsCheckBoss();
	}

}

void Monster::boundsCheckStage1()
{
	Box cube = Box(Vector3(0, 0, 0), 0);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0f;
			spawnPtZ = (rand() % 1400 - 700) + 1.0f;

			pos = Vector3(spawnPtX, 0, spawnPtZ);
			dirChanger = 0;
			moveRight = true;
			monsterDirTime = 0.0;

		} while (isPointInBox(pos, cube));

		firstSpawn = false;
	}

	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	if (isPointXInBox1(pos, cube))

	{
		_collidedX = true;
	}

	if (isPointYInBox1(pos, cube))
	{
		_collidedY = true;
	}

	if (isPointZInBox1(pos, cube))
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
}
void Monster::boundsCheckStage2()
{
	Box tree = Box(Vector3(-10, 0, 10), 170, 170);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0f;
			spawnPtZ = (rand() % 1400 - 700) + 1.0f;

			pos = Vector3(spawnPtX, 0, spawnPtZ);
			dirChanger = 0;
			moveRight = true;
			monsterDirTime = 0.0;

		} while (isPointInBox(pos, tree));

		firstSpawn = false;
	}

	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	if (isPointXInBox1(pos, tree))

	{
		_collidedX = true;
	}

	if (isPointYInBox1(pos, tree))
	{
		_collidedY = true;
	}

	if (isPointZInBox1(pos, tree))
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
}
void Monster::boundsCheckStage3()
{
	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(-200, -10, 0), 50);
	Box hut4 = Box(Vector3(-200, -10, 200), 50);
	Box wall = Box(Vector3(300, 0, 0), 1, 1000, 1);
	Box wall2 = Box(Vector3(-300, 0, 0), 1, 1000, 1);
	Box wall3 = Box(Vector3(0, 0, 890), 500, 1, 1);
	Box wall4 = Box(Vector3(0, 0, -890), 500, 1, 1);

	//Spawn Check not needed for stage3

	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	if (isPointXInBox1(pos, hut1)
		|| isPointXInBox1(pos, hut2)
		|| isPointXInBox1(pos, hut3)
		|| isPointXInBox1(pos, hut4)
		|| isPointXInBox1(pos, wall)
		|| isPointXInBox1(pos, wall2)
		|| isPointXInBox1(pos, wall3)
		|| isPointXInBox1(pos, wall4))

	{
		_collidedX = true;
	}

	if (isPointYInBox1(pos, hut1)
		|| isPointYInBox1(pos, hut2)
		|| isPointYInBox1(pos, hut3)
		|| isPointYInBox1(pos, hut4)
		|| isPointYInBox1(pos, wall)
		|| isPointYInBox1(pos, wall2)
		|| isPointYInBox1(pos, wall3)
		|| isPointYInBox1(pos, wall4))
	{
		_collidedY = true;
	}

	if (isPointZInBox1(pos, hut1)
		|| isPointZInBox1(pos, hut2)
		|| isPointZInBox1(pos, hut3)
		|| isPointZInBox1(pos, hut4)
		|| isPointZInBox1(pos, wall)
		|| isPointZInBox1(pos, wall2)
		|| isPointZInBox1(pos, wall3)
		|| isPointZInBox1(pos, wall4))
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
}
void Monster::boundsCheckBoss()
{
	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(200, -10, -200), 50);
	Box hut4 = Box(Vector3(-200, -10, 0), 50);
	Box hut5 = Box(Vector3(-200, -10, 200), 50);
	Box hut6 = Box(Vector3(-200, -10, -200), 50);
	Box wall = Box(Vector3(300, 0, 0), 1, 1000, 1);
	Box wall2 = Box(Vector3(-300, 0, 0), 1, 1000, 1);
	Box wall3 = Box(Vector3(0, 0, 890), 500, 1, 1);
	Box wall4 = Box(Vector3(0, 0, -890), 500, 1, 1);


	//Spawn Check, so mob wont spawn in objects and outside intended area
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 580 - 290) + 1.0f;
			spawnPtZ = (rand() % 1760 - 880) + 1.0f;

			pos = Vector3(spawnPtX, 0, spawnPtZ);
			dirChanger = 0;
			moveRight = true;
			monsterDirTime = 0.0;

		} while (isPointInBox(pos, hut1)
			|| isPointInBox(pos, hut2)
			|| isPointInBox(pos, hut3)
			|| isPointInBox(pos, hut4)
			|| isPointInBox(pos, hut5)
			|| isPointInBox(pos, hut6));

		firstSpawn = false;
	}

	_collidedX = false;
	_collidedY = false;
	_collidedZ = false;

	if (isPointXInBox1(pos, hut1)
		|| isPointXInBox1(pos, hut2)
		|| isPointXInBox1(pos, hut3)
		|| isPointXInBox1(pos, hut4)
		|| isPointXInBox1(pos, hut5)
		|| isPointXInBox1(pos, hut6)

		|| isPointXInBox1(pos, wall)
		|| isPointXInBox1(pos, wall2)
		|| isPointXInBox1(pos, wall3)
		|| isPointXInBox1(pos, wall4))

	{
		_collidedX = true;
	}

	if (isPointYInBox1(pos, hut1)
		|| isPointYInBox1(pos, hut2)
		|| isPointYInBox1(pos, hut3)
		|| isPointYInBox1(pos, hut4)
		|| isPointYInBox1(pos, hut5)
		|| isPointYInBox1(pos, hut6)

		|| isPointYInBox1(pos, wall)
		|| isPointYInBox1(pos, wall2)
		|| isPointYInBox1(pos, wall3)
		|| isPointYInBox1(pos, wall4))
	{
		_collidedY = true;
	}

	if (isPointZInBox1(pos, hut1)
		|| isPointZInBox1(pos, hut2)
		|| isPointZInBox1(pos, hut3)
		|| isPointZInBox1(pos, hut4)
		|| isPointZInBox1(pos, hut5)
		|| isPointZInBox1(pos, hut6)
		
		|| isPointZInBox1(pos, wall)
		|| isPointZInBox1(pos, wall2)
		|| isPointZInBox1(pos, wall3)
		|| isPointZInBox1(pos, wall4))
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
}


