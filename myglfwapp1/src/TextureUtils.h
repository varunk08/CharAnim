#ifndef _TEXTURE_UTILS_H
#define _TEXTURE_UTILS_H
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <IL\il.h>
#include <IL\ilu.h>
class TextureUtils
{
public:
	static GLuint CreateTexture(const ILchar* path);
};
#endif