#include "monsterBullet.h"
#include "Monster.h"
#include "Camera3.h"

monsterBullet::monsterBullet()
{
}

monsterBullet::monsterBullet(Monster* x, Vector3 y)
{
	pos = x->pos;
	target = (y - pos).Normalize();
}

monsterBullet::~monsterBullet()
{
}

void monsterBullet::move()
{
	pos = pos + target * 7;
}

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

bool monsterBullet::bulletCollide()
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
