#include "OpenGL_MeshModel.h"


void OpenGL_MeshModel::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
		return;
	}
	else {
		printf("Model (%s) loaded successfully: %s", fileName.c_str());
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void OpenGL_MeshModel::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void OpenGL_MeshModel::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	OpenGL_Mesh* newMesh = new OpenGL_Mesh();
	newMesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex - 1); // Note - 1 because we need index, otherwise it returns size I guess
}

void OpenGL_MeshModel::LoadMaterials(const aiScene* scene)
{

	textureList.resize(scene->mNumMaterials);	
	printf("NUM Materials %d\n", scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				//int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data);// .substr(idx + 1);

				std::string texPath = std::string("Textures/") + fileName;

				textureList[i] = new OpenGL_Texture(texPath.c_str());
				printf(texPath.c_str());
				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", fileName);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new OpenGL_Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}

void OpenGL_MeshModel::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) 
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->renderMesh();
	}
}

void OpenGL_MeshModel::destroyModel()
{
	printf("CLEARING");
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			//printf("Deleting mesh %d\n", i);
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			//printf("Deleting Texture %d\n", i);
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}




