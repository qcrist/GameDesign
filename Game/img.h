#pragma once

#include <png.h>
#include "gl.h"

typedef struct
{
	unsigned char r,g,b,a;
}color3ub;

GLuint loadTexture(const char * file, unsigned int & width, unsigned int & height, unsigned int minFilter = GL_NEAREST, unsigned int magFilter = GL_NEAREST, unsigned int wrap = GL_REPEAT);
png_byte* readPNG(const char* file, unsigned int& width, unsigned int& height);