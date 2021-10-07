#include "Entity.h"

Entity::Entity()
{
	Position = { 0.0f, 0.0f, 0.0f };
	//meshList = newMeshList;
	Matrix.transformMatrix = glm::mat4(1.0f);
}

const glm::mat4 Entity::getMatrix()
{
	return Matrix.transformMatrix;
}

#pragma region Transformation
//Needs to happen every time main runs to update position for all non static obj
void Entity::updateMatrix()
{
	//glm::mat4 model(1.0f);
	glm::mat4 newMatrix(1.0f);

	if (!LocalTransform) {

		GLfloat angle = (Rotation.x + Rotation.y + Rotation.z);
		if (angle > 0.0f) {
			newMatrix = glm::scale(glm::mat4(1.0f), Scale) * glm::translate(glm::mat4(1.0), Position) * glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(Rotation.x / 360.0f, Rotation.y / 360.0f, Rotation.z / 360.0f));
		}
		else {
			newMatrix = glm::scale(glm::mat4(1.0f), Scale) * glm::translate(glm::mat4(1.0), Position) * glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}
	else {
		//newMatrix = glm::rotate(newMatrix, glm::radians())
		newMatrix = glm::rotate(newMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
		newMatrix = glm::rotate(newMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
		newMatrix = glm::rotate(newMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
		newMatrix = glm::translate(newMatrix, Position); 
		newMatrix = glm::scale(newMatrix, Scale);
	}


	if (Rotation.x > 360.0f) {
		Rotation.x -= 360.0f;
	}
	else if (Rotation.x < 0.0f) {
		Rotation.x += 360.0f;
	}

	if (Rotation.y > 360.0f) {
		Rotation.y -= 360.0f;
	}
	else if (Rotation.y < 0.0f) {
		Rotation.y += 360.0f;
	}

	if (Rotation.z > 360.0f) {
		Rotation.z -= 360.0f;
	}
	else if (Rotation.z < 0.0f) {
		Rotation.z += 360.0f;
	}

	setMatrix(newMatrix);
}

void Entity::resetMatrix()
{
	LocalTransform = false;
	Position = { 0.0f , 0.0f , 0.0f };
	Rotation = { 0.0f , 0.0f , 0.0f };
	Scale = { 1.0f , 1.0f , 1.0f };

	updateMatrix();
}

void Entity::setMatrix(glm::mat4 newMatrix)
{
	Matrix.transformMatrix = newMatrix;
}

//--Set Transform
void Entity::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	Position = { x , y , z };
	updateMatrix();
}

void Entity::setRotation(GLfloat x, GLfloat y, GLfloat z)
{
	Rotation = { x , y , z };
	updateMatrix();
}

void Entity::setScale(GLfloat x, GLfloat y, GLfloat z)
{
	Scale = { x , y , z };
	updateMatrix();
}
//---

//--Transforming
void Entity::transformPosition(GLfloat x, GLfloat y, GLfloat z)
{
	Position = { Position.x + x , Position.y + y , Position.z + z };
	updateMatrix();
}

void Entity::transformRotation(GLfloat x, GLfloat y, GLfloat z)
{
	Rotation = { Rotation.x + x , Rotation.y + y , Rotation.z + z };
	updateMatrix();
}

void Entity::transformScale(GLfloat x, GLfloat y, GLfloat z)
{
	Scale = { Scale.x + x , Scale.y + y , Scale.z + z };
	updateMatrix();
}
//---

#pragma endregion
