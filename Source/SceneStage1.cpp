#include "SceneStage1.h"
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
#include <cmath>

using namespace std;

SceneStage1::SceneStage1()
{
}

SceneStage1::~SceneStage1()
{
}

void SceneStage1::Init()
{
	//Monster spawn
	srand((unsigned int)time(NULL));

	//Monster Animation
	fodSwingTimer = 0;
	fodLeft = false;
	fodderArmSwing = 0.0;

	//Timer
	elaspeTime = 0.0;
	bulletBounceTime = 0.0;
	monsterTime = elaspeTime + 3.0;
	yArrowTranslate = 40;
	movingUp = true;
	hitmarkerSize = 0;

	for (int i = 0; i < MOBNUM1; i++)
	{
		MonsterPtr[i] = NULL;
	}

	for (int i = 0; i < MOBNUM1; i++)
	{
		MonsterFodderPtr[i] = NULL;
		monsterFodderBoxPtr[i] = NULL;
	}

	monsterFodderTime = elaspeTime + 3.0;

	mobDead = 0;
	nearCageDoor = false;
	inCage = true;
	player = Player::getInstance();
	playerHurtBounceTime = 0.0;
	isNearExit = false;

	completeObjectives = false;
	breakCage = 0.0;
	//************************************COMPG**************************************************
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(500, 10, 500), Vector3(0, 10, 0), Vector3(0, 1, 0));

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

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 4);

	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 800, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5f;
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
	light[1].power = 0;
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
	light[2].power = 0;
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
	light[3].power = 0;
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
	meshList[GEO_BULLETS] = MeshBuilder::GenerateHem("bullets", Color(0.7f, 1.f, 0.7f), 20, 20, 0.5);

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
	//FLOOR
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad1("Sand", Color(1.0f, 1.0f, 1.0f), 1000.0f, 1000.0f, 10.0f);
	meshList[GEO_FLOOR]->textureID = LoadTGAR("Image//Sand2.tga");
	meshList[GEO_FLOOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLOOR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FLOOR]->material.kShininess = 1.f;
	
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


	//TEXT STUFF
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//WALLS
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("cube", "OBJ//bigwall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//wall.tga");
	meshList[GEO_WALL]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_WALL]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_WALL]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_WALL]->material.kShininess = 1.f;

	//Cage
	meshList[GEO_CAGE] = MeshBuilder::GenerateOBJ("cube", "OBJ//cage1.obj");
	meshList[GEO_CAGE]->textureID = LoadTGA("Image//cage.tga");
	meshList[GEO_CAGE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CAGE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_CAGE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CAGE]->material.kShininess = 1.f;
	meshList[GEO_CAGEDOOR] = MeshBuilder::GenerateOBJ("cube", "OBJ//cageDoor.obj");
	meshList[GEO_CAGEDOOR]->textureID = LoadTGA("Image//cage.tga");
	meshList[GEO_CAGEDOOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CAGEDOOR]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_CAGEDOOR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CAGEDOOR]->material.kShininess = 1.f;

	//Player 
	meshList[GEO_PLAYER_TEETH] = MeshBuilder::GenerateOBJ("teeth", "OBJ//PlayerTeeth.obj");
	meshList[GEO_PLAYER_TEETH]->textureID = LoadTGA("Image//PlayerTeeth.tga");
	meshList[GEO_PLAYER_TEETH]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PLAYER_TEETH]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PLAYER_TEETH]->material.kSpecular.Set(0.01f, 0.01f, 0.01f);
	meshList[GEO_PLAYER_TEETH]->material.kShininess = 1.0f;

	//Player health
	meshList[GEO_PLAYERHEALTH] = MeshBuilder::GenerateQuad1("top", Color(1.0f, 1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	meshList[GEO_PLAYERHEALTH]->textureID = LoadTGA("Image//playerHealth.tga");

	//ARROW
	meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("building", "OBJ//arrow.obj");
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//green.tga");

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		bulletPtr[bul] = new bullet();
		//init collision for the bullets here
		bulletBoxPtr[bul] = new Box(bulletPtr[bul]->throws, BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	}
}

//UPDATE FUNCTIONS
void SceneStage1::Update(double dt)
{
	elaspeTime += dt;
	deltaTime = dt;
	start.isShooting = true;
	player->timer += dt;

	UpdateBullets();
	UpdateMonsters();
	UpdateMonsterHitbox();
	UpdateMonsterAnimations();
	camera.Update(dt);
	UpdateInteractions();

	if (!inCage && breakCage < 90)
	{
		breakCage += (float)(77 * dt);
	}

	//Update Arrows
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

	if (player->health <= 0)
	{
		Application::sceneChange = Application::LOSE;
	}

}

//UPDATE PLAYER BULLET
void SceneStage1::UpdateBullets()
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
//CREATE MONSTERS AND DELETE MONSTERS, UPDATE MONTSER POSITION
void SceneStage1::UpdateMonsters()
{
	if (inCage == true)
		return;

	//MonsterFodder
	if (elaspeTime > monsterFodderTime)
	{
		for (int i = 0; i < MOBNUM1; i++)
		{
			if (MonsterFodderPtr[i] == NULL)
			{
				MonsterFodderPtr[i] = new MonsterFodder();
				monsterFodderBoxPtr[i] = new Box(MonsterFodderPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
				monsterFodderTime = elaspeTime + 1.0;
				break;
			}
		}
	}
	for (int i = 0; i < MOBNUM1; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			(*MonsterFodderPtr[i]).moveRand(camera.position, elaspeTime);
			*monsterFodderBoxPtr[i] = Box(MonsterFodderPtr[i]->pos, MOB_SIZE, MOB_SIZE, MOB_SIZE);
		}
	}

	for (int i = 0; i < MOBNUM1; i++)
	{
		if (MonsterFodderPtr[i] != NULL)
		{
			if ((*MonsterFodderPtr[i]).health <= 0)
			{
				delete MonsterFodderPtr[i];
				delete monsterFodderBoxPtr[i];
				MonsterFodderPtr[i] = NULL;
				monsterFodderBoxPtr[i] = NULL;
				
				if (elaspeTime > monsterTime)
				{
					monsterTime = elaspeTime + 1.0;
				}
				mobDead++;
			}
		}
	}
}
//CHECK FOR COLLISION
void SceneStage1::UpdateMonsterHitbox()
{
	bool isHit = false;
	hitmarkerSize = 0;

	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM1; mon++)
		{
			if (!isHit)
			{
				if (bulletBoxPtr[bul] != NULL && monsterBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[0]->isBulletHit(bulletBoxPtr[bul], monsterBoxPtr[mon]);
					if (isHit)
					{
						delete MonsterPtr[mon];
						MonsterPtr[mon] = NULL;
						monsterTime = elaspeTime + 3.0;
					}
				}
				if (isHit)
				{
					bulletPtr[bul]->monsterHit(camera);
				}
			}
		}
	}

	//Monster Fodder
	for (int bul = 0; bul < NO_OF_BULLETS; bul++)
	{
		for (int mon = 0; mon < MOBNUM1; mon++)
		{
			if (!isHit && elaspeTime > bulletBounceTime)
			{
				if (bulletBoxPtr[bul] != NULL && monsterFodderBoxPtr[mon] != NULL)
				{
					isHit = bulletPtr[bul]->isBulletHit(bulletBoxPtr[bul], monsterFodderBoxPtr[mon]);
				}
				if (isHit)
				{
					(*MonsterFodderPtr[mon]).health = (*MonsterFodderPtr[mon]).health - 10;
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

	//Player Touch Monster
	Box *playerBox = new Box(Vector3(camera.position.x, camera.position.y, camera.position.z), 5, 5, 5);

	for (int i = 0; i < MOBNUM1; i++)
	{
		if (monsterFodderBoxPtr[i] != NULL && elaspeTime > playerHurtBounceTime)
		{
			if (bulletPtr[0]->isBulletHit(playerBox, monsterFodderBoxPtr[i]))
			{
				player->health -= 10;
				if (!Application::muted)
				{
					engine->play2D("Sound/dinosaurHiss.wav", false);
				}
				playerHurtBounceTime = elaspeTime + 0.1;
			}
		}
	}

	delete playerBox;

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

//CHECKS FOR INTERACTIONS
void SceneStage1::UpdateInteractions()
{
	Box cageDoor = Box(Vector3(443, 0, 502.5), 7, 70, 70);
	Box exit = Box(Vector3(-600, 0, 0), 30,30,30);
	nearCageDoor = false;
	isNearExit = false;

	if (isNearObject(camera, exit))
	{
		isNearExit = true;
	}

	//Cage
	if (inCage == true)
	{
		Vector3 view = (camera.target - camera.position).Normalized();

		if (camera.position.x < 445)
		{
			camera.position.x = 445;
			camera.target = camera.position + view;
		}
	}
	if (inCage == true)
	{
		if (isNearObject(camera, cageDoor))
		{
			nearCageDoor = true;
		}
	}

	if (Application::IsKeyPressed('E'))
	{
		if (inCage == true && isNearObject(camera, cageDoor) == true)
		{
			if (!Application::muted)
			{
				engine->play2D("Sound/gateCrash.wav", false);
			}
			inCage = false;
		}
	}

	if (Application::IsKeyPressed('E'))
	{
		if (isNearExit && mobDead >= 10)
		{
			player->points += 100;
			Application::sceneChange = Application::STAGE2;
		}
	}

}

//ANIMATION
void SceneStage1::UpdateMonsterAnimations()
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
}

//RENDER FUNCTIONS
void SceneStage1::Render()
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

	Vector3 defaultView = Vector3(0, 0, 1).Normalize();

	//DRAW MOBS
	for (int i = 0; i < MOBNUM1; i++)
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

	RenderBullets();

	//WALLS

	//Front wall
	modelStack.PushMatrix();
	modelStack.Translate(-500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	if (mobDead >= 10 && !inCage)
	{
		//Left Wall
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-530, 20, -600);
		modelStack.Scale(1, 1, 4);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(530, 20, -600);
		modelStack.Scale(1, 1, 4);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}
	else
	{
		//Left Wall
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-500, 20, -600);
		modelStack.Scale(1, 1, 4);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(500, 20, -600);
		modelStack.Scale(1, 1, 4);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}
	//Right Wall
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(-500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	//Back Wall
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(-500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(500, 20, -600);
	modelStack.Scale(1, 1, 4);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	//CAGE
	modelStack.PushMatrix();
	modelStack.Translate(500, -10, 500);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_CAGE], false);
	modelStack.PushMatrix();
	modelStack.Translate(-3.2f, 0.2f, 0);
	modelStack.Rotate(breakCage, 0, 0, 1);
	modelStack.Translate(3.2f, -0.2f, 0);
	RenderMesh(meshList[GEO_CAGEDOOR], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	if (nearCageDoor == true)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Press E to break cage bars", Color(0, 1, 1), 2.5, 5, 5);
	}

	if (isNearExit == true && mobDead < 10)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Kill at least 10 of the peasants first!", Color(0, 1, 1), 2.5, 5, 5);
	}
	else if (isNearExit == true && mobDead >= 10)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Press E to escape to grass field", Color(0, 1, 1), 2.5, 5, 5);
	}

	if (mobDead >= 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-600, yArrowTranslate + 60, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_ARROW], false);
		modelStack.PopMatrix();
	}

	//Player 
	RenderTopTeeth();
	RenderBottomTeeth();
	RenderObjectives();
	RenderPlayerHealth();
	RenderHitmarker();

	//std::ostringstream timer;
	//timer << std::fixed << std::setprecision(3);
	//timer << player->timer << " Seconds";
	//modelStack.PushMatrix();
	//RenderTextOnScreen(meshList[GEO_TEXT], timer.str(), Color(0, 1, 0), 2, 1, 17);
	//modelStack.PopMatrix();

	std::ostringstream sFps;
	sFps << std::fixed << std::setprecision(3);
	sFps << 1.0 / deltaTime << "fps";
	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], sFps.str(), Color(1, 1, 1), 2, 1, 29);
	modelStack.PopMatrix();

}

void SceneStage1::RenderMesh(Mesh *mesh, bool enableLight)
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
void SceneStage1::RenderText(Mesh* mesh, std::string text, Color color)
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
void SceneStage1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void SceneStage1::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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

void SceneStage1::RenderBullets()
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
void SceneStage1::RenderHitmarker()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(0, 1, 1), 5, 8.3f, 6.1f);
	RenderTextOnScreen(meshList[GEO_TEXT], "o", Color(1, 0, 0), (float)hitmarkerSize, 8.3f, 6.1f);
}
void SceneStage1::RenderTopTeeth()
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

	RenderMesh(meshList[GEO_PLAYER_TEETH], false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage1::RenderBottomTeeth()
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

	RenderMesh(meshList[GEO_PLAYER_TEETH], false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}
void SceneStage1::RenderObjectives()
{
	modelStack.PushMatrix(); 
	string escape = "Escape from the cage";
	string escape2 = "Escape from the arena";
	std::ostringstream monsLeft;
	monsLeft << std::fixed << std::setprecision(1);
	monsLeft << "kill 10 Humans(" << mobDead << "/10)";

	RenderTextOnScreen(meshList[GEO_TEXT], "Objective", Color(0, 1, 0), 2, 34, 25);
	RenderTextOnScreen(meshList[GEO_TEXT], "============", Color(0, 1, 0), 2, 32, 24);
	
	if (inCage)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], escape, Color(0, 1, 0), 2, 26.2f, 23);
	}
	if (mobDead < 10 && !inCage)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], monsLeft.str(), Color(0, 1, 0), 2, 27, 23);
	}
	if (mobDead >= 10 && !inCage)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], escape2, Color(0, 1, 0), 2, 26, 23);
	}
	modelStack.PopMatrix();
}
void SceneStage1::RenderPlayerHealth()
{
	int vertical = player->health / 50;
	int horizontal = (player->health - (vertical * 50)) / 10;

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (float)(j * 4.3f), 48 - (float)(i * 4), 1, 1);
		}
	}
	for (int i = 0; i < horizontal; i++)
	{
		RenderMeshOnScreen(meshList[GEO_PLAYERHEALTH], 2.5f + (float)(i * 4.3f), 48 - (float)(vertical * 4), 1, 1);
	}
}

//EXTRA FUNCTIONS
bool SceneStage1::isNearObject(Camera3 camera, Box object)
{
	return ((camera.position.x >= object.minX && camera.position.x <= object.maxX) &&
		(camera.position.y >= object.minY && camera.position.y <= object.maxY) &&
		(camera.position.z >= object.minZ && camera.position.z <= object.maxZ));
}

void SceneStage1::Exit()
{

	for (int i = 0; i < MOBNUM1; i++)
	{
		delete MonsterPtr[i];
		delete monsterBoxPtr[i];
	}
	for (int i = 0; i < MOBNUM1; i++)
	{
		delete MonsterFodderPtr[i];
		delete monsterFodderBoxPtr[i];
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

	//glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
