#ifndef SCENE_STAGE3
#define SCENE_STAGE3

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
#include "MonsterArcher.h"
#include "monsterBullet.h"
#include "bullet.h"
#include "Player.h"
#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

#define NO_OF_BULLETS 20
#define BULLET_SIZE 1
#define MOBNUM3 30
#define MOB_SIZE 10
#define MOBBULLETNUM 100
#define MOBBULLETDELAY 2.0

class SceneStage3 : public Scene
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
		GEO_CUBE,
		GEO_CUBE2,
		GEO_SPHERE,
		GEO_BULLETS,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_PLAYER_TEETH,

		GEO_FLOOR,
		GEO_FENCE,
		GEO_BUILDING,
		GEO_FLAME,

		GEO_FODDER_BODY,
		GEO_FODDER_HAND,
		GEO_DODGER_BODY,
		GEO_DODGER_HAND,
		GEO_DODGER_LEG,
		GEO_DODGER_WEAPON,
		GEO_ARCHER_BODY,
		GEO_ARCHER_HAND,
		GEO_ARCHER_LEG,
		GEO_ARCHER_WEAPON,

		GEO_GRASS_LINE,

		GEO_TEXT,

		GEO_PLAYERHEALTH,

		GEO_ARROW,

		NUM_GEOMETRY,
	};

public:
	SceneStage3();
	~SceneStage3();

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

	Camera3 camera;

	double elaspeTime;
	double deltaTime;
	double monsterSpawnDelay1;
	double monsterSpawnDelay2;
	double monsterSpawnDelay3;
	double monsterSpawnDelay4;
	double initialObjectiveTime;
	int monstersLeft;
	int hutsBurned;

	bool gameOver;

	Light light[4];
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

	bool isNearObject(Camera3 camera, Box object);
	//SP2 Stuff
	void UpdateBullets();
	void UpdateMonsters();
	void UpdateMonsterBullets();
	void UpdateMonsterHitbox();
	void UpdateInteractions();
	void UpdateMonsterAnimations();

	void RenderBullets();
	void RenderHitmarker();
	void RenderTopTeeth();
	void RenderBottomTeeth();
	void RenderPlayerHealth();
	void RenderNearHut();
	void RenderObjectives();

	//Times 
	double bulletBounceTime;
	double playerHurtBounceTime;

	bool hut1Burned;
	bool hut2Burned;
	bool hut3Burned;
	bool hut4Burned;

	bool nearHut1;
	bool nearHut2;
	bool nearHut3;
	bool nearHut4;

	bool isNearExit;

	float hitmarkerSize;
	float hitmarkerTimer;

	//Monsters
	Monster *MonsterPtr[MOBNUM3];
	Monster *MonsterFodderPtr[MOBNUM3];
	Monster *MonsterArcherPtr[MOBNUM3];
	//Monter Hitboxes
	Box *monsterBoxPtr[MOBNUM3];
	Box *monsterFodderBoxPtr[MOBNUM3];
	Box *monsterArcherBoxPtr[MOBNUM3];

	monsterBullet *monsterBulletPtr[MOBBULLETNUM];
	monsterBullet *monsterArcherBulletPtr[MOBBULLETNUM];

	double monsterBulletDelay[MOBNUM3];
	double monsterArcherBulletDelay[MOBNUM3];

	bullet *bulletPtr[NO_OF_BULLETS];
	bullet start;
	Box *bulletBoxPtr[NO_OF_BULLETS];
	Player *player;

	//Monster Animations Logic
	int fodSwingTimer;
	bool fodLeft;
	float fodderArmSwing;
	int dodSwingTimer;
	bool dodLeft;
	float dodgerArmSwing;
	float dodgerLegSwing;
	int arcSwingTimer;
	bool arcLeft;
	float archerArmSwing;
	float archerLegSwing;
	float dRot;
	float fRot;
	float aRot;
	float bRot;
	float bdRot;

	//Fire
	void RenderFire();
	void UpdateFireAnimation(double dt);

	float scaleAll;
	float scaleAll2;
	bool expanding;
	bool expanding2;

	float yTranslate;
	float yTranslate2;
	float yTranslate3;
	float yArrowTranslate;
	bool movingUp;

	ISoundEngine* engine;

};

#endif
