#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OpenGL_Mesh.h"
#include "OpenGL_Texture.h"
#include "../Entity.h"

class OpenGL_MeshModel : public Entity
{
public:
	OpenGL_MeshModel() = default;
	~OpenGL_MeshModel() = default;

	void LoadModel(/*const std::string& fileName*/const aiScene* scene);
	//void RenderModel();
	void destroyModel();
	std::vector<std::string>* getChildren() { return &Children; }
	size_t getMeshCount() { return meshList.size(); }
	OpenGL_Mesh* getMesh(size_t index) { return &meshList.at(index); }
	std::vector<std::string> LoadMaterials(const aiScene* Scene);

	const std::vector<uint32_t>* getMeshToTex() const { return &meshToTex; }
	void setMeshToTex(int MeshIndex, int TexID) { meshToTex.at(MeshIndex) = TexID; }
	void resizeMeshToTex(size_t SIZE) { meshToTex.resize(SIZE); }
	//std::vector<OpenGL_Texture*> textureList;
private:

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	
	std::vector<std::string> Children;

	std::vector<OpenGL_Mesh> meshList;
	
	std::vector<uint32_t> meshToTex;

};

