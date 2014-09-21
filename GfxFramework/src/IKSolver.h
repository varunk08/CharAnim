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
	float delE = 10000;
	Vector3f endEffPos = GetEndPos();
	Vector3f rotAxis(0, 0, 1);
	//Find current EndEffectorPos
	
		//Vector3f endEffPos = GetEndPos();
		Vector3f E = mTargetPosition - endEffPos;
		delE = E.norm();
		MatrixXf jacobian(E.rows(), mNumLinks);
		jacobian.setZero();
		//compute jacobian
		/*For each joint get the populated data*/
		for (int i = 0; i < mNumLinks; ++i)
		{
			Vector3f diff = endEffPos - mJointPositions[i];
			Vector3f elem = rotAxis.cross(diff);
			jacobian(0, i) = elem.x;
			jacobian(1, i) = elem.x;
			jacobian(2, i) = elem.x;
			
		}
		std::cout << jacobian << std::endl;
		MatrixXf jtj = jacobian.transpose();
		MatrixXf pJ = 
		float d = jjt.determinant();
		if (!d){
			invJ = jacobian.transpose();
			std::cout << "fallback to jacobian transpose!\n";
		}
		else{
			invJ = jjt.inverse()*jacobian.transpose();
			std::cout << "jacobian pseudo-inverse!\n";
		}

	
	
}
/*
Return position of the end effector in world space by going through the tree
*/
Vector3f IKSolver::GetEndPos()
{
	Node* endNode = mIKArm;
	int index = 0;
	while(index < mNumLinks)
	{
		if(endNode->mChild)
		{
			endNode = endNode->mChild;
		}
		index ++;
	}
	//std::cout<< "\nEnd effector pos: " << glm::to_string(endNode->mEndPos) << std::endl;
	return Vector3f(glm::value_ptr(endNode->mEndPos));
	
}
void IKSolver::PopulateData()
{
	Node* temp = mIKArm;
	mAngles.clear();
	for(int i =0; i < mNumLinks; i++)
	{
		mAngles.push_back(temp->GetAngle());
		mJointPositions.push_back(Vector3f(glm::value_ptr(temp->GetWorldPosition())));
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