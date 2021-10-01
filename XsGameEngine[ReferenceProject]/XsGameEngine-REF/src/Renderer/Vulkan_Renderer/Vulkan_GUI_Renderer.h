#pragma once
#include "../../CommonValues.h"
#include "../../AssetLoading/Vulkan_Assets/Vulkan_Assets.h"



#include "../../../vendor/ImGUI/imgui.h"
#include "../../../vendor/ImGUI/imgui_impl_glfw.h"
#include "../../../vendor/ImGUI/Vulkan/imgui_impl_vulkan.h"


class Vulkan_GUI_Renderer
{
public:
	Vulkan_GUI_Renderer();
	Vulkan_GUI_Renderer(	
		GLFWwindow& window,
		VkInstance& instance,
		VkDevice& Device,
		VkPhysicalDevice& PhysicalDevice,
		VkQueue& Queue, 
		uint32_t GraphicsFamily,
		VkRenderPass& RenderPass,
		VkCommandPool& GraphicsCommandPool,
		VkSampleCountFlagBits& msaaSampleCount,
		bool& ApplyGraphics,
		GraphicSettings& CurrentGraphicSettings,

		Vulkan_Assets& AssetManager);



	ImVec4 clearColor = { 0.15f, 0.15f ,0.15f , 1.0f };

	void createImGuiInstance();
	void init_imGUI_Vulkan();


	void RenderMenus(bool* LocalTransform, glm::vec3* translation, glm::vec3* rotation, glm::vec3* scale, int* selectedObject, const Vulkan_Assets::AllAssets* AssetList);
	void CleanUpGUI(VkDevice Device);
	void CleanUpGuiComponents(VkDevice Device);
	~Vulkan_GUI_Renderer() = default;

private:

	//menu infos
	struct sharedMenuInfo {
		int* selectedObject{};
		const Vulkan_Assets::AllAssets* allAssets{};
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
		VkInstance* instance{};
		VkDevice* Device{};
		VkPhysicalDevice* PhysicalDevice{};
		VkQueue* Queue{};
		uint32_t GraphicsFamily{};
		VkRenderPass* RenderPass{};
		VkCommandPool* GraphicsCommandPool{};

		//SETTINGS
		VkSampleCountFlagBits* MsaaSampleCount{};
		bool* ApplyGraphics{};
		GraphicSettings* CurrentGraphicSettings{};

		//OBJECTS
		Vulkan_Assets* AssetManager{};
	} IgInitInfo;
	


	VkDescriptorPool imguiPool{};

	
	void ShowFileMenu();
	void mainMenuBar();

	void ShowOutlinerTree(const char* prefix, int uid);
	void renderOutlinerMenu();

	void renderDetailsMenu();
	void renderContentMenu();
	void renderStatsOverlay();

};

