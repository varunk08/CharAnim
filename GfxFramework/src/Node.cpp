#include "Node.h"
/***
position is dummy now.
doing everything with model transform.
***/
Node::Node(glm::vec3 position)
{
	this->mPosition = position;
	mDrawable = new Cube(position);
	mChild = nullptr;
	mModelTransform = glm::mat4(1.0f);
}
void Node::CreateNewChildNode()
{
	//create a new node
	Node* child = new Node(glm::vec3(0.0f, 0.0f, 0.0f));
	//translate it to be at top end of parent node and then multiply with parent's model transform
	child->SetModelTransform(
								this->mModelTransform *
								glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,1.0f,0.0f))
								* glm::rotate(glm::mat4(1.0f), 20.0f, glm::vec3(0,0,-1.0f))
							);
	this->mChild = child;

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
void Node::UpdateChildTransform(glm::mat4 newRotation)
{
	
}
void Node::SetModelTransform(glm::mat4 transform)
{
	this->mModelTransform = transform;
	this->mDrawable->SetModelTransform(transform);
	if(mChild)
	{
		mChild->SetModelTransform(this->mModelTransform * mChild->mModelTransform);
	}
}
void Node::Render()
{
	mDrawable->Render();
	if(mChild) {
		mChild->Render();
	}
}
Node::~Node()
{
}