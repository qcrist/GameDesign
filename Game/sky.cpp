#include "sky.h"
#include "img.h"

unsigned int skyVBO;
unsigned int skyTexture;


#define SKY_SIZE 1000

static float sky_data[] = {
	//x,y,z,tx,ty,r,g,b
	//Front
	-SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	0,0,	0,0,0,
	SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	1,0,	1,0,0,
	SKY_SIZE,SKY_SIZE,-SKY_SIZE,	1,1,	1,1,0,
	-SKY_SIZE,SKY_SIZE,-SKY_SIZE,	0,1,	0,1,0,
	//Back
	-SKY_SIZE,-SKY_SIZE,SKY_SIZE,	0,0,	0,0,1,
	-SKY_SIZE,SKY_SIZE,SKY_SIZE,	0,1,	0,1,1,
	SKY_SIZE,SKY_SIZE,SKY_SIZE,		1,1,	1,1,1,
	SKY_SIZE,-SKY_SIZE,SKY_SIZE,	1,0,	1,0,1,
	//Left
	-SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	0,0,	0,0,0,
	-SKY_SIZE,SKY_SIZE,-SKY_SIZE,	0,1,	0,1,0,
	-SKY_SIZE,SKY_SIZE,SKY_SIZE,	1,1,	0,1,1,
	-SKY_SIZE,-SKY_SIZE,SKY_SIZE,	1,0,	0,0,1,
	//Right
	SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	0,0,	1,0,0,
	SKY_SIZE,-SKY_SIZE,SKY_SIZE,	1,0,	1,0,1,
	SKY_SIZE,SKY_SIZE,SKY_SIZE,		1,1,	1,1,1,
	SKY_SIZE,SKY_SIZE,-SKY_SIZE,	0,1,	1,1,0,
	//TOP
	-SKY_SIZE,SKY_SIZE,-SKY_SIZE,	0,0,	0,1,0,
	SKY_SIZE,SKY_SIZE,-SKY_SIZE,	0,1,	1,1,0,
	SKY_SIZE,SKY_SIZE,SKY_SIZE,		1,1,	1,1,1,
	-SKY_SIZE,SKY_SIZE,SKY_SIZE,	1,0,	0,1,1,
	//BOTTOM
	-SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	0,0,	0,0,0,
	-SKY_SIZE,-SKY_SIZE,SKY_SIZE,	1,0,	0,0,1,
	SKY_SIZE,-SKY_SIZE,SKY_SIZE,	1,1,	1,0,1,
	SKY_SIZE,-SKY_SIZE,-SKY_SIZE,	0,1,	1,0,0,

};

void initSky(const char* texture)
{
	unsigned int width,height;
	skyTexture = loadTexture("sky.png",width,height);
	glGenBuffers(1,&skyVBO);
	glBindBuffer(GL_ARRAY_BUFFER,skyVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sky_data),sky_data,GL_STATIC_DRAW);
}

void drawSky()
{
	glBindTexture(GL_TEXTURE_2D,skyTexture);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor3f(0,0,1);
//	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER,skyVBO);
	glVertexPointer(3,GL_FLOAT,sizeof(float)*8,0);
	glTexCoordPointer(2,GL_FLOAT,sizeof(float)*8,(void*)(sizeof(float)*3));
	glColorPointer(3,GL_FLOAT,sizeof(float)*8,(void*)(sizeof(float)*5));
	glDrawArrays(GL_QUADS,0,24);
}
