#ifndef _LIGHT_H
#define _LIGHT_H
#include <iostream>
#include <vector>
#include <list>
#include <glm\glm.hpp>

class Light
{
public:
	//Properties
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec4 mAmbientColor;


	//methods
	Light(glm::vec3 pos, glm::vec3 dir);
	void SetAmbient(glm::vec4 ambientColor);
	~Light();

private:

};

Light::Light(glm::vec3 pos, glm::vec3 dir)
{

}

Light::~Light()
{
}
#endif _LIGHT_H