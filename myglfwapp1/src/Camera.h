#ifndef _CAMERA_H
#define _CAMERA_H
#include <iostream>
#include <glm\glm.hpp>

class Camera
{
public:
	//Properties
	glm::mat4 projectionTransform;
	glm::mat4 modelTransform;
	glm::vec3 cameraPosition;
	glm::mat4 viewTransform;
	//Member Functions
	Camera();
	void SetPosition(glm::vec3 newPos);
	void SetProjectionMatrix(glm::mat4 projMatrix);
	void SetViewTransform(glm::vec3 Eye, glm::vec3 COI, glm::vec3 Up);
	glm::mat4 GetViewTransform();

	~ Camera();

private:

};

Camera:: Camera()
{
}

void Camera::SetPosition(glm::vec3 newPos)
{
	this->cameraPosition = newPos;
}
void Camera::SetProjectionMatrix(glm::mat4 projMatrix)
{
	this->projectionTransform = projMatrix;
}
 Camera::~ Camera()
{
}

#endif