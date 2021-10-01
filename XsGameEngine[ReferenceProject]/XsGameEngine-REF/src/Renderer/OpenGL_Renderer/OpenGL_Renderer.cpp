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

	

	glfwSwapInterval(1);

	#ifdef GUI_LAYER
	bool test = true;
	GUI_Renderer = OpenGL_GUI_Renderer(*newWindow, test, AssetManager);
	GUI_Renderer.createImGuiInstance();
	#endif

	/// <summary>
	std::string Obj1 = "Models/plane.obj";
	std::string Obj2 = "Models/basicOBJ.obj";
	AssetManager.createOpenGL_MeshModel(Obj1.c_str());
	AssetManager.createOpenGL_MeshModel(Obj2.c_str());

	return 0;
}


//void OpenGL_Renderer::pushModel(const char* fileName)
//{
//	OpenGL_MeshModel* myModel = new OpenGL_MeshModel();
//	myModel->LoadModel(fileName);
//	modelList.push_back(myModel);
//}

void GLMmovements(OpenGL_MeshModel mod, GLuint MVPuniform, int modelIndex, const Shaders& shaders)
{
//	glm::mat4 model(1.0f);
//	if (modelIndex == 0)
//	{
//#pragma region Updates
//
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
//		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
//#pragma endregion
//	}
//	else if (modelIndex == 2)
//	{
//		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
//		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
//	}
//	else {
//		//model = glm::rotate(model, curAngle, glm::vec3(0.0f, 1.0f, 0.0f));
//		//model = glm::translate(model, LightPos);
//		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
//	}

	glm::mat4 ViewMat = shaders.GetViewMatrix();
	glm::mat4 ModelMat = mod.getMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, MVPuniform);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &ModelMat);
	//glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &projection);
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &ViewMat);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void OpenGL_Renderer::draw()
{
	glClearColor(GUI_Renderer.clearColor.x,GUI_Renderer.clearColor.y,GUI_Renderer.clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glUseProgram(shaders.getProgram());

	
	for (size_t i = 0; i < AssetManager.OpenGL_MeshModelList.size(); i++)
	{
		
		AssetManager.OpenGL_MeshModelList[i].RenderModel();
		GLMmovements(AssetManager.OpenGL_MeshModelList[i], shaders.getMvpLocation(), 0, shaders);	// This sets the model Uniform
	}
	//printf("size %i",(modelList.size()));

	//shaders.SetDirectionalLight(&mainLight);
	//shaders.SetPointLight(pointLights, pointLightCount);
	//shinyMaterial.useMaterial(uniformSpecularIntensityLocation, uniformShininessLocation);

	//shaders.SetSpotLight(spotLights, spotLightCount);

	//glUniformMatrix4fv(shaders.getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	//glUniform3f(shaders.getEyePositionLocation(), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	glUseProgram(0);

	
	#ifdef GUI_LAYER
		AssetManager.OpenGL_MeshModelList[*AssetManager.SetSelected()].updateMatrix();
		OpenGL_MeshModel* mesh = &AssetManager.OpenGL_MeshModelList[*AssetManager.SetSelected()];
		GUI_Renderer.RenderMenus(mesh->getTransformType(),mesh->getPosition(),mesh->getRotation(),mesh->getScale(),AssetManager.SetSelected() ,AssetManager.getAssetInfo());
		//The ImGUI Function That Renders The GUI
		if (ImGui::GetCurrentContext())
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif

	glfwSwapBuffers(window);
}



void OpenGL_Renderer::shutdown_Renderer()
{
	printf("Deleting Renderer\n");

	#ifdef GUI_LAYER
		GUI_Renderer.CleanUpGuiComponents();
		GUI_Renderer.CleanUpGUI();
	#endif
}

#pragma endregion


