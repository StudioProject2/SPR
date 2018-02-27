#include "monsterBullet.h"
#include "Monster.h"
#include "Camera3.h"
#include "Application.h"

monsterBullet::monsterBullet()
{
}

monsterBullet::monsterBullet(Vector3 x, Vector3 y)
{
	pos = x;
	target = (y - pos).Normalize();
}

monsterBullet::~monsterBullet()
{
}

//MOVES THE BULLET
void monsterBullet::move()
{
	pos = pos + target * 7;
}

//CHECK IF BULLET IS IN A BOX
bool monsterBullet::isBulletInBox(Box box)
{
	if ((pos.x >= box.minX && pos.x <= box.maxX)
		&& (pos.y >= box.minY && pos.y <= box.maxY)
		&& (pos.z >= box.minZ && pos.z <= box.maxZ))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//CHECKS FOR COLLISION
bool monsterBullet::bulletCollide()
{
	if (Application::whatScene == Application::STAGE1)
	{
		return bulletCollideStage1();
	}
	if (Application::whatScene == Application::STAGE2)
	{
		return bulletCollideStage2();
	}
	if (Application::whatScene == Application::STAGE3)
	{
		return bulletCollideStage3();
	}
	if (Application::whatScene == Application::STAGE4)
	{
		return bulletCollideStage4();
	}
	else
	{
		return false;
	}
}

bool monsterBullet::bulletCollideStage1()
{
	if (pos.x > 800 || pos.x < -800 || pos.y > 800 || pos.y < 0 || pos.z > 800 || pos.z < -800)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool monsterBullet::bulletCollideStage2()
{
	if (pos.x > 800 || pos.x < -800 || pos.y > 800 || pos.y < 0 || pos.z > 800 || pos.z < -800)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool monsterBullet::bulletCollideStage3()
{
	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(-200, -10, 0), 50);
	Box hut4 = Box(Vector3(-200, -10, 200), 50);

	if (pos.x > 800 || pos.x < -800 || pos.y > 800 || pos.y < 0 || pos.z > 800 || pos.z < -800
		|| isBulletInBox(hut1)
		|| isBulletInBox(hut2)
		|| isBulletInBox(hut3)
		|| isBulletInBox(hut4)
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool monsterBullet::bulletCollideStage4()
{
	Box hut1 = Box(Vector3(200, -10, 0), 50);
	Box hut2 = Box(Vector3(200, -10, 200), 50);
	Box hut3 = Box(Vector3(200, -10, -200), 50);
	Box hut4 = Box(Vector3(-200, -10, 0), 50);
	Box hut5 = Box(Vector3(-200, -10, 200), 50);
	Box hut6 = Box(Vector3(-200, -10, -200), 50);

	if (pos.x > 800 || pos.x < -800 || pos.y > 800 || pos.y < 0 || pos.z > 800 || pos.z < -800
		|| isBulletInBox(hut1)
		|| isBulletInBox(hut2)
		|| isBulletInBox(hut3)
		|| isBulletInBox(hut4)
		|| isBulletInBox(hut5)
		|| isBulletInBox(hut6)
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}
