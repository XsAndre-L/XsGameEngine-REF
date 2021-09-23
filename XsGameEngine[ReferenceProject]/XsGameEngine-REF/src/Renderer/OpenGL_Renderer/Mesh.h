#pragma once
#include <GLEW/glew.h>

class Mesh
{
public:
	Mesh();
	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	~Mesh();
	
	void renderMesh();

	GLuint getIndexCount();

private:

	GLuint VAO, VBO, IBO;
	GLuint IndexCount;

};

