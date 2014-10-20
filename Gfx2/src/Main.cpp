#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Eigen\Dense>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Plane.h"
#include "ShaderUtils.h"
#include "Cube.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#define NUM_LINKS 3
#define EIGEN_DONT_ALIGN_STATICALLY 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1
using namespace Eigen;
using namespace std;

double mCursorX, mCursorY;
Camera *cam;
Plane* plane;
Cube* cube;
GLuint shaderProg;
glm::vec3 target(0, 0, 0);
Light* light;
Sphere* sphere;
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
		cube->SetModelTransform(
			glm::translate(glm::mat4(1.0f), target)*
			glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f,0.5f))
			);
			
	light->mWorldPosition = target;
	}
	
}
void InitScene()
{
	//light
	light = new Light(glm::vec3(10.0, 10.0, 0.0));
	light->SetShader(shaderProg);
	//Objects
	plane = new Plane(glm::vec3(0.0, 0.0, 0.0));
	cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f));
	cube->SetShader(shaderProg);
	plane->SetShader(shaderProg);
	


	//Transforms
	plane->SetModelTransform(
		//glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,-5.0f)) *
		glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(5.0f))
		);
	cube->SetModelTransform(
		glm::translate(glm::mat4(1.0f), target)*
		glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f))
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
	GLint camposAttr = glGetUniformLocation(shaderProg, "viewpos");
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

		//update cam view pos
		glUniform3fv(camposAttr,1 , glm::value_ptr(cam->mWorldPosition));

		//light update
		light->Update();

		//Plane
		glUniformMatrix4fv(modelAttr,1,GL_FALSE,glm::value_ptr(plane->GetModelTransform()));
		plane->Render();
		
		//Cube
		//glUniformMatrix4fv(modelAttr,1, GL_FALSE, glm::value_ptr(cube->GetModelTransform()));
		cube->Render(cam->GetViewTransform());
		
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}