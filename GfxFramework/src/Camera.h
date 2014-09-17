#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL\glew.h>
#include <iostream>
#include <Eigen\Dense>
#include <math.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
	void UpdateCamRotation(float dTheta, float dPhi);
	void UpdateViewDistance(float dDist);
	void SetViewTransform();
	glm::mat4 GetViewTransform();
	~Camera();

private:
	glm::mat4 mProjectionTransformation;
	glm::mat4 mViewTransform;
	glm::vec3 viewVec;
	glm::vec3 camX;
};
Camera::Camera()
{
}
Camera::Camera(glm::vec3 pos, glm::vec3 COI)
{
	this->mWorldPosition = pos;
	this->mCOI = COI;
	//calculate up vector
	viewVec = mWorldPosition - mCOI;
	SetViewTransform();

}
void Camera::SetViewTransform()
{
	camX = glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), viewVec);
	this->mUp = glm::cross(viewVec, camX);
	glm::normalize(this->mUp);
	this->mViewTransform = glm::lookAt(mWorldPosition, mCOI, mUp);
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
void Camera::UpdateCamRotation(float dTheta, float dPhi)
{
	viewVec = glm::rotate(viewVec, dTheta, glm::vec3(0.0f, 1.0f, 0.0f) ); //change to cam up/ right
	viewVec = glm::rotate(viewVec,dPhi, camX );
	//std::cout << "rot: "<< glm::to_string(viewVec)<<std::endl;
	mWorldPosition = mCOI + viewVec;
	SetViewTransform();
}
void Camera::UpdateViewDistance(float dDist)
{
	if(dDist > 0  && viewVec.length() < 100.0f)
	{
			viewVec = 1.2f * viewVec;
	}
	else if( dDist < 0 && viewVec.length() >0.5f )
	{
		viewVec = 0.8f * viewVec;
	}
	mWorldPosition = mCOI + viewVec;
	SetViewTransform();
}

Camera::~Camera()
{
}

#endif