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
	 Camera *cam = new Camera();
	 cam->SetPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	 //cam->mProjectionTransformation.setIdentity();
	 cout << cam->mProjectionTransformation<<endl;
	Cube *cube = new Cube(Vector3f(0.0f,0.0f,-2.0f));
	GLuint vs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.vert",GL_VERTEX_SHADER);
	GLuint fs = ShaderUtils::CreateShaderFromFile("../src/shaders/simple.frag", GL_FRAGMENT_SHADER);
	GLuint shaderProg = ShaderUtils::CreateProgramFromShaders(vs,fs);
	cube->SetShader(shaderProg);
	GLint projAttr = glGetUniformLocation(shaderProg, "projection");
	GLint modelAttr = glGetUniformLocation(shaderProg, "model");
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 persp = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f); 
	Matrix4f ePersp(glm::value_ptr(persp));
	cout << ePersp <<endl;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProg);
	
		rot = glm::rotate(rot, 0.01f,glm::vec3(0.0,1.0,0.0));
		Matrix4f model(glm::value_ptr(rot));
		cube->mModelTransform = cube->mModelTransform * model;
		
		glUniformMatrix4fv(projAttr,1,GL_FALSE,(GLfloat*)cam->mProjectionTransformation.data());
		//glUniformMatrix4fv(projAttr,1,GL_FALSE,glm::value_ptr(persp));
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