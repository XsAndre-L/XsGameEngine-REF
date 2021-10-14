#pragma once
//#include "CommonValues.h"
//#include <GLEW/glew.h>
#include "Glad/glad.h"
#include "../../Window/Public/GLFW_Window.h"

#include "Shaders.h"

//#include "OpenGL_GUI_Renderer.h"
#ifdef OPENGL
#include "../GUI_Renderer.h"
#endif
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
	glm::mat4* getViewMatrix(){ return shaders.ViewMatrix(); }

	void shutdown_Renderer();
private:
	

	GLFWwindow* window;

	Shaders shaders;

public:

	OpenGL_Assets AssetManager;


	//OpenGL_GUI_Renderer GUI_Renderer;
	//std::vector<OpenGL_MeshModel*> modelList;
#ifdef OPENGL
	GUI_Renderer new_GUI_Renderer;
#endif
};

