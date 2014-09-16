#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Eigen\Dense>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Plane.h"
#include "ShaderUtils.h"
#include "Cube.h"
#include "Camera.h"

#define EIGEN_DONT_ALIGN_STATICALLY 1
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT 1
using namespace Eigen;
using namespace std;

double mCursorX, mCursorY;
Camera *cam;
void MouseScrollCallback(GLFWwindow* window, double xoff, double yoff)
{
	cam->UpdateViewDistance((float)yoff);
}
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	double dx = xpos - mCursorX ;
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
int main(int argc, char* argv[])
{
	
	
	 GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
	 cam = new Camera(glm::vec3(2.0, 0.0f, 2.0f),glm::vec3(0.0f,0.0f,0.0f));
	 cam->SetPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	
	//Shaders
	GLuint vs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.vert",GL_VERTEX_SHADER);
	GLuint fs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.frag", GL_FRAGMENT_SHADER);
	GLuint shaderProg = ShaderUtils::CreateProgramFromShaders(vs,fs);
	GLint projAttr = glGetUniformLocation(shaderProg, "projection");
	GLint modelAttr = glGetUniformLocation(shaderProg, "model");
	GLint viewAttr = glGetUniformLocation(shaderProg, "view");

	//Objects
	Plane *plane = new Plane(glm::vec3(0.0,0.0,0.0));
	Cube *cube = new Cube(Vector3f(0.0f,0.0f, 0.0f));
	cube->SetShader(shaderProg);
	plane->SetShader(shaderProg);
	

	//GL state
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	
	//Transforms
	plane->SetModelTransform(
		//glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,-5.0f)) *
		 glm::rotate(glm::mat4(1.0f),-90.0f,glm::vec3(1.0f,0.0f,0.0f))
		 * glm::scale(glm::mat4(1.0f),glm::vec3(5.0f))
	);

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
		glUniformMatrix4fv(modelAttr,1, GL_FALSE, (const GLfloat*)cube->GetModelTransform().data());
		cube->Render();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}