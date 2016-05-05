#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
//Image Loader class for image loading
#include "ImageLoader.h"

class Texture
{
public:
	GLuint createTexture(std::string path);

};

#endif
