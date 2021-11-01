#include "PreprocessorDefs.h"

#ifdef VULKAN
	#include "Renderer/Vulkan_Renderer/Vulkan_Renderer.h"
	Vulkan_Renderer Renderer;
#elif OPENGL
	#include "Renderer/OpenGL_Renderer/OpenGL_Renderer.h"

	OpenGL_Renderer Renderer;
#else

#endif



#include "Time.h"

GLFW_Window newWindow;

Time programTime;

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

#ifdef VULKAN
	//CREATE VULKAN INSTANCE -- needs the window so we get the window 
	if (Renderer.init_Vulkan_Renderer(newWindow.getWindow()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
#elif OPENGL
	//CREATE VULKAN INSTANCE -- needs the window so we get the window 
	if (Renderer.init_OpenGL_Renderer(newWindow.getWindow()) == EXIT_FAILURE) 
	{
		return EXIT_FAILURE;
	}
#endif
	
	float deltaTime = 0.0f;
	float angle = 0.0f;
	
	//camera needs to be set as active to change the vulkan view matrix
	camera.setAsActive(Renderer.getViewMatrix());

	static glm::vec3 translation = { 0.0f,0.0f,0.0f };
	static glm::vec3 rotation = { 0.0f,0.0f,0.0f };
	//static float f = 0.0f;
	int counter = 0;

	
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
		
		//#if defined GUI_LAYER && defined VULKAN
		//	Vulkan_MeshModel* mesh;
		//	if (Renderer.AssetManager.MeshModelList.size() > 0) {
		//		Renderer.AssetManager.MeshModelList[*Renderer.AssetManager.SetSelected()].updateMatrix();
		//		//Referense to selected mesh
		//		 mesh = &Renderer.AssetManager.MeshModelList[*Renderer.AssetManager.SetSelected()];
		//	}
		//	else {
		//		mesh = nullptr;
		//	}
		//	Vulkan_Assets::AllAssets* Assets = Renderer.AssetManager.getAssetInfo();
		//	Renderer.GUI.RenderMenus<Vulkan_Assets::AllAssets*>(mesh->getTransformType(), mesh->getPosition(), mesh->getRotation(), mesh->getScale(), Renderer.AssetManager.SetSelected(), Assets);
		//
		//	//Vulkan_Renderer.AssetManager.MeshModelList[SelectedMesh].updateMatrix();

		//	Renderer.clearColor = Renderer.GUI.clearColor;
		//#endif

		Renderer.draw();

	}


	//CLEAN UP
	Renderer.shutdown_Renderer();

	glfwDestroyWindow(newWindow.getWindow());
	glfwTerminate();

	return 0;
}

