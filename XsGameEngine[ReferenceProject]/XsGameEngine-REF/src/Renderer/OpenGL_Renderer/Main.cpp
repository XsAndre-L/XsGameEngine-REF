#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <vector>
#include <assimp/Importer.hpp>

#include "CommonValues.h"

#include "ProgramWindow.h"
#include "Shaders.h"
#include "Mesh.h"
#include "Camera.h"

#include "Texture.h"
#include "DirectionalLight.h"
//#include "PointLight.h"
//#include "SpotLight.h"

#include "Model.h"
#include "Material.h"

#include "Time.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


std::vector<Mesh*> meshList;

Time tTime;



std::vector<Model*> modelList;



void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void AddMesh() 
{
	#pragma region Piramid
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//x		y	  z	     u	   v		nx    ny    nz
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};
	#pragma endregion

	#pragma region Icon
	unsigned int indices2[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices2[] = {
		//x		y	  z	     u	   v		nx    ny    nz
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};
	#pragma endregion
	
#pragma region Plane
	unsigned int indices3[] = {
		0, 1, 2,
		1, 2, 3

	};

	GLfloat vertices3[] = {
		//x		y	  z			u	   v		nx    ny    nz
		-10.0f,10.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-10.0f,-10.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		10.0f,10.0f,0.0f,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		10.0f,-10.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f
	};
#pragma endregion

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(indices2, 12, vertices2, 32, 8, 5);
	

	Mesh *newMesh = new Mesh();
	newMesh->createMesh(vertices, indices, 32, 12);
	
	Mesh* newMesh2 = new Mesh();
	newMesh2->createMesh(vertices2, indices2, 32, 12);

	Mesh* newMesh3 = new Mesh();
	newMesh3->createMesh(vertices3, indices3, 32, 6);

	meshList.push_back(newMesh);
	meshList.push_back(newMesh2);
	meshList.push_back(newMesh3);

}

Assimp::Importer importer = Assimp::Importer();

glm::vec3 LightPos(-4.0f, 0.0f, 0.0f);

void GLMmovements(GLuint uniformModel, int modelIndex, ProgramWindow& window, Shaders& shaders)
{
	glm::mat4 model(1.0f);
	if (modelIndex == 0 )
	{
		#pragma region Updates
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		#pragma endregion
	}
	else if(modelIndex == 2)
	{
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else {
		//model = glm::rotate(model, curAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, LightPos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window.getWindow(), &bufferWidth, &bufferHeight);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformModel);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &model);
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &projection);
	glBufferSubData(GL_UNIFORM_BUFFER, 128, 64, shaders.GetViewMatrix());

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void pushModel(const char* fileName) 
{
	Model* myModel = new Model();
	myModel->LoadModel(fileName);
	modelList.push_back(myModel);
}

//opengl32.lib
int main() {

	ProgramWindow window{};
	

	if (!window.createWindow(1256, 720))
	{
		glfwTerminate();
		return 1;
	}

	Shaders shaders{};
	window.setShaders(&shaders);
	Texture brickTexture;

	//DirectionalLight mainLight;
	//PointLight pointLights[MAX_POINT_LIGHTS];
	//SpotLight spotLights[MAX_SPOT_LIGHTS];

	Material shinyMaterial;

	

	pushModel("src/Renderer/OpenGL_Renderer/Models/plane.obj");
	pushModel("src/Renderer/OpenGL_Renderer/Models/hatchet.obj");
	/*myModel2.LoadModel("Models/test.obj");
	myModel.LoadModel("Models/hatchet.obj");*/
	

	glfwSwapInterval(0);
	
	AddMesh();
	
	GLuint Program = shaders.getProgram();
	

	#pragma region Set Projection
	glUseProgram(Program);
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window.getWindow(), &bufferWidth, &bufferHeight);

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);
	//glUniformMatrix4fv(shaders.getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection));
	
	glUseProgram(0);
	#pragma endregion

	Camera camera = Camera();
	camera.setShaders(&shaders);
	//Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.5f);

	//brickTexture = Texture("Textures/brick.png");
	//brickTexture.LoadTextureA();
	//brickTexture.UseTexture();


	//mainLight.LightDirectional(1.0f, 1.0f, 1.0f, 1.0f);
	/*mainLight = DirectionalLight(
		0.9f, 1.0f, 1.0f, 
		0.1f, 1.0f,
		2.0f, -1.0f, -2.0f
	);*/
		/*
	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(
		0.0f, 1.0f, 0.0f,
		0.1f, 1.0f,
		-4.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f
	);

	pointLightCount++;


	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.2f, 0.1f, 50.0f
	);

	spotLightCount++;
	*/
	shinyMaterial = Material(1.0f, 32);

	//GLuint uniformSpecularIntensityLocation, uniformShininessLocation;

	float change = 0.01;
	int count = 0;
	bool check = false;

    
	float deltaTime = 0.0f;

	while (!window.shouldClose())
	{
		/*if (check == false)
		{
			count++;
		}
		else {
			count--;
		}

		if (count % 1000 == 0)
		{
			check = !check;
		}*/

		//GLfloat now = glfwGetTime();
		tTime.startTimeTracker();
		deltaTime = tTime.getDeltaTime();
		tTime.getFps();
		
		

		glfwPollEvents();
		camera.keyControl(window.getKeys(), window.getScroll(), deltaTime);

		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.setCamera(window.getXChange(), window.getYChange(), deltaTime);
		//printf("POS : %f %f %f \n", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//printf("modd : %f %f %f \n", modelList[i])
		//LightPos = {-4.0f + (count * change), 0.0f, 0.0f};
		/*pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
									0.0f, 1.0f,
									LightPos.x, LightPos.y, LightPos.z,
									0.3f, 0.2f, 0.1f);*/
		
		//spotLights[0].SetFlash(camera.getCameraPosition(), camera.getCameraDirection());

		//uniformSpecularIntensityLocation = shaders.getSpecularIntensityLocation();
		//uniformShininessLocation = shaders.getShininessLocation();
		camera.calculateViewMatrix();
		glUseProgram(Program);

		GLMmovements(shaders.getUniformModel(), 0, window, shaders);	// This sets the model Uniform
		for (size_t i = 0; i < modelList.size(); i++)
		{
			modelList[i]->RenderModel();
		}
		//printf("size %i",(modelList.size()));

		//shaders.SetDirectionalLight(&mainLight);
		//shaders.SetPointLight(pointLights, pointLightCount);
		//shinyMaterial.useMaterial(uniformSpecularIntensityLocation, uniformShininessLocation);

		//shaders.SetSpotLight(spotLights, spotLightCount);

		//glUniformMatrix4fv(shaders.getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//glUniform3f(shaders.getEyePositionLocation(), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glUseProgram(0);


		glfwSwapBuffers(window.getWindow());
	}

	return 0;
}

