#ifndef _TEXTURE_UTILS_H
#define _TEXTURE_UTILS_H
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <IL\il.h>
#include <IL\ilu.h>
#include <string>
class TextureUtils
{
public:
	static GLuint CreateTexture(std::string path);
};
#endif