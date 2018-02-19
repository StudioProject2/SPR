#include "SceneStage2.h"
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

SceneStage2::SceneStage2()
{
}

SceneStage2::~SceneStage2()
{
	for (int i = 0; i < NO_OF_BULLETS; i++)
	{
		if (bulletPtr[i] != NULL)
		{
			delete bulletPtr[i];
			delete bulletBoxPtr[i];
		}
	}
}

void SceneStage2::Init()
{
	//Monster spawn
	srand((unsigned int)time(NULL));
	//Timer
	elaspeTime = 0.0;
	deltaTime = 0.0;
	monsterTime = elaspeTime + 3.0;

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

	camera.Init(Vector3(0, 100, 600), Vector3(0, 100, 0), Vector3(0, 1, 0));

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
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(720, 150, 720);
	light[1].color.Set(1, 1, 1);
	light[1].power = 20;
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
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateHem("Sphere", Color(1.0f, 1.0f, 1.0f), 20, 20, 0.5);
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
	meshList[GEO_BOTTOM]->textureID = LoadTGAR("Image//Stage2//skybox_bottom.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;
	
	//Bullet
	meshList[GEO_SPHERE] = MeshBuilder::GenerateHem("Bullet", Color(1.0f, 1.0f, 1.0f), 10, 10, 1);

	//tree
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//stage2//Tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//stage2//objtextures//Tree2.tga");

	//grass, flower and rocks
	meshList[GEO_GRASS_PATCH] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//Grass_Patch.obj");
	meshList[GEO_GRASS_PATCH]->textureID = LoadTGA("Image//stage2//objtextures//Grass2.tga");
	meshList[GEO_GRASS_LINE] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//Grass_Line.obj");
	meshList[GEO_GRASS_LINE]->textureID = LoadTGA("Image//stage2//objtextures//Grass2.tga");
	meshList[GEO_FLOWER] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//flowerOBJ.obj");
	meshList[GEO_FLOWER]->textureID = LoadTGA("Image//stage2//objtextures//flowerTextured.tga");
	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//rockOBJ.obj");
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//stage2//objtextures//gray.tga");


	//Debuggging Cube
	meshList[GEO_CUBE] = MeshBuilder::GenerateOBJ("cube", "OBJ//Cube.obj");

	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//Monsters

	for (int i = 0; i < 25; i++)
	{
		monsterBulletPtr[i] = NULL;
	}

	gameOver = false;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		//init collision for the bullets here
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	}
}

void SceneStage2::Update(double dt)
{
	static const float LSPEED = 10.0f;
	elaspeTime += dt;
	deltaTime = dt;
	deltaTime = dt;
	start.isShooting = true;

	UpdateBullets();
	UpdateMonsters();
	//UpdateMonsterBullets();
	UpdateMonsterHitbox();


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
void SceneStage2::UpdateBullets()
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

void SceneStage2::UpdateMonsterBullets()
{
	Box player = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);

	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			for (int j = 0; j < MOBBULLETNUM; j++)
			{
				if (elaspeTime > monsterBulletDelay[i] && monsterBulletPtr[j] == NULL)
				{
					monsterBulletPtr[j] = new monsterBullet(MonsterPtr[i], camera.position);
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
			if (monsterBulletPtr[i]->isBulletInBox(player))
			{
				gameOver = true;
			}
			if (monsterBulletPtr[i]->bulletCollide())
			{
				monsterBulletPtr[i] = NULL;
				delete monsterBulletPtr[i];
			}
		}
	}

}
void SceneStage2::UpdateMonsters()
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

void SceneStage2::UpdateMonsterHitbox()
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
					delete MonsterPtr[mon];
					delete monsterBoxPtr[mon];
					MonsterPtr[mon] = NULL;
					monsterBoxPtr[mon] = NULL;
					monsterTime = elaspeTime + 3.0;
				}
			}
		}
	}
	if (isHit)
	{
		hitmarkerTimer = 30;
	}
	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

void SceneStage2::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();
	
	//Lights && Skybox
	RenderLights();
	RenderSkybox();

	//World Objs
	RenderObj();
	
	//MOBS
	RenderMonster();
	RednerMonsterBullets();
	
	//Player
	RenderBullets();
	RenderUi();
	RenderHitmarker();

	if (gameOver)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "GAME OVER", Color(1, 1, 1), 5, 4, 5);
	}

}

void SceneStage2::RenderMesh(Mesh *mesh, bool enableLight)
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

void SceneStage2::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneStage2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneStage2::RenderSkybox()
{
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
	modelStack.Translate(0, -10, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

}
void SceneStage2::RenderObj()
{
	//boundaries
	for (int i = 0; i < 1401; i += 350)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-810, -10, -700 + i);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(810, -10, -700 + i);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();
	}
	for (int i = 0; i < 1401; i += 350)
	{
		modelStack.PushMatrix();
		modelStack.Translate(650 - i, -10, 810);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(650 - i, -10, -810);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_GRASS_LINE], false);
		modelStack.PopMatrix();
	}

	//tree of LIFE
	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_TREE], false);
	modelStack.PopMatrix();

	//grass
	for (int x = 0; x < 1351; x += 225)
	{
		for (int z = 0; z < 1351; z += 225)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-675 + x, -10, -675 + z);
			modelStack.Scale(40, 5, 40);
			RenderMesh(meshList[GEO_GRASS_PATCH], false);
			modelStack.PopMatrix();
		}
	}

	//rocks
	for (int i = 0; i < 360; i += 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -10, 0);
		modelStack.PushMatrix();
		modelStack.Rotate(i, 0, 1, 0);
		modelStack.Translate(100, 0, 0);
		RenderMesh(meshList[GEO_ROCK], false);
		modelStack.Scale(3, 3, 3);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	RenderMisc();
}
void SceneStage2::RenderMisc()
{
	//flowers
	modelStack.PushMatrix();
	modelStack.Translate(100, -10, 100);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(240, -10, 500);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-530, -10, 200);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, -10, -350);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(750, -10, -200);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-340, -10, 305);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -10, 100);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -10, 100);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -10, 100);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, -10, 100);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_FLOWER], false);
	modelStack.PopMatrix();
}

void SceneStage2::RenderLights()
{
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

}
void SceneStage2::RenderBullets()
{
	for (int i = 0; i < NO_OF_BULLETS; i++)
	{
		if (bulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate(bulletPtr[i]->throws.x, bulletPtr[i]->throws.y + bulletPtr[i]->offsetY, bulletPtr[i]->throws.z);
			RenderMesh(meshList[GEO_BULLETS], false);
			modelStack.PopMatrix();
		}
	}
}
void SceneStage2::RenderMonster()
{
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y, (*MonsterPtr[i]).pos.z);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_CUBE], false);
			modelStack.PopMatrix();
		}
	}
}
void SceneStage2::RednerMonsterBullets()
{
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

}
void SceneStage2::RenderHitmarker()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0, 1, 1), 5, 8.3, 6.1);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1, 0, 0), hitmarkerSize, 8.3, 6.1);
}
void SceneStage2::RenderUi()
{
	std::ostringstream sFps;
	sFps << std::fixed << std::setprecision(3);
	sFps << 1.0 / deltaTime << "fps";


	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], sFps.str(), Color(1, 1, 1), 2, 1, 29);
	modelStack.PopMatrix();

	std::ostringstream timePast;
	timePast << std::fixed << std::setprecision(1);
	timePast << elaspeTime << "time past.";
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], timePast.str(), Color(1, 1, 1), 2, 29, 29);
	modelStack.PopMatrix();

}

void SceneStage2::Exit()
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
