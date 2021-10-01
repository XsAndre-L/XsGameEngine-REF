#pragma once
//This Class in meant to keep track of all assets that are created 
// Keeps track of all "Models" , "Textures" , "Lights"
#include "../../CommonValues.h"
#include "Vulkan_MeshModel.h"
#include "Vulkan_Texture.h"
#include "../Lights/DirectionalLight.h"

#include <thread>
#include <mutex>


class Vulkan_Assets 
{
public:
	Vulkan_Assets() = default;
	Vulkan_Assets(
		VkPhysicalDevice& physicalDevice,
		VkDevice& logicalDevice,
		VkQueue& transferQueue,
		VkCommandPool& graphicsCommandPool,
		VkSampler& textureSampler,
		VkDescriptorPool& samplerDescriptorPool,
		VkDescriptorSetLayout& samplerSetLayout
	);

	void asyncAssets(std::string ModelPath);
	void createAsset(std::string ModelPath);

	#pragma region 3D Models
	int* SetSelected() { return &selectedModel; }
	
	int createVulkan_MeshModel(std::string modelFile);
	void destroyVulkan_MeshModel(int MeshId);

	//Scene Objects
	std::vector<std::string> Vulkan_MeshModelNames;
	std::vector<Vulkan_MeshModel> Vulkan_MeshModelList;
	#pragma endregion


	#pragma region Textures
	uint16_t addTexture(std::string fileName);

	std::vector<std::string> textureNames;
	std::vector<Vulkan_Texture> textureList;
	#pragma endregion


	#pragma region Lights
	void addLight();
	DirectionalLight newLight;

	std::vector<DirectionalLight> LightList;
	//AllLights AllL;

	struct AllLights
	{
		DirectionalLight::DirectionalLightInfo directionalLight[MAX_DIRECTIONAL_LIGHTS];

	}allLights;
	#pragma endregion


	struct AllAssets
	{
		std::vector<std::string>* ModelNames{};
		std::vector<Vulkan_MeshModel>* modelPointers{};
		std::vector<Vulkan_Texture>* texturePointers{};
		AllLights* lightPointers{};
	}allAssets;

	AllAssets* getAssetInfo();

	
	

	void cleanUpAssets();

private:
	int selectedModel = 0;

	VkPhysicalDevice* PhysicalDevice{ nullptr };
	VkDevice* Device{ nullptr };
	VkQueue* TransferQueue{ nullptr };
	VkCommandPool* GraphicsCommandPool{ nullptr };

	VkSampler* TextureSampler{ nullptr };
	VkDescriptorPool* SamplerDescriptorPool{ nullptr };
	VkDescriptorSetLayout* SamplerSetLayout{ nullptr };


	std::thread* AssetLoadThread{ nullptr };
	
};

