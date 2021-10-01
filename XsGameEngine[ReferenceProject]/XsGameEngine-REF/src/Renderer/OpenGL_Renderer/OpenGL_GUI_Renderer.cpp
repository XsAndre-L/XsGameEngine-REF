#include "OpenGL_GUI_Renderer.h"
#include <iostream>
#include <filesystem>
#include <math.h>

OpenGL_GUI_Renderer::OpenGL_GUI_Renderer()
{
}

OpenGL_GUI_Renderer::OpenGL_GUI_Renderer
(
	GLFWwindow &window, 
	bool& ApplyGraphics,/*, 
	GraphicSettings& CurrentGraphicSettings, 
	OpenGL_Assets& assetManager*/
	OpenGL_Assets& AssetManager
)
{
	IgInitInfo.mainWindow = &window;
	
	IgInitInfo.ApplyGraphics = &ApplyGraphics;
	/*IgInitInfo.CurrentGraphicSettings = &CurrentGraphicSettings;*/

	IgInitInfo.AssetManager = &AssetManager;
	
	//init_imGUI_Vulkan();
}


//Create an Instance of ImGUI (Context)
void OpenGL_GUI_Renderer::createImGuiInstance()
{
	// Setup Dear ImGui context
	#pragma region ImGui
	const char* glsl_version = "#version 130";

	static int CheckInstalls = 0;
	if (CheckInstalls < 1) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		
		//ImGui::StyleColorsClassic();
		
			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(IgInitInfo.mainWindow, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
			CheckInstalls++;
	}
	

	#pragma endregion
}

//void ImGuiFontSubmit(VkDevice device, VkCommandPool commandPool, VkQueue queue) {
//
//	// Upload Fonts
//
//		// Use any command queue
//	//vkResetCommandPool(device, commandPool, 0);
//	VkCommandBuffer commandBuffer = beginCommandBuffer(device, commandPool);
//	
//	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
//
//	endAndSubmitCommandBuffer(device, commandPool, queue, commandBuffer);
//
//	//ImGui_ImplVulkan_DestroyFontUploadObjects();
//
//}

//void OpenGL_GUI_Renderer::init_imGUI_Vulkan()
//{
//	//1: create descriptor pool for IMGUI
//	// the size of the pool is very oversize, but it's copied from imgui demo itself.
//	VkDescriptorPoolSize pool_sizes[] =
//	{
//		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
//		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
//		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
//		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
//		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
//		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
//	};
//
//	VkDescriptorPoolCreateInfo pool_info = {};
//	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
//	pool_info.maxSets = 1000;
//	pool_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
//	pool_info.pPoolSizes = pool_sizes;
//
//	VkResult result = vkCreateDescriptorPool(*IgInitInfo.Device, &pool_info, nullptr, &imguiPool);
//
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("could not create descriptor pool for in init_imGUI()!");
//	}
//
//
//	ImGui_ImplVulkan_InitInfo init_info = {};
//	init_info.Instance = *IgInitInfo.instance;
//	init_info.PhysicalDevice = *IgInitInfo.PhysicalDevice;
//	init_info.Device = *IgInitInfo.Device;
//	init_info.Queue = *IgInitInfo.Queue;
//	init_info.QueueFamily = IgInitInfo.GraphicsFamily;
//	//init_info.PipelineCache = pipelineCache;
//	init_info.DescriptorPool = imguiPool;
//	init_info.Allocator = nullptr;
//	init_info.MinImageCount = 3;
//	init_info.ImageCount = 3;
//	//init_info.CheckVkResultFn = 0;
//	init_info.MSAASamples = *IgInitInfo.MsaaSampleCount;
//
//	ImGui_ImplVulkan_Init(&init_info, *IgInitInfo.RenderPass);
//
//	//execute a gpu command to upload imgui font textures
//
//		
//	ImGuiFontSubmit(*IgInitInfo.Device, *IgInitInfo.GraphicsCommandPool, *IgInitInfo.Queue);
//
//	//clear font textures from cpu data
//	ImGui_ImplVulkan_DestroyFontUploadObjects();
//		
//	
//	//add the destroy the imgui created structures
//
//}

//Main Render Menus Function
void OpenGL_GUI_Renderer::RenderMenus(bool* LocalTransform, glm::vec3* position, glm::vec3* rotation, glm::vec3* scale, int* selectedObject, const OpenGL_Assets::AllAssets* AssetList)
{
	//Hide and unhide Content Browser
	if (ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_Space]) && ImGui::GetIO().KeysDown[GLFW_KEY_LEFT_CONTROL]) {
		browserInfo.ShowMenu = !browserInfo.ShowMenu;
	}

	//glfwGetWindowSize(IgInitInfo.mainWindow, &sharedInfo.width, &sharedInfo.height);
	//Set Info
	//sharedInfo
	sharedInfo.selectedObject = selectedObject;
	//sharedInfo.modelNames = AssetList->ModelNames;
	sharedInfo.allAssets = AssetList;

	//menu 'OBJECT DETAILS'
	detailsInfo.LocalTransform = LocalTransform;
	detailsInfo.position = position;
	detailsInfo.rotation = rotation;
	detailsInfo.scale = scale;

	//menu 'WORLD OUTLINER'
	//NON

	//menu 'Content Browser'
	//browserInfo.AssetLoadThread = AssetLoadThread;

	#pragma region ImGUI Render Function

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	//imgui commands
	ImGui::ShowDemoWindow();
	
	mainMenuBar();

	if(outlinerInfo.ShowMenu)
		renderOutlinerMenu();

	if(detailsInfo.ShowMenu)
		renderDetailsMenu();

	if(browserInfo.ShowMenu)
		renderContentMenu();

	if (statsInfo.ShowMenu)
		renderStatsOverlay();

	

	// Rendering
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
	if (!is_minimized)
	{

	}

#pragma endregion
}


//TODO : Function to handle shortcuts for selected window
//void OpenGL_GUI_Renderer::ShortCutKeyInput(){}

#pragma region Main Bar
void OpenGL_GUI_Renderer::ShowFileMenu()
{
	ImGui::MenuItem("(demo menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowFileMenu();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}

	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		static bool b = true;
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}

	if (ImGui::MenuItem("Quit", "Alt+F4")) { glfwSetWindowShouldClose(IgInitInfo.mainWindow, GL_TRUE); }
}

void OpenGL_GUI_Renderer::mainMenuBar() {
	if (ImGui::BeginMainMenuBar())
	{
		//FILE BUTTON
		if (ImGui::BeginMenu("File"))
		{
			ShowFileMenu();
			ImGui::EndMenu();
		}
		//EDIT BUTTON
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		//WINDOWS BUTTON
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Details", "", &detailsInfo.ShowMenu)) {}
			if (ImGui::MenuItem("Outliner", "", &outlinerInfo.ShowMenu)) {}
			if (ImGui::MenuItem("Content Browser", "", &browserInfo.ShowMenu)) {}
			if (ImGui::MenuItem("Stats For Nerds", "", &statsInfo.ShowMenu)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

#pragma endregion

#pragma region Outliner

void OpenGL_GUI_Renderer::ShowOutlinerTree(const char* prefix, int uid)
{
	// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
	ImGui::PushID(uid);
	static int ID = 0;
	// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();

	bool node_open = ImGui::TreeNodeEx(prefix, ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);//"Object", "%s_%u", prefix, uid);

	if (ImGui::IsItemClicked(0) && !node_open) {
		*sharedInfo.selectedObject = uid;
		printf("Select Object");
	}

	if (ImGui::IsAnyMouseDown && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(prefix, NULL, 0);
		ImGui::Text("This is a drag and drop source");
		ImGui::EndDragDropSource();
	}

	//ImGui::OpenPopupOnItemClick("Pop", ImGuiPopupFlags_MouseButtonRight);
	bool options = ImGui::BeginPopupContextItem("Pop", ImGuiPopupFlags_MouseButtonRight);
	
	if(options){
		ImGui::OpenPopup("OptionsPopUp");

		if (ImGui::Button("Delete")) {
			IgInitInfo.AssetManager->destroyOpenGL_MeshModel(uid);
		}
		
		ImGui::EndPopup();
	}
	
	if (node_open)
	{
		std::vector<std::string> ChildNodes = *IgInitInfo.AssetManager->OpenGL_MeshModelList.at(uid).getChildren();
		//static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
		for (int i = 0; i < ChildNodes.size(); i++)
		{
			ImGui::PushID(i); // Use field index as identifier.
			if (i < 20)
			{
				ShowOutlinerTree(ChildNodes.at(i).c_str(), i);
			}
			else
			{
				// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
				ImGui::TableNextRow();

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet ;
				ImGui::TreeNodeEx("Field", flags, "Field_%d", i);
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

//All active rendered meshes
void OpenGL_GUI_Renderer::renderOutlinerMenu()
{

	ImGui::Begin("World Outliner");

	if (ImGui::BeginTable("Outliner Table", 1,  ImGuiTableFlags_Resizable)) //ImGuiTableFlags_BordersOuter |
	{
		std::vector<std::string> namesVector = *sharedInfo.allAssets->ModelNames;
		for (int obj_i = 0; obj_i < sharedInfo.allAssets->ModelNames->size(); obj_i++)
		{
			std::string nameString = namesVector.at(obj_i);
			if (nameString == "") { continue; }
			ShowOutlinerTree(nameString.c_str(), obj_i);
		}
		ImGui::EndTable();
	}

	ImGui::End();

}

#pragma endregion


#pragma region Details Menu
//Entiy Has Transform
//void Entity_Details() 
//{
//
//}

void OpenGL_GUI_Renderer::renderDetailsMenu()
{
	ImGui::Begin("Details");

	//Transform
	if (ImGui::CollapsingHeader("TRANSFORM"))
	{
		ImGui::Checkbox("Local Transform", detailsInfo.LocalTransform);

		ImGui::InputFloat3("Position", &detailsInfo.position->x);						//Current Position
		ImGui::SliderFloat3("Rotation", &detailsInfo.rotation->x, 0.0f, 360.0f);		//Current Rotation
		ImGui::InputFloat3("Scale", &detailsInfo.scale->x);								//Current Scale
	}

	//Material
	if (ImGui::CollapsingHeader("MATERIAL"))
	{
		ImGui::Text("Material Selection Goes Here");
	}


	//Directional Light
	if (ImGui::CollapsingHeader("DIRECTIONAL LIGHT"))
	{
		//ImGui::SliderFloat3(" Direction", &sharedInfo.allAssets->lightPointers->directionalLight[0].lightDirection.x, -1.0f, 1.0f);
		//ImGui::ColorEdit3(" LightColor", &sharedInfo.allAssets->lightPointers->directionalLight[0].base.colour.x);
	}

	//ImGui::Checkbox("Show Another Window", &show_another_window);


	//ImGui::SliderInt("SelectedMesh", counter, 0 ,1);
	//ImGui::InputFloat3("Pos", &detailsInfo.position->x);


	if (ImGui::CollapsingHeader("Atmosphere"))
	{
		ImGui::ColorEdit3("Atmosphere Color", (float*)&clearColor);
	}

	if (ImGui::CollapsingHeader("Graphics"))
	{
		static int MSAA;
		//ImGui::InputInt("MSAA", &IgInitInfo.CurrentGraphicSettings->MSAA , 1, 1);

		if (ImGui::Button("Apply")) {
			/*if (!*IgInitInfo.ApplyGraphics) {
				*IgInitInfo.ApplyGraphics = true;
				 
			}*/
		}
	}


	ImGui::End();

}
#pragma endregion




void OpenGL_GUI_Renderer::renderContentMenu() {
	float ySize = 300.0f;
	ImGui::SetNextWindowSize({ ImGui::GetMainViewport()->Size.x, ySize});
	ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->GetCenter().x - (ImGui::GetMainViewport()->Size.x/2.0f), (ImGui::GetMainViewport()->GetCenter().y + (ImGui::GetMainViewport()->Size.y / 2.0f)) - ySize }, false);
	bool open;
	ImGui::Begin("Content Browser", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	static int FolderIndex = 0;

	if (ImGui::Button("3D models", ImVec2(200.0f, 20.0f)))
	{
		FolderIndex = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Textures", ImVec2(200.0f, 20.0f)))
	{
		FolderIndex = 1;
	}

	if (FolderIndex == 0) 
	{
		ImGui::Text("Program/Models");
		std::string path = "Models";
		for (const auto& entry : std::filesystem::directory_iterator(path)) {

			std::filesystem::path modelPath = entry;
			if (modelPath.extension() == ".obj") {
				std::string nameString = modelPath.filename().string();//namesVector.at(i);

				if (ImGui::Button(nameString.c_str(), ImVec2(100.0f, 100.0f)))
				{
					if (IgInitInfo.AssetManager->OpenGL_MeshModelList.size() < MAX_OBJECTS) {
						IgInitInfo.AssetManager->createAsset(modelPath.string());
					}
				}
				ImGui::SameLine();
			}
		}
	}
	
	if (FolderIndex == 1)
	{
		ImGui::Text("Program/Textures");
		std::string path = "Textures";
		for (const auto& entry : std::filesystem::directory_iterator(path)) {

			std::filesystem::path texPath = entry;
			if (texPath.extension() == ".png" || texPath.extension() == ".jpg") {
				std::string nameString = texPath.filename().string();//namesVector.at(i);

				if (ImGui::Button(nameString.c_str(), ImVec2(100.0f, 100.0f)))
				{
					if (IgInitInfo.AssetManager->textureList.size() < MAX_OBJECTS) {
						printf("Attempting to load %s", nameString.c_str());
						IgInitInfo.AssetManager->addTexture(nameString.c_str());
					}
				}
				ImGui::SameLine();
			}
		}
	}

	ImGui::End();
}

void OpenGL_GUI_Renderer::renderStatsOverlay()
{
	const float PAD = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	
	if (ImGui::Begin("Stats For Nerds", &statsInfo.ShowMenu, window_flags))
	{
		ImGui::Text("(right-click to change position)");
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);

		
			
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (statsInfo.ShowMenu && ImGui::MenuItem("Close")) statsInfo.ShowMenu = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

//Secondly Destroys
void OpenGL_GUI_Renderer::CleanUpGUI()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

//First Destroys
void OpenGL_GUI_Renderer::CleanUpGuiComponents()
{
	ImGui_ImplOpenGL3_Shutdown();
	
}
