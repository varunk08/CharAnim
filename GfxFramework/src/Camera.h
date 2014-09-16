#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL\glew.h>
#include <iostream>
#include <Eigen\Dense>
#include <math.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#define _USE_MATH_DEFINES
class Camera
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Eigen::Vector3f mPosition;
	Eigen::Matrix4f mModelTransformation;
	Eigen::Matrix4f mViewTransformation;
	
	
	//Member functions
	Camera();
	void SetPerspectiveProjection(float fovydeg, float aspect, float near, float far);
	glm::mat4 GetProjectionMatrix();
	~Camera();

private:
	glm::mat4 mProjectionTransformation;

};

Camera::Camera()
{
}
void Camera::SetPerspectiveProjection(float fovydeg, float aspect, float near, float far)
{
	mProjectionTransformation = glm::perspective(fovydeg,aspect,near,far);
}
glm::mat4 Camera::GetProjectionMatrix()
{
	return this->mProjectionTransformation;
}
Camera::~Camera()
{
}

#endif