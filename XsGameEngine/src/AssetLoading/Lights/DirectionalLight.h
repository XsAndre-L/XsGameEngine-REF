#pragma once

#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir
	);

	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~DirectionalLight();

	struct DirectionalLightInfo
	{
		LightInfo base;								//24
		alignas(16) glm::vec3 lightDirection;		//12
	}dlight;

private:

	Light light;
	//glm::vec3 direction;
};

