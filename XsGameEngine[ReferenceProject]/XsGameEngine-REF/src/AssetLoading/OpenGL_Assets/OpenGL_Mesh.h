#pragma once
//#include <GLEW/glew.h>
#include "Glad/glad.h"
#include <stdint.h>

class OpenGL_Mesh
{
public:
	OpenGL_Mesh();
	~OpenGL_Mesh() = default;

	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	
	uint32_t getVertexCount() { return 0; };
	uint32_t getIndexCount() { return IndexCount; };
	
	void renderMesh();

	//GLuint getIndexCount();

private:

	GLuint VAO, VBO, IBO;
	uint32_t IndexCount{};

};

