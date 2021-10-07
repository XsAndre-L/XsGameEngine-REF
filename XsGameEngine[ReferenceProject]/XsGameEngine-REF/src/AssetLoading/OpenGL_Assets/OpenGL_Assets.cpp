#include "OpenGL_Assets.h"
#include <vector>
#include <sstream>
//#include <condition_variable>
//#include <queue>
//#include <functional>
//#include <chrono>

OpenGL_Assets::AllAssets* OpenGL_Assets::getAssetInfo()
{
	allAssets.ModelNames = &OpenGL_MeshModelNames;
	allAssets.modelPointers = &OpenGL_MeshModelList;
	//allAssets.lightPointers = &allLights;
	allAssets.texturePointers = &textureList;
	return &allAssets;
}


//Create an instance for asset creation
//OpenGL_Assets::OpenGL_Assets()
//{
//
//
//	//addLight();
//}


#pragma region Create Asset	
void OpenGL_Assets::asyncAssets(std::string ModelPath) 
{
	//addTexture(TexturePath);
	//createOpenGL_MeshModel(ModelPath.c_str());
	static std::mutex lockFuncM;
	while (true) {
		// try to lock mutex
		if (lockFuncM.try_lock()) {
			break;	// If successful we exit the loop
		}
		else {
			// wait for previous load to finish
			std::chrono::milliseconds interval(100);
			std::this_thread::sleep_for(interval);
		}
	}

	if (!shouldADD) {

#pragma region Read Vulkan_MeshModel from file to ADD model
		path = ModelPath;
		bool ShouldRead = true;
		for (size_t i = 0; i < OpenGL_MeshModelNames.size(); i++)
		{
			if (OpenGL_MeshModelNames.at(i) == ModelPath.substr(7, ModelPath.size()))
			{

				shouldADD = true;
				ShouldRead = false;
			}
		}
		if (ShouldRead) {
			// Import "scene" with multiple meshes
			double StartTime = glfwGetTime();

			importer = new Assimp::Importer();
			scene = importer->ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
			//delete importer;

			if (!scene)
			{
				throw std::runtime_error("Failed to load model! (" + ModelPath + ")");
			}
			else {
				shouldADD = true;
				double StopTime = glfwGetTime();
				printf("second - %f \n", (StopTime - StartTime));
			}
		}
#pragma endregion
	}
	lockFuncM.unlock();
}

void OpenGL_Assets::createAsset(std::string ModelPath) {
	
	try
	{
		if (AssetLoadThread != nullptr)
		{
			AssetLoadThread->detach();

			delete(AssetLoadThread);
		}
		AssetLoadThread = new std::thread(&OpenGL_Assets::asyncAssets, this, ModelPath);
	}
	catch (const std::exception& e)
	{
		printf("ERROR: %s\n", e.what());
	}
}
#pragma endregion


#pragma region Model Loading

int OpenGL_Assets::createOpenGL_MeshModel(/*const std::string modelFile*/)
{
	static std::mutex lockFuncM;
	
	if (!lockFuncM.try_lock()) { return 1; }


	printf("creating: %s\n", path.c_str());
	OpenGL_MeshModel model = OpenGL_MeshModel();

	//---Checks if model already exists
	for (size_t i = 0; i < OpenGL_MeshModelNames.size(); i++)
	{
		if (OpenGL_MeshModelNames.at(i) == path.substr(7, path.size()))
		{
			model = OpenGL_MeshModelList.at(i);
			model.resetMatrix();
			//model.makeInstance();

			OpenGL_MeshModelList.push_back(model);
			//This gets the model name without the entire Path and adds it to the list of names
			OpenGL_MeshModelNames.push_back(path.substr(7, path.size()));

			shouldADD = false; 
			lockFuncM.unlock();

			return 0;
		}
	}
	//---
	/*
	#pragma region Read Vulkan_MeshModel from file

	// Import "scene" with multiple meshes
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	#pragma endregion
	*/

	/*std::vector<std::string> textureNames = model.LoadMaterials(scene);

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
	}*/
		
	/*for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames[i].empty())
		{
			matToTex[i] = 0;
		}
		else {
			matToTex[i] = addTexture(textureNames[i]);
		}
	}*/

	//model.LoadNode(scene->mRootNode, scene);
	OpenGL_MeshModel myModel = OpenGL_MeshModel();
	myModel.LoadModel(scene);
	//modelList.push_back(myModel);

	OpenGL_MeshModelList.push_back(myModel);
	//delete myModel;
	//This gets the model name without the entire Path and adds it to the list of names
	OpenGL_MeshModelNames.push_back(path.substr(7, path.size()));
	shouldADD = false; //TODO
	delete importer;
	lockFuncM.unlock();

	return 0;
}

void OpenGL_Assets::destroyOpenGL_MeshModel(int ModelID) 
{
	//vkDeviceWaitIdle(*Device);
	OpenGL_MeshModelList.at(ModelID).destroyModel();
	OpenGL_MeshModelNames.at(ModelID) = "";
}

#pragma endregion


uint16_t OpenGL_Assets::addTexture(std::string fileName)
{
	static std::mutex lockFuncT;

	if (!lockFuncT.try_lock()){ return 0; }

	for (size_t i = 0; i < textureNames.size(); i++)
	{
		if (textureNames.at(i) == fileName)
		{
			if (selectedModel != -1)
				for (size_t j = 0; j < OpenGL_MeshModelList.at(selectedModel).getMeshCount(); j++)
				{
					OpenGL_MeshModelList.at(selectedModel).setMeshToTex(j, i);// .getMesh(j)->setTexId(i);
					printf("Texture Changed");
				}

			lockFuncT.unlock();
			return i;
		}
	}

	OpenGL_Texture* tex = new OpenGL_Texture(fileName.c_str());
	bool result = tex->LoadTexture();

	if (!result) {

		lockFuncT.unlock();
		printf("Failed To Load Texture");
		return 0;
	}
	printf("ja");
	OpenGL_MeshModelList.at(selectedModel).textureList.push_back(tex);
	textureNames.push_back(fileName.c_str());

	if (selectedModel < OpenGL_MeshModelList.size()) {
		for (size_t j = 0; j < OpenGL_MeshModelList.at(selectedModel).getMeshCount(); j++)
		{
			OpenGL_MeshModelList.at(selectedModel).setMeshToTex(j, (OpenGL_MeshModelList.at(selectedModel).textureList.size() - 1));
			printf("Texture Changed");
		}
	}
		
	lockFuncT.unlock();

	return textureList.size() - 1;
}

#pragma region Lights
void OpenGL_Assets::addLight()
{
	//newLight = DirectionalLight();
	//LightList.push_back(DirectionalLight());


	//allLights.directionalLight[0] = newLight.dlight;

	//newLight = DirectionalLight(1.0f,0.0f,0.0f,0.2f,1.0f,-1.0f,0.0f,0.0f);

	//allLights.directionalLight[1] = newLight.dlight;

}
#pragma endregion


#pragma region CleanUp
void OpenGL_Assets::cleanUpAssets()
{
	if (AssetLoadThread != nullptr) {

		AssetLoadThread->detach();


		delete(AssetLoadThread);

	}

	for (size_t i = 0; i < OpenGL_MeshModelList.size(); i++)
	{
		OpenGL_MeshModelList[i].destroyModel();
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		textureList[i].ClearTexture();
	}
}
#pragma endregion




