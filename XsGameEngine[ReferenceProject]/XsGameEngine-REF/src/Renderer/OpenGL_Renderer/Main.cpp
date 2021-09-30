#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>



#include "OpenGL_Renderer.h"
//#include "../../Window/Public/GLFW_Window.h"


GLFW_Window newWindow;
OpenGL_Renderer newRenderer_opengl;

//std::vector<OpenGL_Mesh*> meshList;

Time tTime;

Camera camera;

//void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
//{
//	for (size_t i = 0; i < indiceCount; i += 3)
//	{
//		unsigned int in0 = indices[i] * vLength;
//		unsigned int in1 = indices[i + 1] * vLength;
//		unsigned int in2 = indices[i + 2] * vLength;
//		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
//		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
//		glm::vec3 normal = glm::cross(v1, v2);
//		normal = glm::normalize(normal);
//
//		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
//		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
//		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
//		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
//	}
//
//	for (size_t i = 0; i < verticeCount / vLength; i++)
//	{
//		unsigned int nOffset = i * vLength + normalOffset;
//		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
//		vec = glm::normalize(vec);
//		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
//	}
//}

//void AddMesh() 
//{
//	#pragma region Piramid
//	unsigned int indices[] = {
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2
//	};
//
//	GLfloat vertices[] = {
//		//x		y	  z	     u	   v		nx    ny    nz
//		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
//		0.0f, -1.0f, 1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
//	};
//	#pragma endregion
//
//	#pragma region Icon
//	unsigned int indices2[] = {
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2
//	};
//
//	GLfloat vertices2[] = {
//		//x		y	  z	     u	   v		nx    ny    nz
//		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
//		0.0f, -1.0f, 1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
//	};
//	#pragma endregion
//	
//#pragma region Plane
//	unsigned int indices3[] = {
//		0, 1, 2,
//		1, 2, 3
//
//	};
//
//	GLfloat vertices3[] = {
//		//x		y	  z			u	   v		nx    ny    nz
//		-10.0f,10.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
//		-10.0f,-10.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
//		10.0f,10.0f,0.0f,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
//		10.0f,-10.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f
//	};
//#pragma endregion
//
//	//calcAverageNormals(indices, 12, vertices, 32, 8, 5);
//	//calcAverageNormals(indices2, 12, vertices2, 32, 8, 5);
//	
//
//	OpenGL_Mesh*newMesh = new OpenGL_Mesh();
//	newMesh->createMesh(vertices, indices, 32, 12);
//	
//	OpenGL_Mesh* newMesh2 = new OpenGL_Mesh();
//	newMesh2->createMesh(vertices2, indices2, 32, 12);
//
//	OpenGL_Mesh* newMesh3 = new OpenGL_Mesh();
//	newMesh3->createMesh(vertices3, indices3, 32, 6);
//
//	meshList.push_back(newMesh);
//	meshList.push_back(newMesh2);
//	meshList.push_back(newMesh3);
//
//}

Assimp::Importer importer = Assimp::Importer();

glm::vec3 LightPos(-4.0f, 0.0f, 0.0f);





//opengl32.lib
int main() {
	//CREATE WINDOW
	if (!newWindow.initWindow("WIN", 1920, 1080)) {		//1280, 720
		return EXIT_FAILURE;
	}

	
	newRenderer_opengl.init_OpenGL_Renderer(newWindow.getWindow());


	newRenderer_opengl.AssetManager.createOpenGL_MeshModel("src/Renderer/OpenGL_Renderer/Models/plane.obj");
	newRenderer_opengl.AssetManager.createOpenGL_MeshModel("src/Renderer/OpenGL_Renderer/Models/hatchet.obj");


	glfwSwapInterval(0);
	
	//AddMesh();
	
	//GLuint Program = shaders.getProgram();

    
	float deltaTime = 0.0f;
	camera.setAsActive(newRenderer_opengl.getShaders()->ViewMatrix());	// TO SET SPICIFIC CAMERA AS THE ACTIVE CAMERA

	while (!newWindow.shouldClose())
	{

		//GLfloat now = glfwGetTime();
		tTime.startTimeTracker();
		deltaTime = tTime.getDeltaTime();
		tTime.getFps();

		glfwPollEvents();

		camera.checkInputs(newWindow.getCursorMode(), newWindow.getKeys(), newWindow.getScroll(), deltaTime);
		camera.setCamera(newWindow.getCursorMode(), newWindow.cursorXChange(), newWindow.cursorYChange(), deltaTime);


		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//here DRAW
		newRenderer_opengl.draw();

		glfwSwapBuffers(newWindow.getWindow());
	}

	
	
	//glfwDestroyWindow(newWindow.getWindow());
	newRenderer_opengl.shutdown_Renderer();

	return 0;
}

