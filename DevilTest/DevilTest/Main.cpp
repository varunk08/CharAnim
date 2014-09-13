#include <iostream>
#include <IL\il.h>
#include <string>
#include <conio.h>
int main(int argc, char* argv[])
{
	ilInit();
	ILuint image =0,w,h;
	ilGenImages(1, &image);
	ilBindImage(image);
		ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	std::string path = "../images/cat.jpg";
	ILboolean loaded = ilLoadImage((ILstring)path.c_str());
	if(loaded == IL_FALSE)
	{
		std::cout << "Failed to load image "<<path <<std::endl;
		w= ilGetInteger(IL_IMAGE_WIDTH);
		h= ilGetInteger(IL_IMAGE_HEIGHT);
		std::cout<<w<<std::endl;
		std::cout<<h<<std::endl;
	}
	w= ilGetInteger(IL_IMAGE_WIDTH);
		h= ilGetInteger(IL_IMAGE_HEIGHT);
		std::cout<<w<<std::endl;
		std::cout<<h<<std::endl;
	getch();
	return 0;
}