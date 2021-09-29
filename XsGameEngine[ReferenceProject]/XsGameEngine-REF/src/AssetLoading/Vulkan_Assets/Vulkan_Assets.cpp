#include "Vulkan_Assets.h"
#include <vector>
#include <sstream>
//#include <condition_variable>
//#include <queue>
//#include <functional>
//#include <chrono>

Vulkan_Assets::AllAssets* Vulkan_Assets::getAssetInfo()
{
	allAssets.ModelNames = &Vulkan_MeshModelNames;
	allAssets.modelPointers = &Vulkan_MeshModelList;
	allAssets.lightPointers = &allLights;
	allAssets.texturePointers = &textureList;
	return &allAssets;
}


//Create an instance for asset creation
Vulkan_Assets::Vulkan_Assets
(
	VkPhysicalDevice& physicalDevice, 
	VkDevice& logicalDevice, 
	VkQueue& transferQueue, 
	VkCommandPool& graphicsCommandPool, 
	VkSampler& textureSampler, 
	VkDescriptorPool& samplerDescriptorPool, 
	VkDescriptorSetLayout& samplerSetLayout
)
{
	PhysicalDevice = &physicalDevice;
	Device = &logicalDevice;
	TransferQueue = &transferQueue;
	GraphicsCommandPool = &graphicsCommandPool;
	TextureSampler = &textureSampler;
	SamplerDescriptorPool = &samplerDescriptorPool;
	SamplerSetLayout = &samplerSetLayout;

	addLight();
}


#pragma region Create Asset	
void Vulkan_Assets::asyncAssets(std::string ModelPath) {
	//addTexture(TexturePath);
	createVulkan_MeshModel(ModelPath);
}

void Vulkan_Assets::createAsset(std::string ModelPath) {
	
	try
	{
		if (AssetLoadThread != nullptr)
		{
			AssetLoadThread->detach();

			delete(AssetLoadThread);
		}
		AssetLoadThread = new std::thread(&Vulkan_Assets::asyncAssets, this, ModelPath);
	}
	catch (const std::exception& e)
	{
		printf("ERROR: %s\n", e.what());
	}
}
#pragma endregion


#pragma region Model Loading

int Vulkan_Assets::createVulkan_MeshModel(std::string modelFile)
{
	static std::mutex lockFuncM;
	//std::lock_guard<std::mutex> lock(lockFuncM);
	
	if (!lockFuncM.try_lock()) { return 1; }


	printf("success");
	printf("creating: %s\n", modelFile.c_str());
	Vulkan_MeshModel model = Vulkan_MeshModel();

	//---Checks if model already exists
	for (size_t i = 0; i < Vulkan_MeshModelNames.size(); i++)
	{
		if (Vulkan_MeshModelNames.at(i) == modelFile.substr(7, modelFile.size())) 
		{
			model = Vulkan_MeshModelList.at(i);
			model.resetMatrix();
			model.makeInstance();

			Vulkan_MeshModelList.push_back(model);
			//This gets the model name without the entire Path and adds it to the list of names
			Vulkan_MeshModelNames.push_back(modelFile.substr(7, modelFile.size()));

			lockFuncM.unlock();

			return 0;
		}
	}
	//---

	#pragma region Read Vulkan_MeshModel from file

	// Import "scene" with multiple meshes
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	#pragma endregion


	std::vector<std::string> textureNames = model.LoadMaterials(scene);

	std::vector<uint16_t> matToTex;
	if (textureNames.size() > 0) {
		matToTex.resize(textureNames.size());
	}
	else {
		matToTex.resize(1);
		if (textureList.size() == 0) {
			matToTex[0] = addTexture("plain.png");
		}
		else {
			matToTex[0] = 0;
		}
	}
		
	for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames[i].empty())
		{
			matToTex[i] = 0;
		}
		else {
			matToTex[i] = addTexture(textureNames[i]);
		}
	}

	model.LoadNode(PhysicalDevice, Device, TransferQueue, GraphicsCommandPool, scene->mRootNode, scene, matToTex);

	Vulkan_MeshModelList.push_back(model);
	//This gets the model name without the entire Path and adds it to the list of names
	Vulkan_MeshModelNames.push_back(modelFile.substr(7, modelFile.size()));

	lockFuncM.unlock();

	return 0;
}

void Vulkan_Assets::destroyVulkan_MeshModel(int ModelID) 
{
	vkDeviceWaitIdle(*Device);
	Vulkan_MeshModelList.at(ModelID).destroyModel();
	Vulkan_MeshModelNames.at(ModelID) = "";
}

#pragma endregion


uint16_t Vulkan_Assets::addTexture(std::string fileName)
{
	static std::mutex lockFuncT;

	if (!lockFuncT.try_lock()){ return 0; }

	for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames.at(i) == fileName)
		{
			if (selectedModel != -1)
				for (size_t j = 0; j < Vulkan_MeshModelList.at(selectedModel).getMeshCount(); j++)
				{
					Vulkan_MeshModelList.at(selectedModel).getMesh(j)->setTexId(i);
					printf("Texture Changed");
				}

			lockFuncT.unlock();
			return i;
		}
	}

	Vulkan_Texture texture(PhysicalDevice, Device, TransferQueue, GraphicsCommandPool, TextureSampler, SamplerDescriptorPool, SamplerSetLayout);
	int result = texture.createTexture(fileName);

	if (result == 1) {

		lockFuncT.unlock();
		printf("Failed To Load Texture");
		return 0;
	}
	printf("ja");
	textureList.push_back(texture);
	textureNames.push_back(fileName.c_str());

	if (selectedModel < Vulkan_MeshModelList.size()) {
		for (size_t j = 0; j < Vulkan_MeshModelList.at(selectedModel).getMeshCount(); j++)
		{
			Vulkan_MeshModelList.at(selectedModel).getMesh(j)->setTexId(textureList.size() - 1);
			printf("Texture Changed");
		}
	}
		
	lockFuncT.unlock();

	return textureList.size() - 1;
}

#pragma region Lights
void Vulkan_Assets::addLight()
{
	newLight = DirectionalLight();
	//LightList.push_back(DirectionalLight());


	allLights.directionalLight[0] = newLight.dlight;

	//newLight = DirectionalLight(1.0f,0.0f,0.0f,0.2f,1.0f,-1.0f,0.0f,0.0f);

	//allLights.directionalLight[1] = newLight.dlight;

}
#pragma endregion




#pragma region CleanUp
void Vulkan_Assets::cleanUpAssets()
{
	if (AssetLoadThread != nullptr) {

		AssetLoadThread->detach();


		delete(AssetLoadThread);

	}

	for (size_t i = 0; i < Vulkan_MeshModelList.size(); i++)
	{
		Vulkan_MeshModelList[i].destroyModel();
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		textureList[i].cleanUpTexture();
	}
}
#pragma endregion




