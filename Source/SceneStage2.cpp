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
	//Monster Animation
	fodSwingTimer = 0;
	fodLeft = false;
	fodderArmSwing = 0.0;

	dodSwingTimer = 0;
	dodLeft = false;
	dodgerArmSwing = 0.0;
	dodgerLegSwing = 0.0;
	//Timer
	elaspeTime = 0.0;
	deltaTime = 0.0;
	monsterTime = elaspeTime + 3.0;
	hitmarkerSize = 0;
	bulletBounceTime = 0.0;
	playerHurtBounceTime = 0.0;
	//Interaction text Size
	interactionSize = 0;
	//monster objective counter
	monDead = 0;
	//tree and flower
	flowersAmt = 0;
	flowerOneLife = true;
	flowerTwoLife = true;
	flowerThreeLife = true;
	treeLifeOne = true;
	treeLifeTwo = true;
	treeLifeThree = true;
	treeY = -10;
	treeRotate = 0;
	treeFallTimer = 0;
	fallingStage = 0;
	//pickups
	pickupsTimer = 0;
	pickupsSpawn = false;
	pickupsFlying = false;
	pickupsY = 10;
	pickupsZ = 0;
	//player
	player = Player::getInstance();
	//objectives
	gameOver = false;
	objectiveOne = false;
	objectiveTwo = false;
	objectiveThree = false;
	objectiveFour = false;


	//INIT monsters
	for (int i = 0; i < MOBNUM2; i++)
	{
		MonsterPtr[i] = NULL;
		monsterBoxPtr[i] = NULL;
		monsterBulletDelay[i] = elaspeTime + 4.0;
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		MonsterFodderPtr[i] = NULL;
		monsterFodderBoxPtr[i] = NULL;
	}
	monsterFodderTime = 0.0;

	for (int i = 0; i < 25; i++)
	{
		monsterBulletPtr[i] = NULL;
	}

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
		bulletBoxPtr[bul]->position.y += bulletPtr[bul]->offsetY;
	}

	//*****************************************Init Lights and Meshes*******************************************************
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
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
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
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

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 4);

	glEnable(GL_DEPTH_TEST);

	//***************************************First Light*****************************************
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 150, 0);
	light[0].color.Set(0, 1, 0);
	light[0].power = 100;
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
	light[1].position.Set(240, 0, 500);
	light[1].color.Set(0, 1, 0);
	light[1].power = 4;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(10));
	light[1].cosInner = cos(Math::DegreeToRadian(5));
	light[1].exponent = 3.0f;
	light[1].spotDirection.Set(0.f, 0.f, 0.f);

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
	light[2].position.Set(750, 0, -200);
	light[2].color.Set(0, 1, 0);
	light[2].power = 4;
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
	light[3].position.Set(-530, 0, 200);
	light[3].color.Set(0, 1, 0);
	light[3].power = 4;
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
	meshList[GEO_SPHERE] = MeshBuilder::GenerateOBJ("bullets", "OBJ//MonstersOBJ//MonsterBulletOBJ.obj");
	meshList[GEO_SPHERE]->textureID = LoadTGA("Image//MonsterTextures//ArcherWeaponTexture.tga");

	//SKYBOX STUFF
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad1("front", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Stage2//skybox_front.tga");
	meshList[GEO_FRONT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FRONT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FRONT]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FRONT]->material.kShininess = 0.9f;
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad1("back", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Stage2//skybox_back.tga");
	meshList[GEO_BACK]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BACK]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BACK]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_BACK]->material.kShininess = 0.9f;
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad1("left", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Stage2//skybox_right.tga");
	meshList[GEO_LEFT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_LEFT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_LEFT]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_LEFT]->material.kShininess = 0.9f;
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad1("right", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Stage2//skybox_left.tga");
	meshList[GEO_RIGHT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_RIGHT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_RIGHT]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_RIGHT]->material.kShininess = 0.9f;
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Stage2//skybox_top.tga");
	meshList[GEO_TOP]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TOP]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TOP]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TOP]->material.kShininess = 0.9f;
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad1("bottom", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGAR("Image//Stage2//skybox_bottom.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOTTOM]->material.kShininess = 0.9f;
	
	//barrier
	meshList[GEO_BARRIER] = MeshBuilder::GenerateQuad("Barrier", Color(1.0f, 1.0f, 1.0f), 10, 10);
	meshList[GEO_BARRIER]->textureID = LoadTGAR("Image//Stage2//Barrier.tga");

	//Player
	meshList[GEO_BULLETS] = MeshBuilder::GenerateHem("bullets", Color(0.7f, 1.0f, 0.7f), 20, 20, 0.5);
	meshList[GEO_PLAYER_TEETH] = MeshBuilder::GenerateOBJ("teeth", "OBJ//PlayerTeeth.obj");
	meshList[GEO_PLAYER_TEETH]->textureID = LoadTGA("Image//PlayerTeeth.tga");
	meshList[GEO_PLAYER_TEETH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLAYER_TEETH]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PLAYER_TEETH]->material.kSpecular.Set(0.01f, 0.01f, 0.01f);
	meshList[GEO_PLAYER_TEETH]->material.kShininess = 1.0f;
	meshList[GEO_PLAYERHEALTH] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	meshList[GEO_PLAYERHEALTH]->textureID = LoadTGA("Image//playerHealth.tga");


	//Player Health
	meshList[GEO_PLAYERHEALTH] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	meshList[GEO_PLAYERHEALTH]->textureID = LoadTGA("Image//playerHealth.tga");

	//tree
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//stage2//Tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//stage2//objtextures//Tree2.tga");
	meshList[GEO_TREE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TREE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TREE]->material.kSpecular.Set(0.01f, 0.01f, 0.01f);
	meshList[GEO_TREE]->material.kShininess = 0.01f;

	//grass, flower and rocks
	meshList[GEO_GRASS_PATCH] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//Grass_Patch.obj");
	meshList[GEO_GRASS_PATCH]->textureID = LoadTGA("Image//stage2//objtextures//Grass2.tga");
	meshList[GEO_GRASS_PATCH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GRASS_PATCH]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_GRASS_PATCH]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_GRASS_PATCH]->material.kShininess = 1.f;
	meshList[GEO_GRASS_LINE] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//Grass_Line.obj");
	meshList[GEO_GRASS_LINE]->textureID = LoadTGA("Image//stage2//objtextures//Grass2.tga");
	meshList[GEO_GRASS_LINE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GRASS_LINE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_GRASS_LINE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_GRASS_LINE]->material.kShininess = 1.f;
	meshList[GEO_FLOWER] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//flowerOBJ.obj");
	meshList[GEO_FLOWER]->textureID = LoadTGA("Image//stage2//objtextures//flowerTextured.tga");
	meshList[GEO_FLOWER]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOWER]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLOWER]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FLOWER]->material.kShininess = 1.f;
	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("grass", "OBJ//stage2//rockOBJ.obj");
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//stage2//objtextures//gray.tga");
	meshList[GEO_ROCK]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ROCK]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ROCK]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ROCK]->material.kShininess = 1.f;

	//monster models
	meshList[GEO_FODDER_BODY] = MeshBuilder::GenerateOBJ("fodder", "OBJ//MonstersOBJ//FodderBodyOBJ.obj");
	meshList[GEO_FODDER_BODY]->textureID = LoadTGA("Image//MonsterTextures//FodderTexture.tga");
	meshList[GEO_FODDER_BODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FODDER_BODY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FODDER_BODY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FODDER_BODY]->material.kShininess = 1.f;
	meshList[GEO_FODDER_HAND] = MeshBuilder::GenerateOBJ("fodder", "OBJ//MonstersOBJ//FodderHandOBJ.obj");
	meshList[GEO_FODDER_HAND]->textureID = LoadTGA("Image//MonsterTextures//FodderTexture.tga");
	meshList[GEO_FODDER_HAND]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FODDER_HAND]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FODDER_HAND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FODDER_HAND]->material.kShininess = 1.f;
	meshList[GEO_DODGER_BODY] = MeshBuilder::GenerateOBJ("dodger", "OBJ//MonstersOBJ//DodgerBodyOBJ.obj");
	meshList[GEO_DODGER_BODY]->textureID = LoadTGA("Image//MonsterTextures//DodgerTexture.tga");
	meshList[GEO_DODGER_BODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DODGER_BODY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_DODGER_BODY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_DODGER_BODY]->material.kShininess = 1.f;
	meshList[GEO_DODGER_HAND] = MeshBuilder::GenerateOBJ("dodger", "OBJ//MonstersOBJ//DodgerHandOBJ.obj");
	meshList[GEO_DODGER_HAND]->textureID = LoadTGA("Image//MonsterTextures//DodgerTexture.tga");
	meshList[GEO_DODGER_HAND]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DODGER_HAND]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_DODGER_HAND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_DODGER_HAND]->material.kShininess = 1.f;
	meshList[GEO_DODGER_LEG] = MeshBuilder::GenerateOBJ("dodger", "OBJ//MonstersOBJ//DodgerLegOBJ.obj");
	meshList[GEO_DODGER_LEG]->textureID = LoadTGA("Image//MonsterTextures//DodgerTexture.tga");
	meshList[GEO_DODGER_LEG]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DODGER_LEG]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_DODGER_LEG]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_DODGER_LEG]->material.kShininess = 1.f;
	meshList[GEO_DODGER_WEAPON] = MeshBuilder::GenerateOBJ("dodger", "OBJ//MonstersOBJ//DodgerWeaponOBJ.obj");
	meshList[GEO_DODGER_WEAPON]->textureID = LoadTGA("Image//MonsterTextures//DodgerWeaponTexture.tga");
	meshList[GEO_DODGER_WEAPON]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_DODGER_WEAPON]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_DODGER_WEAPON]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_DODGER_WEAPON]->material.kShininess = 1.f;

	//pickup model
	meshList[GEO_PICKUP] = MeshBuilder::GenerateOBJ("pickups", "OBJ//PickupOBJ.obj");
	meshList[GEO_PICKUP]->textureID = LoadTGA("Image//HealthPickupTexture.tga");

	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//Objective Indicator
	meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("building", "OBJ//arrow.obj");
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//green.tga");

	engine = createIrrKlangDevice();
}

bool SceneStage2::isNearObject(Camera3 camera, Box object)
{
	return ((camera.position.x >= object.minX && camera.position.x <= object.maxX) &&
			(camera.position.y >= object.minY && camera.position.y <= object.maxY) &&
			(camera.position.z >= object.minZ && camera.position.z <= object.maxZ));
}
bool SceneStage2::isInObjectX(Camera3 camera, Box object)
{
	return (((camera.position.x >= object.minX - 2 && camera.position.x <= object.minX + 2)
		&& (camera.position.y >= object.minY && camera.position.y <= object.maxY)
		&& (camera.position.z >= object.minZ && camera.position.z <= object.maxZ))
		|| ((camera.position.x >= object.maxX - 2 && camera.position.x <= object.maxX + 2)
			&& (camera.position.y >= object.minY && camera.position.y <= object.maxY)
			&& (camera.position.z >= object.minZ && camera.position.z <= object.maxZ)));
}
bool SceneStage2::isInObjectY(Camera3 camera, Box object)
{
	return (((camera.position.x >= object.minX && camera.position.x <= object.maxX)
		&& (camera.position.y >= object.minY - 2 && camera.position.y <= object.minY + 2)
		&& (camera.position.z >= object.minZ && camera.position.z <= object.maxZ))
		|| ((camera.position.x >= object.minX && camera.position.x <= object.maxX)
			&& (camera.position.y >= object.maxY - 2 && camera.position.y <= object.maxY + 2)
			&& (camera.position.z >= object.minZ && camera.position.z <= object.maxZ)));
	
}
bool SceneStage2::isInObjectZ(Camera3 camera, Box object)
{
	return (((camera.position.x >= object.minX && camera.position.x <= object.maxX)
		&& (camera.position.y >= object.minY && camera.position.y <= object.maxY)
		&& (camera.position.z >= object.minZ - 2 && camera.position.z <= object.minZ + 2))
		|| ((camera.position.x >= object.minX && camera.position.x <= object.maxX)
			&& (camera.position.y >= object.minY && camera.position.y <= object.maxY)
			&& (camera.position.z >= object.maxZ - 2 && camera.position.z <= object.maxZ + 2)));
}

void SceneStage2::Update(double dt)
{
	if (objectiveThree)
	{
		for (int mon = 0; mon < MOBNUM2; mon++)
		{
			delete MonsterPtr[mon];
			delete monsterBoxPtr[mon];
			MonsterPtr[mon] = NULL;
			monsterBoxPtr[mon] = NULL;
		}
	}
	static const float LSPEED = 10.0f;
	elaspeTime += dt;
	deltaTime = dt;
	player->timer += dt;
	start.isShooting = true;

	UpdateBullets();
	if (!objectiveThree)
	{
		UpdateMonsters();
		UpdateMonsterAnimations();
	}
	UpdateMonsterBullets();
	UpdateMonsterHitbox();
	UpdateInteractions();
	UpdatePickups();

	if (player->health <= 0)
	{
		Application::sceneChange = Application::LOSE;
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
	UpdateCollision();
	UpdateObjective();

	if (gameOver)
	{
		player->health = 100;
		player->damage = 10;
		player->points = 0;
		Application::sceneChange = 0;
	}
	if (movingUp == true)
	{
		yArrowTranslate += (float)(30 * dt);
	}
	if (movingUp == false)
	{
		yArrowTranslate -= (float)(30 * dt);
	}
	if (yArrowTranslate > 50)
	{
		movingUp = false;
	}
	if (yArrowTranslate < 30)
	{
		movingUp = true;
	}
}

//checks the objectives progression
void SceneStage2::UpdateObjective()
{
	//finishes objective 1 if amount of dead monsters is more than amount need to kill
	if (monDead >= MOBNUM_TO_KILL)
	{	
		objectiveOne = true;
	}
	//finishes objective 2 when all three flower are deadeadead
	if (objectiveOne)
	{
		if (!flowerOneLife && !flowerTwoLife && !flowerThreeLife)
		{
			objectiveTwo = true;
		}
	}
	//finishes objective 3 when Tree Of Life is dead
	if (objectiveTwo)
	{
		if (!treeLifeThree)
		{
			objectiveThree = true;
		}
	}

}
//updates the player's collision for this stage
void SceneStage2::UpdateCollision()
{
	//inits
	bool hitX = false;
	bool hitY = false;
	bool hitZ = false;
	Box treeOfLife;
	Box treeBarrier;
	Box treeFallen;
	Vector3 view = (camera.target - camera.position).Normalized();
	
	//changes tree collision box when tree has fallen
	if (objectiveThree)
	{
		treeFallen = Box(Vector3(143, 0, -122), 87, 87);
	}
	if (!hitX)
	{
		hitX = isInObjectX(camera, treeFallen);
	}
	if (!hitY)
	{
		hitY = isInObjectY(camera, treeFallen);
	}
	if (!hitZ)
	{
		hitZ = isInObjectZ(camera, treeFallen);
	}

	//tree collision for when tree is not fallen
	if (!objectiveThree)
	{
		treeOfLife = Box(Vector3(-10, 0, 10), 25, 25);
	}
	if (!hitX)
	{
		hitX = isInObjectX(camera, treeOfLife);
	}
	if (!hitY)
	{
		hitY = isInObjectY(camera, treeOfLife);
	}
	if (!hitZ)
	{
		hitZ = isInObjectZ(camera, treeOfLife);
	}

	//barrier collision
	if (!objectiveTwo)
	{
		treeBarrier = Box(Vector3(-10, 0, 10), 70, 70);
	}
	if (!hitX)
	{
		hitX = isInObjectX(camera, treeBarrier);
	}
	if (!hitY)
	{
		hitY = isInObjectY(camera, treeBarrier);
	}
	if (!hitZ)
	{
		hitZ = isInObjectZ(camera, treeBarrier);
	}

	//when player has collided logic
	if (hitX)
	{
		camera.position.x = camera.prevPosX;
		camera.target = camera.position + view;
	}
	if (hitY)
	{
		camera.position.y = camera.prevPosY;
		camera.target = camera.position + view;
	}
	if (hitZ)
	{
		camera.position.z = camera.prevPosZ;
		camera.target = camera.position + view;
	}

}

//Updates player bullet
void SceneStage2::UpdateBullets()
{
	Vector3 view = (camera.target - camera.position).Normalized();
	for (int i = 0; i < NO_OF_BULLETS; i++)
	{
		if (bulletPtr[i] != NULL)
		{
			if (i == 0) //Logic for first bullet
			{
				bulletPtr[0]->updateBullet(view, camera, start);
				*bulletBoxPtr[0] = Box(bulletPtr[0]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
			}
			else //Logic for the rest of the bullets
			{
				bulletPtr[i]->updateBullet(view, camera, *bulletPtr[i - 1]);
				*bulletBoxPtr[i] = Box(bulletPtr[i]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
			}
		}
	}
}
//checks if monster bullet hits the player
void SceneStage2::UpdateMonsterBullets()
{
	Box playerBox = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);

	for (int i = 0; i < MOBNUM2; i++)
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
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
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

}
//create, delete monster, update the monsters and their hitbox position
void SceneStage2::UpdateMonsters()
{
	
	//Monster DODGER
	if (elaspeTime > monsterTime)
	{
		for (int i = 0; i < MOBNUM2; i++)
		{
			if (MonsterPtr[i] == NULL)
			{
				MonsterPtr[i] = new Monster();
				monsterBoxPtr[i] = new Box(MonsterPtr[i]->pos, 7, 10, 10, 15);
				monsterTime = elaspeTime + 3.0;
				break;
			}
		}
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			(*MonsterPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterBoxPtr[i] = Box(MonsterPtr[i]->pos, 7, 10, 10, 15);
		}
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			if ((*MonsterPtr[i]).health <= 0)
			{
				delete MonsterPtr[i];
				delete monsterBoxPtr[i];
				MonsterPtr[i] = NULL;
				monsterBoxPtr[i] = NULL;
				monDead += 1;
			}
		}
	}
	
	//Monster Fodder
	if (elaspeTime > monsterFodderTime)
	{
		for (int i = 0; i < MOBNUM2; i++)
		{
			if (MonsterFodderPtr[i] == NULL)
			{
				MonsterFodderPtr[i] = new MonsterFodder();
				monsterFodderBoxPtr[i] = new Box(MonsterFodderPtr[i]->pos, 10, 10, 30);
				monsterFodderTime = elaspeTime + 3.0;
				break;
			}
		}
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			(*MonsterFodderPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterFodderBoxPtr[i] = Box(MonsterFodderPtr[i]->pos, 10, 10, 12);
		}
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			if ((*MonsterFodderPtr[i]).health <= 0)
			{
				delete MonsterFodderPtr[i];
				delete monsterFodderBoxPtr[i];
				MonsterFodderPtr[i] = NULL;
				monsterFodderBoxPtr[i] = NULL;
				monDead += 1;
			}
		}
	}
}
//checks if bullet hits monster and if monster touches player
void SceneStage2::UpdateMonsterHitbox()
{
	//inits
	bool isHit = false;
	hitmarkerSize = 0;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++) //checks every bullet
	{
		for (int mon = 0; mon < MOBNUM2; mon++)	//against every monster
		{
			if (!isHit && elaspeTime > bulletBounceTime)	//checks if they intersect
			{
				if (bulletBoxPtr[bul] != NULL && monsterBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[bul]->isBulletHit(bulletBoxPtr[bul], monsterBoxPtr[mon]);
				}
				if (isHit)
				{
					(*MonsterPtr[mon]).health = (*MonsterPtr[mon]).health - player->damage;
					if (!Application::muted)
					{
						engine->play2D("Sound/highHumanHit.wav", false);
					}
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

	//Monster Fodder
	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM2; mon++)
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
					if (!Application::muted)
					{
						engine->play2D("Sound/femaleHit.wav", false);
					}
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

	//When player touch monster hitbox
	Box *playerBox = new Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);

	for (int i = 0; i < MOBNUM2; i++)
	{
		if (monsterBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterBoxPtr[i]))
			{
				player->health -= 10;
				playerHurtBounceTime = elaspeTime + 0.5;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
			}
		}

		if (monsterFodderBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterFodderBoxPtr[i]))
			{
				player->health -= 10;
				playerHurtBounceTime = elaspeTime + 0.5;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
			}
		}
	}

	delete playerBox;

	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

//Updates for interactions and animations logic
void SceneStage2::UpdateInteractions()
{
	//inits
	Box flowerOfLifeThree;
	Box flowerOfLifeTwo;
	Box flowerOfLifeOne;
	Box treeOfLife;
	Box healthPack;
	Box exit;

	interactionSize = 0;
	bool inRange = false;
	bool inPickupRange = false;
	bool inExit = false;

	//interaction range for the exit of stage 2
	if (objectiveThree)
	{
		exit = Box(Vector3(0, 0, -790), 1000, 26);
	}
	if (!inExit)
	{
		inExit = isNearObject(camera, exit);
	}
	if (inExit)
	{
		objectiveFour = true;
	}
	//collect drops from the tree of life
	if (pickupsSpawn)
	{
		healthPack = Box(Vector3(0, pickupsY, pickupsZ), 10, 10);
	}
	if (!inPickupRange)
	{
		inPickupRange = isNearObject(camera, healthPack);
	}
	if (inPickupRange)
	{
		pickupsSpawn = false;
		player->health += 40;
	}
	//allows interaction with tree if tree is still alive
	if (treeLifeThree && objectiveTwo)
	{
		treeOfLife = Box(Vector3(-10, 0, 10), 50, 50);
	}
	if (!inRange)
	{
		inRange = isNearObject(camera, treeOfLife);
	}
	//interaction with the glowing flowers
	if (objectiveOne)
	{
		if (flowerOneLife)
		{
			flowerOfLifeOne = Box(Vector3(240, 0, 500), 15);
		}
		if (flowerTwoLife)
		{
			flowerOfLifeTwo = Box(Vector3(750, 0, -200), 15);
		}
		if (flowerThreeLife)
		{
			flowerOfLifeThree = Box(Vector3(-530, 0, 200), 15);
		}
	}
	if (!inRange)
	{
		inRange = isNearObject(camera, flowerOfLifeOne);
	}
	if (!inRange)
	{
		inRange = isNearObject(camera, flowerOfLifeTwo);
	}
	if (!inRange)
	{
		inRange = isNearObject(camera, flowerOfLifeThree);
	}
	if (inRange)
	{
		interactionSize = 4;
	}
	// logic for tree interactions
	if (Application::IsKeyPressed('E'))
	{
		if (treeFallTimer == 0 && objectiveTwo)
		{
			if (isNearObject(camera, treeOfLife))
			{
				if (treeLifeOne) 
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/treeCut1.wav", false);
					}
					treeFallTimer = 60;
					fallingStage = 1;
				}
				else if (treeLifeTwo && !treeLifeOne) 
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/treeCut1.wav", false);
					}
					treeFallTimer = 60;
					fallingStage = 2;
				}
				else if (treeLifeThree && !treeLifeTwo)
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/treeCut2.wav", false);
					}
					treeFallTimer = 60;
					fallingStage = 3;
				}
			}
		}
		if (objectiveOne)
		{
			if (isNearObject(camera, flowerOfLifeOne))
			{
				flowerOneLife = false;
				flowersAmt += 1;
				light[1].power = 0;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
				if (flowersAmt == 3)
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/bigDing.wav", false);
					}
				}
				else
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/ding.wav", false);
					}
				}
			}
			if (isNearObject(camera, flowerOfLifeTwo))
			{
				flowerTwoLife = false;
				flowersAmt += 1;
				light[2].power = 0;
				glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
				if (flowersAmt == 3)
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/bigDing.wav", false);
					}
				}
				else
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/ding.wav", false);
					}
				}
			}
			if (isNearObject(camera, flowerOfLifeThree))
			{
				flowerThreeLife = false;
				flowersAmt += 1;
				light[3].power = 0;
				glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
				if (flowersAmt == 3)
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/bigDing.wav", false);
					}
				}
				else
				{
					if (!Application::muted)
					{
						engine->play2D("Sound/ding.wav", false);
					}
				}
			}
		}
	}
	// tree falling animations
	if (treeFallTimer > 0 && fallingStage == 1)
	{
		treeFallTimer -= 1;
		treeY -= 0.2f;
		interactionSize = 0;
		if (treeFallTimer == 0)
		{
			treeLifeOne = false;
			light[0].power = 66;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
	}
	else if (treeFallTimer > 0 && fallingStage == 2)
	{
		treeFallTimer -= 1.f;
		treeY -= 0.4f;
		treeRotate -= 0.4f;
		interactionSize = 0.f;
		light[0].position.Set(0.f + treeY, 150.f, 0.f);
		if (treeFallTimer == 0)
		{
			treeLifeTwo = false;
			light[0].power = 33;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
	}
	else if (treeFallTimer > 0 && fallingStage == 3)
	{
		treeFallTimer -= 1.f;
		treeRotate -= 0.8f;
		interactionSize = 0.f;
		if (treeFallTimer == 0)
		{
			pickupsSpawn = true;
			treeLifeThree = false;
			light[0].power = 0;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		}
	}

	if (Application::IsKeyPressed('E') && inExit && objectiveFour)
	{
		player->points += 100;
		Application::sceneChange = Application::STAGE3;
	}

}
void SceneStage2::UpdatePickups()
{

	if (pickupsSpawn)
	{
		if (!pickupsFlying)
		{
			if (pickupsTimer < 60)
			{
				pickupsTimer += 1;
				pickupsY += 1;
			}
			else
			{
				pickupsFlying = true;
			}
		}
		else
		{
			if (pickupsTimer > 0)
			{
				pickupsTimer -= 1;
				pickupsY -= 1.2f;
				pickupsZ += 1;
			}
		}
	}
}
void SceneStage2::UpdateMonsterAnimations()
{
	//Fodder
	if (!fodLeft)
	{
		if (fodSwingTimer < 30)
		{
			fodSwingTimer += 1;
			fodderArmSwing += 4;
		}
		else
		{
			fodLeft = true;
		}
	}
	else
	{
		if (fodSwingTimer > 0)
		{
			fodSwingTimer -= 1;
			fodderArmSwing -= 4;
		}
		else
		{
			fodLeft = false;
		}
	}
	//Dodger
	if (!dodLeft)
	{
		if (dodSwingTimer < 30)
		{
			dodSwingTimer += 1;
			dodgerArmSwing += 4;
			dodgerLegSwing += 1.5;
		}
		else
		{
			dodLeft = true;
		}
	}
	else
	{
		if (dodSwingTimer > 0)
		{
			dodSwingTimer -= 1;
			dodgerArmSwing -= 4;
			dodgerLegSwing -= 1.5;
		}
		else
		{
			dodLeft = false;
		}
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
	RenderPickups();

	//MOBS
	if (!objectiveThree)
	{
		RenderMonster();
	}
	RenderMonsterBullets();
	
	//Exit Indicator
	if (objectiveThree)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, yArrowTranslate + 15, -910);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_ARROW], false);
		modelStack.PopMatrix();
	}

	//Player
	RenderBullets();
	RenderTopTeeth();
	RenderBottomTeeth();
	RenderObjectives();
	RenderPlayerHealth();
	RenderUi();
	RenderPlayerHealth();
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
		characterSpacing.SetToTranslation(i * 0.7f, 0, 0); //1.0f is the spacing of each character, you may change this value
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

void SceneStage2::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneStage2::RenderTopTeeth()
{
	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(-1.3f, 8, -17);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.5f, 1, 1);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage2::RenderBottomTeeth()
{

	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(1.3f, -8, -17);
	modelStack.Scale(1.5f, 1, 1);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage2::RenderPlayerHealth()
{
	
	int vertical = player->health / 50;
	int horizontal = (player->health - (vertical * 50)) / 10;

	for (float i = 0; i < vertical; i++)
	{
		for (float j = 0; j < 5; j++)
		{
			RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (j * 4.3f), 48.f - (i * 4.f), 1.f, 1.f);
		}
	}
	for (float i = 0; i < horizontal; i++)
	{
		RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (i * 4.3f), 48.f - (vertical * 4.f), 1.f, 1.f);
	}

}
void SceneStage2::RenderSkybox()
{
	//SKYBOX + FLOOR
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -1000.f);
	RenderMesh(meshList[GEO_FRONT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-1000.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1000.f, 0.f, 0.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 1000.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 1000.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TOP], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, -10.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

}
void SceneStage2::RenderObj()
{
	//boundaries
	for (float i = 0; i < 1401; i += 350)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-810.f, -10.f, -700.f + i);
		modelStack.Rotate(90.f, 0, 1.f, 0);
		modelStack.Scale(10.f, 30.f, 10.f);
		RenderMesh(meshList[GEO_GRASS_LINE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(810.f, -10.f, -700.f + i);
		modelStack.Rotate(270.f, 0, 1.f, 0);
		modelStack.Scale(10.f, 30.f, 10.f);
		RenderMesh(meshList[GEO_GRASS_LINE], true);
		modelStack.PopMatrix();
	}
	for (float i = 0; i < 1401; i += 350)
	{
		
		modelStack.PushMatrix();
		modelStack.Translate(650.f - i, -10.f, 810.f);
		modelStack.Scale(10.f, 30.f, 10.f);
		RenderMesh(meshList[GEO_GRASS_LINE], true);
		modelStack.PopMatrix();
		
		if (!objectiveThree)
		{
			modelStack.PushMatrix();
			modelStack.Translate(650.f - i, -10.f, -810.f);
			modelStack.Scale(10.f, 30.f, 10.f);
			RenderMesh(meshList[GEO_GRASS_LINE], true);
			modelStack.PopMatrix();
		}
	}

	//tree of LIFE
	modelStack.PushMatrix();
	modelStack.Translate(0, treeY, 0);
	modelStack.Rotate(treeRotate, 1.f, 0, 1.f);
	modelStack.Scale(20.f, 20.f, 20.f);
	RenderMesh(meshList[GEO_TREE], false);
	modelStack.PopMatrix();

	//grass
	for (float x = 0; x < 1351; x += 225)
	{
		for (float z = 0; z < 1351; z += 225)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-675.f + x, -10.f, -675.f + z);
			modelStack.Scale(40.f, 7.f, 40.f);
			RenderMesh(meshList[GEO_GRASS_PATCH], true);
			modelStack.PopMatrix();
		}
	}

	//rocks
	for (float i = 0; i < 360; i += 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.f, -10.f, 0.f);
		modelStack.PushMatrix();
		modelStack.Rotate(i, 0.f, 1.f, 0.f);
		modelStack.Translate(150.f, 0.f, 0.f);
		modelStack.Scale(1.5f, 1.5f, 1.5f);
		RenderMesh(meshList[GEO_ROCK], false);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	RenderMisc();
}
void SceneStage2::RenderMisc()
{
	//flowers
	if (flowerOneLife)
	{
		modelStack.PushMatrix();
		modelStack.Translate(240.f, -10.f, 500.f);
		modelStack.Scale(1.f, 2.f, 1.f);
		RenderMesh(meshList[GEO_FLOWER], true);
		modelStack.PopMatrix();
	}
	if (flowerTwoLife)
	{
		modelStack.PushMatrix();
		modelStack.Translate(750.f, -10.f, -200.f);
		modelStack.Scale(1.f, 2.f, 1.f);
		RenderMesh(meshList[GEO_FLOWER], true);
		modelStack.PopMatrix();
	}
	if (flowerThreeLife)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-530.f, -10.f, 200.f);
		modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
		modelStack.Scale(1.f, 2.f, 1.f);
		RenderMesh(meshList[GEO_FLOWER], true);
		modelStack.PopMatrix();
	}

	if (!objectiveTwo)
	{
		modelStack.PushMatrix();
		modelStack.Translate(50.f, 0.f, 10.f);
		modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
		modelStack.Scale(6.f, 3.f, 1.f);
		RenderMesh(meshList[GEO_BARRIER], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-70.f, 0.f, 10.f);
		modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
		modelStack.Scale(6.f, 3.f, 1.f);
		RenderMesh(meshList[GEO_BARRIER], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-10.f, 0.f, 70.f);
		modelStack.Scale(6.f, 3.f, 1.f);
		RenderMesh(meshList[GEO_BARRIER], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-10.f, 0.f, -50.f);
		modelStack.Scale(6.f, 3.f, 1.f);
		RenderMesh(meshList[GEO_BARRIER], false);
		modelStack.PopMatrix();
	}
}
void SceneStage2::RenderLights()
{
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//********************SECOND LIGHT***************************
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//*******************THIRD LIGHT******************************
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//*******************FOURTH LIGHT******************************
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
			modelStack.Translate(bulletPtr[i]->throws.x, bulletPtr[i]->throws.y, bulletPtr[i]->throws.z);
			RenderMesh(meshList[GEO_BULLETS], false);
			modelStack.PopMatrix();
		}
	}
}
void SceneStage2::RenderMonster()
{
	Vector3 defaultView = Vector3(0, 0, 1).Normalize();
	float dRot;
	float fRot;

	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			Vector3 B = MonsterPtr[i]->pos - camera.position;
			B.y = MonsterPtr[i]->pos.y;

			float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180.f / 3.14f);

			if (B.x > 0 && B.z < 0)
				dRot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				dRot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				dRot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				dRot = 180 - rotation;
			else
				dRot = rotation;

			modelStack.PushMatrix();
			modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y - 10.f, (*MonsterPtr[i]).pos.z);
			modelStack.Rotate(dRot + 90.f, 0.f, 1.f, 0.f);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderMesh(meshList[GEO_DODGER_BODY], true);
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.2f, 0.f);
				modelStack.Translate(0.f, 2.f, 0.f);
				modelStack.Rotate(dodgerArmSwing + 300.f, 0.f, 0.f, 1.f);
				modelStack.Translate(0.f, -2.f, 0.f);
				RenderMesh(meshList[GEO_DODGER_HAND], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.2f, 0.f);
				modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
				modelStack.Translate(0.f, 2.f, 0.f);
				modelStack.Rotate(dodgerArmSwing - 60.f, 0.f, 0.f, 1.f);
				modelStack.Translate(0.f, -2.f, 0.f);
				RenderMesh(meshList[GEO_DODGER_HAND], true);
					modelStack.PushMatrix();
					modelStack.Translate(1.f, 1.5f, 0.8f);
					modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
					modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
					modelStack.Scale(0.5f, 0.5f, 0.5f);
					RenderMesh(meshList[GEO_DODGER_WEAPON], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
					modelStack.PushMatrix();
					modelStack.Translate(0.f, 0.f, -0.05f);
					modelStack.Translate(0.f, 1.5f, 0.f);
					modelStack.Rotate(dodgerLegSwing + 340.f, 0.f, 0.f, 1.f);
					modelStack.Translate(0.f, -1.5f, 0.f);
					RenderMesh(meshList[GEO_DODGER_LEG], true);
					modelStack.PopMatrix();

					modelStack.PushMatrix();
					modelStack.Translate(0.f, 0.f, 0.05f);
					modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
					modelStack.Translate(0.f, 1.5f, 0.f);
					modelStack.Rotate(dodgerLegSwing - 20.f, 0.f, 0.f, 1.f);
					modelStack.Translate(0.f, -1.5f, 0.f);
					RenderMesh(meshList[GEO_DODGER_LEG], true);
					modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			Vector3 B = MonsterFodderPtr[i]->pos - camera.position;
			B.y = MonsterFodderPtr[i]->pos.y;

			float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180.f / 3.14f);

			if (B.x > 0 && B.z < 0)
				fRot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				fRot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				fRot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				fRot = 180 - rotation;
			else
				fRot = rotation;

			modelStack.PushMatrix();
			modelStack.Translate((*MonsterFodderPtr[i]).pos.x, (*MonsterFodderPtr[i]).pos.y - 10.f, (*MonsterFodderPtr[i]).pos.z);
			modelStack.Rotate(fRot + 90.f, 0.f, 1.f, 0.f);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderMesh(meshList[GEO_FODDER_BODY], true);
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, 0.05f);
				modelStack.Translate(0.f, 1.5f, 0.f);
				modelStack.Rotate(fodderArmSwing + 300.f, 0.f, 0.f, 1.f);
				modelStack.Translate(0.f, -1.5f, 0.f);
				RenderMesh(meshList[GEO_FODDER_HAND], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, -0.05f);
				modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
				modelStack.Translate(0.f, 1.5f, 0.f);
				modelStack.Rotate(fodderArmSwing - 60.f, 0.f, 0.f, 1.f);
				modelStack.Translate(0.f, -1.5f, 0.f);
				RenderMesh(meshList[GEO_FODDER_HAND], true);
				modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}

}
void SceneStage2::RenderMonsterBullets()
{
	Vector3 defaultView = Vector3(0, 0, 1).Normalize();
	float dRot;

	for (int i = 0; i < MOBBULLETNUM; i++)
	{
		if (monsterBulletPtr[i] != NULL)
		{
			Vector3 B = monsterBulletPtr[i]->pos - camera.position;
			B.y = monsterBulletPtr[i]->pos.y;

			float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180.f / 3.14f);

			if (B.x > 0 && B.z < 0)
				dRot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				dRot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				dRot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				dRot = 180 - rotation;
			else
				dRot = rotation;

			modelStack.PushMatrix();
			modelStack.Translate((*monsterBulletPtr[i]).pos.x, (*monsterBulletPtr[i]).pos.y, (*monsterBulletPtr[i]).pos.z);
			modelStack.Rotate(dRot, 0.f, 1.f, 0.f);
			modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_SPHERE], false);
			modelStack.PopMatrix();
		}
	}

}
void SceneStage2::RenderHitmarker()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0.f, 1.f, 1.f), 5.f, 8.3f, 6.1f);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1.f, 0.f, 0.f), hitmarkerSize, 8.3f, 6.1f);
}
void SceneStage2::RenderUi()
{
	std::ostringstream sFps;
	sFps << std::fixed << std::setprecision(3);
	sFps << 1.0 / deltaTime << "fps";
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], sFps.str(), Color(1, 1, 1), 2, 1, 29);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "Press E to", Color(1, 1, 1), interactionSize, 6, 7);
	RenderTextOnScreen(meshList[GEO_TEXT], "DEVOUR", Color(1, 0, 0), interactionSize, 7, 6);
	modelStack.PopMatrix();

	//std::ostringstream timer;
	//timer << std::fixed << std::setprecision(3);
	//timer << player->timer << " Seconds";
	//modelStack.PushMatrix();
	//RenderTextOnScreen(meshList[GEO_TEXT], timer.str(), Color(0.f, 0.8f, 1.f), 2, 1, 17);
	//modelStack.PopMatrix();

}
void SceneStage2::RenderPickups()
{
	if (pickupsSpawn)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0.f, pickupsY, pickupsZ);
		modelStack.Scale(5.f, 2.f, 5.f);
		RenderMesh(meshList[GEO_PICKUP], false);
		modelStack.PopMatrix();
	}
}
void SceneStage2::RenderObjectives()
{
	std::ostringstream monsLeft;
	monsLeft << std::fixed << std::setprecision(1);
	monsLeft << "kill 5 guardians(" << (int)monDead << "/5)";
	modelStack.PushMatrix();
	std::ostringstream barrierLeft;
	barrierLeft << std::fixed << std::setprecision(1);
	barrierLeft << "the glowing roots (" << (int)flowersAmt << "/3)";
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "Objective", Color(0.f, 0.8f, 1.f), 2.f, 34.f, 25.f);
	RenderTextOnScreen(meshList[GEO_TEXT], "============", Color(0.f, 0.8f, 1.f), 2.f, 32.f, 24.f);
	if (!objectiveOne)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], monsLeft.str(), Color(0.f, 0.8f, 1.f), 2.f, 26.2f, 23.f);
	}
	if (objectiveOne && !objectiveTwo)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Find and DEVOUR", Color(0.f, 0.3f, 1.f), 2.f, 29.f, 23.f);
		RenderTextOnScreen(meshList[GEO_TEXT], barrierLeft.str(), Color(0.f, 0.8f, 1.f), 2.f, 24.2f, 22.f);
	}
	if (objectiveTwo && !objectiveThree)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "DEVOUR THE TREE OF LIFE", Color(0.f, 0.8f, 1.f), 3.f, 10.5f, 15.3f);
	}
	if (objectiveThree)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Head towards the opening", Color(0.f, 0.8f, 1.f), 3.f, 10.5f, 15.3f);
	}
	modelStack.PopMatrix();
	if (objectiveFour)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press E to go to the village", Color(0.f, 1.f, 1.f), 2.5f, 8.f, 5.f);
	}
}

void SceneStage2::Exit()
{
	for (int i = 0; i < MOBNUM2; i++)
	{
		delete MonsterPtr[i];
		delete monsterBoxPtr[i];
	}
	for (int i = 0; i < MOBNUM2; i++)
	{
		delete MonsterFodderPtr[i];
		delete monsterFodderBoxPtr[i];
	}

	for (int i = 0; i < 25; i++)
	{
		delete monsterBulletPtr[i];
	}

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		delete bulletPtr[bul];
		delete bulletBoxPtr[bul];
	}

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList[i] != NULL)
		{
			delete meshList[i];
		}
		meshList[i] = NULL;
	}

	if (engine)
	{
		engine->drop();
	}

	glDeleteProgram(m_programID);
}
