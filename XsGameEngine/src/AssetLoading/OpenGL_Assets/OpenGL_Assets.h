#pragma once
//This Class in meant to keep track of all assets that are created 
// Keeps track of all "Models" , "Textures" , "Lights"
#include "OpenGL_MeshModel.h"
#include "OpenGL_Texture.h"
//#include "Lights/DirectionalLight.h"
//#include "PointLight.h"
//#include "SpotLight.h"
#include <iostream>
#include <thread>
#include <mutex>

class OpenGL_Assets 
{
public:
	std::vector<Assimp::Importer*> importer;
	std::vector<const aiScene*> addedModels;
	bool Load_Model_Files = false;			//TODO
	std::vector<std::string> addedModelPaths;

	OpenGL_Assets() = default;

	void asyncAssets(std::string ModelPath);
	void createAsset(std::string ModelPath);

	#pragma region 3D Models
	int* SetSelected() { return &selectedModel; }
	
	int createOpenGL_MeshModel();
	void destroyOpenGL_MeshModel(int MeshId);

	//Scene Objects
	std::vector<std::string> MeshModelNames;
	std::vector<OpenGL_MeshModel> MeshModelList;
	#pragma endregion


	#pragma region Textures
	uint16_t addTexture(std::string fileName);

	std::vector<std::string> textureNames;
	std::vector<OpenGL_Texture> textureList;
	#pragma endregion


	#pragma region Lights
	void addLight();
	//DirectionalLight newLight;

	//std::vector<DirectionalLight> LightList;
	//AllLights AllL;

	struct AllLights
	{
		//DirectionalLight::DirectionalLightInfo directionalLight[MAX_DIRECTIONAL_LIGHTS];

	}allLights;
	#pragma endregion


	struct AllAssets
	{
		std::vector<std::string>* ModelNames{};
		std::vector<OpenGL_MeshModel>* modelPointers{};
		std::vector<OpenGL_Texture>* texturePointers{};
		//AllLights* lightPointers{};
	}allAssets;

	AllAssets* getAssetInfo();

	
	

	void cleanUpAssets();

private:
	int selectedModel = 0;

	/*VkPhysicalDevice* PhysicalDevice{ nullptr };
	VkDevice* Device{ nullptr };
	VkQueue* TransferQueue{ nullptr };
	VkCommandPool* GraphicsCommandPool{ nullptr };

	VkSampler* TextureSampler{ nullptr };
	VkDescriptorPool* SamplerDescriptorPool{ nullptr };
	VkDescriptorSetLayout* SamplerSetLayout{ nullptr };*/


	std::vector<std::thread*> AssetLoadThread;
	
};

