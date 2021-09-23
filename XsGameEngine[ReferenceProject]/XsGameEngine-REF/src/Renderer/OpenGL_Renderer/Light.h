#pragma once
#include <GLEW/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(
		GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat aIntensity, GLfloat dIntensity
	);
	
	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation);

	~Light();
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity{1.0f};
	GLfloat diffuseIntensity{ 1.0f };
};

