#ifndef _PLANE_H
#define _PLANE_H

#include <iostream>
#include <Eigen\Dense>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
//#include "Node.h"

using namespace Eigen;

/*Creates a plane with dimensions from (1,1) to (-1,-1) on the X-Z plane centered around the origin */
class Plane 
{
public:
	//Properties
	glm::vec3 mPosition;
	GLuint mVertexBuf;
	GLuint mVAO;
	Plane(glm::vec3 pos);
	void SetGeometry();
	void InitBuffers();
	void Render();
	void SetShader(GLuint shaderID);
	void SetModelTransform(glm::mat4 transform);
	glm::mat4 GetModelTransform();
	~Plane();

private:
	GLuint mIndexBuf;
	GLuint mPositionAttr;
	GLuint mColorAttr;
	GLuint mTexCoordAttr;
	glm::mat4 mModelTransform;
};

Plane::Plane(glm::vec3 pos)
{
	this->mPosition = pos;
	
}
void Plane::SetShader(GLuint shaderID)
{
	glUseProgram(shaderID);
	mPositionAttr = glGetAttribLocation(shaderID, "position");
	this->InitBuffers();
	glUseProgram(0);
}
void Plane::SetModelTransform(glm::mat4 transform)
{
	mModelTransform = transform;
}
glm::mat4 Plane::GetModelTransform()
{
	return this->mModelTransform;
}
void Plane::InitBuffers()
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVertexBuf);
	glGenBuffers(1, &mIndexBuf);
/*	GLfloat vertexData[] = {
		-1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f
	};
*/
	GLfloat vertexData[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	GLuint indexData[] = { 
		0,1,2,
		0,2,3
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexData),indexData,GL_STATIC_DRAW);
	glEnableVertexAttribArray(mPositionAttr);
	glVertexAttribPointer(mPositionAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
void Plane::Render()
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	
}
Plane::~Plane(){
	glDeleteVertexArrays(1, &mVAO);
}
#endif