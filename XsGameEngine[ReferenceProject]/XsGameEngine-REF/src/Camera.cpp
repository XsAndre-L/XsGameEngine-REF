#include "Camera.h"


// Making a camera with defaul position and rotation
Camera::Camera()
{
	yaw = -90.0f;
	pitch = 0.0f;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
}

// Making a camera with a new position and rotation
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


// Calculating the view Matrix for the camera
bool Camera::setAsActive(glm::mat4* currentViewMatrix)
{
	if(currentViewMatrix != nullptr)
	{
		viewMatrix = currentViewMatrix;
		return true;
	}
	
	return false;
}

// change the direction values and then run the update function to calculate the new direction
void Camera::setCamera(bool cursorMode, GLfloat xChange, GLfloat yChange, float deltaTime)
{
	//if cursor mode this means cursor is active and we dont want to move
	if (cursorMode != false) { return; }
	
	yaw += xChange * turnSpeed *deltaTime;// *mouseSensitivity;
	pitch += yChange * turnSpeed *deltaTime;// *mouseSensitivity;

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

// WASD movement Code
void Camera::checkInputs(bool cursorMode, bool* keys, float scroll, GLfloat deltaTime)
{
	//if cursor mode this means cursor is active and we dont want to move
	if (cursorMode != false) { return; }
	glm::vec3 newPos = glm::vec3(0.0f,0.0f,0.0f);
	//move forward
	if (keys[GLFW_KEY_W])
	{
		newPos += front;
	}
	//move back
	if (keys[GLFW_KEY_S])
	{
		newPos -= front;
	}
	//move left
	if (keys[GLFW_KEY_A])
	{
		newPos -= right;
	}
	//move right
	if (keys[GLFW_KEY_D])
	{
		newPos += right;
	}

	position += (newPos * moveSpeed * scroll) * deltaTime;
}

//update camera direction
void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	//Updating the given view Matrix
	if (viewMatrix != nullptr) {
		glm::mat4 newViewMatrix = lookAt(position, position + front, up);
		*viewMatrix = newViewMatrix;
	}
}
