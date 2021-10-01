#pragma once
#include "../Entity.h"
#include "Vulkan_Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Vulkan_MeshModel : public Entity
{
public:
	Vulkan_MeshModel();
	~Vulkan_MeshModel() = default;
	
	size_t getMeshCount();

	Vulkan_Mesh* getMesh(size_t index);


	std::vector<std::string> LoadMaterials(
		const aiScene * scene);

	std::vector<Vulkan_Mesh> LoadNode(
		VkPhysicalDevice* newPhysicalDevice, 
		VkDevice* newDevice, 
		VkQueue* transferQueue, 
		VkCommandPool* transferCommandPool,
		aiNode* node, 
		const aiScene* scene, 
		std::vector<uint16_t> matToTex);

	Vulkan_Mesh* LoadMesh(
		VkPhysicalDevice newPhysicalDevice, 
		VkDevice newDevice, 
		VkQueue transferQueue, 
		VkCommandPool transferCommandPool,
		aiMesh* mesh, 
		const aiScene* scene, 
		std::vector<uint16_t> matToTex);

	std::vector<std::string>* getChildren() { return &Children; }
	bool getState() { return DESTROYED; }
	void makeInstance() { MODEL_INSTANCE = true; }

	void destroyModel();
	

private:

	std::vector<std::string> Children;

	bool MODEL_INSTANCE = false;
	bool DESTROYED = false;


	std::vector<Vulkan_Mesh> MeshList;



	std::vector<int> TextureMap;

};

