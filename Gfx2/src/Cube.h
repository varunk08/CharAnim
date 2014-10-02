#ifndef _CUBE_H
#define _CUBE_H

#include <iostream>
#include <Eigen\Dense>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <Eigen/Geometry>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace Eigen;

/*Creates a Cube with dimensions from (-0.5, -0.5, -0.5) to (0.5,0.5,0.5) on the X-Z Cube centered around the origin */
class Cube 
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	//Properties
	glm::vec3 mPosition;//doing nothing with position now. using model transform directly
	GLuint mVertexBuf;
	GLuint mVAO;
	glm::mat4 mModelTransform;
	glm::mat3 mNormalMatrix;
	float ka, kd, ks;

	//Methods
	Cube(glm::vec3 pos);
	void SetGeometry();
	void InitBuffers();
	void Render(glm::mat4 viewMatrix);
	void SetShader(GLuint shaderID);
	void SetModelTransform(glm::mat4 transform);
	glm::mat4 GetModelTransform();
	~Cube();

private:
	GLuint mIndexBuf;
	GLuint mColBuf;
	GLuint mNormalBuf;
	GLuint mColAttr;
	GLuint mPositionAttr;
	GLuint mColorAttr;
	GLuint mNormalAttr;
	GLuint mTexCoordAttr;
	GLuint mModelTransAttr;
	GLuint mNormalMatrixAttr;
	GLuint kaloc, ksloc, kdloc;
};


#endif