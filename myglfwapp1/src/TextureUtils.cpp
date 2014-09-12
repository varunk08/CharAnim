#include "TextureUtils.h"

GLuint TextureUtils::CreateTexture(const ILchar* path)
{
	
	ILuint image = ilGenImage();
	ilBindImage(image);
	ILboolean loadSuccess = ilLoadImage(path);
	std::cout << loadSuccess << std::endl;
	if ( !loadSuccess )
	{
		std::wcout << L"Failed to load image! " << path << std::endl;
		
		ilBindImage(NULL);
		ilDeleteImage(image);
		return NULL;
	}
	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);
	std::cout << "Image w: " << w << " Image h: " << h << std::endl;
	ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	if (!convertSuccess)
	{
		std::wcout << L"Failed to convert image! " << path << std::endl;
		ilBindImage(NULL);
		ilDeleteImage(image);
		return NULL;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_TYPE),
		ilGetData()
	);
	glBindTexture(GL_TEXTURE_2D, NULL);
	ilBindImage(NULL);
	ilDeleteImage(image);
	return texture;
}