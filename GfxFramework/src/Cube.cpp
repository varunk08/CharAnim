#include "Cube.h"

Cube::Cube(glm::vec3 pos)
{
	this->mPosition = pos;
	
}
void Cube::SetShader(GLuint shaderID)
{
	glUseProgram(shaderID);
	mPositionAttr = glGetAttribLocation(shaderID, "position");
	mColAttr = glGetAttribLocation(shaderID, "color");
	mModelTransAttr = glGetUniformLocation(shaderID, "model");
	this->SetModelTransform(glm::mat4(1.0f));
	this->InitBuffers();
	glUseProgram(0);
}
void Cube::SetModelTransform(glm::mat4 transform)
{
	mModelTransform = transform;
}
glm::mat4 Cube::GetModelTransform()
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
		-0.25f, -0.0f, -0.25f, //A 0
		 0.25f, -0.0f, -0.25f, //B 1
		 0.25f, 1.0f, -0.25f, //C 2 
		-0.25f, 1.0f, -0.25f, //D 3
		-0.25f, -0.0f, 0.25f, //E 4
		 0.25f, -0.0f, 0.25f, //F 5
		 0.25f, 1.0f, 0.25f, //G 6
		-0.25f, 1.0f, 0.25f, //H 7
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
		1.0f,  0.0f, 1.0f,
		1.0f,  0.0f, 1.0f,
		1.0f,  0.0f, 1.0f,
		1.0f,  0.0f, 1.0f,
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
	glUniformMatrix4fv(mModelTransAttr,1, GL_FALSE, glm::value_ptr(mModelTransform));
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL);
	
}
Cube::~Cube(){
	glDeleteVertexArrays(1, &mVAO);
}