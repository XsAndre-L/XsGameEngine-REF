#pragma once
#include "../../ProgramLayers/ProgramLayers.h"
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include <string>


class GLFW_Window
{
public:

	GLFW_Window();
	
	bool initWindow(std::string wName, const int width, const int height);
	GLFWwindow* getWindow() { return mainWindow; }

	double cursorXChange();
	double cursorYChange();

	bool getCursorMode() { return CursorNormal; }
	bool* getKeys() { return keys; }
	double getScroll() { return scrollPos; }

	bool shouldClose() { return glfwWindowShouldClose(mainWindow); }
	~GLFW_Window();

private:
	GLFWwindow* mainWindow;
	//Get buffer size information
	//int bufferWidth, bufferHeight;

	//Cursor Shown or Hidden
	bool CursorNormal = true;


	void createCallbacks();


	bool keys[1024];
	static void key_callback(GLFWwindow* window, int key, int code, int action, int mode);

	double lastX{};
	double lastY{};
	double xChange{ 0.0f };
	double yChange{ 0.0f };
	bool mouseFirstMoved;
	static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

	bool SelectAction = false;
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	double scrollPos = 1.0f;
	double scrollChanges = 0.0f;
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

	static void onMouse(int button, int state, int x, int y);


	void GetMaxMonitorResolution(int* w, int* h, int* refresh);
};







