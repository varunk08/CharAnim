#include <GL/glew.h>
#include <iostream>
#include <GLFW\glfw3.h>
#include "ShaderUtils.h"

int main(int argc, char* argv[])
{
	GLint glfwStatus = glfwInit();
	if(glfwStatus != GL_TRUE){
		std::cout << "GLFWinit() failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	GLFWwindow* window = glfwCreateWindow(640,480, "My GLFW 1", nullptr, nullptr);
	if(window == NULL){
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	std::cout << glGetString(GL_VERSION) << std::endl;
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( err != GLEW_OK ){
		std::cout << "Glew init error" << std::endl;
		glfwTerminate();
		return -1;
	}
	GLuint myVBO;
	glGenBuffers(1, &myVBO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	
	GLfloat bufferData[] = {
		0.0, 0.0,
		-0.5,-0.5,
		0.5,-0.5
	};

	glBufferData(GL_ARRAY_BUFFER, 48, bufferData, GL_STATIC_DRAW);

	GLint vShader = ShaderUtils::CreateShaderFromFile("D:/OpenGLFall2014/myglfwapp1/src/glsl/vert.glsl", GL_VERTEX_SHADER);
	GLint fShader = ShaderUtils::CreateShaderFromFile("D:/OpenGLFall2014/myglfwapp1/src/glsl/frag.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgram = ShaderUtils::CreateProgramFromShaders(vShader, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	while(!glfwWindowShouldClose(window)){
		GLint windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		
		glClearColor(1.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}