#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp , GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, float scroll, GLfloat deltaTime);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();

	void setCamera(GLfloat xChange, GLfloat yChange, float deltaTime);
	
	void setShaders(Shaders* shadersPointer) { shaders = shadersPointer; }

	~Camera();
private:
	Shaders* shaders;

	glm::vec3 position{};
	glm::vec3 front{};
	glm::vec3 up{};
	glm::vec3 right{};
	glm::vec3 worldUp{};

	GLfloat yaw{};
	GLfloat pitch{};
	GLfloat roll{};

	GLfloat moveSpeed{};
	GLfloat turnSpeed{};

	void update();

};

