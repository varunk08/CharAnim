#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL\glew.h>
#include <iostream>
#include <Eigen\Dense>
#include <math.h>
#define _USE_MATH_DEFINES
class Camera
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	Eigen::Vector3f mPosition;
	Eigen::Matrix4f mModelTransformation;
	Eigen::Matrix4f mViewTransformation;
	Eigen::Matrix4f mProjectionTransformation;
	
	//Member functions
	Camera();
	void SetPerspectiveProjection(float fovydeg, float aspect, float near, float far);
	~Camera();

private:

};

Camera::Camera()
{
}
void Camera::SetPerspectiveProjection(float fovydeg, float aspect, float near, float far)
{
	float fovr = fovydeg * M_PI / 180.0f;
	float tanhalffov = tan(fovr / 2.0f);
	//mProjectionTransformation(4,4);
	mProjectionTransformation.setZero();
	mProjectionTransformation(0,0) =  1.0f / (aspect * tanhalffov);//, 0, 0, 0,
	mProjectionTransformation(1,1) =  1.0f/ tanhalffov;//, 0, 0,
	mProjectionTransformation(2,2) = -(far + near)/(far - near);//, -1.0f,
	mProjectionTransformation(2,3) = -(2.0f * far * near)/(far - near);
	mProjectionTransformation(3,2) = -1.0f;
}
Camera::~Camera()
{
}

#endif