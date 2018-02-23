#include "SceneStage3.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "bullet.h"
#include <cstdlib>
#include <iomanip>
#include <sstream>

using namespace std;

SceneStage3::SceneStage3()
{
}

SceneStage3::~SceneStage3()
{
}

void SceneStage3::Init()
{
	//Monster spawn
	srand((unsigned int)time(NULL));
	//Timer
	elaspeTime = 0.0;

	hitmarkerSize = 0;

	//Player
	player = Player::getInstance();

	bulletBounceTime = 0.0;
	playerHurtBounceTime = 0.0;

	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterPtr[i] = NULL;
		monsterBoxPtr[i] = NULL;
		monsterBulletDelay[i] = elaspeTime + 4.0;
	}
	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterFodderPtr[i] = NULL;
		monsterFodderBoxPtr[i] = NULL;
		//monsterBulletDelay[i] = elaspeTime + 4.0;
	}
	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterArcherPtr[i] = NULL;
		monsterArcherBoxPtr[i] = NULL;
		monsterArcherBulletDelay[i] = elaspeTime + 4.0;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	//Enable culling
	//glEnable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(0, 10, 600), Vector3(0, 10, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);


	Color colour;
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");

	//TEXT STUFF
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 4);

	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(-630, 50, -650);
	light[0].color.Set(1, 1, 1);
	light[0].power = 4;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(100));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.0f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	//***************************************Second Light*****************************************
	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(720, 150, 720);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(10));
	light[1].cosInner = cos(Math::DegreeToRadian(5));
	light[1].exponent = 3.0f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	//***************************************Third Light*****************************************
	light[2].type = Light::LIGHT_POINT;
	light[2].position.Set(-720, 150, 720);
	light[2].color.Set(1, 1, 1);
	light[2].power = 20;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(10));
	light[2].cosInner = cos(Math::DegreeToRadian(5));
	light[2].exponent = 3.0f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	//***************************************Fourth Light*****************************************
	light[3].type = Light::LIGHT_POINT;
	light[3].position.Set(720, 150, -720);
	light[3].color.Set(1, 1, 1);
	light[3].power = 20;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(10));
	light[3].cosInner = cos(Math::DegreeToRadian(5));
	light[3].exponent = 3.0f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	//Init meshList
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	//Others
	meshList[GEO_BULLETS] = MeshBuilder::GenerateHem("bullets", Color(0.5f, 0.5f, 0.5f), 20, 20, 0.5);

	//SKYBOX STUFF
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad1("front", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Stage2//skybox_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad1("back", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Stage2//skybox_back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad1("left", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Stage2//skybox_right.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad1("right", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Stage2//skybox_left.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Stage2//skybox_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad1("bottom", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Stage2//skybox_bottom.tga");

	//FLOOR
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad1("Sand", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 10.0f);
	meshList[GEO_FLOOR]->textureID = LoadTGAR("Image//Sand2.tga");
	meshList[GEO_FLOOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLOOR]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_FLOOR]->material.kShininess = 1.f;
	//Bullet
	meshList[GEO_SPHERE] = MeshBuilder::GenerateHem("Bullet", Color(1.0f, 1.0f, 1.0f), 10, 10, 1);

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", 1, 1, 1);

	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//FENCE
	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Fence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//Boss Stage/Fence.tga");

	//GRASS
	meshList[GEO_GRASS_LINE] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//Grass_Line.obj");
	meshList[GEO_GRASS_LINE]->textureID = LoadTGA("Image//stage2//objtextures//Grass2.tga");

	//Teeth
	meshList[GEO_PLAYER_TEETH] = MeshBuilder::GenerateOBJ("teeth", "OBJ//PlayerTeeth.obj");
	meshList[GEO_PLAYER_TEETH]->textureID = LoadTGA("Image//PlayerTeeth.tga");
	meshList[GEO_PLAYER_TEETH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLAYER_TEETH]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PLAYER_TEETH]->material.kSpecular.Set(0.01f, 0.01f, 0.01f);
	meshList[GEO_PLAYER_TEETH]->material.kShininess = 1.0f;

	//Player Health
	meshList[GEO_PLAYERHEALTH] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	meshList[GEO_PLAYERHEALTH]->textureID = LoadTGA("Image//playerHealth.tga");

	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Hut.obj");
	meshList[GEO_BUILDING]->textureID = LoadTGA("Image//Boss Stage/Hut.tga");

	//Monsters
	for (int i = 0; i < 25; i++)
	{
		monsterBulletPtr[i] = NULL;
	}
	for (int i = 0; i < 25; i++)
	{
		monsterArcherBulletPtr[i] = NULL;
	}
	gameOver = false;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		//init collision for the bullets here
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	}

	hut1Burned = false;
	hut2Burned = false;
	hut3Burned = false;
	hut4Burned = false;

	nearHut1 = false;
	nearHut2 = false;
	nearHut3 = false;
	nearHut4 = false;

	monsterSpawnDelay1 = 0;
	monsterSpawnDelay2 = 0;
	monsterSpawnDelay3 = 0;
	monsterSpawnDelay4 = 0;

	initialObjectiveTime = 5.0;
	monstersLeft = 0;
	hutsBurned = 0;
}

bool SceneStage3::isNearObject(Camera3 camera, Box object)
{
	return ((camera.position.x >= object.minX && camera.position.x <= object.maxX) &&
		(camera.position.y >= object.minY && camera.position.y <= object.maxY) &&
		(camera.position.z >= object.minZ && camera.position.z <= object.maxZ));
}

void SceneStage3::Update(double dt)
{
	static const float LSPEED = 10.0f;
	elaspeTime += dt;
	start.isShooting = true;

	UpdateBullets();
	UpdateMonsters();
	UpdateMonsterBullets();
	UpdateMonsterHitbox();
	UpdateInteractions();

	if (player->health <= 0)
	{
		gameOver = true;
	}
	camera.Update(dt);
}
void SceneStage3::UpdateBullets()
{
	Vector3 view = (camera.target - camera.position).Normalized();

	for (int i = 0; i < NO_OF_BULLETS; i++)
	{
		if (i == 0)
		{
			bulletPtr[0]->updateBullet(view, camera, start);
			//update first bullet collision box
			*bulletBoxPtr[0] = Box(bulletPtr[0]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
		}
		else
		{
			bulletPtr[i]->updateBullet(view, camera, *bulletPtr[i - 1]);
			//update rest of bullets collision box
			*bulletBoxPtr[i] = Box(bulletPtr[i]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
		}
	}
}

void SceneStage3::UpdateMonsterBullets()
{
	Box playerBox = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			for (int j = 0; j < MOBBULLETNUM; j++)
			{
				if (elaspeTime > monsterBulletDelay[i] && monsterBulletPtr[j] == NULL)
				{
					monsterBulletPtr[j] = new monsterBullet(MonsterPtr[i]->pos, camera.position);
					monsterBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
					return;
				}
			}
		}
	}

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterBulletPtr[i] != NULL)
		{
			monsterBulletPtr[i]->move();
			if (monsterBulletPtr[i]->isBulletInBox(playerBox))
			{
				player->health -= 10;
				delete monsterBulletPtr[i];
				monsterBulletPtr[i] = NULL;
				
			}
			if (monsterBulletPtr[i] != NULL)
			{
				if (monsterBulletPtr[i]->bulletCollide())
				{
					delete monsterBulletPtr[i];
					monsterBulletPtr[i] = NULL;
				}
			}
		}
	}

	//Monster Archer
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			for (int j = 0; j < MOBBULLETNUM; j++)
			{
				if (elaspeTime > monsterArcherBulletDelay[i] && monsterArcherBulletPtr[j] == NULL)
				{
					monsterArcherBulletPtr[j] = new monsterBullet(MonsterArcherPtr[i]->pos, camera.position);
					monsterArcherBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
					return;
				}
			}
		}
	}

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterArcherBulletPtr[i] != NULL)
		{
			monsterArcherBulletPtr[i]->move();
			if (monsterArcherBulletPtr[i]->isBulletInBox(playerBox))
			{
				player->health -= 10;
				delete monsterArcherBulletPtr[i];
				monsterArcherBulletPtr[i] = NULL;

			}
			if (monsterArcherBulletPtr[i] != NULL)
			{
				if (monsterArcherBulletPtr[i]->bulletCollide())
				{
					delete monsterArcherBulletPtr[i];
					monsterArcherBulletPtr[i] = NULL;
				}
			}
		}
	}

}
void SceneStage3::UpdateMonsters()
{

	if (monsterSpawnDelay1 != 0 && elaspeTime > monsterSpawnDelay1)
	{
		MonsterPtr[0] = new Monster(Vector3(100, 0, 93));
		monsterBoxPtr[0] = new Box(MonsterPtr[0]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterPtr[1] = new Monster(Vector3(290, 0, -85));
		monsterBoxPtr[1] = new Box(MonsterPtr[1]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[0] = new MonsterFodder(Vector3(272, 0, -80));
		monsterFodderBoxPtr[0] = new Box(MonsterFodderPtr[0]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[1] = new MonsterFodder(Vector3(120, 0, 90));
		monsterFodderBoxPtr[1] = new Box(MonsterFodderPtr[1]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[2] = new MonsterFodder(Vector3(115, 0, 100));
		monsterFodderBoxPtr[2] = new Box(MonsterFodderPtr[2]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[0] = new MonsterArcher(Vector3(101, 0, -100));
		monsterArcherBoxPtr[0] = new Box(MonsterArcherPtr[0]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[1] = new MonsterArcher(Vector3(295, 0, -80));
		monsterArcherBoxPtr[1] = new Box(MonsterArcherPtr[1]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		monsterSpawnDelay1 = 0;
		monstersLeft += 7;
	}

	if (monsterSpawnDelay2 != 0 && elaspeTime > monsterSpawnDelay2)
	{
		MonsterPtr[2] = new Monster(Vector3(280, 0, 280));
		monsterBoxPtr[2] = new Box(MonsterPtr[2]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterPtr[3] = new Monster(Vector3(100, 0, 293));
		monsterBoxPtr[3] = new Box(MonsterPtr[3]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[3] = new MonsterFodder(Vector3(272, 0, 125));
		monsterFodderBoxPtr[3] = new Box(MonsterFodderPtr[3]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[4] = new MonsterFodder(Vector3(120, 0, 290));
		monsterFodderBoxPtr[4] = new Box(MonsterFodderPtr[4]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[5] = new MonsterFodder(Vector3(115, 0, 300));
		monsterFodderBoxPtr[5] = new Box(MonsterFodderPtr[5]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[2] = new MonsterArcher(Vector3(101, 0, 100));
		monsterArcherBoxPtr[2] = new Box(MonsterArcherPtr[2]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[3] = new MonsterArcher(Vector3(285, 0, 295));
		monsterArcherBoxPtr[3] = new Box(MonsterArcherPtr[3]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		monsterSpawnDelay2 = 0;
		monstersLeft += 7;
	}

	if (monsterSpawnDelay3 != 0 && elaspeTime > monsterSpawnDelay3)
	{
		MonsterPtr[4] = new Monster(Vector3(-300, 0, 93));
		monsterBoxPtr[4] = new Box(MonsterPtr[4]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterPtr[5] = new Monster(Vector3(-110, 0, -85));
		monsterBoxPtr[5] = new Box(MonsterPtr[5]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[6] = new MonsterFodder(Vector3(-128, 0, -80));
		monsterFodderBoxPtr[6] = new Box(MonsterFodderPtr[6]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[7] = new MonsterFodder(Vector3(-280, 0, 90));
		monsterFodderBoxPtr[7] = new Box(MonsterFodderPtr[7]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[8] = new MonsterFodder(Vector3(-295, 0, 100));
		monsterFodderBoxPtr[8] = new Box(MonsterFodderPtr[8]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[4] = new MonsterArcher(Vector3(-299, 0, -100));
		monsterArcherBoxPtr[4] = new Box(MonsterArcherPtr[4]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[5] = new MonsterArcher(Vector3(-105, 0, -80));
		monsterArcherBoxPtr[5] = new Box(MonsterArcherPtr[5]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		monsterSpawnDelay3 = 0;
		monstersLeft += 7;
	}

	if (monsterSpawnDelay4 != 0 && elaspeTime > monsterSpawnDelay4)
	{
		MonsterPtr[6] = new Monster(Vector3(-300, 0, 280));
		monsterBoxPtr[6] = new Box(MonsterPtr[6]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterPtr[7] = new Monster(Vector3(-110, 0, 115));
		monsterBoxPtr[7] = new Box(MonsterPtr[7]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[9] = new MonsterFodder(Vector3(-128, 0, 110));
		monsterFodderBoxPtr[9] = new Box(MonsterFodderPtr[9]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[10] = new MonsterFodder(Vector3(-280, 0, 295));
		monsterFodderBoxPtr[10] = new Box(MonsterFodderPtr[10]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterFodderPtr[11] = new MonsterFodder(Vector3(-295, 0, 100));
		monsterFodderBoxPtr[11] = new Box(MonsterFodderPtr[11]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[6] = new MonsterArcher(Vector3(-299, 0, 300));
		monsterArcherBoxPtr[6] = new Box(MonsterArcherPtr[6]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		MonsterArcherPtr[7] = new MonsterArcher(Vector3(-105, 0, 120));
		monsterArcherBoxPtr[7] = new Box(MonsterArcherPtr[7]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		monsterSpawnDelay4 = 0;
		monstersLeft += 7;
	}

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			(*MonsterPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterBoxPtr[i] = Box(MonsterPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		}
	}

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			if ((*MonsterPtr[i]).health <= 0)
			{
				delete MonsterPtr[i];
				delete monsterBoxPtr[i];
				MonsterPtr[i] = NULL;
				monsterBoxPtr[i] = NULL; 
				monstersLeft--;
			}
		}
	}
	
	//Monster Fodder
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			(*MonsterFodderPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterFodderBoxPtr[i] = Box(MonsterFodderPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		}
	}

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			if ((*MonsterFodderPtr[i]).health <= 0)
			{
				delete MonsterFodderPtr[i];
				delete monsterFodderBoxPtr[i];
				MonsterFodderPtr[i] = NULL;
				monsterFodderBoxPtr[i] = NULL;
				monstersLeft--;
			}
		}
	}
	
	//Monster Archer
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			(*MonsterArcherPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterArcherBoxPtr[i] = Box(MonsterArcherPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		}
	}

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			if ((*MonsterArcherPtr[i]).health <= 0)
			{
				delete MonsterArcherPtr[i];
				delete monsterArcherBoxPtr[i];
				MonsterArcherPtr[i] = NULL;
				monsterArcherBoxPtr[i] = NULL;
				monstersLeft--;
			}
		}
	}
	
}

void SceneStage3::UpdateMonsterHitbox()
{
	bool isHit = false;
	Box *playerBox =new Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);
	int monNum;
	hitmarkerSize = 0;

	//Normal Monster
	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM; mon++)
		{
			if (!isHit && elaspeTime > bulletBounceTime)
			{
				if (bulletBoxPtr[bul] != NULL && monsterBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[bul]->isBulletHit(bulletBoxPtr[bul], monsterBoxPtr[mon]);
				}
				if (isHit)
				{
					(*MonsterPtr[mon]).health = (*MonsterPtr[mon]).health - player->damage;
					cout << "HIT " << endl;
				}
				if (isHit)
				{
					hitmarkerTimer = 50;
				}
				if (isHit)
				{
					bulletPtr[bul]->monsterHit(camera);
					bulletBoxPtr[bul]->position = bulletPtr[bul]->throws;
					bulletBounceTime = elaspeTime + 0.1;
					isHit = false;
				}
			}
		}
	}

	//Montser Fodder
	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM; mon++)
		{
			if (!isHit && elaspeTime > bulletBounceTime)
			{
				if (bulletBoxPtr[bul] != NULL && monsterFodderBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[bul]->isBulletHit(bulletBoxPtr[bul], monsterFodderBoxPtr[mon]);
				}
				if (isHit)
				{
					(*MonsterFodderPtr[mon]).health = (*MonsterFodderPtr[mon]).health - player->damage;
					cout << "HIT " << endl;
				}
				if (isHit)
				{
					hitmarkerTimer = 50;
				}
				if (isHit)
				{
					bulletPtr[bul]->monsterHit(camera);
					bulletBoxPtr[bul]->position = bulletPtr[bul]->throws;
					bulletBounceTime = elaspeTime + 0.1;
					isHit = false;
				}
			}
		}
	}

	//Monster Archer
	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM; mon++)
		{
			if (!isHit && elaspeTime > bulletBounceTime)
			{
				if (bulletBoxPtr[bul] != NULL && monsterArcherBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[bul]->isBulletHit(bulletBoxPtr[bul], monsterArcherBoxPtr[mon]);
				}
				if (isHit)
				{
					(*MonsterArcherPtr[mon]).health = (*MonsterArcherPtr[mon]).health - player->damage;
					cout << "HIT " << endl;
				}
				if (isHit)
				{
					hitmarkerTimer = 50;
				}
				if (isHit)
				{
					bulletPtr[bul]->monsterHit(camera);
					bulletBoxPtr[bul]->position = bulletPtr[bul]->throws;
					bulletBounceTime = elaspeTime + 0.1;
					isHit = false;
				}
			}
		}
	}

	//FULL ON CHEESE
	for (int i = 0; i < MOBNUM; i++)
	{
		if (monsterBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterBoxPtr[i]))
			{
				player->health -= 10;
				playerHurtBounceTime = elaspeTime + 0.5;
			}
		}

		if (monsterFodderBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterFodderBoxPtr[i]))
			{
				player->health -= 10;
				playerHurtBounceTime = elaspeTime + 0.5;
			}
		}

		if (monsterArcherBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterArcherBoxPtr[i]))
			{
				player->health -= 10;
				playerHurtBounceTime = elaspeTime + 0.5;
			}
		}
	}


	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}

}

void SceneStage3::UpdateInteractions()
{
	Box hut1 = Box(Vector3(200, -10, 0), 70);
	Box hut2 = Box(Vector3(200, -10, 200), 70);
	Box hut3 = Box(Vector3(-200, -10, 0), 70);
	Box hut4 = Box(Vector3(-200, -10, 200), 70);

	if (isNearObject(camera, hut1))
	{
		nearHut1 = true;
	}
	else
	{
		nearHut1 = false;
	}
	if (isNearObject(camera, hut2))
	{
		nearHut2 = true;
	}
	else
	{
		nearHut2 = false;
	}
	if (isNearObject(camera, hut3))
	{
		nearHut3 = true;
	}
	else
	{
		nearHut3 = false;
	}
	if (isNearObject(camera, hut4))
	{
		nearHut4 = true;
	}
	else
	{
		nearHut4 = false;
	}

	if (nearHut1 && Application::IsKeyPressed('E') && !hut1Burned)
	{
		hut1Burned = true;
		hutsBurned++;
		monsterSpawnDelay1 = elaspeTime + 1.0;
	}
	if (nearHut2 && Application::IsKeyPressed('E') && !hut2Burned)
	{
		hut2Burned = true;
		hutsBurned++;
		monsterSpawnDelay2 = elaspeTime + 1.0;
	}
	if (nearHut3 && Application::IsKeyPressed('E') && !hut3Burned)
	{
		hut3Burned = true;
		hutsBurned++;
		monsterSpawnDelay3 = elaspeTime + 1.0;
	}
	if (nearHut4 && Application::IsKeyPressed('E') && !hut4Burned)
	{
		hut4Burned = true;
		hutsBurned++;
		monsterSpawnDelay4 = elaspeTime + 1.0;
	}
}

void SceneStage3::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//********************SECOND LIGHT***************************
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//*******************THIRD LIGHT******************************
	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//*******************FOURTH LIGHT******************************
	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//SKYBOX + FLOOR
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-1000, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1000, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 1000);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 1000, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, -1000, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	Vector3 defaultView = Vector3(0, 0, 1);

	//Vector3 dir = Vector3(0, 0, 0);
	double coolrot;

	//RENDER ALL MOBS
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{

			Vector3 B = MonsterPtr[i]->pos - camera.position;
			B.y = MonsterPtr[i]->pos.y;

			double rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180 / 3.14);

			if (B.x > 0 && B.z < 0)
				coolrot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				coolrot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				coolrot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				coolrot = 180 - rotation;
			else
				coolrot = rotation;
			//cout << rotation << endl;
			modelStack.PushMatrix();
			modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y, (*MonsterPtr[i]).pos.z);
			modelStack.Rotate(coolrot, 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
	}
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			float ans = acos(defaultView.Dot((*MonsterFodderPtr[i]).view) / (defaultView.Length() * (*MonsterFodderPtr[i]).view).Length());
			ans = ans * (180.0 / 3.141592653589793238462643383279502884197169399375105820974944592307816406286);

			if (camera.position.x < (*MonsterFodderPtr[i]).pos.x)
			{
				ans = ans * -1;
			}

			modelStack.PushMatrix();
			modelStack.Translate((*MonsterFodderPtr[i]).pos.x, (*MonsterFodderPtr[i]).pos.y, (*MonsterFodderPtr[i]).pos.z);
			modelStack.Rotate(ans, 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
	}

		for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			float ans = acos(defaultView.Dot((*MonsterArcherPtr[i]).view) / (defaultView.Length() * (*MonsterArcherPtr[i]).view).Length());
			ans = ans * (180.0 / 3.141592653589793238462643383279502884197169399375105820974944592307816406286);

			if (camera.position.x < (*MonsterArcherPtr[i]).pos.x)
			{
				ans = ans * -1;
			}

			modelStack.PushMatrix();
			modelStack.Translate((*MonsterArcherPtr[i]).pos.x, (*MonsterArcherPtr[i]).pos.y, (*MonsterArcherPtr[i]).pos.z);
			modelStack.Rotate(ans, 0, 1, 0); 
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
	}


	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterBulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*monsterBulletPtr[i]).pos.x, (*monsterBulletPtr[i]).pos.y, (*monsterBulletPtr[i]).pos.z);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_SPHERE], false);
			modelStack.PopMatrix();
		}
	}

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterArcherBulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*monsterArcherBulletPtr[i]).pos.x, (*monsterArcherBulletPtr[i]).pos.y, (*monsterArcherBulletPtr[i]).pos.z);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_SPHERE], false);
			modelStack.PopMatrix();
		}
	}

	//FENCES
	for (int i = 0; i < 1800; i += 30)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-300, -10, -900 + i);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(300, -10, -900 + i);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 660; i += 30)
	{
		modelStack.PushMatrix();
		modelStack.Translate( -i - 100, -10, 890);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(+i + 100, -10, 890);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(300 - i, -10, -910);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
		
	}

	//RENDER GRASSES
	for (int i = 0; i < 1401; i += 350)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-610, -10, -700 + i);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(610, -10, -700 + i);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();
	}
	for (int i = 0; i < 1401; i += 350)
	{
		modelStack.PushMatrix();
		modelStack.Translate(650 - i, -10, 910);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();

	}

	modelStack.PushMatrix();
	modelStack.Translate(200, -10, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200, -10, 200);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, -10, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, -10, 200);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	RenderBullets();
	RenderTopTeeth();
	RenderBottomTeeth();
	RenderPlayerHealth();
	RenderHitmarker();
	RenderNearHut();
	RenderObjectives();

	if (elaspeTime < initialObjectiveTime)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "BURN DOWN THE HUTS", Color(0.0f, 0.0f, 0.0f), 4, 2, 7);
	}

	if (gameOver)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "GAME OVER", Color(1, 1, 1), 5, 4, 5);
	}
}
void SceneStage3::RenderTopTeeth()
{
	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(-1.3, 8, -17);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.5, 1, 1);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage3::RenderBottomTeeth()
{

	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(1.3, -8, -17);
	modelStack.Scale(1.5, 1, 1);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage3::RenderPlayerHealth()
{
	/*for (int i = 0; i < player->health; i += 10)
	{
	RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2 + (i * 1.5 / 10), 48, 1, 1);
	}*/

	int vertical = player->health / 50;
	int horizontal = (player->health - (vertical * 50)) / 10;

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5 + (j * 4.3), 48 - (i * 4), 1, 1);
		}
	}
	for (int i = 0; i < horizontal; i++)
	{
		RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5 + (i * 4.3), 48 - (vertical * 4), 1, 1);
	}

	//RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5, 48, 1, 1);
	//RenderTextOnScreen(meshList[GEO_TEXT], "x" + to_string(player->health), Color(0, 0, 0), 2, 4, 24);
}

void SceneStage3::RenderNearHut()
{
	if (elaspeTime > initialObjectiveTime && ((nearHut1 && !hut1Burned) || (nearHut2 && !hut2Burned) || (nearHut3 && !hut3Burned) || (nearHut4 && !hut4Burned)))
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press E to burn", Color(0.0f, 0.0f, 0.0f), 4, 4, 7);
	}
}

void SceneStage3::RenderObjectives()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "Objective", Color(0, 0, 0), 2, 32, 25);
	RenderTextOnScreen(meshList[GEO_TEXT], "============", Color(0, 0, 0), 2, 31, 24);
	if (hutsBurned != 4)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Burn the huts", Color(0.1, 0.1, 0.1), 2, 28, 23);
		RenderTextOnScreen(meshList[GEO_TEXT], "Huts burned:" + to_string(hutsBurned) + "/4", Color(0.1, 0.1, 0.1), 2, 26, 22);
	}
	else if (monstersLeft != 0)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Kill all humans", Color(0.1, 0.1, 0.1), 2, 26, 23);
		RenderTextOnScreen(meshList[GEO_TEXT], "Humans left:" + to_string(monstersLeft), Color(0.1, 0.1, 0.1), 2, 27, 22);
	}
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Go to the next part", Color(0.1, 0.1, 0.1), 3, 4, 12);
		RenderTextOnScreen(meshList[GEO_TEXT], "of the village", Color(0.1, 0.1, 0.1), 3, 5.5, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "and kill the chief!", Color(0.1, 0.1, 0.1), 3, 4.5, 10);
	}
}

void SceneStage3::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}


	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
void SceneStage3::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
void SceneStage3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
void SceneStage3::RenderMeshOnScreen(Mesh * mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(sizex, sizey, 0);
	modelStack.Translate(x, y, 0);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneStage3::RenderBullets()
{
	for (int i = 0; i < NO_OF_BULLETS; i++)
	{
		if (bulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate(bulletPtr[i]->throws.x, bulletPtr[i]->throws.y, bulletPtr[i]->throws.z);
			RenderMesh(meshList[GEO_BULLETS], false);
			modelStack.PopMatrix();
		}
	}
}

void SceneStage3::RenderHitmarker()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0, 1, 1), 5, 8.3, 6.1);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1, 0, 0), hitmarkerSize, 8.3, 6.1);
}

void SceneStage3::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i] != NULL)
		{
			delete meshList[i];
		}
		meshList[i] = NULL;

	}

	//glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

