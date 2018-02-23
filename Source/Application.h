
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "irrKlang.h"

#pragma comment(lib, "irrKlang.lib")

using namespace irrklang;

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	//static void GetMousePos(double &xpos, double &ypos);
	static bool IsKeyPressed(unsigned short key);
	static void SetMousePosition(double xPos, double yPos);
	static void GetMousePosition(double &xPos, double &yPos);
	static int sceneChange;
	static int whatScene;
	static bool muted;
	static bool muteToggle;
	static bool inMenu;

	enum sceneType
	{
		MAINMENU,
		LEVELMENU,
		STAGE1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGELOSE,
		STAGETEST
	};

private:

	//Declare a window object
	StopWatch m_timer;
	ISoundEngine* engine = createIrrKlangDevice();
};

#endif