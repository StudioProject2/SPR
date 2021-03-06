
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneBoss.h"
#include "SceneStage1.h"
#include "SceneStage2.h"
#include "SceneStage3.h"

#include "MainMenu.h"
#include "LevelSelect.h"
#include "SceneWin.h"
#include "SceneLose.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int Application::sceneChange = 0;
int Application::whatScene = 0;
bool Application::muted = false;
bool Application::muteToggle = false;
bool Application::inMenu = false;
//init watscene;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::SetMousePosition(float xPos, float yPos)
{
	glfwSetCursorPos(m_window, xPos, yPos);
}

void Application::GetMousePosition(double &xPos, double &yPos)
{
	glfwGetCursorPos(m_window, &xPos, &yPos);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}


void Application::Init()
{
	//Disable Cursor
	

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(800, 600, "Computer Graphics", NULL, NULL);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{

	Scene *sceneBoss = new SceneBoss();
	Scene *scene1 = new SceneStage1();
	Scene *scene2 = new SceneStage2();
	Scene *scene3 = new SceneStage3();

	Scene *sceneMenu = new MainMenu();
	Scene *sceneLevel = new LevelSelect();
	Scene *sceneWin = new SceneWin();
	Scene *sceneLose = new LoseScene();

	Scene *scene = sceneMenu;

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		if (Application::muted)
		{
			engine->stopAllSounds();
		}
		if (Application::muteToggle)
		{
			if (Application::whatScene == MAINMENU)
			{
				Application::sceneChange = MAINMENU;
			}
			Application::muted = false;
			Application::muteToggle = false;
		}
		if (Application::sceneChange == MAINMENU)
		{
			ShowCursor(true);
			scene = sceneMenu;
			scene->Init();
			whatScene = MAINMENU;
			if (!muted && !Application::inMenu)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/menuBGM.mp3", true);
				Application::inMenu = true;
			}
			Application::sceneChange = 10;
			
		}
		if (Application::sceneChange == LEVELMENU)
		{
			ShowCursor(true);
			scene = sceneLevel;
			scene->Init();
			whatScene = LEVELMENU;
			if (!muted && !Application::inMenu)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/menuBGM.mp3", true);
				Application::inMenu = true;
			}
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == STAGE1)
		{
			scene = scene1;
			scene->Init();
			whatScene = STAGE1;
			if (!muted)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/stage1BGM.mp3", true);
			}
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == STAGE2)
		{
			ShowCursor(false);
			scene = scene2;
			scene->Init();
			whatScene = STAGE2;
			if (!muted)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/stage2BGM.mp3", true);
			}
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == STAGE3)
		{
			scene = scene3;
			scene->Init();
			whatScene = STAGE3;
			if (!muted)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/stage3BGM.mp3", true);
			}
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == STAGE4)
		{
			scene = sceneBoss;
			ShowCursor(false);
			scene->Init();
			whatScene = STAGE4;
			if (!muted)
			{
				engine->stopAllSounds();
				engine->play2D("Sound/bossBGM.mp3", true);
			}
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == WIN)
		{
			scene = sceneWin;
			scene->Init();
			whatScene = WIN;
			Application::sceneChange = 10;
		}
		if (Application::sceneChange == LOSE)
		{
			scene = sceneLose;
			scene->Init();
			whatScene = LOSE;
			Application::sceneChange = 10;
		}
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	} //Check if the ESC key had been pressed or if the window had been closed

	sceneBoss->Exit();
	scene1->Exit();
	scene2->Exit();
	scene3->Exit();
	sceneMenu->Exit();
	sceneLevel->Exit();
	sceneWin->Exit();
	sceneLose->Exit();

	delete sceneBoss;
	delete scene1;
	delete scene2;
	delete scene3;
	delete sceneMenu;
	delete sceneLevel;
	delete sceneWin;
	delete sceneLose;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
