#include "Light.h"

Light::Light()
{
	light.colour = glm::vec3(0.0f, 1.0f, 1.0f);
	light.ambientIntensity = 0.2f;
	light.diffuseIntensity = 0.8f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	light.colour = glm::vec3(red, green, blue);
	light.ambientIntensity = aIntensity;
	light.diffuseIntensity = dIntensity;
}

Light::~Light()
{
}
