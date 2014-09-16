#ifndef _NODE_H
#define _NODE_H
#include <iostream>
#include <GL\glew.h>
#include <glm/glm.hpp>

class Node
{
public:
	//Properties
	glm::mat4 ModelTransform;
	
	//Member functions
	Node();
	void InitGeometry(); //create buffers and populate buffer data
	void SetModelTransform(glm::mat4 transform);
	void Render();
	void SetShader(GLuint shaderID);
	 ~Node();

private:

};

Node::Node()
{
}

Node::~Node()
{
}
#endif