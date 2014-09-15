#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <Eigen\Dense>

using namespace Eigen;

class Node
{
public:
	//Properties
	Vector3f position;
	

	Node();
	void SetGeometry();
	void InitBuffers();
	void DrawNode();
	void SetShader();
	void SetModelTransform();
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