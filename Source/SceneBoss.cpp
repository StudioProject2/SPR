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

	//Monster Animation
	fodSwingTimer = 0;
	fodLeft = false;
	fodderArmSwing = 0.0;

	dodSwingTimer = 0;
	dodLeft = false;
	dodgerArmSwing = 0.0;
	dodgerLegSwing = 0.0;

	arcSwingTimer = 0;
	arcLeft = false;
	archerArmSwing = 0.0;
	archerLegSwing = 0.0;

	bossSwingTimer = 0;
	bossLeft = false;
	bossArmSwing = 0.0;
	bossLegSwing = 0.0;

	//Timer
	spinToWin = 0;
	elaspeTime = 0.0;
	monsterTime = elaspeTime + 3.0;
	bossMovementChangeTime = elaspeTime + 10.0;
	bossPlayerShootTime = elaspeTime + 3.0;
	bossRingShootTime = elaspeTime + 3.0;
	groundSignalBlinkTime = elaspeTime + 0.5;
	bossChangeGroundTargetTime = elaspeTime + 5.0;
	bossGroundAttackDelayTime = elaspeTime + 1.0;
	bulletBounceTime = 0.0;
	monsterFodderTime = elaspeTime + 3.0;
	monsterArcherTime = elaspeTime + 3.0;

	bossMovement = STRAIGHT;
	bossBox = new Box;
	printGroundSignal = true;
	win = false;
	gameOver = false;

	player = Player::getInstance();
	hitmarkerSize = 0;

	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterPtr[i] = NULL;
		monsterBulletDelay[i] = elaspeTime + 4.0;
	}

	//MonsterArcher and MonsterFodder 
	for (int i = 0; i < MOBNUM; i++)
	{
		MonsterFodderPtr[i] = NULL;
		monsterFodderBoxPtr[i] = NULL;
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
	meshList[GEO_BULLETS] = MeshBuilder::GenerateHem("bullets", Color(0.7f, 1.0f, 0.7f), 20, 20, 0.5);

	//SKYBOX STUFF
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad1("front", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Boss Stage/skybox_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad1("back", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Boss Stage/skybox_back.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad1("left", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Boss Stage/skybox_right.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad1("right", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Boss Stage/skybox_left.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Boss Stage/skybox_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad1("bottom", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Boss Stage/skybox_bottom.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;

	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Hut.obj");
	meshList[GEO_BUILDING]->textureID = LoadTGA("Image//Boss Stage/Hut.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("building", "OBJ//Boss Stage/Fence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//Boss Stage/Fence.tga");

	//Monsters
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
	meshList[GEO_ARCHER_BODY] = MeshBuilder::GenerateOBJ("archer", "OBJ//MonstersOBJ//ArcherBodyOBJ.obj");
	meshList[GEO_ARCHER_BODY]->textureID = LoadTGA("Image//MonsterTextures//ArcherTexture.tga");
	meshList[GEO_ARCHER_BODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ARCHER_BODY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARCHER_BODY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARCHER_BODY]->material.kShininess = 1.f;
	meshList[GEO_ARCHER_HAND] = MeshBuilder::GenerateOBJ("archer", "OBJ//MonstersOBJ//ArcherHandOBJ.obj");
	meshList[GEO_ARCHER_HAND]->textureID = LoadTGA("Image//MonsterTextures//ArcherTexture.tga");
	meshList[GEO_ARCHER_HAND]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ARCHER_HAND]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARCHER_HAND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARCHER_HAND]->material.kShininess = 1.f;
	meshList[GEO_ARCHER_LEG] = MeshBuilder::GenerateOBJ("archer", "OBJ//MonstersOBJ//ArcherLegOBJ.obj");
	meshList[GEO_ARCHER_LEG]->textureID = LoadTGA("Image//MonsterTextures//ArcherTexture.tga");
	meshList[GEO_ARCHER_LEG]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ARCHER_LEG]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARCHER_LEG]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARCHER_LEG]->material.kShininess = 1.f;
	meshList[GEO_ARCHER_WEAPON] = MeshBuilder::GenerateOBJ("archer", "OBJ//MonstersOBJ//ArcherWeaponOBJ.obj");
	meshList[GEO_ARCHER_WEAPON]->textureID = LoadTGA("Image//MonsterTextures//ArcherWeaponTexture.tga");
	meshList[GEO_ARCHER_WEAPON]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ARCHER_WEAPON]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARCHER_WEAPON]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARCHER_WEAPON]->material.kShininess = 1.f;

	//Boss Monster
	meshList[GEO_BOSS_BODY] = MeshBuilder::GenerateOBJ("boss", "OBJ//MonstersOBJ//BossBodyOBJ.obj");
	meshList[GEO_BOSS_BODY]->textureID = LoadTGA("Image//MonsterTextures//BossTexture.tga");
	meshList[GEO_BOSS_BODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOSS_BODY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOSS_BODY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOSS_BODY]->material.kShininess = 1.f;
	meshList[GEO_BOSS_HAND] = MeshBuilder::GenerateOBJ("boss", "OBJ//MonstersOBJ//BossHandOBJ.obj");
	meshList[GEO_BOSS_HAND]->textureID = LoadTGA("Image//MonsterTextures//BossTexture.tga");
	meshList[GEO_BOSS_HAND]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOSS_HAND]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOSS_HAND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOSS_HAND]->material.kShininess = 1.f;
	meshList[GEO_BOSS_LEG] = MeshBuilder::GenerateOBJ("boss", "OBJ//MonstersOBJ//BossLegOBJ.obj");
	meshList[GEO_BOSS_LEG]->textureID = LoadTGA("Image//MonsterTextures//BossTexture.tga");
	meshList[GEO_BOSS_LEG]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BOSS_LEG]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOSS_LEG]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOSS_LEG]->material.kShininess = 1.f;

	//Bullet
	meshList[GEO_SPHERE] = MeshBuilder::GenerateOBJ("bullets", "OBJ//MonstersOBJ//MonsterBulletOBJ.obj");
	meshList[GEO_SPHERE]->textureID = LoadTGA("Image//MonsterTextures//ArcherWeaponTexture.tga");

	//Debuggging Cube
	meshList[GEO_CUBE] = MeshBuilder::GenerateOBJ("cube", "OBJ//Cube.obj");

	meshList[GEO_GROUNDSIGNAL] = MeshBuilder::GenerateQuad("ground signal", Color(1.0f, 0.0f, 0.0f), 100, 100);

	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

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

	//Boss Health
	meshList[GEO_BOSSHEALTHBAR] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 1.0f);
	meshList[GEO_BOSSHEALTHBAR]->textureID = LoadTGA("Image//bossHpBorder.tga");
	
	meshList[GEO_BOSSHEALTHBACK] = MeshBuilder::GenerateQuad1("top", Color(0.15f, 0.15f, 0.15f), 1.0f, 1.0f, 1.0f);

	meshList[GEO_BOSSHEALTH] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f);

	//Monsters

	for (int i = 0; i < DIRECTBULLETNUM; i++)
	{
		directBulletPtr[i] = NULL;
	}

	for (int i = 0; i < RINGBULLETNUM; i++)
	{
		ringBulletPtr[i] = NULL;
	}

	for (int i = 0; i < GROUNDBULLETNUM; i++)
	{
		groundBulletPtr[i] = NULL;
	}

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		//init collision for the bullets here
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	}

	engine = createIrrKlangDevice();
}

void SceneBoss::Update(double dt)
{
	spinToWin += 30;
	elaspeTime += dt;
	start.isShooting = true;

	if (win)
	{
		Application::sceneChange = Application::WIN;
	}
	if (gameOver)
	{
		Application::sceneChange = Application::LOSE;
	}
	UpdateBullets();

	if (boss.getHealth() <= 500 && !gameOver)//Spawn and update monsters only when boss is below 500hp and player didn't lose
	{
		UpdateMonsters();
		UpdateCreateMonsterBullets();
		UpdateMonsterHitbox();
	}
	if (boss.getHealth() > 0 && !gameOver)
	{
		UpdateBossMovement();
		UpdateBossBullets();
		UpdateBossHitbox();
	}

	UpdateEnemyBullets();
	UpdateMonsterAnimations();
	UpdateBossHealth();

	if (player->health <= 0)
	{
		Application::sceneChange = Application::LOSE;
	}

	if (boss.getHealth() <= 0)
	{
		win = true;
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

	camera.Update(dt);
}

//UPDATE PLAYER BULLET
void SceneBoss::UpdateBullets()
{
	Vector3 view = (camera.target - camera.position).Normalized();
	Box playerBox = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 7, 7, 10);

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

//CREATE BULLETS
void SceneBoss::UpdateCreateMonsterBullets()
{
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterPtr[i] != NULL)
		{
			for (int j = 0; j < DIRECTBULLETNUM; j++)
			{
				if (elaspeTime > monsterBulletDelay[i] && directBulletPtr[j] == NULL)
				{
					//Create bullet and reset bounce time
					directBulletPtr[j] = new monsterBullet(MonsterPtr[i]->pos, camera.position);
					monsterBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
					return;
				}
			}
		}
	}

	//Monster Archer
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			for (int j = 0; j < DIRECTBULLETNUM; j++)
			{
				if (elaspeTime > monsterArcherBulletDelay[i] && directBulletPtr[j] == NULL)
				{
					//Create bullet and reset bounce time
					directBulletPtr[j] = new monsterBullet(MonsterArcherPtr[i]->pos, camera.position);
					monsterArcherBulletDelay[i] = elaspeTime + MOBBULLETDELAY;
					return;
				}
			}
		}
	}
}

//CREATE MONSTERS, MOVE IT IF IT EXISTS AND UPDATE HITBOX FOR MONSTER
void SceneBoss::UpdateMonsters()
{
	//MonsterNormal
	
	if (elaspeTime > monsterTime)
	{
		for (int i = 0; i < MOBNUM; i++)
		{
			if (MonsterPtr[i] == NULL)
			{
				MonsterPtr[i] = new Monster();
				monsterBoxPtr[i] = new Box(MonsterPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
				monsterTime = elaspeTime + 5.0;
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
				monsterTime = elaspeTime + 7.0;
			}
		}
	}
	
	//Monster Fodder
	if (elaspeTime > monsterFodderTime)
	{
		for (int i = 0; i < MOBNUM; i++)
		{
			if (MonsterFodderPtr[i] == NULL)
			{
				MonsterFodderPtr[i] = new MonsterFodder();
				monsterFodderBoxPtr[i] = new Box(MonsterFodderPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
				monsterFodderTime = elaspeTime + 5.0;
				break;
			}
		}
	}
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
				monsterFodderTime = elaspeTime + 7.0;
			}
		}
	}

	//Monster Archer
	if (elaspeTime > monsterArcherTime)
	{
		for (int i = 0; i < MOBNUM; i++)
		{
			if (MonsterArcherPtr[i] == NULL)
			{
				MonsterArcherPtr[i] = new MonsterArcher();
				monsterArcherBoxPtr[i] = new Box(MonsterArcherPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
				monsterArcherTime = elaspeTime + 5.0;
				break;
			}
		}
	}
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
				monsterArcherTime = elaspeTime + 7.0;
			}
		}
	}

}

//CHANGES STATE OF BOSS AND MOVE IT BASED ON THAT STATE
void SceneBoss::UpdateBossMovement()
{
	if (elaspeTime > bossMovementChangeTime) //Bounce time of 15s
	{
		if (bossMovement == CHARGE) //Reset to first state if boss is at last state
		{
			bossMovement = STRAIGHT;
		}
		else
		{
			//Reset boss y position and change its state
			boss.resetY();
			bossMovement++;
		}
		boss.resetVariables(); //Reset variables for boss
		bossMovementChangeTime = elaspeTime + 15.0; //Reset bounce time
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

//UPDATES BOSS HITBOX AND CHECKS FOR COLLISIONS
void SceneBoss::UpdateBossHitbox()
{
	bool isHit = false;
	hitmarkerSize = 0;
	*bossBox = Box(Vector3(boss.getPos()), 10, 10, 25); //Update hitbox for boss

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
		if (!Application::muted)
		{
			engine->play2D("Sound/deepHumanHit.wav", false);
		}
		boss.setHealth(boss.getHealth() - 1);
		hitmarkerTimer = 50;
	}

	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

//CREATE BULLETS, MOVE IT IF IT EXISTS AND CHECK FOR COLLISIONS
void SceneBoss::UpdateBossBullets()
{
	if (elaspeTime > bossPlayerShootTime && bossMovement != ASCEND) //shoot at player
	{
		for (int i = 0; i < DIRECTBULLETNUM; i++)
		{
			if (directBulletPtr[i] == NULL)
			{
				//Create bullet, reset bounce time
				directBulletPtr[i] = new monsterBullet(boss.getPos(), camera.position);
				bossPlayerShootTime = elaspeTime + 2.0;
				return;
			}
		}
	}

	if (elaspeTime > bossRingShootTime) //shoot in ring
	{
		for (int i = 0; i < RINGBULLETNUM; i++)
		{
			if (ringBulletPtr[i] == NULL
				&& ringBulletPtr[i + 1] == NULL
				&& ringBulletPtr[i + 2] == NULL
				&& ringBulletPtr[i + 3] == NULL
				&& ringBulletPtr[i + 4] == NULL
				&& ringBulletPtr[i + 5] == NULL
				&& ringBulletPtr[i + 6] == NULL
				&& ringBulletPtr[i + 7] == NULL)
			{
				//Create bullet targets
				Vector3 bullet1 = Vector3(boss.getPos().x + 1, boss.getPos().y, boss.getPos().z);
				Vector3 bullet2 = Vector3(boss.getPos().x, boss.getPos().y, boss.getPos().z + 1);
				Vector3 bullet3 = Vector3(boss.getPos().x - 1, boss.getPos().y, boss.getPos().z);
				Vector3 bullet4 = Vector3(boss.getPos().x, boss.getPos().y, boss.getPos().z - 1);
				Vector3 bullet5 = Vector3(boss.getPos().x + 1, boss.getPos().y, boss.getPos().z + 1);
				Vector3 bullet6 = Vector3(boss.getPos().x - 1, boss.getPos().y, boss.getPos().z + 1);
				Vector3 bullet7 = Vector3(boss.getPos().x + 1, boss.getPos().y, boss.getPos().z - 1);
				Vector3 bullet8 = Vector3(boss.getPos().x + 1, boss.getPos().y, boss.getPos().z - 1);

				//Create bullets
				ringBulletPtr[i] = new monsterBullet(boss.getPos(), bullet1);
				ringBulletPtr[i + 1] = new monsterBullet(boss.getPos(), bullet2);
				ringBulletPtr[i + 2] = new monsterBullet(boss.getPos(), bullet3);
				ringBulletPtr[i + 3] = new monsterBullet(boss.getPos(), bullet4);
				ringBulletPtr[i + 4] = new monsterBullet(boss.getPos(), bullet5);
				ringBulletPtr[i + 5] = new monsterBullet(boss.getPos(), bullet6);
				ringBulletPtr[i + 6] = new monsterBullet(boss.getPos(), bullet7);
				ringBulletPtr[i + 7] = new monsterBullet(boss.getPos(), bullet8);

				//Reset bounce time
				bossRingShootTime = elaspeTime + 1.0;
				return;
			}
		}
	}

	if (elaspeTime > bossChangeGroundTargetTime)
	{
		//Change ground area center
		groundAreaCenter = Vector3(((float)(rand() % 500) - 250), 0, ((float)(rand() % 1700) - 850));
		bossChangeGroundTargetTime = elaspeTime + 7.0;
		bossGroundAttackDelayTime = elaspeTime + 2.0;
		return;
	}
	if (elaspeTime > bossGroundAttackDelayTime) // shoot from ground up
	{
		if (groundAreaCenter != NULL)
		{
			//Set position and target for bullet
			float randomGroundBulletX = ((rand() % 201) - 100 + groundAreaCenter.x);
			float randomGroundBulletZ = ((rand() % 201) - 100 + groundAreaCenter.z);
			Vector3 randomGroundBullet = Vector3(randomGroundBulletX, -5, randomGroundBulletZ);
			Vector3 randomGroundBulletTarget = Vector3(randomGroundBulletX, 1, randomGroundBulletZ);

			for (int i = 0; i < GROUNDBULLETNUM; i++)
			{
				if (groundBulletPtr[i] == NULL)
				{
					//Create bullet
					groundBulletPtr[i] = new monsterBullet(randomGroundBullet, randomGroundBulletTarget);
					return;
				}
			}
		}
	}
}

//UPDATE BOSS HEALTH BAR BASED ON BOSS HEALTH
void SceneBoss::UpdateBossHealth()
{
	//Scale and translate boss health bar
	bossHealthScale = (float)boss.getHealth() / 44.4f;
	bossHealthTranslate = ((40.f - ((1000.f - (float)boss.getHealth()) / 45.f)) / bossHealthScale);
}

//UPDATE MONSTER BULLETS, UPDATE PLAYER HITBOX, MOVE BULLET AND CHECK FOR COLLISIONS
void SceneBoss::UpdateEnemyBullets()
{
	Box playerBox = Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 7, 7, 10);

	for (int i = 0; i < DIRECTBULLETNUM; i++)
	{
		if (directBulletPtr[i] != NULL)
		{
			directBulletPtr[i]->move();
			if (directBulletPtr[i]->isBulletInBox(playerBox))
			{
				player->health -= DIRECTBULLETDMG;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
				delete directBulletPtr[i];
				directBulletPtr[i] = NULL;
			}
			if (directBulletPtr[i] != NULL)
			{
				if (directBulletPtr[i]->bulletCollideStage4())
				{
					delete directBulletPtr[i];
					directBulletPtr[i] = NULL;
				}
			}
		}
	}

	for (int i = 0; i < RINGBULLETNUM; i++)
	{
		if (ringBulletPtr[i] != NULL)
		{
			ringBulletPtr[i]->move();
			if (ringBulletPtr[i]->isBulletInBox(playerBox))
			{
				player->health -= RINGBULLETDMG;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
				delete ringBulletPtr[i];
				ringBulletPtr[i] = NULL;
			}
			if (ringBulletPtr[i] != NULL)
			{
				if (ringBulletPtr[i]->bulletCollide())
				{
					delete ringBulletPtr[i];
					ringBulletPtr[i] = NULL;
				}
			}
		}
	}

	for (int i = 0; i < GROUNDBULLETNUM; i++)
	{
		if (groundBulletPtr[i] != NULL)
		{
			groundBulletPtr[i]->move();
			if (groundBulletPtr[i]->isBulletInBox(playerBox))
			{
				player->health -= GROUNDBULLETDMG;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
				delete groundBulletPtr[i];
				groundBulletPtr[i] = NULL;
			}
			if (groundBulletPtr[i] != NULL)
			{
				if (groundBulletPtr[i]->bulletCollide())
				{
					delete groundBulletPtr[i];
					groundBulletPtr[i] = NULL;
				}
			}
		}
	}
}

//CHECK FOR COLLISION
void SceneBoss::UpdateMonsterHitbox()
{
	bool isHit = false;
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
					(*MonsterPtr[mon]).health = (*MonsterPtr[mon]).health - player->damage;
					hitmarkerTimer = 50;
					bulletPtr[bul]->monsterHit(camera);
					bulletBoxPtr[bul]->position = bulletPtr[bul]->throws;
					if (!Application::muted)
					{
						engine->play2D("Sound/highHumanHit.wav", false);
					}
					isHit = false;
				}
			}
		}
	}

	//STUFF THEO ADDED
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
					if (!Application::muted)
					{
						engine->play2D("Sound/femaleHit.wav", false);
					}
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
					if (!Application::muted)
					{
						engine->play2D("Sound/humanHit.wav", false);
					}
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

	if (hitmarkerTimer > 0)
	{
		hitmarkerTimer -= 1;
		hitmarkerSize = 5;
	}
}

//ANIMATION
void SceneBoss::UpdateMonsterAnimations()
{
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


	//Archer animations
	if (!arcLeft)
	{
		if (arcSwingTimer < 30)
		{
			arcSwingTimer += 1;
			archerLegSwing += 1.5;
		}
		else
		{
			arcLeft = true;
		}
	}
	else
	{
		if (arcSwingTimer > 0)
		{
			arcSwingTimer -= 1;
			archerLegSwing -= 1.5;
		}
		else
		{
			arcLeft = false;
		}
	}
	
	//boss
	if (!bossLeft)
	{
		if (bossSwingTimer < 30)
		{
			bossSwingTimer += 1;
			bossArmSwing += 2;
			bossLegSwing += 1.5;
		}
		else
		{
			bossLeft = true;
		}
	}
	else
	{
		if (bossSwingTimer > 0)
		{
			bossSwingTimer -= 1;
			bossArmSwing -= 2;
			bossLegSwing -= 1.5;
		}
		else
		{
			bossLeft = false;
		}
	}
}

//RENDERS
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
	modelStack.Translate(0.f, 0.f, -1000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-1000.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1000.f, 0.f, 0.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 1000.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 1000.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.f, -10.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200.f, -10.f, 0.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200.f, -10.f, 200.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200.f, -10.f, -200.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200.f, -10.f, 0.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200.f, -10.f, 200.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200.f, -10.f, -200.f);
	modelStack.Scale(8.f, 8.f, 8.f);
	RenderMesh(meshList[GEO_BUILDING], true);
	modelStack.PopMatrix();

	for (int i = 0; i < 1800; i += 30)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-300.f, -10.f, -900.f + i);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(300.f, -10.f, -900.f + i);
		modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
		modelStack.Scale(20.f, 20.f, 20.f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 660; i += 30)
	{
		modelStack.PushMatrix();
		modelStack.Translate(300.f - i, -10.f, 890.f);
		modelStack.Scale(20.f, 20.f, 20.f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(300.f - i, -10.f, -910.f);
		modelStack.Scale(20.f, 20.f, 20.f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();
	}

	//Monsters
	Vector3 defaultView = Vector3(0.f, 0.f, 1.f).Normalize();

	for (int i = 0; i < MOBNUM; i++)
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
			modelStack.Translate((*MonsterFodderPtr[i]).pos.x, (*MonsterFodderPtr[i]).pos.y - 10, (*MonsterFodderPtr[i]).pos.z);
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
	for (int i = 0; i < MOBNUM; i++)
	{
		if (MonsterArcherPtr[i] != NULL)
		{
			Vector3 B = MonsterArcherPtr[i]->pos - camera.position;
			B.y = MonsterArcherPtr[i]->pos.y;

			float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180.f / 3.14f);

			if (B.x > 0 && B.z < 0)
				aRot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				aRot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				aRot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				aRot = 180 - rotation;
			else
				aRot = rotation;

			modelStack.PushMatrix();
			modelStack.Translate((*MonsterArcherPtr[i]).pos.x, (*MonsterArcherPtr[i]).pos.y - 10, (*MonsterArcherPtr[i]).pos.z);
			modelStack.Rotate(aRot + 90.f, 0.f, 1.f, 0.f);
			modelStack.Scale(10.f, 10.f, 10.f);
			RenderMesh(meshList[GEO_ARCHER_BODY], true);
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, 4.3f);
				modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
				modelStack.Translate(0.f, 2.f, 0.f);
				modelStack.Rotate(80.f, 0.f, 0.f, 1.f);
				modelStack.Translate(0.f, -2.f, 0.f);
				RenderMesh(meshList[GEO_ARCHER_HAND], true);
					modelStack.PushMatrix();
					modelStack.Translate(0.f, 1.f, 3.f);
					modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
					modelStack.Rotate(160.f, 1.f, 0.f, 0.f);
					modelStack.Scale(0.5f, 0.5f, 0.5f);
					RenderMesh(meshList[GEO_ARCHER_WEAPON], true);
					modelStack.PopMatrix();
				modelStack.PopMatrix();
					modelStack.PushMatrix();
					modelStack.Translate(0.f, 0.f, -0.05f);
					modelStack.Translate(0.f, 1.5f, 0.f);
					modelStack.Rotate(archerLegSwing + 340.f, 0.f, 0.f, 1.f);
					modelStack.Translate(0.f, -1.5f, 0.f);
					RenderMesh(meshList[GEO_ARCHER_LEG], true);
					modelStack.PopMatrix();
					modelStack.PushMatrix();
					modelStack.Translate(0.f, 0.f, 4.3f);
					modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
					modelStack.Translate(0.f, 1.5f, 0.f);
					modelStack.Rotate(archerLegSwing - 20.f, 0.f, 0.f, 1.f);
					modelStack.Translate(0.f, -1.5f, 0.f);
					RenderMesh(meshList[GEO_ARCHER_LEG], true);
					modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}

	//SPAWN MOBS
	for (int i = 0; i < MOBNUM; i++)
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
			modelStack.Translate((*MonsterPtr[i]).pos.x, (*MonsterPtr[i]).pos.y - 10, (*MonsterPtr[i]).pos.z);
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
			modelStack.Translate(1.0f, 1.5f, 0.8f);
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

	for (int i = 0; i < DIRECTBULLETNUM; i++)
	{
		if (directBulletPtr[i] != NULL)
		{
			Vector3 B = directBulletPtr[i]->pos - camera.position;
			B.y = directBulletPtr[i]->pos.y;

			float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
			rotation = rotation * (180.f / 3.14f);

			if (B.x > 0 && B.z < 0)
				bfRot = 180 + rotation;
			else if (B.x > 0 && B.z > 0)
				bfRot = 180 + rotation;
			else if (B.x < 0 && B.z > 0)
				bfRot = 180 - rotation;
			else if (B.x < 0 && B.z < 0)
				bfRot = 180 - rotation;
			else
				bfRot = rotation;

			modelStack.PushMatrix();
			modelStack.Translate((*directBulletPtr[i]).pos.x, (*directBulletPtr[i]).pos.y, (*directBulletPtr[i]).pos.z);
			modelStack.Rotate(bfRot, 0.f, 1.f, 0.f);
			modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_SPHERE], false);
			modelStack.PopMatrix();
		}
	}

	for (int i = 0; i < RINGBULLETNUM; i++)
	{
		if (ringBulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*ringBulletPtr[i]).pos.x, (*ringBulletPtr[i]).pos.y, (*ringBulletPtr[i]).pos.z);
			modelStack.Rotate(spinToWin, 0.f, 1.f, 0.f);
			modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	for (int i = 0; i < GROUNDBULLETNUM; i++)
	{
		if (groundBulletPtr[i] != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*groundBulletPtr[i]).pos.x, (*groundBulletPtr[i]).pos.y, (*groundBulletPtr[i]).pos.z);
			modelStack.Scale(2.f, 2.f, 2.f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}

	
		Vector3 B = boss.getPos() - camera.position;
		B.y = boss.getPos().y;

		float rotation = acos(defaultView.Dot(B) / (defaultView.Length() * B.Length()));
		rotation = rotation * (180.f / 3.14f);

		if (B.x > 0 && B.z < 0)
			bRot = 180 + rotation;
		else if (B.x > 0 && B.z > 0)
			bRot = 180 + rotation;
		else if (B.x < 0 && B.z > 0)
			bRot = 180 - rotation;
		else if (B.x < 0 && B.z < 0)
			bRot = 180 - rotation;
		else
			bRot = rotation;
		modelStack.PushMatrix();
		modelStack.Translate(boss.getPos().x, boss.getPos().y + 5, boss.getPos().z);
		modelStack.Rotate(bRot + 90.f, 0.f, 1.f, 0.f);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_BOSS_BODY], true);
			modelStack.PushMatrix();
			modelStack.Translate(0.f, 0.2f, -0.05f);
			modelStack.Translate(0.f, 0.5f, 0.f);
			modelStack.Rotate(bossArmSwing + 330.f, 0.f, 0.f, 1.f);
			modelStack.Translate(0.f, -0.5f, 0.f);
			RenderMesh(meshList[GEO_BOSS_HAND], true);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(0.f, 0.2f, 0.05f);
			modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
			modelStack.Translate(0.f, 0.5f, 0.f);
			modelStack.Rotate(bossArmSwing - 30.f, 0.f, 0.f, 1.f);
			modelStack.Translate(0.f, -0.5f, 0.f);
			RenderMesh(meshList[GEO_BOSS_HAND], true);
			modelStack.PopMatrix();
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, -0.05f);
				modelStack.Rotate(bossLegSwing + 340.f, 0.f, 0.f, 1.f);
				RenderMesh(meshList[GEO_BOSS_LEG], true);
				modelStack.PopMatrix();
				modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, -0.4f);
				modelStack.Rotate(-bossLegSwing -340.f, 0.f, 0.f, 1.f);
				RenderMesh(meshList[GEO_BOSS_LEG], true);
				modelStack.PopMatrix();
		modelStack.PopMatrix();
	

	if (printGroundSignal && groundAreaCenter != NULL)
	{
		modelStack.PushMatrix();
		modelStack.Translate(groundAreaCenter.x, groundAreaCenter.y - 9.9f, groundAreaCenter.z);
		modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
		RenderMesh(meshList[GEO_GROUNDSIGNAL], false);
		modelStack.PopMatrix();
	}

	RenderBullets();
	RenderTopTeeth();
	RenderBottomTeeth();
	RenderPlayerHealth();
	RenderBossHealth();
	RenderHitmarker();
}

void SceneBoss::RenderTopTeeth()
{
	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(-1.3f, 8.f, -17.f);
	modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	modelStack.Scale(1.5f, 1.f, 1.f);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneBoss::RenderBottomTeeth()
{

	Mtx44 ortho;
	ortho.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(1.3f, -8.f, -17.f);
	modelStack.Scale(1.5f, 1.f, 1.f);

	RenderMesh(meshList[GEO_PLAYER_TEETH], true);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneBoss::RenderPlayerHealth()
{
	int vertical = player->health / 50; //Amount of rows (since vertical is int, it will be rounded down)
	int horizontal = (player->health - (vertical * 50)) / 10; //Amount of hearts on the last line

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (j * 4.3f), 48.f - (i * 4.f), 1.f, 1.f);
		}
	}
	for (int i = 0; i < horizontal; i++)
	{
		RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (i * 4.3f), 48.f - (vertical * 4.f), 1.f, 1.f);
	}
}
void SceneBoss::RenderBossHealth()
{
	RenderMeshOnScreen(meshList[GEO_BOSSHEALTHBAR], 1.6f, 3.2f, 25.f, 18.f);
	RenderMeshOnScreen(meshList[GEO_BOSSHEALTHBACK], 1.78f, 72.f, 22.5f, 0.8f);
	RenderMeshOnScreen(meshList[GEO_BOSSHEALTH], bossHealthTranslate, 72.f, bossHealthScale, 0.8f);
	RenderTextOnScreen(meshList[GEO_TEXT], "Chieftain:" + to_string(boss.getHealth()) + "/1000", Color(1.0f, 1.0f, 1.0f), 2.f, 12.f, 28.8f);
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
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0.f, 1.f, 1.f), 5.f, 8.3f, 6.1f);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1.f, 0.f, 0.f), hitmarkerSize, 8.3f, 6.1f);
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
void SceneBoss::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

	if (engine)
	{
		engine->drop();
	}

	//glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
