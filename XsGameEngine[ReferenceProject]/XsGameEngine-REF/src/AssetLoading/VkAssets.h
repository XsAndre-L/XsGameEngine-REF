#pragma once
//This Class in meant to keep track of all assets that are created 
// Keeps track of all "Models" , "Textures" , "Lights"
#include "MeshModel.h"
#include "VkTexture.h"
#include "Lights/DirectionalLight.h"
#include <thread>
#include <mutex>

static class VkAssets 
{
public:
	VkAssets();
	VkAssets(
		VkPhysicalDevice* physicalDevice,
		VkDevice* logicalDevice,
		VkQueue* transferQueue,
		VkCommandPool* graphicsCommandPool,
		VkSampler* textureSampler,
		VkDescriptorPool* samplerDescriptorPool,
		VkDescriptorSetLayout* samplerSetLayout
	);

	void asyncAssets(std::string ModelPath);
	void createAsset(std::string ModelPath);

	#pragma region 3D Models
	int* SetSelected() { return &selectedModel; }
	
	int createMeshModel(std::string modelFile);
	void destroyMeshModel(int MeshId);

	//Scene Objects
	std::vector<std::string> meshModelNames;
	std::vector<MeshModel> meshModelList;
	#pragma endregion


	#pragma region Textures
	int addTexture(std::string fileName);

	std::vector<std::string> textureNames;
	std::vector<VkTexture> textureList;
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
		std::vector<std::string>* ModelNames;
		std::vector<MeshModel>* modelPointers;
		std::vector<VkTexture>* texturePointers;
		AllLights* lightPointers;
	}allAssets;

	AllAssets* getAssetInfo();

	
	

	void cleanUpAssets();

private:
	int selectedModel = 0;

	VkPhysicalDevice* PhysicalDevice;
	VkDevice* Device;
	VkQueue* TransferQueue;
	VkCommandPool* GraphicsCommandPool;

	VkSampler* TextureSampler;
	VkDescriptorPool* SamplerDescriptorPool;
	VkDescriptorSetLayout* SamplerSetLayout;


	std::thread* AssetLoadThread = nullptr;
	
};

