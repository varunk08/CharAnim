#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <list>
#include <Eigen\Dense>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Cube.h"

using namespace Eigen;
const glm::vec3 NODE_LENGTH(0.0f, 1.0f, 0.0f);
class Node
{
public:
	//Properties
	glm::vec3 mPosition; //position in object coordinates. position is same as joint position
	float mWAngle;
	float mLength;
	Node* mChild;	
	glm::mat4 mModelTransform;
	Cube *mDrawable; //Drawable
    bool mTranslate;
	glm::mat4 mRotationMat; //model space rotation matrix
	glm::vec3 mEndPos;

	//Methods
	Node(glm::vec3 pos);
	void SetShader(GLint shaderID);
	void Render();
	void UpdateChildTransform(glm::mat4 newRotation);
	void SetChild(Node* childNode);
	void CreateNewChildNode();
	void SetRotation(glm::mat4 parentTrans, std::list<glm::mat4> rotList, std::list<float> angles); //must clear self transform and set new transform
	float* GetChildAngles();
	void SetTranslate(bool isChild);
	glm::vec3 GetWorldPosition();
	void SetAngle(float angle);
	float GetAngle();
	
	~Node();

private:

};


#endif