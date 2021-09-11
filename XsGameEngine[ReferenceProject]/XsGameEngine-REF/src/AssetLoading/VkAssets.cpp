#include "VkAssets.h"
#include <vector>
#include <sstream>
//#include <condition_variable>
//#include <queue>
//#include <functional>
//#include <chrono>

VkAssets::AllAssets* VkAssets::getAssetInfo()
{

	allAssets.ModelNames = &meshModelNames;
	allAssets.modelPointers = &meshModelList;
	allAssets.lightPointers = &allLights;
	allAssets.texturePointers = &textureList;
	return &allAssets;
}

VkAssets::VkAssets()
{

}

//Create an instance for asset creation
VkAssets::VkAssets(
	VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
	VkQueue& transferQueue, VkCommandPool& graphicsCommandPool, 
	VkSampler& textureSampler, VkDescriptorPool& samplerDescriptorPool, 
	VkDescriptorSetLayout& samplerSetLayout)
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
void VkAssets::asyncAssets(std::string ModelPath) {
	//addTexture(TexturePath);
	createMeshModel(ModelPath);
}

void VkAssets::createAsset(std::string ModelPath) {
	
	try
	{
		if (AssetLoadThread != nullptr)
		{
			AssetLoadThread->detach();

			delete(AssetLoadThread);
		}
		AssetLoadThread = new std::thread(&VkAssets::asyncAssets, this, ModelPath);
	}
	catch (const std::exception& e)
	{
		printf("ERROR: %s\n", e.what());
	}
}
#pragma endregion


#pragma region Model Loading

int VkAssets::createMeshModel(std::string modelFile)
{
	static std::mutex lockFuncM;
	//std::lock_guard<std::mutex> lock(lockFuncM);
	
	if (!lockFuncM.try_lock()) { return 1; }


	printf("success");
	printf("creating: %s\n", modelFile.c_str());
	MeshModel model = MeshModel();

	//---Checks if model already exists
	for (size_t i = 0; i < meshModelNames.size(); i++)
	{
		if (meshModelNames.at(i) == modelFile.substr(7, modelFile.size())) 
		{
			model = meshModelList.at(i);
			model.resetMatrix();
			model.makeInstance();

			meshModelList.push_back(model);
			//This gets the model name without the entire Path and adds it to the list of names
			meshModelNames.push_back(modelFile.substr(7, modelFile.size()));

			lockFuncM.unlock();

			return 0;
		}
	}
	//---

	#pragma region Read MeshModel from file

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

	meshModelList.push_back(model);
	//This gets the model name without the entire Path and adds it to the list of names
	meshModelNames.push_back(modelFile.substr(7, modelFile.size()));

	lockFuncM.unlock();

	return 0;
}

void VkAssets::destroyMeshModel(int ModelID) 
{
	vkDeviceWaitIdle(*Device);
	meshModelList.at(ModelID).destroyModel();
	meshModelNames.at(ModelID) = "";
}

#pragma endregion


uint16_t VkAssets::addTexture(std::string fileName)
{
	static std::mutex lockFuncT;

	if (!lockFuncT.try_lock()){ return 0; }

	for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames.at(i) == fileName)
		{
			if (selectedModel != -1)
				for (size_t j = 0; j < meshModelList.at(selectedModel).getMeshCount(); j++)
				{
					meshModelList.at(selectedModel).getMesh(j)->setTexId(i);
					printf("Texture Changed");
				}

			lockFuncT.unlock();
			return i;
		}
	}

	VkTexture texture(PhysicalDevice, Device, TransferQueue, GraphicsCommandPool, TextureSampler, SamplerDescriptorPool, SamplerSetLayout);
	int result = texture.createTexture(fileName);

	if (result == 1) {

		lockFuncT.unlock();
		printf("Failed To Load Texture");
		return 0;
	}
	printf("ja");
	textureList.push_back(texture);
	textureNames.push_back(fileName.c_str());

	if (selectedModel < meshModelList.size()) {
		for (size_t j = 0; j < meshModelList.at(selectedModel).getMeshCount(); j++)
		{
			meshModelList.at(selectedModel).getMesh(j)->setTexId(textureList.size() - 1);
			printf("Texture Changed");
		}
	}
		
	lockFuncT.unlock();

	return textureList.size() - 1;
}

#pragma region Lights
void VkAssets::addLight()
{
	newLight = DirectionalLight();
	//LightList.push_back(DirectionalLight());


	allLights.directionalLight[0] = newLight.dlight;

	//newLight = DirectionalLight(1.0f,0.0f,0.0f,0.2f,1.0f,-1.0f,0.0f,0.0f);

	//allLights.directionalLight[1] = newLight.dlight;

}
#pragma endregion




#pragma region CleanUp
void VkAssets::cleanUpAssets()
{
	if (AssetLoadThread != nullptr) {

		AssetLoadThread->detach();


		delete(AssetLoadThread);

	}

	for (size_t i = 0; i < meshModelList.size(); i++)
	{
		meshModelList[i].destroyModel();
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		textureList[i].cleanUpTexture();
	}
}
#pragma endregion




