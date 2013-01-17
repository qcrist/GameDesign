#include <png.h>
#include <gl/glew.h>
#include <gl/glut.h>

typedef struct
{
	unsigned char r,g,b,a;
}color3ub;

GLuint png_texture_load(const char * file, unsigned int & width, unsigned int & height);
png_byte* readPNG(const char* file, unsigned int& width, unsigned int& height);