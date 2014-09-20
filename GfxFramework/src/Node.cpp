#include "Node.h"
/***
Position is in object or model coordinates
***/
Node::Node(glm::vec3 position)
{
	this->mPosition = position;
	mDrawable = new Cube(position);
	mChild = nullptr;
	mModelTransform = glm::mat4(1.0f);
	mRotationMat = glm::mat4(1.0f);
	mTranslate = false;
	this->mLength = glm::length(NODE_LENGTH);
}
void Node::CreateNewChildNode()
{
	//create a new node
	Node* child = new Node(glm::vec3(0.0f, 0.0f, 0.0f));
	child->SetAngle(0.0f);
	child->SetTranslate(true);
	//translate it to be at top end of parent node and then multiply with parent's model transform
	child->ChainModelTransform(
								this->mModelTransform *
								glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,1.0f,0.0f))
							);
	this->mChild = child;

}
void Node::SetTranslate(bool isChild)
{
	this->mTranslate = isChild;
}
void Node::SetChild(Node* childNode)
{
	this->mChild = childNode;
}
void Node::SetShader(GLint shaderID)
{
	mDrawable->SetShader(shaderID);
	if(mChild)
	{
		mChild->SetShader(shaderID);
	}
}

void Node::ChainModelTransform(glm::mat4 transform)
{
	this->mPosition = glm::vec3(transform * glm::vec4(this->mPosition, 1.0f));
	this->mModelTransform = transform;
	this->mDrawable->SetModelTransform(transform);
	if(mChild)
	{
		mChild->ChainModelTransform(this->mModelTransform * mChild->mModelTransform);
	}
}
void Node::Render()
{
	mDrawable->Render();
	if(mChild) {
		mChild->Render();
	}
}
void Node::SetRotation(std::list<glm::mat4> rotList)
{
	//list has rotation pushed in this order: node n (end), node n-1,.., node 1.
	//pop the list for self then send the remanining to child
	//push_back list, pop_back list
	glm::mat4 newRotTrans;
	if( !rotList.empty() )
	{
		std::cout << "Rot list pop " << rotList.size() << std::endl;
		newRotTrans = rotList.back();
		rotList.pop_back();
	}
	this->mRotationMat = newRotTrans;
	// construct new model transformation
	glm::mat4 newModelTransform;
	if(mTranslate)
	{
		std::cout<< "Child: " <<mTranslate<<std::endl;
		newModelTransform = glm::translate(glm::mat4(1.0f), NODE_LENGTH);
	}
	newModelTransform = newModelTransform * newRotTrans;
	mModelTransform = newModelTransform; //update self model transform to new rot matrix
	if(mChild)
	{
		mChild->SetRotation(rotList); //refresh all children to have the updated rot matrix
	}

	ChainModelTransform(mModelTransform); // transform all children hierarchically

}
glm::vec3 Node::GetWorldPosition()
{
	return glm::vec3(this->mModelTransform * glm::vec4(this->mPosition, 1.0f));
}
void Node::SetAngle(float angle)
{
	this->mWAngle = angle;
}
float Node::GetAngle()
{
	return this->mWAngle;
}
Node::~Node()
{
}