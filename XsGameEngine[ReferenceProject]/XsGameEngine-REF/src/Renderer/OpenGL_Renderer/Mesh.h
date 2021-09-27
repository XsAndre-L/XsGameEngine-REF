#pragma once
#include <GLEW/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh() = default;

	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	
	
	void renderMesh();

	GLuint getIndexCount();

private:

	GLuint VAO, VBO, IBO;
	GLuint IndexCount;

};

