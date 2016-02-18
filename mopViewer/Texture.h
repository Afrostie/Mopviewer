#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//STB for image loading
#include "stb/stb_image.h"

class Texture
{
public:
	GLuint createTexture(const GLchar* path);

};

#endif