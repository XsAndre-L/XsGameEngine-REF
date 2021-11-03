#pragma once
#include "../CommonValues.h"

#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_glfw.h"

#ifdef VULKAN
	#include "../AssetLoading/Vulkan_Assets/Vulkan_Assets.h"
	#include "ImGUI/Vulkan/imgui_impl_vulkan.h"
#elif OPENGL
	#include "../AssetLoading/OpenGL_Assets/OpenGL_Assets.h"
	#include "ImGUI/OpenGL/imgui_impl_opengl3.h"
#endif


class GUI_Renderer
{
public:

	GUI_Renderer() = default;
#ifdef VULKAN
	GUI_Renderer(	
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
#elif OPENGL
	GUI_Renderer(
		GLFWwindow& window,
		bool& ApplyGraphics,
		/*GraphicSettings& CurrentGraphicSettings,*/

		OpenGL_Assets& AssetManager
	);  
#endif
	~GUI_Renderer() = default;

	ImVec4 clearColor = { 0.15f, 0.15f ,0.15f , 1.0f };

	void createImGuiInstance();
	void init_imGUI_Vulkan();

	void CleanUpGUI();
	void CleanUpGuiComponents();
	

private:

	// --- Initialization Info ---
#pragma region Initialization Info Structs

#ifdef VULKAN
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

#elif OPENGL
		struct IgInitInfo {
			//CREATION
			GLFWwindow* mainWindow{};

			//SETTINGS
			bool* ApplyGraphics{};
			//GraphicSettings* CurrentGraphicSettings{};

			////OBJECTS
			OpenGL_Assets* AssetManager{};
		}IgInitInfo;
#endif

#pragma endregion

	// --- Menu Info Structs ---
#pragma region Menu Info Structs

		struct SharedInfo {

			int* selectedObject{};
			#ifdef VULKAN
				const Vulkan_Assets::AllAssets* allAssets{};
			#elif OPENGL
				const OpenGL_Assets::AllAssets* allAssets{};
			#endif

		}sharedInfo;				// Shared by more than one menu

		struct MenuOutlinerInfo {
			bool ShowMenu = true;

		}outlinerInfo;		// World Outliner

		struct MenuDetailsInfo {
			bool ShowMenu = true;

			bool* LocalTransform;
			glm::vec3* position{};
			glm::vec3* rotation{};
			glm::vec3* scale{};

		}detailsInfo;			// Details Menu

		struct MenuContentBrowserInfo {
			bool ShowMenu = true;
			//std::thread* AssetLoadThread;
			//std::reference_wrapper<std::thread> AssetLoadThread;
		}browserInfo;	// Content Browser

		struct MenuStatsOverlayInfo {
			bool ShowMenu = true;
			//std::thread* AssetLoadThread;
			//std::reference_wrapper<std::thread> AssetLoadThread;
		}statsInfo;		// Stats Overlay

#pragma endregion

	// --- Render Menu Functions ---
#pragma region Menu Functions

	void ShowFileMenu();
	void MainMenuBar();

	void RenderOutlinerMenu();
	void ShowOutlinerTree(const char* prefix, int uid);
	
	void RenderDetailsMenu();

	void RenderContentMenu();

	void RenderStatsOverlay();

#pragma endregion

public:
	
	template<class T>
	void RenderMenus(
		bool* LocalTransform, 
		glm::vec3* translation, 
		glm::vec3* rotation, 
		glm::vec3* scale, 
		int* selectedObject, 
		const T assetList
	)
	{

#pragma region Hotkeys
		
		//Shortcuts should happen in a diffirent funcion
		// This function should check what menu is currently being used and the shortcut could be asigned an unique function dependent on what menu we are on
			//Hide and unhide Content Browser							 [ Left_Ctrl + Space ]
			if (ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_Space]) && ImGui::GetIO().KeysDown[GLFW_KEY_LEFT_CONTROL]) {
				browserInfo.ShowMenu = !browserInfo.ShowMenu;
			}

#pragma endregion


			//Set Info
			//sharedInfo
			sharedInfo.selectedObject = selectedObject;
			//sharedInfo.modelNames = AssetList->ModelNames;
			sharedInfo.allAssets = assetList;

			//menu 'OBJECT DETAILS'
			detailsInfo.LocalTransform = LocalTransform;
			detailsInfo.position = translation;
			detailsInfo.rotation = rotation;
			detailsInfo.scale = scale;

			//menu 'WORLD OUTLINER'
			//NON

			//menu 'Content Browser'
			

			#pragma region ImGUI Render Function

			// Start the Dear ImGui frame
			#ifdef VULKAN
				ImGui_ImplVulkan_NewFrame();
			#elif OPENGL
				ImGui_ImplOpenGL3_NewFrame();
			#endif
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			//imgui commands
			//ImGui::ShowDemoWindow();

			MainMenuBar();

			if (outlinerInfo.ShowMenu)
				RenderOutlinerMenu();

			if (detailsInfo.ShowMenu)
				RenderDetailsMenu();

			if (browserInfo.ShowMenu)
				RenderContentMenu();

			if (statsInfo.ShowMenu)
				RenderStatsOverlay();



			// Rendering
			ImGui::Render();
			ImDrawData* draw_data = ImGui::GetDrawData();
			const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
			if (!is_minimized)
			{

			}

			#pragma endregion
	}

};

