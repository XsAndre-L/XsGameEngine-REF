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
	OpenGL_Assets() = default;
	/*OpenGL_Assets(

	);*/

	void asyncAssets(std::string ModelPath);
	void createAsset(std::string ModelPath);

	#pragma region 3D Models
	int* SetSelected() { return &selectedModel; }
	
	int createOpenGL_MeshModel(std::string modelFile);
	void destroyOpenGL_MeshModel(int MeshId);

	//Scene Objects
	std::vector<std::string> OpenGL_MeshModelNames;
	std::vector<OpenGL_MeshModel> OpenGL_MeshModelList;
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


	std::thread* AssetLoadThread{ nullptr };
	
};

