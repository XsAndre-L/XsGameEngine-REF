#include "OpenGL_Renderer.h"

#pragma region Public Functions

int OpenGL_Renderer::init_OpenGL_Renderer(GLFWwindow* newWindow)
{
	window = newWindow;

	int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (gladStatus == 0)
	{
		printf("Glad initialization failed!");
		glfwDestroyWindow(newWindow);
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ARB_gl_spirv);	//Allows for spir-v Shaders	//TODO

	int width, height;
	glfwGetWindowSize(newWindow, &width, &height);
	glViewport(0, 0, width, height);


	shaders.CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	shaders.SetProjection(projection);

	
	glfwSwapInterval(1);

	#if defined GUI_LAYER && defined OPENGL
	bool test = true;
	
	new_GUI_Renderer = GUI_Renderer(*newWindow, test, AssetManager);
	new_GUI_Renderer.createImGuiInstance();
	#endif

	/// <summary>
	std::string Obj1 = "Models/plane.obj";
	std::string Obj2 = "Models/basicOBJ.obj";

	AssetManager.createAsset(Obj1.c_str());
	AssetManager.createAsset(Obj2.c_str());
	//AssetManager.addTexture("Textures/plain.png");
	//AssetManager.createOpenGL_MeshModel(Obj3.c_str());

	return 0;
}

//AT the moment this function only refreshes the View and Model Matrices
void GLMmovements(OpenGL_MeshModel mod, GLuint MVPuniform, int modelIndex, const Shaders& shaders)
{
	glm::mat4 ViewMat = shaders.GetViewMatrix();
	glm::mat4 ModelMat = mod.getMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, MVPuniform);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &ModelMat);
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &ViewMat);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void OpenGL_Renderer::draw()
{
	if (AssetManager.Load_Model_Files)
	{
		AssetManager.createOpenGL_MeshModel();
	}


#if defined GUI_LAYER && defined OPENGL
	glClearColor(new_GUI_Renderer.clearColor.x, new_GUI_Renderer.clearColor.y, new_GUI_Renderer.clearColor.z, 1.0f);
#else
	glClearColor(40.0f, 40.0f, 40.0f, 1.0f);
	
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glUseProgram(shaders.getProgram());

	
	for (size_t i = 0; i < AssetManager.MeshModelList.size(); i++)	// TODO
	{
		GLMmovements(AssetManager.MeshModelList[i], shaders.getMvpLocation(), 0, shaders);	// This sets the model Uniform
		//AssetManager.OpenGL_MeshModelList[i].RenderModel();
		auto currModel = AssetManager.MeshModelList[i];
		for (size_t i = 0; i < currModel.getMeshCount(); i++)
		{

			uint32_t materialIndex = currModel.getMeshToTex()->at(i);//meshToTex[i];

			if (materialIndex < AssetManager.textureList.size() && &AssetManager.textureList[materialIndex] != nullptr)
			{
				AssetManager.textureList[materialIndex].UseTexture();
			}

			//currModel.meshList[i].renderMesh();
			currModel.getMesh(i)->renderMesh();
		}
		
	}
	//printf("size %i",(modelList.size()));

	//shaders.SetDirectionalLight(&mainLight);
	//shaders.SetPointLight(pointLights, pointLightCount);
	//shinyMaterial.useMaterial(uniformSpecularIntensityLocation, uniformShininessLocation);

	//shaders.SetSpotLight(spotLights, spotLightCount);

	//glUniformMatrix4fv(shaders.getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	//glUniform3f(shaders.getEyePositionLocation(), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	glUseProgram(0);

	
	#if defined GUI_LAYER && defined OPENGL
	OpenGL_MeshModel* mesh;
	if (AssetManager.MeshModelList.size() > 0) {
		AssetManager.MeshModelList[*AssetManager.SetSelected()].updateMatrix();
		mesh = &AssetManager.MeshModelList[*AssetManager.SetSelected()];
	}
	else
	{
		mesh = nullptr;
	}
		new_GUI_Renderer.RenderMenus<OpenGL_Assets::AllAssets*>(mesh->getTransformType(),mesh->getPosition(),mesh->getRotation(),mesh->getScale(),AssetManager.SetSelected() ,AssetManager.getAssetInfo());
		//The ImGUI Function That Renders The GUI
		if (ImGui::GetCurrentContext())
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif

	glfwSwapBuffers(window);
}



void OpenGL_Renderer::shutdown_Renderer()
{
	printf("Deleting Renderer\n");

	#if defined GUI_LAYER && defined OPENGL
		new_GUI_Renderer.CleanUpGuiComponents();
		new_GUI_Renderer.CleanUpGUI();
	#endif
}

#pragma endregion


