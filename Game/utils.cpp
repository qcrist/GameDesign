#include "utils.h"

void projection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void ortho()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(window_size.x,0,window_size.y,0);
	glMatrixMode(GL_MODELVIEW);
}