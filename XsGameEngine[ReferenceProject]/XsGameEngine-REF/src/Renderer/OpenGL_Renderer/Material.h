#pragma once
#include <GLEW/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	GLfloat getSpecularIntensity();
	GLfloat getShininess();

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

