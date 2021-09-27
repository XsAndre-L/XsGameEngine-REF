#pragma once

#include <GLEW/glew.h>
#include "../../Window/Public/GLFW_Window.h"
#include <glm/glm.hpp>

#include "../../Camera.h"

class OpenGL_Renderer
{
public:
	OpenGL_Renderer() = default;
	~OpenGL_Renderer() = default;

	int init_OpenGL_Renderer(GLFWwindow* newWindow);		//initialize

	void shutdown_Renderer();
private:
	GLFWwindow* window;
};

