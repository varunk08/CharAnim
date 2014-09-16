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
	glm::vec3 mWorldPosition;
	glm::vec3 mUp;
	glm::vec3 mCOI;
	glm::vec3 mModelTransformation;
	Eigen::Matrix4f mViewTransformation;
	
	
	//Member functions
	Camera();
	Camera(glm::vec3 pos, glm::vec3 COI);
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	void SetPerspectiveProjection(float fovydeg, float aspect, float near, float far);
	glm::mat4 GetProjectionMatrix();
	void UpdateCamTheta(double dTheta);
	void UpdateCamPhi(double dPhi);
	void SetViewTransform();
	glm::mat4 GetViewTransform();
	~Camera();

private:
	glm::mat4 mProjectionTransformation;
	glm::mat4 mViewTransform;
	double theta;
	double phi;
	glm::vec3 viewVec;
};
Camera::Camera()
{
}
Camera::Camera(glm::vec3 pos, glm::vec3 COI)
{
	this->mWorldPosition = pos;
	this->mCOI = COI;
	//calculate up vector
	viewVec = mCOI - mWorldPosition;
	glm::vec3 camX = glm::cross(viewVec, glm::vec3(0.0f, 1.0f, 0.0f));
	this->mUp = glm::cross(viewVec, camX);
	glm::normalize(this->mUp);
	SetViewTransform();

	//calculate theta and phi based on position
	//create coordinate system at COI using lookat()

}
void Camera::SetViewTransform()
{
	mViewTransform = glm::lookAt(mWorldPosition, mCOI, mUp);
}
glm::mat4 Camera::GetViewTransform()
{
	return this->mViewTransform;
}
void Camera::SetPerspectiveProjection(float fovydeg, float aspect, float near, float far)
{
	mProjectionTransformation = glm::perspective(fovydeg,aspect,near,far);
}
glm::mat4 Camera::GetProjectionMatrix()
{
	return this->mProjectionTransformation;
}
void Camera::UpdateCamTheta(double dTheta)
{
	//glm::rotate(mModelTransformation, )
}
void Camera::UpdateCamPhi(double dPhi)
{

}

Camera::~Camera()
{
}

#endif