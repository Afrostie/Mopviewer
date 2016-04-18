/*
	Based off of basic classes given in the tutorials at learnopengl.com
*/

#include "Texture.h"

GLuint Texture::createTexture(std::string path)
{
	ImageLoader imageLoader;
	GLuint texture;
	glGenTextures(1, &texture);
	//Make sure all 2D texture calls use the right texture object
	glBindTexture(GL_TEXTURE_2D, texture);
	//Set texture values
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set texture filtering values
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load the image using stb
	unsigned char* image = imageLoader.loadImage(path);
	int width = imageLoader.returnWidth();
	int height = imageLoader.returnHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//Make OpenGL create the mipmap versions of texture
	glGenerateMipmap(GL_TEXTURE_2D);
	//Image no longer required after textures are generated
	imageLoader.freeData(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
