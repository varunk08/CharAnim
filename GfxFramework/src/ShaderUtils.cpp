#include "ShaderUtils.h"

GLuint ShaderUtils::CreateShaderFromFile(const GLchar* path, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	std::ifstream fin;
	fin.open(path);
	if (!fin.is_open()){
		fin.close();
		std::cout << "File not found: " << path << std::endl;
		return 0;
	}

	std::string source = std::string(std::istreambuf_iterator<GLchar>(fin), std::istreambuf_iterator<GLchar>());
	fin.close();
	const GLchar* shaderSource = source.c_str();
	glShaderSource(shaderID, 1, &shaderSource, NULL);

	glCompileShader(shaderID);
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE){
		std::cout << "Shader failed to compile!\n" << path << std::endl;
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength + 1, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;

		return 0;
	}

	return shaderID;
	
}

GLuint ShaderUtils::CreateProgramFromShaders(const GLuint vertexShader, const GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != GL_TRUE){
		std::cout << "Program link failed!" << std::endl;
		GLint infoLogLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderProgram, infoLogLength + 1, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;

		return 0;
	}

	return shaderProgram;
}