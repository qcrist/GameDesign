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

bool color3ub::operator==(color3ub& color)
{
	return r==color.r && g==color.g && b==color.b;
}

color3ub::color3ub(unsigned char rr,unsigned char gg,unsigned char bb,unsigned char aa):r(rr),g(gg),b(bb),a(aa)
{
}