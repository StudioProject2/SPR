#include "MonsterFodder.h"
#include "Application.h"
#include <cstdlib>


MonsterFodder::MonsterFodder()
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

MonsterFodder::MonsterFodder(Vector3 x)
{
	health = 100;

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

MonsterFodder::~MonsterFodder()
{
}

void MonsterFodder::move(Vector3 view)
{

}

void MonsterFodder::moveRand(Vector3 camPos, double elaspeTime)
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

	pos.x = pos.x + view.x;
	pos.z = pos.z + view.z;

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

