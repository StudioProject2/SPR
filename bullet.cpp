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

void bullet::monsterHit(Camera3 camera, bool isHit)
{
	if (isHit)
	{
		offsetY = WEAPON_OFFSET_Y;
		throws = camera.position;
		throwed = false;
	}
}