#include "MonsterFodder.h"
#include "Application.h"
#include <cstdlib>


MonsterFodder::MonsterFodder()
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

