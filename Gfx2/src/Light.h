#ifndef _LIGHT_H
#define _LIGHT_H
#include <iostream>
#include <vector>
#include <list>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>
class Light
{
public:
	//Properties
	glm::vec3 mWorldPosition;
	glm::vec3 mDirection;
	glm::vec3 mAmbientColor;
	glm::vec3 mDiffuseColor;
	glm::vec3 mSpecutlarColor;


	//methods
	Light(glm::vec3 pos);
	void Update();
	void SetShader(GLuint shaderID);
	void SetAmbient(glm::vec4 ambientColor);
	~Light();

private:
	GLuint lightPosUnifLoc;
	GLuint lightDiffuseUnifLoc;
};

/*Only directional light now*/
Light::Light(glm::vec3 pos)
{
	this->mWorldPosition = pos;
	this->mDiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

/*Get location of unifs from shader*/
void Light::SetShader(GLuint shaderID)
{
	glUseProgram(shaderID);
	lightPosUnifLoc = glGetUniformLocation(shaderID, "lightpos");
	lightDiffuseUnifLoc = glGetUniformLocation(shaderID, "lightDiffuse");
}
/*Update light pos to shaders*/
void Light::Update()
{
	glUniform3fv(lightPosUnifLoc, 1, glm::value_ptr(this->mWorldPosition));
	glUniform3fv(lightDiffuseUnifLoc, 1, glm::value_ptr(this->mDiffuseColor));
}
Light::~Light()
{
}
#endif _LIGHT_H