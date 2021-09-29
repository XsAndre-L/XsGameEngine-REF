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

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void destroyModel();

	
	void LoadMaterials(const aiScene* Scene);
private:

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	

	std::vector<OpenGL_Mesh*> meshList;
	std::vector<OpenGL_Texture*> textureList;
	std::vector<unsigned int> meshToTex;

};

