#include <GL/glew.h>
#include <iostream>
#include <GLFW\glfw3.h>
#include "ShaderUtils.h"
#include "TextureUtils.h"
#include <wchar.h>
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

	
	GLfloat bufferData[] = {
		-0.5, 0.5, 0, 2,
		-0.5,-0.5, 0, 0,
		0.5,-0.5, 2, 0,
		0.5,0.5, 2, 2
	};

	GLfloat colors[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		/*1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, */
		
	};

	GLushort indexData[] = {
		0, 1, 2,
		2, 3, 0
	
	};

	GLint vShader = ShaderUtils::CreateShaderFromFile("D:/OpenGLFall2014/myglfwapp1/src/glsl/vert.glsl", GL_VERTEX_SHADER);
	GLint fShader = ShaderUtils::CreateShaderFromFile("D:/OpenGLFall2014/myglfwapp1/src/glsl/frag.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgram = ShaderUtils::CreateProgramFromShaders(vShader, fShader);
	glUseProgram(shaderProgram);
	GLint positionAttr = glGetAttribLocation(shaderProgram, "position");
	//GLint colorAttr = glGetAttribLocation(shaderProgram, "color");
	GLint texcoordAttr = glGetAttribLocation(shaderProgram, "texCoord");
	GLint texLoc = glGetUniformLocation(shaderProgram, "tex");

	

	GLuint elemBuf;
	glGenBuffers(1, &elemBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	//GLuint colBuf;
	//glGenBuffers(1, &colBuf);
	//glBindBuffer(GL_ARRAY_BUFFER, colBuf);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(colorAttr);
	//glVertexAttribPointer(colorAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLuint myVBO;
	glGenBuffers(1, &myVBO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
	GLuint myVAO;
	glGenVertexArrays(1, &myVAO);
	glBindVertexArray(myVAO);
	glEnableVertexAttribArray(texcoordAttr);
	glEnableVertexAttribArray(positionAttr);
	glVertexAttribPointer(positionAttr, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*) (0*sizeof(GLfloat)));
	glVertexAttribPointer(texcoordAttr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	ilInit();
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilEnable(IL_ORIGIN_SET);
	
	std::string file = "D:/OpenGLFall2014/myglfwapp1/src/textures/bryan.jpg";

	GLuint texture = TextureUtils::CreateTexture(file);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texLoc, 0);

	while(!glfwWindowShouldClose(window)){
		GLint viewportWidth, viewportHeight;
		glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
		glViewport(0, 0, viewportWidth, viewportHeight);
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &elemBuf);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
