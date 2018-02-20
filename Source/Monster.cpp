#include "Monster.h"
#include "Application.h"
#include <cstdlib>

bool Monster::isPointInBox(Vector3 position, Box box)
{
	return (position.x >= box.minX&&position.x <= box.maxX) &&
		(position.y >= box.minY&&position.y <= box.maxY) &&
		(position.z >= box.minZ && position.z <= box.maxZ);
}

Monster::Monster()
{
	//float spawnPtX = 300;
	//float spawnPtZ = 100;

	//spawnPtX = (rand() % 1400 - 700) + 1.0;
	//spawnPtZ = (rand() % 1400 - 700) + 1.0;

	//Box cube = Box(Vector3(0, 0, 0), 120);
	//do
	//{
	//	spawnPtX = (rand() % 1400 - 700) + 1.0;
	//	spawnPtZ = (rand() % 1400 - 700) + 1.0;

	//	pos = Vector3(spawnPtX, 0, spawnPtZ);
	//	dirChanger = 0;
	//	moveRight = true;
	//	monsterDirTime = 0.0;
	//} while (isPointInBox(pos, cube));

	health = 100;

	//Vector3
	Vector3 pos;
	Vector3 target;
	Vector3 view;

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

Monster::~Monster()
{
}

bool Monster::isPointXInBox1(Vector3 position, Box box)
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

bool Monster::isPointYInBox1(Vector3 position, Box box)
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

bool Monster::isPointZInBox1(Vector3 position, Box box)
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

	//if (moveRight == true)
	//{
	//	pos = pos + right;
	//	pos.x = pos.x + view.x;
	//	pos.z = pos.z + view.z;
	//}
	//if (moveRight == false)
	//{
	//	pos = pos - right;
	//	pos.x = pos.x + view.x;
	//	pos.z = pos.z + view.z;
	//}

	//if (elaspeTime > monsterDirTime)
	//{
	//	if (moveRight == true)
	//		moveRight = false;
	//	else if (moveRight == false)
	//		moveRight = true;

	//	monsterDirTime = elaspeTime + 2.0;
	//}

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
	Box cube = Box(Vector3(0, 0, 0), 120);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0;
			spawnPtZ = (rand() % 1400 - 700) + 1.0;

			pos = Vector3(spawnPtX, 10, spawnPtZ);
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
	Box tree = Box(Vector3(0, 0, 0), 20, 20);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0;
			spawnPtZ = (rand() % 1400 - 700) + 1.0;

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
	Box cube = Box(Vector3(0, 0, 0), 0);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0;
			spawnPtZ = (rand() % 1400 - 700) + 1.0;

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

	//Vector3 view = (target - pos).Normalized();

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
	Box cube = Box(Vector3(0, 0, 0), 120);

	//Spawn Check, so mob wont spawn in objects
	if (firstSpawn == true)
	{
		float spawnPtX;
		float spawnPtZ;

		do
		{
			spawnPtX = (rand() % 1400 - 700) + 1.0;
			spawnPtZ = (rand() % 1400 - 700) + 1.0;

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


