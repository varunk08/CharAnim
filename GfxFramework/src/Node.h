#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <list>
#include <Eigen\Dense>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Cube.h"

using namespace Eigen;
const glm::vec3 NODE_LENGTH(0.0f, 1.0f, 0.0f);
class Node
{
public:
	//Properties
	glm::vec3 mPosition; //doing nothing with position now. using model transform directly
	Node* mChild;	
	glm::mat4 mModelTransform;
	Cube *mDrawable; //Drawable
    bool mTranslate;
	//Methods
	Node(glm::vec3 pos);
	void SetShader(GLint shaderID);
	void Render();
	void UpdateChildTransform(glm::mat4 newRotation);
	void ChainModelTransform(glm::mat4 transform);
	void SetChild(Node* childNode);
	void CreateNewChildNode();
	void SetRotation(std::list<glm::mat4> rotList); //must clear self transform and set new transform
	float* GetChildAngles();
	void SetTranslate(bool isChild);
	~Node();

private:

};


#endif