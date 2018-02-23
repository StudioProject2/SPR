#pragma once
#ifndef BULLET_H
#define BULLET_H


#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadOBJ.h"
#include "box.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

#define PLAYER_WEAPON_RANGE 100
#define PLAYER_THROWING_SPEED 10
#define WEAPON_OFFSET_Y -50

class bullet
{
public:
	bullet();
	~bullet();

	Vector3 throws;
	Vector3 shootTarget;
	void updateBullet(Vector3 view, Camera3 camera, bullet &isShot);
	void monsterHit(Camera3);
	bool isBulletHit(Box *bullets, Box *monster);
	bool throwed;
	int offsetY;
	
	bool isShooting;
	bool shot;

	int weaponShootingTimer;
	int weaponTimer;

	ISoundEngine* engine = createIrrKlangDevice();
};

#endif 