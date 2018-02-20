#include "SceneBoss.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"

#include <cstdlib>
#include <iomanip>
#include <sstream>

using namespace std;

SceneBoss::SceneBoss()
{
}

SceneBoss::~SceneBoss()
{
}

void SceneBoss::Init()
{
	//Monster spawn
	srand((unsigned int)time(NULL));
	//Timer
	elaspeTime = 0.0;
	deltaTime = 0.0;
	monsterTime = elaspeTime + 3.0;
	bossMovementChangeTime = elaspeTime + 10.0;
	bossMovement = STRAIGHT;
	bossPlayerShootTime = elaspeTime + 3.0;
	bossRingShootTime = elaspeTime + 3.0;
	bossGroundAttackTime = elaspeTime + 0.001;
	bossBox = new Box;
	groundSignalBlinkTime = elaspeTime + 0.5;
	bossChangeGroundTargetTime = elaspeTime + 5.0;
	bossGroundAttackDelayTime = elaspeTime + 1.0;
	printGroundSignal = true;

	hitmarkerSize = 0;

	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterPtr[i] = NULL;
		monsterBulletDelay[i] = elaspeTime + 4.0;
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
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Stage 2/skybox_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad1("back", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Stage 2/skybox_back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad1("left", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Stage 2/skybox_right.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad1("right", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Stage 2/skybox_left.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Stage 2/skybox_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad1("bottom", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Stage 2/skybox_bottom.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;

	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Hut.obj");
	meshList[GEO_BUILDING]->textureID = LoadTGA("Image//Boss Stage/Hut.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Fence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//Boss Stage/Fence.tga");

	//Bullet
	meshList[GEO_SPHERE] = MeshBuilder::GenerateHem("Bullet", Color(1.0f, 1.0f, 1.0f), 10, 10, 1);

	//Debuggging Cube
	meshList[GEO_CUBE] = MeshBuilder::GenerateOBJ("cube", "OBJ//Cube.obj");

	meshList[GEO_GROUNDSIGNAL] = MeshBuilder::GenerateQuad("ground signal", Color(1.0f, 0.0f, 0.0f), 100, 100);

	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//Monsters

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		monsterBulletPtr[i] = NULL;
	}

	for (int i = 0; i < BOSSBULLETNUM; i++)
	{
		bossBulletPtr[i] = NULL;
	}

	gameOver = false;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		//init collision for the bullets here
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	}
}

void SceneBoss::Update(double dt)
{
	static const float LSPEED = 10.0f;
	elaspeTime += dt;
	deltaTime = dt;
	deltaTime = dt;
	start.isShooting = true;

	UpdateBullets();
	//UpdateMonsters();
	UpdateMonsterBullets();
	UpdateMonsterHitbox();
	UpdateBossMovement();
	UpdateBossBullets();
	UpdateBossHitbox();

	Box player = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 10);

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterBulletPtr[i] != NULL)
		{
			monsterBulletPtr[i]->move();
			if (monsterBulletPtr[i]->isBulletInBox(player))
			{
				gameOver = true;
			}
			if (monsterBulletPtr[i]->bulletCollideStage4())
			{
				monsterBulletPtr[i] = NULL;
				delete monsterBulletPtr[i];
			}
		}
	}

	for (int i = 0; i < BOSSBULLETNUM; i++)
	{
		if (bossBulletPtr[i] != NULL)
		{
			bossBulletPtr[i]->move();
			if (bossBulletPtr[i]->isBulletInBox(player))
			{
				gameOver = true;
			}
			if (bossBulletPtr[i]->bulletCollide())
			{
				bossBulletPtr[i] = NULL;
				delete bossBulletPtr[i];
			}
		}
	}

	if (elaspeTime > groundSignalBlinkTime)
	{
		if (printGroundSignal)
		{
			printGroundSignal = false;
		}
		else if (!printGroundSignal)
		{
			printGroundSignal = true;
		}
		groundSignalBlinkTime = elaspeTime + 0.5;
	}

	if (Application::IsKeyPressed('1'))
	{
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('2'))
	{
		glDisable(GL_CULL_FACE);
	}

	camera.Update(dt);
}

void SceneBoss::UpdateBullets()
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

void SceneBoss::UpdateMonsterBullets()
{

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			for (int j = 0; j < MOBBULLETNUM; j++)
			{
				if (elaspeTime > monsterBulletDelay[i] && monsterBulletPtr[j] == NULL)
				{
					if (i == 0)//bullet fire all around
					{
						if (monsterBulletPtr[j + 1] == NULL
							&& monsterBulletPtr[j + 2] == NULL
							&& monsterBulletPtr[j + 3] == NULL)
						{
							Vector3 bullet1 = Vector3(MonsterPtr[i]->pos.x + 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z);
							Vector3 bullet2 = Vector3(MonsterPtr[i]->pos.x, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z + 1);
							Vector3 bullet3 = Vector3(MonsterPtr[i]->pos.x - 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z);
							Vector3 bullet4 = Vector3(MonsterPtr[i]->pos.x, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z - 1);
							Vector3 bullet5 = Vector3(MonsterPtr[i]->pos.x + 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z + 1);
							Vector3 bullet6 = Vector3(MonsterPtr[i]->pos.x - 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z + 1);
							Vector3 bullet7 = Vector3(MonsterPtr[i]->pos.x + 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z - 1);
							Vector3 bullet8 = Vector3(MonsterPtr[i]->pos.x + 1, MonsterPtr[i]->pos.y, MonsterPtr[i]->pos.z - 1);

							monsterBulletPtr[j] = new monsterBullet(MonsterPtr[i]->pos, bullet1);
							monsterBulletPtr[j + 1] = new monsterBullet(MonsterPtr[i]->pos, bullet2);
							monsterBulletPtr[j + 2] = new monsterBullet(MonsterPtr[i]->pos, bullet3);
							monsterBulletPtr[j + 3] = new monsterBullet(MonsterPtr[i]->pos, bullet4);
							monsterBulletPtr[j + 4] = new monsterBullet(MonsterPtr[i]->pos, bullet5);
							monsterBulletPtr[j + 5] = new monsterBullet(MonsterPtr[i]->pos, bullet6);
							monsterBulletPtr[j + 6] = new monsterBullet(MonsterPtr[i]->pos, bullet7);
							monsterBulletPtr[j + 7] = new monsterBullet(MonsterPtr[i]->pos, bullet8);

							monsterBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
							return;
						}
					}
					else//bullet fires towards player
					{
						monsterBulletPtr[j] = new monsterBullet(MonsterPtr[i]->pos, camera.position);
						monsterBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
						return;
					}
				}
			}
		}
	}

}

void SceneBoss::UpdateMonsters()
{

	if (elaspeTime > monsterTime)
	{
		for (int i = 0; i < MOBNUM; i++)
		{
			if (MonsterPtr[i] == NULL)
			{
				MonsterPtr[i] = new Monster();
				monsterBoxPtr[i] = new Box(MonsterPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
				monsterTime = elaspeTime + 3.0;
				break;
			}
		}
	}

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			(*MonsterPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterBoxPtr[i] = Box(MonsterPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		}
	}
}

void SceneBoss::UpdateBossMovement()
{
	if (elaspeTime > bossMovementChangeTime)
	{
		if (bossMovement == CHARGE)
		{
			bossMovement = STRAIGHT;
		}
		else
		{
			bossMovement++;
		}
		bossMovementChangeTime = elaspeTime + 15.0;
	}

	if (bossMovement == STRAIGHT)
	{
		boss.move(camera.position);
	}
	else if (bossMovement == ZIGZAG)
	{
		boss.moveZigZag(camera.position, elaspeTime);
	}
	else if (bossMovement == ASCEND)
	{
		boss.leap(camera.position);
	}
	else if (bossMovement == CHARGE)
	{
		boss.charge(camera.position, elaspeTime);
	}
}

void SceneBoss::UpdateBossHitbox()
{
	bool isHit = false;
	int monNum;
	hitmarkerSize = 0;
	*bossBox = Box(Vector3(boss.pos), 10);

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		if (!isHit)
		{
			if (bulletBoxPtr[bul] != NULL)
			{
				isHit = bulletPtr[0]->isBulletHit(bulletBoxPtr[bul], bossBox);
			}
		}
	}

	if (isHit)
	{
		hitmarkerTimer = 50;
	}
	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

void SceneBoss::UpdateBossBullets()
{
	if (elaspeTime > bossPlayerShootTime && bossMovement != ASCEND) //shoot at player
	{
		for (int i = 0; i < BOSSBULLETNUM; i++)
		{
			if (bossBulletPtr[i] == NULL)
			{
				bossBulletPtr[i] = new monsterBullet(boss.pos, camera.position);
				bossPlayerShootTime = elaspeTime + 2.0;
				return;
			}
		}
	}

	if (elaspeTime > bossRingShootTime) //shoot in ring
	{
		for (int i = 0; i < BOSSBULLETNUM; i++)
		{
			if (bossBulletPtr[i] == NULL
				&& bossBulletPtr[i + 1] == NULL
				&& bossBulletPtr[i + 2] == NULL
				&& bossBulletPtr[i + 3] == NULL
				&& bossBulletPtr[i + 4] == NULL
				&& bossBulletPtr[i + 5] == NULL
				&& bossBulletPtr[i + 6] == NULL
				&& bossBulletPtr[i + 7] == NULL)
			{
				Vector3 bullet1 = Vector3(boss.pos.x + 1, boss.pos.y, boss.pos.z);
				Vector3 bullet2 = Vector3(boss.pos.x, boss.pos.y, boss.pos.z + 1);
				Vector3 bullet3 = Vector3(boss.pos.x - 1, boss.pos.y, boss.pos.z);
				Vector3 bullet4 = Vector3(boss.pos.x, boss.pos.y, boss.pos.z - 1);
				Vector3 bullet5 = Vector3(boss.pos.x + 1, boss.pos.y, boss.pos.z + 1);
				Vector3 bullet6 = Vector3(boss.pos.x - 1, boss.pos.y, boss.pos.z + 1);
				Vector3 bullet7 = Vector3(boss.pos.x + 1, boss.pos.y, boss.pos.z - 1);
				Vector3 bullet8 = Vector3(boss.pos.x + 1, boss.pos.y, boss.pos.z - 1);

				bossBulletPtr[i] = new monsterBullet(boss.pos, bullet1);
				bossBulletPtr[i + 1] = new monsterBullet(boss.pos, bullet2);
				bossBulletPtr[i + 2] = new monsterBullet(boss.pos, bullet3);
				bossBulletPtr[i + 3] = new monsterBullet(boss.pos, bullet4);
				bossBulletPtr[i + 4] = new monsterBullet(boss.pos, bullet5);
				bossBulletPtr[i + 5] = new monsterBullet(boss.pos, bullet6);
				bossBulletPtr[i + 6] = new monsterBullet(boss.pos, bullet7);
				bossBulletPtr[i + 7] = new monsterBullet(boss.pos, bullet8);

				bossRingShootTime = elaspeTime + 1.0;
				return;
			}
		}
	}

	if (elaspeTime > bossChangeGroundTargetTime)
	{
		groundAreaCenter = Vector3(((rand() % 500) - 250), 0, ((rand() % 1700) - 850));
		bossChangeGroundTargetTime = elaspeTime + 7.0;
		bossGroundAttackDelayTime = elaspeTime + 2.0;
		return;
	}
	if (elaspeTime > bossGroundAttackTime && elaspeTime > bossGroundAttackDelayTime) // shoot from ground up
	{
		if (groundAreaCenter != NULL)
		{
			float randomGroundBulletX = ((rand() % 201) - 100 + groundAreaCenter.x);
			float randomGroundBulletZ = ((rand() % 201) - 100 + groundAreaCenter.z);
			Vector3 randomGroundBullet = Vector3(randomGroundBulletX, -5, randomGroundBulletZ);
			Vector3 randomGroundBulletTarget = Vector3(randomGroundBulletX, 1, randomGroundBulletZ);

			for (int i = 0; i < BOSSBULLETNUM; i++)
			{
				if (bossBulletPtr[i] == NULL)
				{
					bossBulletPtr[i] = new monsterBullet(randomGroundBullet, randomGroundBulletTarget);
					//bossGroundAttackTime = elaspeTime + 0.0000001;
					return;
				}
			}
		}
	}
}

void SceneBoss::UpdateMonsterHitbox()
{
	bool isHit = false;
	int monNum;
	hitmarkerSize = 0;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM; mon++)
		{
			if (!isHit)
			{
				if (bulletBoxPtr[bul] != NULL && monsterBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[0]->isBulletHit(bulletBoxPtr[bul], monsterBoxPtr[mon]);
				}
				if (isHit)
				{
					monNum = mon;
					bulletPtr[bul]->monsterHit(camera);
				}
			}
		}
	}

	if (isHit)
	{
		hitmarkerTimer = 50;
	}
	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

void SceneBoss::Render()
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
	//RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();
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
	modelStack.Translate(0, -10, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

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
	modelStack.Translate(200, -10, -200);
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

	modelStack.PushMatrix();
	modelStack.Translate(-200, -10, -200);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

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
		modelStack.Translate(300 - i, -10, 890);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(300 - i, -10, -910);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	//SPAWN MOBS
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			if (i == 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y, (*MonsterPtr[i]).pos.z);
				modelStack.Scale(12, 12, 12);
				RenderMesh(meshList[GEO_SPHERE], true);
				modelStack.PopMatrix();
			}
			modelStack.PushMatrix();
			modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y, (*MonsterPtr[i]).pos.z);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_CUBE], true);
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
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	for (int i = 0; i < BOSSBULLETNUM; i++)
	{
		if (bossBulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*bossBulletPtr[i]).pos.x, (*bossBulletPtr[i]).pos.y, (*bossBulletPtr[i]).pos.z);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(boss.pos.x, boss.pos.y, boss.pos.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_CUBE], true);
	modelStack.PopMatrix();

	if (printGroundSignal && groundAreaCenter != NULL)
	{
		modelStack.PushMatrix();
		modelStack.Translate(groundAreaCenter.x, groundAreaCenter.y - 9.9, groundAreaCenter.z);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_GROUNDSIGNAL], false);
		modelStack.PopMatrix();
	}

	RenderBullets();

	//FPS
	std::ostringstream sFps;
	sFps << std::fixed << std::setprecision(3);
	sFps << 1.0 / deltaTime << "fps";
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], sFps.str(), Color(1, 1, 1), 2, 1, 29);
	modelStack.PopMatrix();

	RenderHitmarker();

	if (gameOver)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "GAME OVER", Color(1, 1, 1), 5, 4, 5);
	}
}

void SceneBoss::RenderMesh(Mesh *mesh, bool enableLight)
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

void SceneBoss::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneBoss::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneBoss::RenderBullets()
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

void SceneBoss::RenderHitmarker()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0, 1, 1), 5, 8.3, 6.1);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1, 0, 0), hitmarkerSize, 8.3, 6.1);
}

void SceneBoss::Exit()
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
