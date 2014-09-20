#ifndef _IKSOLVER_H
#define _IKSOLVER_H
#include <iostream>
#include <vector>
#include <list>
#include <Eigen\Dense>
#include <glm\glm.hpp>
#include "Node.h"
using namespace Eigen;

class IKSolver
{
public:
	//Properties
	std::vector<float> mAngles;
	std::vector<Vector3f> mJointPositions;
	Node* mIKArm;
	int mNumLinks;
	Vector3f mRootWPosition;
	Vector3f mTargetPosition;
	Vector3f mRotationAxis;
	Vector3f mEndPosition;
	MatrixXf mJacobian;

	//Methods
	IKSolver(Node* ikArm, int numlinks);
	void IKUpdate();
	void SetTargetPosition(Vector3f target);
	std::list<glm::mat4> GetUpdatedAngles();
	Vector3f GetEndPos();
	
	~IKSolver();

private:
	void PopulateData();

};

IKSolver::IKSolver(Node* ikArm, int num)
{
	this->mIKArm = ikArm;
	this->mNumLinks = num;
	this->SetTargetPosition(Vector3f(0,0,0));
	PopulateData();
}

void IKSolver::IKUpdate()
{
	float delE = 0.0f;
	Vector3f endEffPos = GetEndPos();
	//Find current EndEffectorPos
	while(delE > 0.1f){
		Vector3f endEffPos = GetEndPos();
		
	}
	//Calculate delS = Targetpos - EndEffectorPos
}
/*
Return position of the end effector in world space
*/
Vector3f IKSolver::GetEndPos()
{
	Node* endNode = mIKArm;
	int index = 1;
	while(index < mNumLinks)
	{
		endNode = mIKArm->mChild;
		index ++;
	}

	glm::vec3 endpos(0,0,0), modelDir(0,1,0);
	modelDir = glm::vec3(endNode->mRotationMat * glm::vec4(modelDir,0.0));
	endpos = endNode->GetWorldPosition() + modelDir;
	std::cout << "End effector pos: " << glm::to_string(endpos) << std::endl;
	return Vector3f(glm::value_ptr(endpos));
	
}
void IKSolver::PopulateData()
{
	Node* temp = mIKArm;
	mAngles.clear();
	for(int i =0; i < mNumLinks; i++)
	{
		mAngles.push_back(temp->GetAngle());
		mJointPositions.push_back(Vector3f(glm::value_ptr(temp->GetWorldPosition())));

		std::cout << "Link "<<i<<": \n"<<"Angle: "<<mAngles[i]<<"\n"<<"Position:\n"<<mJointPositions[i]<<std::endl;
		temp = temp->mChild;
	}
	IKUpdate();
}
void IKSolver::SetTargetPosition(Vector3f pos)
{
	this->mTargetPosition = pos;
}
IKSolver::~IKSolver()
{
}
#endif