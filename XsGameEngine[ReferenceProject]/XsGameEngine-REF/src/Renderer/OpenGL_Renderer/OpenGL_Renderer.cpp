#include "OpenGL_Renderer.h"

#pragma region Public Functions

int OpenGL_Renderer::init_OpenGL_Renderer(GLFWwindow* newWindow)
{
	window = newWindow;

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew initialization failed!");
		glfwDestroyWindow(newWindow);
		return 1;

	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_KHR_vulkan_glsl);
	glEnable(GL_ARB_gl_spirv);	//Allows for spir-v Shaders

	int width, height;
	glfwGetWindowSize(newWindow, &width, &height);
	glViewport(0, 0, width, height);


	shaders.CompileShaders();
	/*int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(newWindow.getWindow(), &bufferWidth, &bufferHeight);*/
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	shaders.SetProjection(projection);

	/// <summary>
	AssetManager.createOpenGL_MeshModel("src/Renderer/OpenGL_Renderer/Models/plane.obj");
	AssetManager.createOpenGL_MeshModel("src/Renderer/OpenGL_Renderer/Models/hatchet.obj");

	glfwSwapInterval(1);

	bool test = true;
	GUI_Renderer = OpenGL_GUI_Renderer(*newWindow, test);
	GUI_Renderer.createImGuiInstance();

	return 0;
}


//void OpenGL_Renderer::pushModel(const char* fileName)
//{
//	OpenGL_MeshModel* myModel = new OpenGL_MeshModel();
//	myModel->LoadModel(fileName);
//	modelList.push_back(myModel);
//}

void GLMmovements(GLuint MVPuniform, int modelIndex, const Shaders& shaders)
{
	glm::mat4 model(1.0f);
	if (modelIndex == 0)
	{
#pragma region Updates

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
#pragma endregion
	}
	else if (modelIndex == 2)
	{
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else {
		//model = glm::rotate(model, curAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, LightPos);
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	}

	glm::mat4 ViewMat = shaders.GetViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, MVPuniform);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &model);
	//glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &projection);
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &ViewMat);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void OpenGL_Renderer::draw()
{
	

	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glUseProgram(shaders.getProgram());

	GLMmovements(shaders.getMvpLocation(), 0, shaders);	// This sets the model Uniform
	for (size_t i = 0; i < AssetManager.OpenGL_MeshModelList.size(); i++)
	{
		AssetManager.OpenGL_MeshModelList[i].RenderModel();
	}
	//printf("size %i",(modelList.size()));

	//shaders.SetDirectionalLight(&mainLight);
	//shaders.SetPointLight(pointLights, pointLightCount);
	//shinyMaterial.useMaterial(uniformSpecularIntensityLocation, uniformShininessLocation);

	//shaders.SetSpotLight(spotLights, spotLightCount);

	//glUniformMatrix4fv(shaders.getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	//glUniform3f(shaders.getEyePositionLocation(), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	glUseProgram(0);

	GUI_Renderer.RenderMenus();
	
	#ifdef GUI_LAYER
	//The ImGUI Function That Renders The GUI
	if (ImGui::GetCurrentContext())
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif

	glfwSwapBuffers(window);
}



void OpenGL_Renderer::shutdown_Renderer()
{
	printf("Deleting Renderer\n");
	GUI_Renderer.CleanUpGuiComponents();
	GUI_Renderer.CleanUpGUI();
}

#pragma endregion


