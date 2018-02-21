#include "MonsterArcher.h"
#include "Application.h"
#include "Mtx44.h"
#include <cstdlib>
#include <iostream>
using namespace std;

MonsterArcher::MonsterArcher()
{
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
	direction = 0;
}

MonsterArcher::~MonsterArcher()
{
}

void MonsterArcher::move(Vector3 view)
{

}

void MonsterArcher::moveRand(Vector3 camPos, double elaspeTime)
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

	
	float temp;
	temp = (target - pos).Length();

	if (temp > 200  && temp < 400)
	{
		if (moveRight == true)
		{
			//pos = pos + right;
			pos.x = pos.x + right.x * 3;
			pos.z = pos.z + right.z * 3;
			pos.x = pos.x + view.x;
			pos.z = pos.z + view.z;
		}
		if (moveRight == false)
		{
			//pos = pos - right;
			pos.x = pos.x - right.x * 3;
			pos.z = pos.z - right.z * 3;
			pos.x = pos.x + view.x;
			pos.z = pos.z + view.z;
		}
	}
	else if (temp > 400)
	{
		pos.x = pos.x + view.x * 5;
		pos.z = pos.z + view.z * 5;
	}
	else if (temp < 170)
	{
		pos.x = pos.x - view.x * 5;
		pos.z = pos.z - view.z * 5;
	}

	if (elaspeTime > monsterDirTime)
	{
		if (moveRight == true)
			moveRight = false;
		else if (moveRight == false)
			moveRight = true;

		monsterDirTime = elaspeTime + 2.0;
	}

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

