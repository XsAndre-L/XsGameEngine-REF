#pragma once
#include "../Entity.h"



class Light : public Entity
{
public:
	Light();
	Light(
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity
	);

	struct LightInfo
	{
		alignas(16) glm::vec3 colour;			//12
		alignas(4) GLfloat ambientIntensity;		//4
		alignas(4) GLfloat diffuseIntensity;		//4
	}light;

	//void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation);
	LightInfo getLightInfo() { return light; };

	~Light();

	
	
};

