//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Application.h"
#include "SceneExample.h"
#include "SceneManager.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::cursorX = RESOLUTION_X * 0.5f, Application::cursorY = RESOLUTION_Y * 0.5f; //cursor xy
bool Application::leftMouseClick = false, Application::rightMouseClick = false;


//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Define the mouse movement callback
static void cursor_position_callback(GLFWwindow* window, double cursorX, double cursorY)
{
	Application::cursorX = cursorX;
	Application::cursorY = cursorY;
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

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		Application::leftMouseClick = true;
	else
		Application::leftMouseClick = false;

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		Application::rightMouseClick = true;
	else
		Application::rightMouseClick = false;
}

void Application::Init()
{
	
	//set seed for random
	Math::InitRNG();

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
	m_window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Set window resize callback
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	//Sets mouse callback
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetCursorPos(m_window, Application::cursorX, Application::cursorY);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);

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
	//Main Loop
	SceneManager sceneMan;
	Scene* scene = sceneMan.GetFirstScene();
	scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_F4))
	{
		//hide or show cursor
		if (scene->GetCaptureMouse())
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  

		sceneMan.SetCurrentScene(&scene);		//give scenemanager the current scene
		sceneMan.PollForSceneChangeEvent();		//check if any scene changes and then change scene

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
