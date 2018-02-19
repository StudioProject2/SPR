
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

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
	enum sceneType
	{
		MAINMENU,
		LEVELMENU,
		STAGE1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGETEST
	};

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif