#include "VulkanRenderer.h"
#include "Time.h"


GLFW_Window newWindow;
Time programTime;

VulkanRenderer vulkanRenderer;

Camera camera;


//int SelectedMesh = 0;


//Entry Point
int main()
{
	//CREATE WINDOW
	if (!newWindow.initWindow("WIN", 1920, 1080)) {		//1280, 720
		return EXIT_FAILURE;
	}


	//CREATE VULKAN INSTANCE -- needs the window so we get the window 
	if (vulkanRenderer.init_VulkanRenderer(newWindow.getWindow()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	

	float deltaTime = 0.0f;
	float angle = 0.0f;
	
	//camera needs to be set as active to change the vulkan view matrix
	camera.setAsActive(vulkanRenderer.getViewMatrix());

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

		//vulkanRenderer.AssetManager.allLights.directionalLight->lightDirection = camera.getCamFront();
		
		#ifdef GUI_LAYER
			
			vulkanRenderer.AssetManager.meshModelList[*vulkanRenderer.AssetManager.SetSelected()].updateModel();
			//Referense to selected mesh
			MeshModel* mesh = &vulkanRenderer.AssetManager.meshModelList[*vulkanRenderer.AssetManager.SetSelected()];

			vulkanRenderer.GUI.RenderMenus(mesh->getTransformType(), mesh->getPosition(), mesh->getRotation(), mesh->getScale(), vulkanRenderer.AssetManager.SetSelected(), vulkanRenderer.AssetManager.getAssetInfo());

			//vulkanRenderer.AssetManager.meshModelList[SelectedMesh].updateModel();

			vulkanRenderer.clearColor = vulkanRenderer.GUI.clearColor;
		
		#endif

		vulkanRenderer.draw();

	}


	//CLEAN UP
	vulkanRenderer.cleanUp();

	glfwDestroyWindow(newWindow.getWindow());

	glfwTerminate();

	return 0;
}

