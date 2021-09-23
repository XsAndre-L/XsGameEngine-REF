#pragma once
#include "Renderer/Vulkan_Renderer/VulkanUtillities.h"
#include "AssetLoading/VkAssets.h"



#include "../vendor/ImGUI/imgui.h"
#include "../vendor/ImGUI/imgui_impl_glfw.h"
#include "../vendor/ImGUI/Vulkan/imgui_impl_vulkan.h"


class GUI_Renderer
{
public:
	GUI_Renderer();
	GUI_Renderer(	GLFWwindow& window,
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

					VkAssets& AssetManager
	);



	ImVec4 clearColor = { 0.15f, 0.15f ,0.15f , 1.0f };

	void createImGuiInstance();
	void init_imGUI_Vulkan();


	void RenderMenus(bool* LocalTransform, glm::vec3* translation, glm::vec3* rotation, glm::vec3* scale, int* selectedObject, const VkAssets::AllAssets* AssetList);
	void CleanUpGUI(VkDevice Device);
	void CleanUpGuiComponents(VkDevice Device);
	~GUI_Renderer() = default;

private:

	//menu infos
	struct sharedMenuInfo {
		int* selectedObject{};
		const VkAssets::AllAssets* allAssets{};
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
		VkAssets* AssetManager{};
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

