#ifndef SCENE_STAGE_2
#define SCENE_STAGE_2

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera3.h"
#include "CameraDebug.h"
#include "Box.h"
#include "Monster.h"
#include "MonsterFodder.h"
#include "monsterBullet.h"
#include "bullet.h"
#include "Player.h"
#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

#define NO_OF_BULLETS 20
#define BULLET_SIZE 1
#define MOBNUM 5
#define MOBBULLETNUM 100
#define MOBBULLETDELAY 2.0
#define MOBNUM_TO_KILL 5

class SceneStage2 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_SPHERE,

		GEO_BULLETS,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_BARRIER,
		GEO_TREE,
		GEO_GRASS_PATCH,
		GEO_GRASS_LINE,
		GEO_FLOWER,
		GEO_ROCK,
		GEO_PICKUP,

		GEO_ARROW,

		GEO_FODDER_BODY,
		GEO_FODDER_HAND,
		GEO_DODGER_BODY,
		GEO_DODGER_HAND,
		GEO_DODGER_LEG,
		GEO_DODGER_WEAPON,

		GEO_PLAYER_TEETH,
		GEO_PLAYERHEALTH,

		GEO_TEXT,

		NUM_GEOMETRY,
	};

public:
	SceneStage2();
	~SceneStage2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	MS modelStack, viewStack, projectionStack;

	//Base-Render function
	Light light[4];
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

	//AB box checkers
	bool isInObjectZ(Camera3 camera, Box object);
	bool isInObjectY(Camera3 camera, Box object);
	bool isInObjectX(Camera3 camera, Box object);
	bool isNearObject(Camera3 camera, Box object);

	//Updates
	void UpdateBullets();
	void UpdateMonsters();
	void UpdateMonsterBullets();
	void UpdateMonsterHitbox();
	void UpdateInteractions();
	void UpdateCollision();
	void UpdateObjective();
	void UpdatePickups();
	void UpdateMonsterAnimations();

	//Renders
	void RenderPlayerHealth();
	void RenderBullets();
	void RenderHitmarker();
	void RenderMonster();
	void RenderMonsterBullets();
	void RenderLights();
	void RenderSkybox();
	void RenderObj();
	void RenderMisc();
	void RenderUi();
	void RenderPickups();
	void RenderObjectives();
	void RenderTopTeeth();
	void RenderBottomTeeth();

	//inits
	Camera3 camera;
	double elaspeTime;
	double tempElaspeTime;
	double deltaTime;
	double bulletBounceTime;
	int monDead;
	bool gameOver;

	//hit markers
	int hitmarkerSize;
	int hitmarkerTimer;

	//Interaction text size
	int interactionSize;

	//tree and flowers logic
	int flowersAmt;
	bool flowerOneLife;
	bool flowerTwoLife;
	bool flowerThreeLife;
	bool treeLifeOne;
	bool treeLifeTwo;
	bool treeLifeThree;
	double treeY;
	double treeRotate;
	int treeFallTimer;
	int fallingStage;

	//pickups logic
	int pickupsTimer;
	bool pickupsSpawn;
	bool pickupsFlying;
	double pickupsY;
	double pickupsZ;

	//player
	Player* player;
	bullet *bulletPtr[NO_OF_BULLETS];
	bullet start;
	Box *bulletBoxPtr[NO_OF_BULLETS];
	//Player Hurt
	double playerHurtBounceTime;

	//objectives
	bool objectiveOne;
	bool objectiveTwo;
	bool objectiveThree;
	bool objectiveFour;

	//indicator arrow
	float yArrowTranslate;
	bool movingUp;

	//Monster Arrays
	Monster *MonsterPtr[MOBNUM];
	Monster *MonsterFodderPtr[MOBNUM];
	//MonsterHitBoxes
	Box *monsterBoxPtr[MOBNUM];
	Box *monsterFodderBoxPtr[MOBNUM];
	//Monster Bullets
	monsterBullet *monsterBulletPtr[MOBBULLETNUM];
	double monsterBulletDelay[MOBNUM];

	//Monster Spawn Timer
	double monsterFodderTime;
	double monsterTime;

	//Monster Animations Logic
	int fodSwingTimer;
	bool fodLeft;
	double fodderArmSwing;
	int dodSwingTimer;
	bool dodLeft;
	double dodgerArmSwing;
	double dodgerLegSwing;

	ISoundEngine* engine = createIrrKlangDevice();
};

#endif
