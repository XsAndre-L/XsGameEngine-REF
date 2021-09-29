#include "PreprocessorDefs.h"

#include "Renderer/Vulkan_Renderer/Vulkan_Renderer.h"
#include "Time.h"


GLFW_Window newWindow;
Time programTime;

Vulkan_Renderer newVulkan_Renderer;

Camera camera;


//int SelectedMesh = 0;


//Entry Point
/**
* The Entry Point
* @author Andre Loubser
* 
*/


int main()
{
	//CREATE WINDOW
	if (!newWindow.initWindow("WIN", 1920, 1080)) {		//1280, 720
		return EXIT_FAILURE;
	}


	//CREATE VULKAN INSTANCE -- needs the window so we get the window 
	if (newVulkan_Renderer.init_Vulkan_Renderer(newWindow.getWindow()) == EXIT_FAILURE)
	{ return EXIT_FAILURE; }
	

	float deltaTime = 0.0f;
	float angle = 0.0f;
	
	//camera needs to be set as active to change the vulkan view matrix
	camera.setAsActive(newVulkan_Renderer.getViewMatrix());

	static glm::vec3 translation = { 0.0f,0.0f,0.0f };
	static glm::vec3 rotation = { 0.0f,0.0f,0.0f };
	//static float f = 0.0f;
	int counter = 0;

	//bool enitialMove = false;
	
	//PROGRAM LOOP
	while (!newWindow.shouldClose())
	{
		glfwPollEvents();

		programTime.startTimeTracker();

		deltaTime = programTime.getDeltaTime();
		programTime.getFps();


		camera.checkInputs(newWindow.getCursorMode(), newWindow.getKeys(), newWindow.getScroll(), deltaTime);
		camera.setCamera(newWindow.getCursorMode() ,newWindow.cursorXChange(), newWindow.cursorYChange(), deltaTime);

		//Vulkan_Renderer.AssetManager.allLights.directionalLight->lightDirection = camera.getCamFront();
		
		#ifdef GUI_LAYER
			
			newVulkan_Renderer.AssetManager.Vulkan_MeshModelList[*newVulkan_Renderer.AssetManager.SetSelected()].updateMatrix();
			//Referense to selected mesh
			Vulkan_MeshModel* mesh = &newVulkan_Renderer.AssetManager.Vulkan_MeshModelList[*newVulkan_Renderer.AssetManager.SetSelected()];

			newVulkan_Renderer.GUI.RenderMenus(mesh->getTransformType(), mesh->getPosition(), mesh->getRotation(), mesh->getScale(), newVulkan_Renderer.AssetManager.SetSelected(), newVulkan_Renderer.AssetManager.getAssetInfo());

			//Vulkan_Renderer.AssetManager.Vulkan_MeshModelList[SelectedMesh].updateMatrix();

			newVulkan_Renderer.clearColor = newVulkan_Renderer.GUI.clearColor;
		
		#endif

		newVulkan_Renderer.draw();

	}


	//CLEAN UP
	newVulkan_Renderer.cleanUp();

	glfwDestroyWindow(newWindow.getWindow());

	glfwTerminate();

	return 0;
}

