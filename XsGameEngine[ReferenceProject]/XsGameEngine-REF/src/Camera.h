#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera() = default;

	//Look Sensitivity
	float mouseSensitivity = 20;
	//Camera Speed
	GLfloat moveSpeed = 2.0f;
	GLfloat turnSpeed = 8.0f;

	
	bool setAsActive(glm::mat4* viewMatrix);

	void setCamera(bool cursorMode, GLfloat xChange, GLfloat yChange, float deltaTime);
	glm::vec3 getCamFront() { return front * -1.0f; }
	void checkInputs(bool cursorMode, bool* keys, float scroll, GLfloat deltaTime);

private:
	glm::mat4* viewMatrix = nullptr;

	glm::vec3 position{ glm::vec3(0.0f, 2.0f, 5.0f) };
	glm::vec3 front{};
	glm::vec3 up{};
	glm::vec3 right{};
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw{};
	GLfloat pitch{};
	GLfloat roll{};

	

	void update();
};

