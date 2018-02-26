#ifndef SCENE_STAGE_1
#define SCENE_STAGE_1

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
#define MOBNUM 10
#define MOB_SIZE 10
#define MOBBULLETNUM 100
#define MOBBULLETDELAY 2.0

class SceneStage1 : public Scene
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
		GEO_QUAD1,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_HEM,
		GEO_SPHERE,
		GEO_SPHERE1,
		GEO_SPHERE2,
		GEO_SPHERE3,
		GEO_SPHERE4,
		GEO_SPHERE5,
		GEO_SPHERE6,
		GEO_SPHERE7,
		GEO_SPHERE8,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL2,

		GEO_BULLETS,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_FLOOR,
		GEO_FENCE,
		GEO_CHAIR,
		GEO_WALL,
		GEO_WATCHWALL,
		GEO_TUNNEL,
		GEO_BOMB,
		GEO_TENT,
		GEO_LAMP,
		GEO_TANK,
		GEO_WATCHTOWER,
		GEO_GUY,

		GEO_FODDER_BODY,
		GEO_FODDER_HAND,

		GEO_CAGE,
		GEO_CAGEDOOR,

		GEO_PLAYER_TEETH,

		GEO_PLAYERHEALTH,

		GEO_TEXT,

		GEO_ARROW,


		NUM_GEOMETRY,
	};

public:
	SceneStage1();
	~SceneStage1();

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
	double tempElaspeTime;
	double deltaTime;
	double monsterTime;
	double bulletBounceTime;

	bool gameOver;

	Light light[4];
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);

	void UpdateBullets();
	void UpdateMonsters();
	void UpdateMonsterBullets();
	void UpdateMonsterHitbox();
	void UpdateMonsterAnimations();

	void RenderBullets();
	void RenderHitmarker();

	int hitmarkerSize;
	int hitmarkerTimer;

	Monster *MonsterPtr[MOBNUM];
	Box *monsterBoxPtr[MOBNUM];
	monsterBullet *monsterBulletPtr[MOBBULLETNUM];
	double monsterBulletDelay[MOBNUM];
	bullet *bulletPtr[NO_OF_BULLETS];
	bullet start;
	Box *bulletBoxPtr[NO_OF_BULLETS];

	//Monster Fodder
	Monster *MonsterFodderPtr[MOBNUM];
	Box *monsterFodderBoxPtr[MOBNUM];
	double monsterFodderTime;

	//MonsterCount
	int mobDead;

	//Interaction variables/Methods
	bool nearCageDoor;
	bool inCage;
	bool isNearObject(Camera3 camera, Box object);
	bool isNearExit;
	void UpdateInteractions();
	void RenderObjectives();
	float breakCage;

	//Player
	Player *player;
	double playerHurtBounceTime;

	//Render Fps Model
	void RenderTopTeeth();
	void RenderBottomTeeth();

	//Player health
	void RenderPlayerHealth();
	
	//End scene
	bool completeObjectives;

	//Monster Animations Logic
	int fodSwingTimer;
	bool fodLeft;
	double fodderArmSwing;
	
	//Arrow
	float yArrowTranslate;
	bool movingUp;

};

#endif
