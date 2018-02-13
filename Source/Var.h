#ifndef VAR_H
#define VAR_H

//Init in Application.cpp START
extern int sceneChange;
// Init in Application.cpp END

//Init in SceneA2.cpp START
//Bomb
extern bool triggeredBomb;
extern bool nearBomb;
extern bool bombExploded;
//Tower 
extern bool inTower;
extern bool inTower2;
extern bool inTower3;
extern bool nearTower;
extern bool nearTower2;
extern bool nearTower3;
//Time
extern double bounceTime;
extern double elaspeTime;
extern double deltaTime;
extern double bombTime;
//Tent
extern bool nearTent;
extern bool nearTent2;
extern bool isAsleep;
extern bool isMorn;
//Guy
extern bool nearGuy;
extern float guyPosX;
extern float guyPosZ;
extern bool controlGuy;
//Extra stuff
extern int loopCount;
//Init in SceneA2.cpp END
#endif 
