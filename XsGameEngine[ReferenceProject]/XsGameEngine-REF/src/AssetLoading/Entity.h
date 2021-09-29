#pragma once
//#include"../Renderer/Vulkan_Renderer/VulkanUtillities.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLM/ext/matrix_transform.hpp>

struct Matrix {
	glm::mat4 transformMatrix;
};

class Entity
{
public:
	Entity();

	const glm::mat4 getMatrix();
	void updateMatrix();

	void resetMatrix();
	void setMatrix(glm::mat4 newMatrix);

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setRotation(GLfloat x, GLfloat y, GLfloat z);
	void setScale(GLfloat x, GLfloat y, GLfloat z);

	void transformPosition(GLfloat x, GLfloat y, GLfloat z);
	void transformRotation(GLfloat x, GLfloat y, GLfloat z);
	void transformScale(GLfloat x, GLfloat y, GLfloat z);

	bool* getTransformType() { return &LocalTransform; };
	glm::vec3* getPosition() { return &Position; };
	glm::vec3* getRotation() { return &Rotation; };
	glm::vec3* getScale() { return &Scale; };

private:
	bool LocalTransform = false;
	glm::vec3 Position = { 0.0f , 0.0f , 0.0f };
	glm::vec3 Rotation = { 0.0f , 0.0f , 0.0f };
	glm::vec3 Scale = { 1.0f , 1.0f , 1.0f };

	Matrix Matrix;
};

