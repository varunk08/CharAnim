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
	mEndPos = glm::vec3 (NODE_LENGTH);
}
void Node::CreateNewChildNode()
{
	//create a new node
	Node* child = new Node(glm::vec3(0.0f, 0.0f, 0.0f));
	std::vector<float> angles; angles.push_back(0.0f); angles.push_back(0.0f); angles.push_back(0.0f);
	child->SetAngle(angles);
	child->SetTranslate(true);
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


void Node::Render()
{
	mDrawable->Render();
	if(mChild) {
		mChild->Render();
	}
}

/*angles used to set the angle*/
void Node::SetRotation(glm::mat4 parentTrans, std::list<glm::mat4> rotList, std::list<std::vector<float> > angles)
{
	//list has rotation pushed in this order: node n (end), node n-1,.., node 1.
	//pop the list for self then send the remanining to child
	//push_back list, pop_back list
	this->mPosition = glm::vec3(0.0, 0.0, 0.0f);
	this->mEndPos = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 newRotTrans;
	std::vector<float> angle = angles.back();
	this->mWAngle = angle;
	angles.pop_back();
	if( !rotList.empty() )
	{
		//std::cout << "Angle: " << angle  << std::endl;
		newRotTrans = rotList.back();
		rotList.pop_back();
	}
	else return;
	this->mRotationMat = newRotTrans;
	// construct new model transformation
	glm::mat4 newModelTransform(1.0f);
	if(mTranslate)
	{
		//std::cout<< "Child: "<<std::endl;
		newModelTransform = glm::translate(glm::mat4(1.0f), NODE_LENGTH);
	}
	mModelTransform = parentTrans *  newModelTransform * newRotTrans; //update self model transform to new rot matrix
	mDrawable->SetModelTransform(mModelTransform);
	this->mPosition = glm::vec3(mModelTransform  * glm::vec4(this->mPosition, 1.0f));
	this->mEndPos = glm::vec3(mModelTransform  * glm::vec4(this->mEndPos, 1.0f));
	std::cout << mTranslate << " \nPos after Chain transform: " << glm::to_string(mPosition)
		<< " End pos: " << glm::to_string(mEndPos) << std::endl;
	if(mChild)
	{
		mChild->SetRotation(mModelTransform, rotList, angles); //refresh all children to have the updated rot matrix
		
	}

}
glm::vec3 Node::GetWorldPosition()
{
	return this->mPosition;
}
void Node::SetAngle(std::vector<float> angle)
{
	this->mWAngle = angle;
}
std::vector<float> Node::GetAngle()
{
	return this->mWAngle;
}
Node::~Node()
{
}