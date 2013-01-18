#pragma once

#include <png.h>
#include "gl.h"

struct color3ub
{
	unsigned char r,g,b,a;
	bool operator ==(color3ub&);
	color3ub(unsigned char r = 0,unsigned char g = 0,unsigned char b = 0,unsigned char a = 0);
};
typedef color3ub color3ub;

GLuint loadTexture(const char * file, unsigned int & width, unsigned int & height, unsigned int minFilter = GL_NEAREST, unsigned int magFilter = GL_NEAREST, unsigned int wrap = GL_REPEAT);
png_byte* readPNG(const char* file, unsigned int& width, unsigned int& height);