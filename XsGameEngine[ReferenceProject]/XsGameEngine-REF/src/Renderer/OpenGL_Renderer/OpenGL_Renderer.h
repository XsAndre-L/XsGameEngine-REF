#pragma once
#include "CommonValues.h"
#include <GLEW/glew.h>
#include "../../Window/Public/GLFW_Window.h"

#include "Shaders.h"

//#include "Material.h"
#include "../../AssetLoading/OpenGL_Assets/OpenGL_Assets.h"

#include "../../Time.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


#include "../../Camera.h"

class OpenGL_Renderer
{
public:
	OpenGL_Renderer() = default;
	~OpenGL_Renderer() = default;

	int init_OpenGL_Renderer(GLFWwindow* newWindow);		//initialize
	//void pushModel(const char* fileName);
	void draw();

	Shaders* getShaders() { return &shaders; }

	void shutdown_Renderer();
private:
	

	GLFWwindow* window;

	Shaders shaders;

public:

	OpenGL_Assets AssetManager;
	//std::vector<OpenGL_MeshModel*> modelList;
};

