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

	Vector3f endEffPos = GetEndPos();
	Vector3f rotAxis(0, 0, 1);
	//Find current EndEffectorPos
	
		//Vector3f endEffPos = GetEndPos();
		Vector3f E = mTargetPosition - endEffPos;
		if(E.norm() < 0.2f) return;
		MatrixXf jacobian(E.rows(), mNumLinks);
		jacobian.setZero();
		//compute jacobian
		/*For each joint get the populated data*/
		for (int i = 0; i < mNumLinks; ++i)
		{
			Vector3f diff = endEffPos - mJointPositions[i];
			Vector3f elem = rotAxis.cross(diff);
			jacobian(0, i) = elem.x();
			jacobian(1, i) = elem.y();
			jacobian(2, i) = elem.z();
			
		}
		std::cout << jacobian << std::endl;
		MatrixXf jjt = jacobian.transpose() * jacobian;
		float d = jjt.determinant();
		if(!d){
			std::cout<<"Determinant 0"<<std::endl;
		}
		MatrixXf jjtinv = (jacobian * jacobian.transpose()).inverse();
		MatrixXf jpseudo = jacobian.transpose() * jjtinv;
		//jacobian.diagonal() = VectorXf(jacobian.diagonal() + VectorXf(0.0001f));
		Eigen::JacobiSVD<MatrixXf> svd (jacobian,ComputeThinU|ComputeThinV);
		VectorXf dTheta = svd.solve(E);
				
		//MatrixXf invj = jacobian.transpose();
		//VectorXf dTheta = invj * E;

		std::cout <<"dTheta: "<<dTheta<<std::endl;
	
		//update angles
		//create rotation matrices and angles and update node transforms
		std::list<glm::mat4> rotMatList;
		std::list<float> angleList;
		
		for(int j=0; j<mNumLinks;++j)
		{
			mAngles[j] += dTheta[j];
			if( mAngles[j] > 360.0f) mAngles[j] /= 360.0f;
			rotMatList.push_front(glm::rotate(glm::mat4(1.0f),mAngles[j],glm::vec3(0.0,0.0,1.0)));
			angleList.push_front(mAngles[j]);
			
		}
		mIKArm->SetRotation(glm::mat4(1.0f),rotMatList, angleList);

		Node* temp = mIKArm;
		for(int i =0; i < mNumLinks; i++)
		{
			mJointPositions[i] = Vector3f(glm::value_ptr(temp->GetWorldPosition()));
			temp = temp->mChild;
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
	mJointPositions.clear();
	for(int i =0; i < mNumLinks; i++)
	{
		mAngles.push_back(temp->GetAngle());
		mJointPositions.push_back(Vector3f(glm::value_ptr(temp->GetWorldPosition())));
		temp = temp->mChild;
	}
	//IKUpdate();
}
void IKSolver::SetTargetPosition(Vector3f pos)
{
	this->mTargetPosition = pos;
}
IKSolver::~IKSolver()
{
}
#endif