#pragma once

#include "../../ProgramLayers/ProgramLayers.h"

#include <chrono>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "../GUI_Renderer.h"
#include "../../AssetLoading/Vulkan_Assets/Vulkan_Assets.h"

#include "../../Window/Public/GLFW_Window.h"
#include "../../Camera.h"



class Vulkan_Renderer
{
public:
	//Vulkan Setup
	Vulkan_Renderer() = default;
	~Vulkan_Renderer() = default;
	int init_Vulkan_Renderer(GLFWwindow* newWindow);		//initialize


	glm::mat4* getViewMatrix() { return &uboViewProjection.view; }
	glm::mat4* getProjectionMatrix() { return &uboViewProjection.projection; }

	void draw();
	void shutdown_Renderer();
	ImVec4 clearColor = { 0.15f, 0.15f ,0.15f , 1.0f};

	
	

private:
	//Vulkan Setup

	GLFWwindow* window;

	//VULKAN COMPONENTS
	VkInstance Instance{};
	VkSurfaceKHR Surface{};

	struct {
		VkPhysicalDevice	physicalDevice;
		VkDevice			logicalDevice;
	}mainDevice;								

	QueueFamilyIndices indices{};
	VkQueue graphicsQueue{};
	VkQueue presentationQueue{};

	//createSwapChain();
	VkSwapchainKHR		swapchain{};
	//SwapChainDetails	swapChainDetails;
	VkFormat			swapChainImageFormat{};
	VkExtent2D			swapChainExtent{};
	std::vector<SwapChainImage> swapChainImages{};

	VkRenderPass	renderPass{};
	VkFormat		depthImageFormat{};

	#pragma region Uniform Descriptors

	//Scene Settings [View And Projection Matrix]
	struct UboViewProjection {
		glm::mat4 projection;
		glm::mat4 view;
	} uboViewProjection;

	////UNIFORMS
	// Descriptors
	VkDescriptorSetLayout			descriptorSetLayout{};		// DescriptorSetLayout & samplerSetLayout and PushConstantRange needed for Pipeline Creation
	VkDescriptorSetLayout			samplerSetLayout{};
	VkPushConstantRange				pushConstantRange{};

	std::vector<VkBuffer>			uniformBuffer{};
	std::vector<VkDeviceMemory>		uniformBufferMemory{};

	VkDescriptorPool				descriptorPool{};
	std::vector<VkDescriptorSet>	descriptorSets{};


	#pragma endregion

	//Graphics Pipeline
	VkPipeline			graphicsPipeline{};
	VkPipelineLayout	pipelineLayout{};
	VkPipelineCache		pipelineCache{};

	//- Depth
	VkImage				depthBufferImage{};
	VkDeviceMemory		depthBufferImageMemory{};
	VkImageView			depthBufferImageView{};


	//One for every swapChainImage
	std::vector<VkFramebuffer>		swapChainFramebuffers{};  //Helps to bind imageView WIth attachment
	std::vector<VkCommandBuffer>	commandBuffers{};

	int currentFrame = 0;

	

	// - mem POOLS
	VkCommandPool graphicsCommandPool{};

	//- Synchronisation
	std::vector<VkSemaphore>	imageAvailable{};
	std::vector<VkSemaphore>	renderFinished{};
	std::vector<VkFence>		drawFences{};

	

	//Assets
	VkSampler			textureSampler{};
	VkDescriptorPool	samplerDescriptorPool{};
	
private:
	//VULKAN FUNCTIONS
	// 
	//	-- Create Instance --
	void createInstance();															//MAIN VULKAN INSTANCE
	bool checkInstanceExtensionsSupport(std::vector<const char*>* checkExtensions);	//INSTANCE EXTENTION SUPPORT
	void createSurface();															//SURFACE TO RENDER APPON
	//
	//	-- Get Physical Device and Check Suitabillity --
	void getPhysicalDevice();														//GET THE GPU / PHYSICAL DEVICE
	bool checkDeviceSuitable(VkPhysicalDevice device);								//QueueFamilies, DeviceExtentions, SwapchainDetails	
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	//
	//	-- CreateLogicalDevice -- 
	void createLogicalDevice();														//INTERFACE TOO PHYSICAL DEVICE
	//
	//	-- CreateSwapchain --
	void createSwapChain();															//Component that swaps out the pictures too the screen
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseBestSurfaceFormats(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
	//VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevelCount);

	void createRenderPass();

	void createDescriptorSetLayouts(); //Descriptor
	void createPushConstantRange();
	void createUniformBuffers();	//Descriptor
	void createDescriptorPool();	//Descriptor
	void allocateDescriptorSets();

	void createGraphicsPipeline();

	void createDepthResources();				//createImage function needed

	void createFrameBuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronisation();

	
	void updateUniformBuffer(uint32_t imageIndex);

	//Record Functions
	void recordCommands(uint32_t currentFrame);

	VkShaderModule createShaderModule(const std::vector<char>& code);
	//VkImage createImage(uint32_t width, uint32_t height, uint32_t mipLevelCount, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags, VkDeviceMemory *imageMemory);
	VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);


	//int createTextureDescriptor(VkImageView textureImage);
	void createTextureSampler();

	void recreateSwapChain();
	void cleanUpSwapChain();

	//ImGUI AllocationCallbacks
	//VkAllocationCallbacks guiBufferMemory;

	//MSAA
	VkSampleCountFlagBits maxMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	void setCurrentSampleCount(int sampleCount);
	VkSampleCountFlagBits getMaxUsableSampleCount();

	bool applyGraphics = false;
	GraphicSettings CurrentGraphicSettings;


	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	void createColorResources();

public:
	

	#ifdef GUI_LAYER
		GUI_Renderer GUI;
	#else
		const bool GUI_LAYER = false;
	#endif
	
	Vulkan_Assets AssetManager;

};
