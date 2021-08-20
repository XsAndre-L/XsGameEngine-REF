#include "PreprocessorDefs.h"
#include "VulkanRenderer.h"

#include "VkValidation.h"

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
}


#pragma region Public Functions [init, updateModel, draw, cleanUp]
//RECIEVES THE WINDOW AND THE CREATES ALL NEEDED VULKAN COMPONENTS (INSTANCE , Get PHYSICAL DEVICE , LOGICAL DEVICE)
int VulkanRenderer::init_VulkanRenderer(GLFWwindow* newWindow)
{
	window = newWindow;

	try {
		createInstance();			//MUST BE IN THIS SEQUENCE
		createSurface();
		getPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createRenderPass();

		//--Descriptors
		createDescriptorSetLayouts();
		createPushConstantRange();

		createUniformBuffers();
		createDescriptorPool();
		allocateDescriptorSets();
		//---

		createGraphicsPipeline();

		createColorResources();
		createDepthResources();
		createFrameBuffers();

		createCommandPool();
		createCommandBuffers();

		createTextureSampler();

		createSynchronisation();


		#ifdef GUI_LAYER
			GUI = GUI_Renderer(window, &Instance, &mainDevice.logicalDevice, &mainDevice.physicalDevice, &graphicsQueue, indices.graphicsFamily, &renderPass, &graphicsCommandPool, &msaaSamples, &applyGraphics, &CurrentGraphicSettings ,&AssetManager);
		#else
			printf("GUI DISABLED");
		#endif
		
		AssetManager = VkAssets(&mainDevice.physicalDevice, &mainDevice.logicalDevice, &graphicsQueue, &graphicsCommandPool, &textureSampler, &samplerDescriptorPool, &samplerSetLayout);
		
	#pragma region Program


		uboViewProjection.projection = glm::perspective(glm::radians(45.0f), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.01f, 100.0f);
		//uboViewProjection.projection = glm::ortho(0.0f, (float)swapChainExtent.width, (float)swapChainExtent.height, 0.0f, 0.0f, 100.0f);
		//.model = glm::mat4(1.0f);
		uboViewProjection.projection[1][1] *= -1;

		//Vertex Data		Cube
		/*std::vector<Vertex> meshVertices = {
			{{0.5, -0.5, 0.0}, {1.0f, 0.0f, 0.0f }},	//0
			{{0.5, 0.5, 0.0}, {1.0f, 0.0f, 0.0f }},		//1
			{{-0.5, 0.5, 0.0}, {1.0f, 0.0f, 0.0f }},	//2
			{{-0.5, -0.5, 0.0}, {0.0f, 0.0f, 1.0f }},	//3

			//{{0.5, -0.5, 0.0}, {1.0f, 0.0f, 0.0f }},	//0
			{{0.5, -0.5, -1.0}, {0.0f, 1.0f, 0.0f }},	//4
			{{0.5, 0.5, -1.0}, {0.0f, 1.0f, 0.0f }},	//5
			//{{0.5, 0.5, 0.0}, {1.0f, 0.0f, 0.0f }}	//1

			{{-0.5, -0.5, -1.0}, {0.0f, 1.0f, 0.0f }},	//6

			{ {-0.5, 0.5, -1.0}, {0.0f, 1.0f, 0.0f }}
			//{{-0.5, -0.5, 0.0}, {1.0f, 0.0f, 0.0f }}


			//{{-0.1, -0.4, 0.0}, {1.0f, 0.0f, 0.0f }},	//0
			//{{-0.1, 0.4, 0.0}, {0.0f, 1.0f, 0.0f }},	//1
			//{{-0.9, 0.4, 0.0}, {0.0f, 0.0f, 1.0f }},	//2
			//{{-0.9, -0.4, 0.0}, {0.5f, 0.5f, 0.0f }},	//3
		};

		//sirkle
		std::vector<Vertex> meshVertices = {
			{{0.0, 0.0, 0.0}, {0.5f, 0.0f, 0.5f }},	//0
			{{0.0, 0.5, 0.0}, {0.0f, 0.5f, 0.0f }},		//1
			{{-0.35, 0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//2
			{{-0.5, 0.0, 0.0}, {0.0f, 0.5f, 0.0f }},	//3


			{{-0.35, -0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//4
			{{0.0, -0.5, 0.0}, {0.0f, 0.5f, 0.0f }},		//5
			{ {0.35, -0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//6
			{{0.5, 0.0, 0.0}, {0.0f, 0.5f, 0.0f }},
			{{0.35, 0.35, 0.0}, {0.0f, 0.5f, 0.0f }}
		};
		std::vector<Vertex> meshVertices2 = {
			{{0.0, 0.0, 0.0}, {0.5f, 1.0f, 0.5f }},	//0
			{{0.0, 0.5, 0.0}, {0.0f, 0.5f, 0.0f }},		//1
			{{-0.35, 0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//2
			{{-0.5, 0.0, 0.0}, {0.0f, 0.5f, 0.0f }},	//3


			{{-0.35, -0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//4
			{{0.0, -0.5, 0.0}, {0.0f, 0.5f, 0.0f }},		//5
			{ {0.35, -0.35, 0.0}, {0.0f, 0.5f, 0.0f }},	//6
			{{0.5, 0.0, 0.0}, {0.0f, 0.5f, 1.0f }},
			{{0.35, 0.35, 0.0}, {0.0f, 0.5f, 0.0f }}
		};

		std::vector<Vertex> meshVertices3 = {
			{{0.9, -0.4, 0.0}, {1.0f, 0.0f, 0.0f } , {1.0f, 1.0f}},	//0
			{{0.9, 0.4, 0.0}, {0.0f, 1.0f, 0.0f }, {1.0f, 0.0f}},		//1
			{{0.1, 0.4, 0.0}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f}},		//2
			{{0.1, -0.4, 0.0}, {0.5f, 0.5f, 0.0f }, {0.0f, 1.0f}},	//3
		};*/

		std::vector<uint32_t> meshIndices = {
			0,1,2,
			2,3,0,
			0,3,4,
			0,4,5,
			0,5,6,
			0,6,7,
			0,7,8,
			0,8,1

			/*0,1,2,
			2,3,0,
			0,4,5,
			5,1,0,
			3,6,0,
			0,6,4,
			2,7,3,
			3,7,6*/

			/*0,1,2,
			2,3,0*/
		};
		std::vector<uint32_t> meshIndices2 = {
			
			0,1,2,
			2,3,0
		};

		AssetManager.addTexture("plain.png");
		AssetManager.createAsset("Models/plane.obj");
		while (AssetManager.meshModelList.size() <= 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		//AssetManager.createMeshModel("Models/plane.obj");

	#pragma endregion

	}
	catch (const std::runtime_error& e) {
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}

	return 0;
}


//THIS IS USED TO DRAW TO THE SCREEN
void VulkanRenderer::draw()
{

	#pragma region Get Available Image
	//1. Get next available image to draw too, and set something to signal when we're finished with the image.
	// 
	//Wait for given fence to signal (open) from last draw before continuing
	vkWaitForFences(mainDevice.logicalDevice, 1, &drawFences[currentFrame], VK_TRUE, std::numeric_limits<uint32_t>::max()); //std::numeric_limits<uint32_t>::max()
	//Manually close fences
	vkResetFences(mainDevice.logicalDevice, 1, &drawFences[currentFrame]);

	if (applyGraphics) {
		applyGraphics = !applyGraphics;
		printf("APPLY");
		setCurrentSampleCount(CurrentGraphicSettings.MSAA);
	}
	// Get Next Image
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(mainDevice.logicalDevice, swapchain, std::numeric_limits<uint64_t>::max(), imageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex); //std::numeric_limits<uint64_t>::max()
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		printf("recreate");
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	#pragma endregion

	recordCommands(imageIndex);
	updateUniformBuffer(imageIndex);

	#pragma region Submit Command Buffer To Queue
	//2. Submit command buffer to queue for execution, making sure it waits for the image to be signalled as available before drawing
	// Submit Command Buffer To Render
	//Queue Submission info
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailable[currentFrame];
	VkPipelineStageFlags waitStages[] = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT , VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT
	};
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinished[currentFrame];

	//vkGetEventStatus(mainDevice.logicalDevice,);
	result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[currentFrame]);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit the queue!");
	}
	#pragma endregion

	#pragma region Present Image
	//3. Present image too screen when it has signalled finished rendering
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinished[currentFrame];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentationQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	#pragma endregion

	
	//Get next frame (use MAX_FRAME_DRAWS to keep value below MAX_FRAME_DRAWS)
	currentFrame = (currentFrame + 1) % MAX_FRAME_DRAWS;
}

//WHEN PROCRAM STOPS ALL CREATED INSTANCES AND DEVICES ARE DELETED
void VulkanRenderer::cleanUp()
{
	vkDeviceWaitIdle(mainDevice.logicalDevice);

	//Clean Up GUI
	#ifdef GUI_LAYER
		GUI.CleanUpGUI(mainDevice.logicalDevice);
	#else
		printf("GUI DISABLED");
	#endif	
	
	
	/*for (size_t i = 0; i < meshModelList.size(); i++)
	{
		meshModelList[i].destroyModel();
	}*/

	vkDestroyDescriptorPool(mainDevice.logicalDevice, samplerDescriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(mainDevice.logicalDevice, samplerSetLayout, nullptr);

	vkDestroySampler(mainDevice.logicalDevice, textureSampler, nullptr);

	//Texture Images & DestroyModels
	AssetManager.cleanUpAssets();
	/*for (size_t i = 0; i < textureImages.size(); i++)
	{
		vkDestroyImageView(mainDevice.logicalDevice, textureImageViews[i], nullptr);
		vkDestroyImage(mainDevice.logicalDevice, textureImages[i], nullptr);
		vkFreeMemory(mainDevice.logicalDevice, textureImageMemory[i], nullptr);
	}*/

	//Depth buffer Image
	

	vkDestroyDescriptorPool(mainDevice.logicalDevice, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(mainDevice.logicalDevice, descriptorSetLayout, nullptr);
	for (size_t i = 0; i < uniformBuffer.size(); i++)
	{
		vkDestroyBuffer(mainDevice.logicalDevice, uniformBuffer[i], nullptr);
		vkFreeMemory(mainDevice.logicalDevice, uniformBufferMemory[i], nullptr);
	}
	/*for (size_t i = 0; i < meshList.size(); i++)
	{
		meshList[i].destroyBuffers();
	}*/
	
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		vkDestroySemaphore(mainDevice.logicalDevice, renderFinished[i], nullptr);
		vkDestroySemaphore(mainDevice.logicalDevice, imageAvailable[i], nullptr);
		vkDestroyFence(mainDevice.logicalDevice, drawFences[i], nullptr);
	}

	cleanUpSwapChain();
	vkDestroyCommandPool(mainDevice.logicalDevice, graphicsCommandPool, nullptr);

	vkDestroySurfaceKHR(Instance, Surface, nullptr);						//DESTROY SURFACE
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);						//DESTROY LOGICAL DEVICE
	vkDestroyInstance(Instance, nullptr);									//DESTROY VULKAN INSTANCE

}

#pragma endregion

#pragma region Instance [createInstance, checkInstanceExtensionSupport, createSurface]

//CREATE A NEW INSTANCE
void VulkanRenderer::createInstance()
{
	#pragma region Validation Layers
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
	#pragma endregion

	
	#pragma region App Info	

	//INFO ABOUT THE APPLICATION ITSELF
	//MOST DATA HERE DOESN'T AFFECT THE PROGRAM AND IS FOR DEVELOPER CONVENIENCE
	VkApplicationInfo appInfo {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VULKAN APP";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "NO ENGINE";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	#pragma endregion

	#pragma region Instance Create Info	- [AppInfo, checkInstanceExtensionSupport]
	
	//CREATION INFORMATION FOR A VULKAN INSTANCE
	VkInstanceCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;						//	-- APP INFO

	#pragma region Get GLFW Extensions

	//Create list to hold instance extensions
	std::vector<const char*> instanceExtensions = std::vector<const char*>();
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	//GET GLFW EXTENSIONS
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//ADD GLFW EXTENSIONS TO LIST OF EXTENSTIONS
	for (size_t i = 0; i < glfwExtensionCount; i++) {
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	//CHECK IF INSTANCE EXTENSIONS SUPPORTED...
	if (!checkInstanceExtensionsSupport(&instanceExtensions)) {
		throw std::runtime_error("VkInstance does not support required extensions!");
	}

	#pragma endregion

	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}
	
	#pragma endregion

	#pragma region Create Instance - [CreateInfo] [&instance]

	//CREATE INSTANCE
	VkResult result = vkCreateInstance(&createInfo, nullptr, &Instance);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("FAILED TO CREATE A VULKAN INSTANCE");
	}

	#pragma endregion
}

//CHECK IF ALL INSTANCE EXTENSIONS ARE SUPPORTED
bool VulkanRenderer::checkInstanceExtensionsSupport(std::vector<const char*>* checkExtensions)
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	//CREATE LIST OF VK PROPERTIES USING COUNT
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	//CHECK IF GIVEN EXTENSIONS ARE IN LIST OF AVAILABLE EXTENSIONS
	for (const auto& checkExtension : *checkExtensions)
	{
		bool hasExtension = false;

		for (const auto& extension : extensions) {
			if (strcmp(checkExtension, extension.extensionName)) {
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension) {
			return false;
		}
	}
	return true;
}

//CREATE SURFUCE ON WHICH WE CAN RENDER
void VulkanRenderer::createSurface()
{
	//creating a surface (returns result)
	VkResult result = glfwCreateWindowSurface(Instance, window, nullptr, &Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface");
	}
}

#pragma endregion

#pragma region PhysicalDevice [getPhysicalDevice, checkDeviceSuitable, checkDeviceExtentionSupport, getQueueFamilies, getMaxUsableSampleCount]
//GET GPU BY ENUMARATING THREW ALL GPUs
void VulkanRenderer::getPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance, &deviceCount, nullptr);

	//IF NO DEVICES AVAILABLE, THEN NONE SUPPORT FOR VULKAN
	if (deviceCount == 0) {
		throw std::runtime_error("Can't find any GPUs that support Vulkan");
	}

	//GET LIST OF PHYSICAL DEVICES
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(Instance, &deviceCount, deviceList.data());

	for (const auto& device : deviceList)
	{
		if (checkDeviceSuitable(device)) {
			mainDevice.physicalDevice = device;
			maxMsaaSamples = getMaxUsableSampleCount();					//Gets MSAA
			break;
		}
	}
}

//CHECK IF THE GPU DEVICE IS SUITABLE FOR VULKAN
bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	//INFO about the device itself (ID , name , type , vendor, ect)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	*/
	//Info about what the device can do (geomentry shader , tesselation shader, wide lines ,ect)
	#pragma region Validate Features

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	#pragma endregion

	
	
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	indices = getQueueFamilies(device);

	bool swapChainValid = false;
	if (extensionsSupported)
	{
		SwapChainDetails swapChainDetails = getSwapChainDetails(device);
		swapChainValid = !swapChainDetails.formats.empty() && !swapChainDetails.presentationModes.empty();
	}

	return indices.isValid() && extensionsSupported && swapChainValid && deviceFeatures.samplerAnisotropy;
}

//CHECK IF ALL DEVICE EXTENSIONS ARE SUPPORTED
bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	#pragma region Get Available Extensions 
	uint32_t extensionCount = 0;														// Extention Count
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);	// Get extention Count

	//IF NO EXTENSIONS FOUND RETURN FAILURE
	if (extensionCount == 0) {
		return false;
	}

	std::vector<VkExtensionProperties> extensions(extensionCount);								// Make vector with size of extention count
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());	// Put all available extentions inside The vector
	#pragma endregion

	#pragma region Check if Required Extentions Exist inside Available Extentions [Required extensions in Utillities]
	//Check for extension -- Extension that supports the surface
	for (const auto& deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions) {
			if (strcmp(deviceExtension, extension.extensionName)) {
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension)
		{
			return false;
		}
	}

	return true;
	#pragma endregion
}

//CHECKING HOW MANY QUEUES DOES THE DEVICE SUPPORT
QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector <VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	//GO THREW EACH QUEUE FAMILY and check if has at least 1 of the required types of queue
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		//First Check if queue family has at least one queue in the family
		//Queue can be multiple types defined through bitfield. Need to bitwise AND with VK_QUEUE_*_BIT to check if has required
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		//Check if queue family supports presentation
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface, &presentationSupport);
		//Check if Queue is presentation type
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		//check if queue family indices are in a valid state, stop searching if so
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}

//MSAA FUNCTION
VkSampleCountFlagBits VulkanRenderer::getMaxUsableSampleCount() {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(mainDevice.physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
#pragma endregion

#pragma region LogicalDevice [createLogicalDevice]
//CREATE LOGICAL DEVICE TO INTERFACE WITH HARDWARE
void VulkanRenderer::createLogicalDevice()
{
	//GET THE QUEUE FAMILIES FOR THE CHOSEN PHYSICAL DEVICE

	//Vector for queue creation info,and set for family indeces
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilyIndices = { indices.graphicsFamily , indices.presentationFamily };

	#pragma region QueueInfos
	//Queue the Logical Device Needs to Create and info to do so (only 1 for now)
	float priority = 1.0f;
	for (const auto queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &priority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	#pragma endregion

	#pragma region Logical Device Create Info - [QueueCreateInfos]
	//-INFO TO CREATE LOGICAL DEVICE (SOMETIMES CALLED "DEVICE")
	//--PHYSICAL DEVICE FEATURES THE LOGICAL DEVICE WILL USE
	VkPhysicalDeviceFeatures deviceFeatures = {};		
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	#pragma endregion

	#pragma region Create Logical Device - [physicalDevice, deviceCreateInfo, &logicalDevice]
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);  //Creating the logical device

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Logical Device could not be created");
	}
	#pragma endregion

	//Queues are created at the same time as the device
	//to make handle to the queue
	//From given logical device, of given queue family , of given queue index (0 since only one queue), place reference in given vkDeviceQueue
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}
#pragma endregion

#pragma region Swapchain [createSwapChain, getSwapChainDetails, chooseBestSurfaceFormats, chooseBestPresentationMode, chooseSwapExtent, createImageView]
//SwapChain is the info for Iteration between frames, also creates the images and image views
void VulkanRenderer::createSwapChain()
{
	#pragma region Swapchain Checks [SwapchainDetails, BestSurfaceFormats, presentMode, Extent]
	//Get Swap CHain details so we can pick best settings
	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);

	//1. CHOOSE BEST SURFACE FORMAT
	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormats(swapChainDetails.formats);
	//2. CHOOSE BEST PRESENTATION MODE
	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);
	//3. CHOOSE SWAP CHAIN IMAGE RESOLUTION
	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);
	#pragma endregion

	#pragma region MinImage Count
	//HOW MANY IMAGES ARE IN THE SWAP CHAIN? GET ! MORE THAN THE MINIMUM TO ALLOW TRIPLE BUFFERING
	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;
	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapChainDetails.surfaceCapabilities.maxImageCount)
	{
		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;
	}
	#pragma endregion

	#pragma region Swapchain CreateInfo
	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = Surface;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.minImageCount = imageCount;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.clipped = VK_TRUE;
	
	if (indices.graphicsFamily != indices.presentationFamily)
	{
		uint32_t queueFamilyIndices[] = {
			(uint32_t)indices.graphicsFamily,
			(uint32_t)indices.presentationFamily
		};

		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}
	
	
	//IF OLD SWAP CHAIN BEING DESTROYED THIS ONE REPLACES IT , THEN LINK OLD ONE TO HAND OVER RESPONSIBILLITIES
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	
	
	#pragma endregion

	#pragma region Create Swapchain
	
	//CREATE SWAP CHAIN
	if (vkCreateSwapchainKHR(mainDevice.logicalDevice, &swapChainCreateInfo, nullptr, &swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("Swapchain Could Not Be Created!");
	}

	#pragma endregion

	//STORE REFERENCES
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	#pragma region SwapChain Images	[Images and ImageViews]
	uint32_t swapChainImageCount;
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, nullptr);

	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, images.data());

	swapChainImages.clear();
	for (VkImage image : images)
	{
		SwapChainImage swapChainImage = {};
		swapChainImage.image = image;
		swapChainImage.imageView = createImageView(mainDevice.logicalDevice, image, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);	//CREATE IMAGE VIEW

		swapChainImages.push_back(swapChainImage);		//Store Reference
	}
	#pragma endregion
}

//GETTING THE SUPPORTED (CAPABILLITIES, FORMATS, PRESENTATION MODE)
SwapChainDetails VulkanRenderer::getSwapChainDetails(VkPhysicalDevice device)
{
	SwapChainDetails swapChainDetails;

	#pragma region Capabillities
	// --Capabillities--
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &swapChainDetails.surfaceCapabilities);
	#pragma endregion

	#pragma region Formats
	// --Formats--
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, nullptr);

	if (formatCount > 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &formatCount, swapChainDetails.formats.data());
	}
	#pragma endregion

	#pragma region Presentation Modes
	//--PresentationModes--
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentationCount, nullptr);

	if (presentationCount > 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &presentationCount, swapChainDetails.presentationModes.data());
	}
	#pragma endregion

	return swapChainDetails;
}

//	-- CHOOSE THE BEST SURFACE FORMAT
//Format		: VK_FORMAT_R8G8A8_UNORM  || Backup VK_FORMAT_B8G8R8A8_UNORM
//colorSpace	: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormats(const std::vector<VkSurfaceFormatKHR>& formats)
{
	//UNDEFINED MEANS ALL FORMATS ARE AVAILABLE
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_R8G8B8_UNORM , VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	//ELSE ITERATE AND LOOK FOR SPISIFIC FORMATS
	for (const auto& format : formats)
	{
		if ((format.format == VK_FORMAT_R8G8B8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	//ELSE RETURN THE FIRST FORMAT AVAILABLE
	return formats[0];
}

//	-- PRESENTATION MODE
VkPresentModeKHR VulkanRenderer::chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes)
{
	//LOOK FOR MAILBOX PRESENTATION MODE [Optimal]
	for (const auto& presentMode : presentationModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentMode;
		}
	}
	//BACKUP PRESENT MODE FIFO (first in first out)
	return VK_PRESENT_MODE_FIFO_KHR;
}

//	-- EXTENT SIZE
VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surfaceCapabilities.currentExtent;
	}
	else {
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D newExtent = {};
		newExtent.width = static_cast<uint32_t>(width);
		newExtent.height = static_cast<uint32_t>(height);

		//MAKING SURE THE EXTENT IS NOT BIGGER OF SMALLER THAT THE MAX OR MIN IMAGEEXTENT SIZE
		newExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));
		newExtent.height = std::max(surfaceCapabilities.maxImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));

		return newExtent;
	}
}


#pragma endregion

#pragma region RenderPass [createRenderPass]
void VulkanRenderer::createRenderPass()
{
	#pragma region Attatchment Descriptions and References
	//Attachment Descriptions
	//Color Attachment
	VkAttachmentDescription colorAttatchment = {};
	colorAttatchment.format = swapChainImageFormat;
	colorAttatchment.samples = msaaSamples;				//MSAA
	colorAttatchment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttatchment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttatchment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttatchment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttatchment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	if (msaaSamples != VK_SAMPLE_COUNT_1_BIT) {
		colorAttatchment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
	else {
		colorAttatchment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
	

	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = swapChainImageFormat;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	depthImageFormat = chooseSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	//Depth Attachment
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = depthImageFormat;
	depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	
	std::array<VkAttachmentDescription, 3> renderPassAttachmentsMulti = { colorAttatchment, depthAttachment, colorAttachmentResolve };
	std::array<VkAttachmentDescription, 2> renderPassAttachmentsNoM = { colorAttatchment, depthAttachment};

	//Attachment References
	//Color Attachment Reference
	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout =  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Depth Attachment Reference
	VkAttachmentReference depthAttachmentReference = {};
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//Color Attachment Resolve Reference
	VkAttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	#pragma endregion

	#pragma region Subpass Descriptions
	//Subpass Creation
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentReference;
	if (msaaSamples != VK_SAMPLE_COUNT_1_BIT) {
		subpass.pResolveAttachments = &colorAttachmentResolveRef;
	}
	subpass.pDepthStencilAttachment = &depthAttachmentReference;
	#pragma endregion

	#pragma region Subpass Dependencies
	//Need to determine when layout transisions occur using subpass dependencies
	std::array<VkSubpassDependency, 2> subpassDependencies;
	//Conversion from " VK_IMAGE_LAYOUT_UNDEFINED " to " VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL "
	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;

	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[0].dependencyFlags = 0;

	//Conversion from " VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL " to " VK_IMAGE_LAYOUT_PRESENT_SRC_KHR "
	subpassDependencies[1].srcSubpass = 0;
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[1].dependencyFlags = 0;
	#pragma endregion


	#pragma region Create RenderPass
	//Creation Of Render Pass Info
	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	if (msaaSamples != VK_SAMPLE_COUNT_1_BIT) {
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(renderPassAttachmentsMulti.size());
		renderPassCreateInfo.pAttachments = renderPassAttachmentsMulti.data();
	}
	else {
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(renderPassAttachmentsNoM.size());
		renderPassCreateInfo.pAttachments = renderPassAttachmentsNoM.data();
	}
	
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
	renderPassCreateInfo.pDependencies = subpassDependencies.data();

	

	VkResult result = vkCreateRenderPass(mainDevice.logicalDevice, &renderPassCreateInfo, nullptr, &renderPass);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Render Pass Could not be created!");
	}
	#pragma endregion

}

#pragma endregion


#pragma region Descriptor Set Layouts & Push Constant Ranges
//Needs to happen Before Graphics Pipeline is created
void VulkanRenderer::createDescriptorSetLayouts()
{
	#pragma region Uniform Values Descriptor Set Layout
	// UNIFORM VALUES DESCRIPTOR SET LAYOUT
	// How data is binded too shader
	VkDescriptorSetLayoutBinding vpLayoutBinding = {};
	vpLayoutBinding.binding = 0;
	vpLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vpLayoutBinding.descriptorCount = 1;
	vpLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	vpLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding lightLayoutBinding = {};
	lightLayoutBinding.binding = 1;
	lightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightLayoutBinding.descriptorCount = 1;
	lightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	lightLayoutBinding.pImmutableSamplers = nullptr;

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings = { vpLayoutBinding , lightLayoutBinding};

	// Create Descriptor Set Layout with given bindings
	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutCreateInfo.pBindings = layoutBindings.data();

	if (vkCreateDescriptorSetLayout(mainDevice.logicalDevice, &layoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Descriptor Set Layout");
	}
	#pragma endregion

	#pragma region Sampler Descriptor Set Layout
	// CREATE TEXTURE SAMPLER DESCRIPTOR SET LAYOUT
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo textureLayoutCreateInfo = {};
	textureLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	textureLayoutCreateInfo.bindingCount = 1;
	textureLayoutCreateInfo.pBindings = &samplerLayoutBinding;

	//Create Descriptor Set Layout
	if (vkCreateDescriptorSetLayout(mainDevice.logicalDevice, &textureLayoutCreateInfo, nullptr, &samplerSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create SamplerSetLayout");
	}
	#pragma endregion

}

void VulkanRenderer::createPushConstantRange()
{
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(Matrix);
}
#pragma endregion

#pragma region Descriptor Set Allocation [createUniformBuffers(), createDescriptorPool(), allocateDescriptorSets()]
//UniformBuffers are buffers in memory that is made accesible in read only format to shaders, so the shaders can read constant parameter data
void VulkanRenderer::createUniformBuffers()					//Need to bind Descriptor Sets with Uniform Buffer
{
	#pragma region Create Uniform Buffer for Every swapChainImage
	//Buffer size will be size of all Uniform Buffer Object variables
	VkDeviceSize bufferSize = sizeof(UboViewProjection) + sizeof(VkAssets::AllLights);

	// One uniform buffer for each image
	uniformBuffer.resize(swapChainImages.size());
	uniformBufferMemory.resize(swapChainImages.size());

	// Create Uniform buffers
	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		createBuffer(mainDevice.physicalDevice, mainDevice.logicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &uniformBuffer[i], &uniformBufferMemory[i]);
	}
	#pragma endregion
}

//Descriptor Pools are used to allocate Descriptor sets from
void VulkanRenderer::createDescriptorPool()					//Allocated from pool so we Create a Pool
{
	#pragma region Create Decriptor Pool to Allocate Descriptor Sets from

	VkDescriptorPoolSize vpPoolSize = {};
	vpPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vpPoolSize.descriptorCount = static_cast<uint32_t>(uniformBuffer.size());

	VkDescriptorPoolSize lightPoolSize = {};										//EDIT
	lightPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	lightPoolSize.descriptorCount = static_cast<uint32_t>(uniformBuffer.size());

	std::vector<VkDescriptorPoolSize> descriptorPoolSizes = { vpPoolSize , lightPoolSize};

	VkDescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.maxSets = static_cast<uint32_t>(uniformBuffer.size());
	poolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
	poolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

	if (vkCreateDescriptorPool(mainDevice.logicalDevice, &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Descriptor Pool");
	}

	#pragma endregion

	#pragma region Sampler Descriptor Pool

	//Texture sampler pool
	VkDescriptorPoolSize samplerPoolSize = {};
	samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerPoolSize.descriptorCount = MAX_OBJECTS;

	std::vector<VkDescriptorPoolSize> descriptorPoolSizes2 = { samplerPoolSize };

	VkDescriptorPoolCreateInfo samplerPoolCreateInfo = {};
	samplerPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	samplerPoolCreateInfo.maxSets = MAX_OBJECTS;
	samplerPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes2.size());
	samplerPoolCreateInfo.pPoolSizes = descriptorPoolSizes2.data();


	if (vkCreateDescriptorPool(mainDevice.logicalDevice, &samplerPoolCreateInfo, nullptr, &samplerDescriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Sampler Descriptor Pool");
	}

	#pragma endregion

}

//Allocates DescriptorSets from DescriptorPool and binds the Sets to the UniformBuffers
void VulkanRenderer::allocateDescriptorSets()				//Allocate Descriptor Sets and then Bind them with Uniform Buffer
{
	#pragma region Allocate Descriptor Sets
	//Resize Descriptor Set list so one for every buffer
	descriptorSets.resize(uniformBuffer.size());

	std::vector<VkDescriptorSetLayout> setLayouts(uniformBuffer.size(), descriptorSetLayout);

	#pragma region Allocation Info

	// Descriptor Set Allocate info
	VkDescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = descriptorPool;
	setAllocInfo.descriptorSetCount = static_cast<uint32_t>(uniformBuffer.size());
	setAllocInfo.pSetLayouts = setLayouts.data();

	#pragma endregion

	VkResult result = vkAllocateDescriptorSets(mainDevice.logicalDevice, &setAllocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Could not Allocate Descriptor Sets!");
	}
	#pragma endregion

	#pragma region Bind Buffers to Descriptor Sets
	for (size_t i = 0; i < uniformBuffer.size(); i++)
	{
		//Buffer info and data offset
		VkDescriptorBufferInfo vpBufferInfo = {};
		vpBufferInfo.buffer = uniformBuffer[i];
		vpBufferInfo.offset = 0;
		vpBufferInfo.range = sizeof(UboViewProjection);


		//Data about connection between binding and buffer

		VkWriteDescriptorSet vpSetWrite = {};
		vpSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vpSetWrite.dstSet = descriptorSets[i];
		vpSetWrite.dstBinding = 0;
		vpSetWrite.dstArrayElement = 0;
		vpSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vpSetWrite.descriptorCount = 1;
		vpSetWrite.pBufferInfo = &vpBufferInfo;


		VkDescriptorBufferInfo lightBufferInfo = {};
		lightBufferInfo.buffer = uniformBuffer[i];
		lightBufferInfo.offset = sizeof(UboViewProjection);
		lightBufferInfo.range = sizeof(VkAssets::AllLights);


		//Data about connection between binding and buffer

		VkWriteDescriptorSet lightSetWrite = {};
		lightSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		lightSetWrite.dstSet = descriptorSets[i];
		lightSetWrite.dstBinding = 1;
		lightSetWrite.dstArrayElement = 0;
		lightSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		lightSetWrite.descriptorCount = 1;
		lightSetWrite.pBufferInfo = &lightBufferInfo;

		std::vector<VkWriteDescriptorSet> setWrites = { vpSetWrite , lightSetWrite};

		vkUpdateDescriptorSets(mainDevice.logicalDevice, static_cast<uint32_t>(setWrites.size()), setWrites.data(), 0, nullptr);

	}
	#pragma endregion
}
#pragma endregion


//Reads Shaders and then sets up The Pipeline
#pragma region Graphics Pipeline [createGraphicsPipeline(), createShaderModule()]

void VulkanRenderer::createGraphicsPipeline()
{
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

	#pragma region Vertex Shader Create Info
	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderCreateInfo.module = vertexShaderModule;
	vertexShaderCreateInfo.pName = "main";
	#pragma endregion

	#pragma region Fragment Shader Create Info
	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderCreateInfo.module = fragmentShaderModule;
	fragmentShaderCreateInfo.pName = "main";
	#pragma endregion

	//Put Shader stage creation info in to array
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo , fragmentShaderCreateInfo };

	//POS COL TEX
	#pragma region Vertex Descriptions And Attributes
	//How the data for a single vertex (including info such as position, color, texture coords, normals, ect) is as a whole
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0; 
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	//How the data for an attribute is defined within a vertex
	std::array<VkVertexInputAttributeDescription, 4> attributeDescription;
	//Position Attribute
	attributeDescription[0].binding = 0; 
	attributeDescription[0].location = 0;
	attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[0].offset = offsetof(Vertex, pos);

	//Color Attribute
	attributeDescription[1].binding = 0;
	attributeDescription[1].location = 1;
	attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[1].offset = offsetof(Vertex, col);

	//Texture Attribute
	attributeDescription[2].binding = 0;
	attributeDescription[2].location = 2;
	attributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescription[2].offset = offsetof(Vertex, tex);

	//Normal Attribute
	attributeDescription[3].binding = 0;
	attributeDescription[3].location = 3;
	attributeDescription[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[3].offset = offsetof(Vertex, norm);

	#pragma endregion


	#pragma region Vertex Input
	// -- Vertex Input --
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
	vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescription.data();
	#pragma endregion

	#pragma region Input Assembly (TOPOLOGY TYPE)
	//	-- Input Assembly --
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //TRIANGLE
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	#pragma endregion

	#pragma region ViewPort State Creation
	// -- ViewPort & Scissor --
	//Create Viewport Info struct
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	//Create Scissor Info struct
	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;
	#pragma endregion			//ViewPort & Scissor


	#pragma region Dynamic States
	// -- Dynamic States --
	/*std::vector<VkDynamicState> dynamicStateEnables;
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	//Dynamic Viewport : Can resize in command buffer with vkCmdSetViewport(commandBuffer , 0 , 1 ,&viewport);
	dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	//Dynamic Scissor : Can resize command buffer with vkCmdSetScissor(commandBuffer , 0 , 1 , &scissor);

	////Also resize swapChainExtent (destroy current swapchain and pass settings)

	//Dynamic State Creation Info
	VkPipelineDynamicStateCreateInfo  dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();*/
	#pragma endregion


	#pragma region Rasterizer
	//	-- Rasterizer --
	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
	rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;											//Everything beyond the far plane can be rendered as a flat image onto the far plane
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerCreateInfo.lineWidth = 1.0f;
	rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
	#pragma endregion

	#pragma region Multisampling
	//	-- Multisampling --
	VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
	multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
	//if (msaaSamples != VK_SAMPLE_COUNT_1_BIT) {
	multisamplingCreateInfo.rasterizationSamples = msaaSamples;
	//}
	#pragma endregion

	#pragma region Blending
	//	-- Blending --
	//Blending decides how to blend a new colour being written to a fragment, with the old value
	//Blend Attatchment State (how blending is handled)
	VkPipelineColorBlendAttachmentState colorState = {};
	colorState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorState.blendEnable = VK_TRUE;
	//blending uses equation : (srcColorBlendFactor * new color) colorBlendOp (dstColorBlendFactor * old color)
	colorState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorState.colorBlendOp = VK_BLEND_OP_ADD;
	//Summarised	(VK_BLEND_FACTOR_SRC_ALPHA * new color) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old color)
	//				(new color alpha * new color) + ((1 - new color alpha) * old color)
	colorState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorState.alphaBlendOp = VK_BLEND_OP_ADD;
	//Summarised :	(1 * new alpha) + (0 * old alpha) = new alpha

	VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo = {};
	colorBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendingCreateInfo.attachmentCount = 1;
	colorBlendingCreateInfo.pAttachments = &colorState;
	#pragma endregion						//New Colors are Blended over Old colors

	#pragma region Create Pipline Layout
	//	-- Pipeline Layouts --
	std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = {descriptorSetLayout , samplerSetLayout};

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	//Create PipelineLayout
	VkResult result = vkCreatePipelineLayout(mainDevice.logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Pipeline Layout!");
	}
	#pragma endregion

	#pragma region Depth Stencil
	//DEPTH STENCIL TESTING
	VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo = {};
	depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilCreateInfo.depthTestEnable = VK_TRUE;
	depthStencilCreateInfo.depthWriteEnable = VK_TRUE;
	depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilCreateInfo.stencilTestEnable = VK_FALSE;
	#pragma endregion

	
	#pragma region Create Graphics Pipeline
	//Graphics PipelineCreation
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = 2;
	pipelineCreateInfo.pStages = shaderStages;
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pDynamicState = nullptr;
	pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	pipelineCreateInfo.pColorBlendState = &colorBlendingCreateInfo;
	pipelineCreateInfo.pDepthStencilState = &depthStencilCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.subpass = 0;

	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;

	//Create graphics Pipeline
	result = vkCreateGraphicsPipelines(mainDevice.logicalDevice, pipelineCache, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Graphics Pipeline Could not be created!");
	}
	#pragma endregion

	#pragma region Destroy Shader Modules - vertex & fragment
	//Destroy shader modules when no longer needed
	vkDestroyShaderModule(mainDevice.logicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(mainDevice.logicalDevice, vertexShaderModule, nullptr);

	#pragma endregion
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());			//reinterpret_cast - treat data as type it is casted to

	VkShaderModule shaderModule = nullptr;
	VkResult result = vkCreateShaderModule(mainDevice.logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Shader Module!");
	}

	return shaderModule;
}

#pragma endregion


#pragma region Images [createDepthResources(), chooseSupportedFormat()]
void VulkanRenderer::createColorResources() {
	VkFormat colorFormat = swapChainImageFormat;

	colorImage = createImage(mainDevice.physicalDevice, mainDevice.logicalDevice, swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &colorImageMemory);
	colorImageView = createImageView(mainDevice.logicalDevice, colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}
void VulkanRenderer::createDepthResources()
{
	//ChooseSupported format has been run inside createRenderPass()
	depthBufferImage = createImage(mainDevice.physicalDevice, mainDevice.logicalDevice, swapChainExtent.width, swapChainExtent.height, 1 , msaaSamples, depthImageFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &depthBufferImageMemory);

	depthBufferImageView = createImageView(mainDevice.logicalDevice, depthBufferImage, depthImageFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

VkFormat VulkanRenderer::chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
{
	for (VkFormat format : formats)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(mainDevice.physicalDevice, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
	}

	throw std::runtime_error("No Supported Format!");
}

#pragma endregion


#pragma region Frame Buffers [createFrameBuffers()]

void VulkanRenderer::createFrameBuffers()
{
	// Resize framebuffer count to equal swapChain image count
	swapChainFramebuffers.resize(swapChainImages.size());

	// Create a framebuffer for each swap chain image
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		//std::vector<VkImageView> attachments;
		std::array<VkImageView, 3> attachmentsColorImage = {
			colorImageView,
			depthBufferImageView,
			swapChainImages[i].imageView
		};
		std::array<VkImageView, 2> attachmentsNoColorImage = {
			swapChainImages[i].imageView,
			depthBufferImageView
		};


		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass;
		if (msaaSamples != VK_SAMPLE_COUNT_1_BIT) {
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentsColorImage.size());
			framebufferCreateInfo.pAttachments = attachmentsColorImage.data();
		}
		else {
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentsNoColorImage.size());
			framebufferCreateInfo.pAttachments = attachmentsNoColorImage.data();
		}
		
		framebufferCreateInfo.width = swapChainExtent.width;
		framebufferCreateInfo.height = swapChainExtent.height;
		framebufferCreateInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(mainDevice.logicalDevice, &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Frame Buffer Could not be created!");
		}
	}
}

#pragma endregion

#pragma region Command Buffers [createCommandPool(), createCommandBuffers()]

void VulkanRenderer::createCommandPool()
{
	//QueueFamilyIndices queueFamilyIndices = indices;

	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;		// Reseting Command Buffer every Draw Call
	commandPoolInfo.queueFamilyIndex = indices.graphicsFamily;

	VkResult result = vkCreateCommandPool(mainDevice.logicalDevice, &commandPoolInfo, nullptr, &graphicsCommandPool);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Command Pool Could not be created!");
	}

}

void VulkanRenderer::createCommandBuffers()
{
	//Resize command buffer count to one for each frame buffer
	commandBuffers.resize(swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo cbAllocInfo = {};
	cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbAllocInfo.commandPool = graphicsCommandPool;
	cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbAllocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	//Allocate commandBuffers and place handles in array of buffers
	VkResult result = vkAllocateCommandBuffers(mainDevice.logicalDevice, &cbAllocInfo, commandBuffers.data());

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Allocation for Command Buffers was Unsuccesfull");
	}
}
#pragma endregion

#pragma region Record Commands [recordCommands()]
void VulkanRenderer::recordCommands(uint32_t currentFrame)
{
	//printf("req");
	VkCommandBufferBeginInfo bufferBeginInfo = {};
	bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea.offset = { 0,0 };
	renderPassBeginInfo.renderArea.extent = swapChainExtent;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
	clearValues[1].depthStencil.depth = 1.0f;
	
	renderPassBeginInfo.pClearValues = clearValues.data();
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());

	renderPassBeginInfo.framebuffer = swapChainFramebuffers[currentFrame];
		//Start recording commands to command buffer
		VkResult result = vkBeginCommandBuffer(commandBuffers[currentFrame], &bufferBeginInfo);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failerd to begin Command Buffer!");
		}
		//printf("MODELS NUM - %d\n", AssetManager.meshModelList.size());
		{
			vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			//For every meshModel
			for (size_t i = 0; i < AssetManager.meshModelList.size(); i++)
			{
				MeshModel currentMeshModel = AssetManager.meshModelList[i];
				if (AssetManager.meshModelList[i].getState()) { continue; }
				auto temp = currentMeshModel.getModel();
				
				vkCmdPushConstants(commandBuffers[currentFrame], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix), &temp);

				//For every mesh inside the meshModel
				for (size_t j = 0; j < currentMeshModel.getMeshCount(); j++)
				{
					VkMesh* currentMesh = currentMeshModel.getMesh(j);
					VkBuffer vertexBuffers[1] = { currentMesh->getVertexBuffer() };
					VkDeviceSize offsets[1] = { 0 };

					vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
					//vkCmdBind
					vkCmdBindIndexBuffer(commandBuffers[currentFrame], currentMesh->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

					if (AssetManager.textureList[currentMesh->getTexId()].getState()) { continue; }
				
					std::array<VkDescriptorSet, 2>descriptorSetGroup = { descriptorSets[currentFrame],  AssetManager.textureList[currentMesh->getTexId()].getSDS() };//currentMeshModel.getMesh(j)->getTexId()  

					vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0, nullptr);

					vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
						0, 1, &descriptorSets[currentFrame], 0, nullptr);

					//vkCmdDraw(commandBuffers[i], 12 * 3, 1, 0, 0);
					//vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(firstMesh.getVertexCount()), 1, 0, 0);
					vkCmdDrawIndexed(commandBuffers[currentFrame], currentMeshModel.getMesh(j)->getIndexCount(), 1, 0, 0, 0);
				}

				
			}

		#ifdef GUI_LAYER
			//The ImGUI Function That Renders The GUI
			if(ImGui::GetCurrentContext)
				ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffers[currentFrame]);
		#endif
			vkCmdEndRenderPass(commandBuffers[currentFrame]);
		}

		
		result = vkEndCommandBuffer(commandBuffers[currentFrame]);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to end Command Buffer!");
		}
	
}
#pragma endregion

#pragma region Syncronisation [Semaphores and Fences]
void VulkanRenderer::createSynchronisation()
{
	imageAvailable.resize(MAX_FRAME_DRAWS);
	renderFinished.resize(MAX_FRAME_DRAWS);
	drawFences.resize(MAX_FRAME_DRAWS);

	// Semaphore Creation information
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Fences creation info
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		if (vkCreateSemaphore(mainDevice.logicalDevice, &semaphoreCreateInfo, nullptr, &imageAvailable[i]) != VK_SUCCESS
			|| vkCreateSemaphore(mainDevice.logicalDevice, &semaphoreCreateInfo, nullptr, &renderFinished[i]) != VK_SUCCESS
			|| vkCreateFence(mainDevice.logicalDevice, &fenceCreateInfo, nullptr, &drawFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Semaphore Creation Failed and/or fence!");
		}
	}

}

#pragma endregion


void VulkanRenderer::updateUniformBuffer(uint32_t imageIndex)
{
	void* data;
	vkMapMemory(mainDevice.logicalDevice, uniformBufferMemory[imageIndex], 0, sizeof(UboViewProjection), 0, &data);
	memcpy(data, &uboViewProjection, sizeof(UboViewProjection));
	vkUnmapMemory(mainDevice.logicalDevice, uniformBufferMemory[imageIndex]);

	//For every light the
	//for (size_t i = 0; i < AssetManager.LightList.size(); i++)
	//{
	//printf("dirr : %f\n", AssetManager.allLights.directionalLight.lightDirection.x);
	vkMapMemory(mainDevice.logicalDevice, uniformBufferMemory[imageIndex], sizeof(UboViewProjection), sizeof(VkAssets::AllLights), 0, &data);
	memcpy(data, &AssetManager.allLights, sizeof(VkAssets::AllLights));
	vkUnmapMemory(mainDevice.logicalDevice, uniformBufferMemory[imageIndex]);
	//}
	
}

#pragma region Texture Loading

//Vulkan Exclusive (gets called once to create a texture sampler)
void VulkanRenderer::createTextureSampler()
{
	//Sampler Creation Info 
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 14.0f;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;
	samplerCreateInfo.maxAnisotropy = 16;

	VkResult result = vkCreateSampler(mainDevice.logicalDevice, &samplerCreateInfo, nullptr, &textureSampler);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create a texture Sampler");
	}
}

#pragma endregion


#pragma region SwapChain Recreation

void VulkanRenderer::recreateSwapChain()
{
	vkDeviceWaitIdle(mainDevice.logicalDevice);


	cleanUpSwapChain();
	createSwapChain();

	createRenderPass();
	createGraphicsPipeline();

	createColorResources();
	createDepthResources();

	createFrameBuffers();
	createCommandBuffers();

	#ifdef GUI_LAYER
		GUI = GUI_Renderer(window, &Instance, &mainDevice.logicalDevice, &mainDevice.physicalDevice, &graphicsQueue, indices.graphicsFamily, &renderPass, &graphicsCommandPool, &msaaSamples, &applyGraphics, &CurrentGraphicSettings, &AssetManager);
	#endif
}

void VulkanRenderer::cleanUpSwapChain()
{
	#ifdef GUI_LAYER
		GUI.CleanUpGuiComponents(mainDevice.logicalDevice);
	#endif

	//MSAA Image & ImageView
	vkDestroyImageView(mainDevice.logicalDevice, colorImageView, nullptr);
	vkDestroyImage(mainDevice.logicalDevice, colorImage, nullptr);
	vkFreeMemory(mainDevice.logicalDevice, colorImageMemory, nullptr);

	//Depth Buffer Image & ImageView
	vkDestroyImageView(mainDevice.logicalDevice, depthBufferImageView, nullptr);
	vkDestroyImage(mainDevice.logicalDevice, depthBufferImage, nullptr);
	vkFreeMemory(mainDevice.logicalDevice, depthBufferImageMemory, nullptr);

	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(mainDevice.logicalDevice, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(mainDevice.logicalDevice, graphicsCommandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	vkDestroyPipeline(mainDevice.logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(mainDevice.logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(mainDevice.logicalDevice, renderPass, nullptr);

	//DESTROYS ALL SWAPCHAIN IMAGES
	for (const auto& image : swapChainImages) {
		vkDestroyImageView(mainDevice.logicalDevice, image.imageView, nullptr);
		
	}

	vkDestroySwapchainKHR(mainDevice.logicalDevice, swapchain, nullptr);	//DESTROY SWAPCHAIN
}

#pragma endregion


void VulkanRenderer::setCurrentSampleCount(int sampleCount)
{
	
	if (maxMsaaSamples >= static_cast<VkSampleCountFlagBits>(sampleCount))
	{
		vkDeviceWaitIdle(mainDevice.logicalDevice);
		printf("Applly %d\n", sampleCount);
		
		switch (sampleCount)
		{
		case 1:
			msaaSamples = VK_SAMPLE_COUNT_1_BIT;
			printf("MSAA set to 1\n");
			break;
		case 2:
			msaaSamples = VK_SAMPLE_COUNT_2_BIT;
			printf("MSAA set to 2\n");
			break;
		case 4:
			msaaSamples = VK_SAMPLE_COUNT_4_BIT;
			printf("MSAA set to 4\n");
			break;
		case 8:
			msaaSamples = VK_SAMPLE_COUNT_8_BIT;
			printf("MSAA set to 8\n");
			break;
		default:
			msaaSamples = VK_SAMPLE_COUNT_1_BIT;
			printf("MSAA set to 1\n");
			break;
		}
		recreateSwapChain();
	}
	else {
		printf("TooHigh Sample Count\n");
	}
}








