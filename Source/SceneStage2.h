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

#define NO_OF_BULLETS 20
#define BULLET_SIZE 1
#define MOBNUM 5
#define MOB_SIZE 10
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
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_SPHERE,
		GEO_TEST,
		GEO_LIGHTBALL,

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

		GEO_FODDER,
		GEO_FODDER_HAND,

		GEO_PLAYER_TEETH,

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
	
	//inits
	Camera3 camera;
	double elaspeTime;
	double tempElaspeTime;
	double deltaTime;
	double bulletBounceTime;
	double monster1BulletTime;
	double monster2BulletTime;
	double monster3BulletTime;
	double monster4BulletTime;
	double monster5BulletTime;
	int monDead;
	int monLeft;

	bool nextStage;
	bool gameOver;

	//Base-Render function
	Light light[4];
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	//Updates
	void UpdateBullets();
	void UpdateMonsters();
	void UpdateMonsterBullets();
	void UpdateMonsterHitbox();
	void UpdateInteractions();
	void UpdateCollision();
	void UpdateObjective();

	//Renders
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

	//hit markers
	int hitmarkerSize;
	int hitmarkerTimer;

	//text size
	int LoadingTimer;
	int sizeDotOne;
	int sizeDotTwo;
	int sizeDotThree;
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
	int pickupsY;
	int pickupsZ;

	//player
	Player* player;

	//objectives
	bool objectiveOne;
	bool objectiveTwo;
	bool objectiveThree;

	//Monster Arrays
	Monster *MonsterPtr[MOBNUM];
	Monster *MonsterFodderPtr[MOBNUM];

	//MonsterHitBoxes
	Box *monsterBoxPtr[MOBNUM];
	Box *monsterFodderBoxPtr[MOBNUM];

	//Monster Bullets
	monsterBullet *monsterBulletPtr[MOBBULLETNUM];
	double monsterBulletDelay[MOBNUM];

	//Monster Times
	double monsterFodderTime;
	double monsterTime;

	bullet *bulletPtr[NO_OF_BULLETS];
	bullet start;
	Box *bulletBoxPtr[NO_OF_BULLETS];
};

#endif
