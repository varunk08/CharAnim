#ifndef _CUBE_H
#define _CUBE_H

#include <iostream>
#include <Eigen\Dense>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <Eigen/Geometry>
//#include "Node.h"

using namespace Eigen;

/*Creates a Cube with dimensions from (-0.5, -0.5, -0.5) to (0.5,0.5,0.5) on the X-Z Cube centered around the origin */
class Cube 
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	//Properties
	Vector3f mPosition;
	GLuint mVertexBuf;
	GLuint mVAO;
	Affine3f mModelTransform;

	Cube(Vector3f pos);
	void SetGeometry();
	void InitBuffers();
	void Render();
	void SetShader(GLuint shaderID);
	void SetModelTransform();
	Affine3f GetModelTransform();
	~Cube();

private:
	GLuint mIndexBuf;
	GLuint mColBuf;
	GLuint mColAttr;
	GLuint mPositionAttr;
	GLuint mColorAttr;
	GLuint mTexCoordAttr;
};

Cube::Cube(Vector3f pos)
{
	this->mPosition = pos;
	
}
void Cube::SetShader(GLuint shaderID)
{
	glUseProgram(shaderID);
	mPositionAttr = glGetAttribLocation(shaderID, "position");
	mColAttr = glGetAttribLocation(shaderID, "color");
	this->SetModelTransform();
	this->InitBuffers();
	glUseProgram(0);
}
void Cube::SetModelTransform()
{
	mModelTransform.setIdentity();
	mModelTransform.translate(Vector3f(0.0f,0.0f,-5.0f));
	std::cout<<mModelTransform.matrix()<< std::endl;
}
Affine3f Cube::GetModelTransform()
{
	return this->mModelTransform;
}
void Cube::InitBuffers()
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVertexBuf);
	glGenBuffers(1, &mIndexBuf);
	glGenBuffers(1, &mColBuf);

	GLfloat vertexData[] = {
		-0.5f, -0.5f, -0.5f, //A 0
		 0.5f, -0.5f, -0.5f, //B 1
		 0.5f, 0.5f, -0.5f, //C 2 
		-0.5f, 0.5f, -0.5f, //D 3
		-0.5f, -0.5f, 0.5f, //E 4
		 0.5f, -0.5f, 0.5f, //F 5
		 0.5f, 0.5f, 0.5f, //G 6
		-0.5f, 0.5f, 0.5f, //H 7
	};

	GLuint indexData[] = { 
		0, 1, 2,
		0, 2, 3,
		1, 5, 6,
		1, 6, 2,
		5, 4, 7,
		5, 7, 6,
		4, 0, 3,
		4, 3, 7,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7

	};
	GLfloat colorData[] = {
		1.0f,  0.0f, 0.0f,
		1.0f,  0.0f, 0.0f,
		1.0f,  0.0f, 0.0f,
		1.0f,  0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};


	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexData),indexData,GL_STATIC_DRAW);
	glEnableVertexAttribArray(mPositionAttr);
	glVertexAttribPointer(mPositionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, mColBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData,GL_STATIC_DRAW);
	glEnableVertexAttribArray(mColAttr);
	glVertexAttribPointer(mColAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
void Cube::Render()
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL);
	
}
Cube::~Cube(){
	glDeleteVertexArrays(1, &mVAO);
}
#endif