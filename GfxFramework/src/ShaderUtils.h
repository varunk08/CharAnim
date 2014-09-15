#ifndef _SHADERUTILS_H_
#define _SHADERUTILS_H_

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class ShaderUtils
{
public:
	static GLuint CreateShaderFromFile(const GLchar *path, GLenum shaderType);
	static GLuint CreateProgramFromShaders(const GLuint vertexShader, const GLuint fragmentShader);
};
#endif