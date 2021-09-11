#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	light = Light();
	dlight.base = light.getLightInfo();
	//dlight.colour = light.getLightInfo().colour;
	//dlight.ambientIntensity = light.getLightInfo().ambientIntensity;
	//dlight.diffuseIntensity = light.getLightInfo().diffuseIntensity;
	dlight.lightDirection = glm::vec3(1.0f, 0.0f, 0.0f );
	//printf("%d\n", (int)sizeof(DirectionalLightInfo));
}

DirectionalLight::DirectionalLight(
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) 
{
	light = Light(red, green, blue, aIntensity, dIntensity);
	dlight.base = light.getLightInfo();
	//dlight.colour = light.getLightInfo().colour;
	//dlight.ambientIntensity = light.getLightInfo().ambientIntensity;
	//dlight.diffuseIntensity = light.getLightInfo().diffuseIntensity;
	dlight.lightDirection = glm::vec3(xDir, yDir, zDir);
}


//void DirectionalLight::UseLight(
//	GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
//	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
//{
//	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
//	glUniform1f(ambientIntensityLocation, ambientIntensity);
//
//	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
//	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
//}

DirectionalLight::~DirectionalLight()
{
}
