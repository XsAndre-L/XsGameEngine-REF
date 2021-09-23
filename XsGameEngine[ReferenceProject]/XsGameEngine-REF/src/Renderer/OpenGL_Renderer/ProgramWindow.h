#pragma once

#include <stdio.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/ext/matrix_projection.hpp>
#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/glm.hpp>


#include "Shaders.h"

class ProgramWindow
{
public:
	ProgramWindow();
	~ProgramWindow();

	GLFWwindow* getWindow() { return mainWindow; }

	bool createWindow(GLint Width, GLint Height);
	bool shouldClose() { return glfwWindowShouldClose(mainWindow); }
	
	bool* getKeys() { return keys; }
	float getScroll() { return scrollPos; }

	GLfloat getXChange();
	GLfloat getYChange();

	void setShaders(Shaders* shadersPointer) { shaders = shadersPointer; }

	void destroyWindow();

private:
	GLFWwindow* mainWindow{};
	Shaders* shaders{};

	//Get buffer size information
	int bufferWidth{}, bufferHeight{};

	#pragma region Input
	bool keys[1024];
	//Runtime Settings
	bool CursorNormal = false;
	bool SelectAction = false;

	void createCallbacks();
	static void key_callback(GLFWwindow* window, int key, int code, int action, int mode);

	GLfloat lastX{};
	GLfloat lastY{};
	GLfloat xChange = 0.0f;
	GLfloat yChange = 0.0f;
	bool mouseFirstMoved;

	static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	GLfloat scrollPos = 1.0f;
	GLfloat scrollChanges = 0.0f;
	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
	#pragma endregion

	static void onMouse(int button, int state, int x, int y);
	

};

