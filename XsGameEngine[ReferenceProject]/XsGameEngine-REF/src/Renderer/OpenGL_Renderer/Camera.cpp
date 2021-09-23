#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;

	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = 1.0f;
	turnSpeed = 4.0f;
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys,float scroll, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		position += front * moveSpeed * scroll * deltaTime;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed * scroll * deltaTime;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed * scroll * deltaTime;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed * scroll * deltaTime;
	}

	
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return front;
}

glm::mat4 Camera::calculateViewMatrix()
{
	glm::mat4 Calc = lookAt(position, position + front, up);
	shaders->SetViewMatrix(Calc);
	return Calc;
	
}

void Camera::setCamera(GLfloat xChange, GLfloat yChange, float deltaTime)
{

	yaw += xChange * turnSpeed * deltaTime;
	pitch += yChange * turnSpeed * deltaTime;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

Camera::~Camera()
{
}