#pragma once
//#include "VulkanUtillities.h"
//#include <GLEW/glew.h>
#include "../../AssetLoading/OpenGL_Assets/OpenGL_Assets.h"



#include "../../../vendor/ImGUI/imgui.h"
#include "../../../vendor/ImGUI/imgui_impl_glfw.h"
#include "../../../vendor/ImGUI/OpenGL/imgui_impl_opengl3.h"


class OpenGL_GUI_Renderer
{
public:
	OpenGL_GUI_Renderer();
	OpenGL_GUI_Renderer(	GLFWwindow& window,
					bool& ApplyGraphics/*,
					GraphicSettings& CurrentGraphicSettings,

					OpenGL_Assets& AssetManager*/
	);



	ImVec4 clearColor = { 0.15f, 0.15f ,0.15f , 1.0f };

	void createImGuiInstance();
	//void init_imGUI_Vulkan();


	void RenderMenus(/*bool* LocalTransform, glm::vec3* translation, glm::vec3* rotation, glm::vec3* scale, int* selectedObject, const OpenGL_Assets::AllAssets* AssetList*/);
	void CleanUpGUI();
	void CleanUpGuiComponents();
	~OpenGL_GUI_Renderer() = default;

private:

	//menu infos
	struct sharedMenuInfo {
		int* selectedObject{};
		const OpenGL_Assets::AllAssets* allAssets{};
	}sharedInfo;

	//World Outliner
	struct menuOutlinerInfo {
		bool ShowMenu = true;

	}outlinerInfo;

	//Details Menu
	struct menuDetailsInfo {
		bool ShowMenu = true;

		bool* LocalTransform;
		glm::vec3* position{};
		glm::vec3* rotation{};
		glm::vec3* scale{};

	}detailsInfo;

	struct menuContentBrowserInfo {
		bool ShowMenu = true;
		//std::thread* AssetLoadThread;
		//std::reference_wrapper<std::thread> AssetLoadThread;
	}browserInfo;

	struct menuStatsOverlayInfo {
		bool ShowMenu = true;
		//std::thread* AssetLoadThread;
		//std::reference_wrapper<std::thread> AssetLoadThread;
	}statsInfo;

	//Initialization Info
	struct IgInitInfo {
		//CREATION
		GLFWwindow* mainWindow{};

		//SETTINGS
		
		bool* ApplyGraphics{};
		//GraphicSettings* CurrentGraphicSettings{};

		////OBJECTS
		//OpenGL_Assets* AssetManager{};
	} IgInitInfo;
	


	

	
	void ShowFileMenu();
	void mainMenuBar();

	//void ShowOutlinerTree(const char* prefix, int uid);
	//void renderOutlinerMenu();

	//void renderDetailsMenu();
	//void renderContentMenu();
	//void renderStatsOverlay();

};

