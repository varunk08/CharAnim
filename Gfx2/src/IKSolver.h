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
	std::list<std::vector<float> > mAngles;
	std::list<Vector3f> mJointPositions;
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
	std::vector<Vector3f> rotAxes;
	Vector3f rotAxisX;
	Vector3f rotAxisY;
	Vector3f rotAxisZ;
};

IKSolver::IKSolver(Node* ikArm, int num)
{
	this->mIKArm = ikArm;
	this->mNumLinks = num;
	this->SetTargetPosition(Vector3f(0,0,0));
	rotAxisX = Vector3f(1, 0, 0);
	rotAxisY = Vector3f(0, 1, 0);
	rotAxisZ = Vector3f(0, 0, 1);
	rotAxes.push_back(rotAxisX);
	rotAxes.push_back(rotAxisY);
	rotAxes.push_back(rotAxisZ);
	PopulateData();
}

void IKSolver::IKUpdate()
{

	Vector3f endEffPos = GetEndPos();
	//Vector3f rotAxis(0, 0, 1);
	//std::cout<<"End Effector pos: "<< endEffPos<<std::endl;
	
	//Find current EndEffectorPos
	
		//Vector3f endEffPos = GetEndPos();
		Vector3f E = mTargetPosition - endEffPos;
		//std::cout<<"E norm: "<<E.norm()<<std::endl;
		if(E.norm() <= 0.1f) return;
		MatrixXf jacobian(E.rows(), 3 * mNumLinks);
		jacobian.setZero();
		//compute jacobian
		/*For each joint get the populated data*/
		for (int i = 0; i < mNumLinks; ++i)
		{
			Vector3f jointPos = mJointPositions.back(); //child child parent
			mJointPositions.pop_back();
			Vector3f diff = jointPos - endEffPos;
			for (int axes = 0; axes < 3; axes++)
			{
				Vector3f elem = diff.cross(rotAxes[axes]);
				for (int row = 0; row < 3; row++)
				{
					jacobian(row, (3 *i) + axes) = elem.coeff(row);// + 0.000001f;
				}
			}
			
			
		}
		//std::cout << jacobian << std::endl;
		Eigen::JacobiSVD<MatrixXf> svd (jacobian,ComputeThinU|ComputeThinV);
		
		VectorXf dTheta = svd.solve(E);
		

		std::cout <<"dTheta: "<<dTheta<<std::endl;
	
		//update angles
		//create rotation matrices and angles and update node transforms
		std::list<glm::mat4> rotMatList;
		std::list<std::vector<float> > angleList;
		unsigned int j= (3 * mNumLinks) -1;
		/*mAngles is Parent last (the correct order). dTheta is child last*/
		for (std::list<std::vector<float> >::iterator it=mAngles.begin(); it != mAngles.end(); ++it)
		//for(int j=0; j<mNumLinks;++j)
		{
			it->at(2) +=  dTheta[j] * 10.0f;
			it->at(1) += dTheta[--j] * 10.0f;
			it->at(0) += dTheta[--j] * 10.0f;
			
			//std::cout<<"j: "<<j<<"  "<<dTheta[j]<<std::endl;
			//if( mAngles[j] > 360.0f) mAngles[j] /= 360.0f;
			rotMatList.push_back(glm::rotate(glm::mat4(1.0f),it->at(2),glm::vec3(0.0,0.0,1.0))
				* glm::rotate(glm::mat4(1.0f), it->at(1), glm::vec3(0.0, 1.0, 0.0))
				*glm::rotate(glm::mat4(1.0f), it->at(0), glm::vec3(1.0, 0.0, 0.0)));
			angleList.push_back(*it);
			j--;
			
		}
		mIKArm->SetRotation(glm::mat4(1.0f),rotMatList, angleList);

		Node* temp = mIKArm;
		mJointPositions.clear();
		for(int i =0; i < mNumLinks; i++)
		{
			mJointPositions.push_front( Vector3f(temp->mPosition.x, temp->mPosition.y,temp->mPosition.z));
			//std::cout<<i<<" glm position: "<<glm::to_string(temp->mPosition)<<std::endl;
			//std::cout<<i<<" Joint position: "<<mJointPositions.front()<<std::endl;
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
		mAngles.push_front(temp->GetAngle());
		mJointPositions.push_front(Vector3f(glm::value_ptr(temp->GetWorldPosition())));
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