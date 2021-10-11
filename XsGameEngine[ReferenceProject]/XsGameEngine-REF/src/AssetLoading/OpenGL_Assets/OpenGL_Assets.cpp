#include "OpenGL_Assets.h"
#include <vector>
#include <sstream>
//#include <condition_variable>
//#include <queue>
//#include <functional>
//#include <chrono>

OpenGL_Assets::AllAssets* OpenGL_Assets::getAssetInfo()
{
	allAssets.ModelNames = &MeshModelNames;
	allAssets.modelPointers = &MeshModelList;
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
		if (!Load_Model_Files && lockFuncM.try_lock()) {
			
			break;	// If successful we exit the loop
		}
		else {
			// wait for previous load to finish
			
			std::chrono::milliseconds interval(100);
			std::this_thread::sleep_for(interval);
		}
	}

	if (!Load_Model_Files) {

#pragma region Read Vulkan_MeshModel from file to ADD model
		addedModelPaths.push_back(ModelPath);
		bool ShouldRead = true;
		for (size_t i = 0; i < MeshModelNames.size(); i++)
		{
			if (MeshModelNames.at(i) == ModelPath.substr(7, ModelPath.size()))
			{
				Load_Model_Files = true;
				ShouldRead = false;
			}
		}
		if (ShouldRead) {
			// Import "scene" with multiple meshes
			double StartTime = glfwGetTime();

			importer.push_back(new Assimp::Importer());
			addedModels.push_back(importer.at(importer.size()-1)->ReadFile(ModelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices));
			//delete importer;

			if (!addedModels.at(addedModels.size() - 1))
			{
				throw std::runtime_error("Failed to load model! (" + ModelPath + ")");
			}
			else {
				Load_Model_Files = true;
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
		/*if (AssetLoadThread != nullptr)
		{
			AssetLoadThread->detach();

			delete(AssetLoadThread);
		}*/
		AssetLoadThread.push_back(new std::thread(&OpenGL_Assets::asyncAssets, this, ModelPath));
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

	for (size_t i = 0; i < addedModelPaths.size(); i++)
	{


		printf("creating: %s\n", addedModelPaths.at(i).c_str());
		OpenGL_MeshModel model = OpenGL_MeshModel();

		//---Checks if model already exists
		for (size_t i = 0; i < MeshModelNames.size(); i++)
		{
			for (size_t j = 0; j < addedModelPaths.size(); j++)
			{
				if (MeshModelNames.at(i) == addedModelPaths.at(j).substr(7, addedModelPaths.size()))
				{
					model = MeshModelList.at(i);
					model.resetMatrix();
					//model.makeInstance();

					MeshModelList.push_back(model);
					//This gets the model name without the entire Path and adds it to the list of names
					MeshModelNames.push_back(addedModelPaths.at(i).substr(7, addedModelPaths.size()));

					Load_Model_Files = false;
					lockFuncM.unlock();

					return 0;
				}
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

		std::vector<std::string> TempTextureNames = model.LoadMaterials(addedModels.at(i));

		//std::vector<uint16_t> matToTex;
		if (textureNames.size() > 0) {
			model.resizeMeshToTex(textureNames.size());
			//matToTex.resize(textureNames.size());
		}
		else {
			model.resizeMeshToTex(1);

			if (textureList.size() == 0) {
				model.setMeshToTex(0, addTexture("plain.png"));
			}
			else {
				model.setMeshToTex(0, 0);
			}
		}

		for (size_t i = 0; i < textureNames.size(); i++)
		{
			if (textureNames[i].empty())
			{
				model.setMeshToTex(i, 0);
			}
			else {
				model.setMeshToTex(0, addTexture(textureNames[i]));
			}
		}

		//model.LoadNode(scene->mRootNode, scene);
		//OpenGL_MeshModel myModel = OpenGL_MeshModel();
		model.LoadModel(addedModels.at(i));
		//modelList.push_back(myModel);

		MeshModelList.push_back(model);

		//This gets the model name without the entire Path and adds it to the list of names
		std::string modelName = addedModelPaths.at(i);
		MeshModelNames.push_back(modelName.substr(7, addedModelPaths.at(i).size()));

	}
	addedModelPaths.clear();
	addedModels.clear();

	Load_Model_Files = false; //TODO
	for (size_t i = 0; i < importer.size(); i++)
	{
		Assimp::Importer* currImporter = importer.at(i);
		delete currImporter;
	}
	importer.clear();
	lockFuncM.unlock();

	return 0;
}

void OpenGL_Assets::destroyOpenGL_MeshModel(int ModelID) 
{
	//vkDeviceWaitIdle(*Device);
	MeshModelList.at(ModelID).destroyModel();
	MeshModelNames.at(ModelID) = "";
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
			if (selectedModel != 0)
				for (size_t j = 0; j < MeshModelList.at(selectedModel).getMeshCount(); j++)
				{
					MeshModelList.at(selectedModel).setMeshToTex(j, i);// .getMesh(j)->setTexId(i);
					printf("Texture Changed");
				}

			lockFuncT.unlock();
			return i;
		}
	}
	std::string fileLoc = "Textures/" + fileName;
	OpenGL_Texture* tex = new OpenGL_Texture(fileLoc.c_str());
	bool result;
	if (fileName.substr(fileName.length() - 3, 3) == "png") {
		result = tex->LoadTextureA();
	}
	else if (fileName.substr(fileName.length() - 3, 3) == "jpg") {
		result = tex->LoadTexture();
	}

	if (!result) {

		lockFuncT.unlock();
		printf("Failed To Load Texture");
		return 0;
	}
	printf("ja");
	textureList.push_back(*tex);//OpenGL_MeshModelList.at(selectedModel).
	textureNames.push_back(fileName.c_str());

	if (selectedModel < MeshModelList.size()) {
		for (size_t j = 0; j < MeshModelList.at(selectedModel).getMeshCount(); j++)
		{
			MeshModelList.at(selectedModel).setMeshToTex(j, (textureList.size() - 1));//OpenGL_MeshModelList.at(selectedModel).
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
	/*if (AssetLoadThread != nullptr) {

		AssetLoadThread->detach();


		delete(AssetLoadThread);

	}*/
	for(auto var : AssetLoadThread)
	{
		var->detach();
		delete(var);
	}
	

	for (size_t i = 0; i < MeshModelList.size(); i++)
	{
		MeshModelList[i].destroyModel();
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		textureList[i].ClearTexture();
	}
}
#pragma endregion




