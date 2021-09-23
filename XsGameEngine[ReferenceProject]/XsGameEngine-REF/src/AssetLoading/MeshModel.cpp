#include "MeshModel.h"

/*MeshModel::MeshModel()
{

}*/

MeshModel::MeshModel(/*std::vector<VkMesh> newMeshList*/)
{
	meshList.clear();
}

size_t MeshModel::getMeshCount()
{
	return meshList.size();
}

VkMesh* MeshModel::getMesh(size_t index)
{
	if (index >= meshList.size())
	{
		throw std::runtime_error("Attrmpted to access invalid Mesh index!");
	}

	return &meshList[index];
}


void MeshModel::destroyModel()
{
	if (getState()) { return; }
	if (MODEL_INSTANCE) { this->~MeshModel(); return; }

	//printf("Mesh Count In Model : %d\n", getMeshCount());
	for (auto& mesh : meshList)
	{
		mesh.destroyBuffers();
	}
	DESTROYED = true;
}

MeshModel::~MeshModel()
{
}


std::vector<std::string> MeshModel::LoadMaterials(const aiScene* scene)
{	
	//Create 1:1 sized list of textures
	std::vector<std::string> textures;
	textures.resize(scene->mNumMaterials-2);
	//std::vector<std::string> textureList(0);

	for (size_t i = 0; i < textures.size(); i++)
	{
		aiMaterial* material = scene->mMaterials[i+2];

		textures[i] = "";

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				size_t idx = std::string(path.data).rfind("\\");

				std::string fileName = std::string(path.data).substr(idx + 1);
				printf(fileName.c_str());

				textures[i] = fileName;
			}
		}
		
	}
	return textures;
}

//Loads a tree of meshes (mesh parent with child meshes)
std::vector<VkMesh> MeshModel::LoadNode(VkPhysicalDevice* newPhysicalDevice, VkDevice* newDevice, VkQueue* transferQueue, VkCommandPool* transferCommandPool, aiNode* node, const aiScene* scene, std::vector<uint16_t> matToTex)
{
	//std::vector<VkMesh> meshList;
	// Go through each mesh at this node and create it, then add it to our meshList
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		//vkWaitSemaphores(*newDevice, renderFinished->at(*currentFrame), )
		
		meshList.push_back(
			LoadMesh(*newPhysicalDevice, *newDevice, *transferQueue, *transferCommandPool, scene->mMeshes[node->mMeshes[i]], scene, matToTex)
		);
	}
	
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		std::vector<VkMesh> newList = LoadNode(newPhysicalDevice, newDevice, transferQueue, transferCommandPool, node->mChildren[i], scene, matToTex);
		//meshList.insert(meshList.end(), newList.begin(), newList.end());
	}

	return meshList;
}

VkMesh MeshModel::LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, aiMesh* mesh, const aiScene* scene, std::vector<uint16_t> matToTex)
{

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// Resize vertex list to hold all vertices for mesh
	vertices.resize(mesh->mNumVertices);

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[i].pos = { mesh->mVertices[i].x, mesh->mVertices[i].y , mesh->mVertices[i].z };

		if (mesh->mTextureCoords[0])
		{
			vertices[i].tex = { mesh->mTextureCoords[0][i].x , mesh->mTextureCoords[0][i].y };

		}
		else {
			vertices[i].tex = { 0.0f , 0.0f };
		}

		if (mesh->mNormals) {
			vertices[i].norm = { mesh->mNormals[i].x ,mesh->mNormals[i].y ,mesh->mNormals[i].z };
			//printf("x: %d, y: %d, z: %d\n", mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		

		vertices[i].col = { 1.0f, 1.0f, 1.0f };

	}

	

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		//Go through face's indices and add to list
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create new mesh with details
	VkMesh newMesh = VkMesh(newPhysicalDevice, newDevice, transferQueue, transferCommandPool, &vertices, &indices, 0); //matToTex[mesh->mMaterialIndex]

	return newMesh;
}
