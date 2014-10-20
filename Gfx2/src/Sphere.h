#ifndef _SPHERE_H
#define _SPHERE_H
#define USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere
{
public:
	Sphere(int sub);
	~Sphere();
	void SetShader(GLuint shaderID);
	void SetModelTransform(glm::mat4 modelTrans);
	void InitBuffers();
private:
	std::vector<glm::vec3> icoVertices;
	std::vector<glm::vec3> icoColors;
	std::vector<glm::vec3> sphereVertices;
	std::vector<glm::vec3> sphereColors;
	std::vector<glm::vec3> sphereNormals;
	
	GLuint mPositionAttr;
	GLuint	mColAttr;
	GLuint	mNormalAttr;
	GLuint mModelTransAttr;
	GLuint mNormalMatrixAttr;

	
	void sphTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	void CreateIcosahedron();
	void refineIcoSphere(int subdivide);
	void divideTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, unsigned int count);
	
};

Sphere::Sphere(int subdivision)
{
	
		CreateIcosahedron();
		refineIcoSphere(subdivision);
	
}
void Sphere::SetShader(GLuint shaderID)
{
	glUseProgram(shaderID);
	//attributes
	mPositionAttr = glGetAttribLocation(shaderID, "position");
	mColAttr = glGetAttribLocation(shaderID, "color");
	mNormalAttr = glGetAttribLocation(shaderID, "normal");

	//model transform matrices
	mModelTransAttr = glGetUniformLocation(shaderID, "model");
	mNormalMatrixAttr = glGetUniformLocation(shaderID, "normalmatrix");

	//material properties
	/*kaloc = glGetUniformLocation(shaderID, "ka");
	kdloc = glGetUniformLocation(shaderID, "kd");
	ksloc = glGetUniformLocation(shaderID, "ks");*/

	this->SetModelTransform(glm::mat4(1.0f));
	this->InitBuffers();
	glUseProgram(0);
}
void Sphere::refineIcoSphere(int subdivide)
{
	sphereVertices.clear();
	sphereColors.clear();
	const int icoIndices[] = {
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};
	//take triangles of icosahedron
	for (int i = 0; i < 3 * 20; i += 3)
	{
		divideTriangle(icoVertices[icoIndices[i]], icoVertices[icoIndices[i + 1]], icoVertices[icoIndices[i + 2]], subdivide);
	}

}
void Sphere::divideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, unsigned int count)
{
	if (count > 0){
		glm::vec3 ab = glm::normalize(glm::mix(a, b, 0.5));
		glm::vec3 ac = glm::normalize(glm::mix(a, c, 0.5));
		glm::vec3 bc = glm::normalize(glm::mix(b, c, 0.5));

		divideTriangle(a, ab, ac, count - 1);
		divideTriangle(ab, b, bc, count - 1);
		divideTriangle(bc, c, ac, count - 1);
		divideTriangle(ab, bc, ac, count - 1);
	}
	else{
		sphTriangle(a, b, c);
	}
}
void Sphere::sphTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	//right now sphere vertices dont have indices - indices to be generated later
	glm::vec3 col(rand(), rand(), rand());
	//vertices
	sphereVertices.push_back(a);
	sphereVertices.push_back(b);
	sphereVertices.push_back(c);

	//normals - unit sphere 
	sphereNormals.push_back(a);
	sphereNormals.push_back(b);
	sphereNormals.push_back(c);

	//colors
	sphereColors.push_back(col);
	sphereColors.push_back(col);
	sphereColors.push_back(col);
}
/*glm::vec3 Sphere::mix(glm::vec3 a, glm::vec3 b, float scale)
{
	glm::vec3 out;
	out.x = scale * a.x + (1.0 - scale) * b.x;
	out.y = scale * a.y + (1.0 - scale) * b.y;
	out.z = scale * a.z + (1.0 - scale) * b.z;
	return out;
}*/

void Sphere::CreateIcosahedron()
{
	
	icoColors.clear();
	icoVertices.clear();
	
	float t = (1.0 + sqrt(3.0)) / 2.0;
		icoVertices.push_back(glm::vec3(-1, t, 0));
		icoVertices.push_back(glm::vec3(1, t, 0));
		icoVertices.push_back(glm::vec3(-1, -t, 0));
		icoVertices.push_back(glm::vec3(1, -t, 0));

		icoVertices.push_back(glm::vec3(0, -1, t));
		icoVertices.push_back(glm::vec3(0, 1, t));
		icoVertices.push_back(glm::vec3(0, -1, -t));
		icoVertices.push_back(glm::vec3(0, 1, -t));


		icoVertices.push_back(glm::vec3(t, 0, -1));
		icoVertices.push_back(glm::vec3(t, 0, 1));
		icoVertices.push_back(glm::vec3(-t, 0, -1));
		icoVertices.push_back(glm::vec3(-t, 0, 1));

		icoColors.push_back(glm::vec3(1.0, 0.0, 0.0));
		icoColors.push_back(glm::vec3(0.0, 1.0, 0.0));
		icoColors.push_back(glm::vec3(0.0, 0.0, 1.0));
		icoColors.push_back(glm::vec3(0.8, 0.0, 0.0));
		icoColors.push_back(glm::vec3(0.0, 0.8, 0.8));
		icoColors.push_back(glm::vec3(1.0, 1.0, 1.0));
		icoColors.push_back(glm::vec3(0.0, 0.0, 0.0));
		icoColors.push_back(glm::vec3(0.8, 0.8, 0.0));
		icoColors.push_back(glm::vec3(0.8, 0.0, 0.8));
		icoColors.push_back(glm::vec3(0.2, 0.8, 0.0));
		icoColors.push_back(glm::vec3(0.0, 0.2, 0.8));
		icoColors.push_back(glm::vec3(0.0, 0.8, 0.2));

		for (int i = 0; i < icoVertices.size(); i++)
		{
			icoVertices[i] = glm::normalize(icoVertices[i]);
		}

	
}

Sphere::~Sphere()
{
}
#endif