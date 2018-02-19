#include "bullet.h"



bullet::bullet()
{
	throws.x = 0;
	throws.y = 0;
	throws.z = 0;
	offsetY = WEAPON_OFFSET_Y;
	weaponShootingTimer = 0;
	weaponTimer = 0;
	throwed = false;
	shot = false;
	isShooting = false;
}

bullet::~bullet()
{
}

void bullet::updateBullet(Vector3 view, Camera3 camera, bullet &isShot)
{
	if (Application::IsKeyPressed(VK_LBUTTON) && !throwed && isShot.isShooting)
	{
		throwed = true;
		shot = true;
		shootTarget = view;
		throws = camera.position;
		offsetY = 0;
	}
	if (shot)
	{
		if (weaponTimer < 10)
		{
			weaponTimer += 1;
		}
		if (weaponTimer == 10)
		{
			isShooting = true;
			shot = false;
			weaponTimer = 0;
		}
	}

	if (throwed)
	{
		if (weaponShootingTimer < PLAYER_WEAPON_RANGE)
		{
			//shooting process
			throws = throws + shootTarget * PLAYER_THROWING_SPEED;
			weaponShootingTimer += 1;
		}
		else
		{
			//checks if done shooting
			weaponShootingTimer = 0;
			throwed = false;
			isShot.isShooting = false;
		}
	}
	else
	{
		offsetY = WEAPON_OFFSET_Y;
		throws = camera.position;
	}
}

void bullet::monsterHit(Camera3 camera)
{
	offsetY = WEAPON_OFFSET_Y;
	throws = camera.position;
	throwed = false;
}


bool bullet::isBulletHit(Box *bullets, Box *monster)
{
	return (((bullets->maxX >= monster->minX && bullets->maxX <= monster->maxX) &&
		(bullets->maxY >= monster->minY && bullets->maxY <= monster->maxY) &&
		(bullets->maxZ >= monster->minZ && bullets->maxZ <= monster->maxZ))
		||
		((bullets->minX >= monster->minX && bullets->minX <= monster->maxX) &&
		(bullets->minY >= monster->minY && bullets->minY <= monster->maxY) &&
			(bullets->minZ >= monster->minZ && bullets->minZ <= monster->maxZ)));
}