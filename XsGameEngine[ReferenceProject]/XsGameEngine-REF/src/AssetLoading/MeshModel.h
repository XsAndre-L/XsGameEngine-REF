#pragma once
#include "Entity.h"
#include "VkMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class MeshModel : public Entity
{
public:
	MeshModel();
	//MeshModel(std::vector<VkMesh> newMeshList);
	size_t getMeshCount();

	VkMesh* getMesh(size_t index);


	std::vector<std::string> LoadMaterials(const aiScene * scene);
	std::vector<VkMesh> LoadNode(VkPhysicalDevice* newPhysicalDevice, VkDevice* newDevice, VkQueue* transferQueue, VkCommandPool* transferCommandPool,
		aiNode* node, const aiScene* scene, std::vector<uint16_t> matToTex);
	VkMesh LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		aiMesh* mesh, const aiScene* scene, std::vector<uint16_t> matToTex);

	std::vector<std::string>* getChildren() { return &Children; }
	bool getState() { return DESTROYED; }
	void makeInstance() { MODEL_INSTANCE = true; }

	void destroyModel();
	~MeshModel();

private:

	std::vector<std::string> Children;

	bool MODEL_INSTANCE = false;
	bool DESTROYED = false;
	std::vector<VkMesh> meshList;
	std::vector<int> TextureMap;

};

