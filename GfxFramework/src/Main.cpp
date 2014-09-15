#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Eigen\Dense>
#include "Plane.h"
#include "ShaderUtils.h"

using namespace Eigen;
using namespace std;

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
    
	Plane *plane = new Plane(Vector3f(0.0f,0.0f,-2.0f));
	GLuint vs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.vert",GL_VERTEX_SHADER);
	GLuint fs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.frag", GL_FRAGMENT_SHADER);
	GLuint shaderProg = ShaderUtils::CreateProgramFromShaders(vs,fs);
	plane->SetShader(shaderProg);
	
	glClearColor(0.5f,0.5f,0.5f,1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProg);
		plane->Render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}