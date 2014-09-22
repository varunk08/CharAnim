#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Eigen\Dense>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Node.h"
#include "Plane.h"
#include "ShaderUtils.h"
#include "Cube.h"
#include "Camera.h"
#include "IKSolver.h"

#define NUM_LINKS 3
#define EIGEN_DONT_ALIGN_STATICALLY 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1
using namespace Eigen;
using namespace std;

double mCursorX, mCursorY;
Camera *cam;
Plane* plane;
Node* node;
Cube* cube;
GLuint shaderProg;
IKSolver* iksolve;
glm::vec3 target(-2, 2, 0);
void MouseScrollCallback(GLFWwindow* window, double xoff, double yoff)
{
	cam->UpdateViewDistance((float)yoff);
}
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	double dx = mCursorX - xpos;
	double dy = mCursorY - ypos;
	mCursorX = xpos;
	mCursorY = ypos;
	cam->UpdateCamRotation((float)dx, (float)dy);
	
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &mCursorX, &mCursorY);
		glfwSetCursorPosCallback(window, MouseMoveCallback);
	}
	else if (action == GLFW_RELEASE)
	{
		glfwSetCursorPosCallback(window, NULL);
	}
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action = GLFW_PRESS)
	{
	switch (key)
	{
	case GLFW_KEY_W:
		target = glm::vec3(target.x, target.y + 0.1f, target.z);
		break;
	case GLFW_KEY_S:
		target = glm::vec3(target.x, target.y - 0.1f, target.z);
		break;
	case GLFW_KEY_A:
		target = glm::vec3(target.x - 0.1f, target.y, target.z);
		break;
	case GLFW_KEY_D:
		target = glm::vec3(target.x + 0.1f, target.y, target.z);
		break;
	case GLFW_KEY_Q:
		target = glm::vec3(target.x, target.y, target.z + 0.1f);
		break;
	case GLFW_KEY_E:
		target = glm::vec3(target.x, target.y, target.z - 0.1f);
		break;
		}
		iksolve->SetTargetPosition(Vector3f(glm::value_ptr(target)));
		cube->SetModelTransform(
			glm::translate(glm::mat4(1.0f), target)*
			glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.25f,0.5f))
			);
	}
	
}
void InitScene()
{
	//Objects
	plane = new Plane(glm::vec3(0.0, 0.0, 0.0));
	cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f));

	node = new Node(glm::vec3(0.0f, 0.0f, 0.0f));
	
	node->CreateNewChildNode();
	node->mChild->CreateNewChildNode();
	//node->SetAngle(0.0f);
	node->SetShader(shaderProg);

	cube->SetShader(shaderProg);
	plane->SetShader(shaderProg);
	std::list<glm::mat4> rotlist;
	std::vector<float > vAxisRot0;
	glm::mat4 rotnilxyz = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	
	vAxisRot0.push_back(0.0f); //x
	vAxisRot0.push_back(0.0f); //y
	vAxisRot0.push_back(0.0f); //z

	glm::mat4 rot0z = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0, 0.0, 1.0f));
	glm::mat4 rot1z = glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0, 0.0, 1.0f));
	glm::mat4 rot2z = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0, 0.0, 1.0f));

	rotlist.push_back(rot0z);//child 2
	rotlist.push_back(rot1z);//child 1
	rotlist.push_back(rot2z);//parent
	
	std::list<vector<float> > rotAngles;
	std::vector<float> vRot0;
	vRot0.push_back(0.0f);
	vRot0.push_back(0.0f);
	vRot0.push_back(-90.0f);
	std::vector<float> vRot1;
	vRot1.push_back(0.0f);
	vRot1.push_back(0.0f);
	vRot1.push_back(-90.0f);
	std::vector<float> vRot2;
	vRot2.push_back(0.0f);
	vRot2.push_back(0.0f);
	vRot2.push_back(90.0f);

	rotAngles.push_back(vRot0);
	rotAngles.push_back(vRot1);
	rotAngles.push_back(vRot2);

	node->SetRotation(glm::mat4(1.0f), rotlist, rotAngles);

	iksolve = new IKSolver(node, NUM_LINKS);
	iksolve->SetTargetPosition(Vector3f(glm::value_ptr(target)));

	//Transforms
	plane->SetModelTransform(
		//glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,-5.0f)) *
		glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(5.0f))
		);
	cube->SetModelTransform(
		glm::translate(glm::mat4(1.0f), target)*
		glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.25f,0.5f))
		);
}
int main(int argc, char* argv[])
{
	
	 GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
	/* Make the window's context current */
    glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	 if( GLEW_OK != err)
	 {
		 std::cout << "Glew init failed: " << glewGetErrorString(err) << std::endl;
		 return -1;
	 }
	 glfwSetMouseButtonCallback(window, MouseButtonCallback);
	 glfwSetScrollCallback(window, MouseScrollCallback);
	 glfwSetKeyCallback(window, KeyCallback);
	 cam = new Camera(glm::vec3(0.0, 1.0f, 2.0f),glm::vec3(0.0f,0.0f,0.0f));
	 cam->SetPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	
	//Shaders
	GLuint vs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.vert",GL_VERTEX_SHADER);
	GLuint fs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.frag", GL_FRAGMENT_SHADER);
	 shaderProg = ShaderUtils::CreateProgramFromShaders(vs,fs);
	GLint projAttr = glGetUniformLocation(shaderProg, "projection");
	GLint modelAttr = glGetUniformLocation(shaderProg, "model");
	GLint viewAttr = glGetUniformLocation(shaderProg, "view");
	InitScene();
	//GL state
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProg);
		glUniformMatrix4fv(projAttr,1,GL_FALSE,glm::value_ptr(cam->GetProjectionMatrix()));
		glUniformMatrix4fv(viewAttr, 1, GL_FALSE, glm::value_ptr(cam->GetViewTransform()));
		//Plane
		glUniformMatrix4fv(modelAttr,1,GL_FALSE,glm::value_ptr(plane->GetModelTransform()));
		plane->Render();
		
		//Cube
		glUniformMatrix4fv(modelAttr,1, GL_FALSE, glm::value_ptr(cube->GetModelTransform()));
		cube->Render();
		
		iksolve->IKUpdate();
		//node->SetRotation(iksolve->GetUpdatedAngles());
		node->Render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}