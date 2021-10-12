#pragma once
//#include <GLEW/glew.h>
#include "../../../vendor/Glad/glad.h"
#include <stb_image/stb_image.h>


class OpenGL_Texture
{
public:
	OpenGL_Texture();
	OpenGL_Texture(const char * fileLoc);
	~OpenGL_Texture();

	bool LoadTexture();
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	char* fileLocation;
};

