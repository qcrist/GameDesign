#include "listeners.h"

void initListeners()
{
	glutDisplayFunc(draw);
	glutIdleFunc(fps);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(skeydown);
	glutSpecialUpFunc(skeyup);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(mousemove);
}

float r = 0;
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	testKeys();
	glRotated(rot.x,1,0,0);
	glRotated(rot.y,0,1,0);
	glTranslated(pos.x,-pos.y,pos.z);
	//drawSky();
	drawMap();
	drawMiniMap();
	glutSwapBuffers();
}


void testKeys()
{
	double yrad = toRad(rot.y);
	double xrad = toRad(rot.x);
	double cosy = cos(yrad);
	double siny = sin(yrad);

	if (keys['w'])
	{
		pos.z += cosy/10;
		pos.x -= siny/10;
	}
	if (keys['s'])
	{
		pos.z -= cosy/10;
		pos.x += siny/10;
	}
	if (keys['a'])
	{
		pos.z += siny/10;
		pos.x += cosy/10;
	}
	if (keys['d'])
	{
		pos.z -= siny/10;
		pos.x -= cosy/10;
	}
	if (skeys[GLUT_KEY_END])
		exit(0);
}

long next_frame;
long last_frame;
int fps_limit = 60;
void fps()
{
	long now = clock();
	if (now>=next_frame)
	{
		next_frame = now + CLOCKS_PER_SEC/fps_limit;
		float current_fps = CLOCKS_PER_SEC/(float)(now-last_frame);
		last_frame = clock();
		char buf[50];
		sprintf(buf,"FPS: %f",current_fps);
		glutSetWindowTitle(buf);
		draw();
	}else{
		if (next_frame-now > 10)
		{
			sleep(10);
		}
		else 
		{
			sleep(next_frame-now);
		}
	}
}

void resize(int w, int h)
{
	if (h == 0)h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,(float)w/h,.1,5000);
	glMatrixMode(GL_MODELVIEW);
	window_size.x = w;
	window_size.y = h;
}


char keys[256];
char skeys[256]; //<-- Max value of special key

void keydown(unsigned char key,int,int)
{
	keys[key] = true;
}

void keyup(unsigned char key,int,int)
{
	keys[key] = false;
}

void skeydown(int key,int,int)
{
	skeys[key] = true;
}

void skeyup(int key,int,int)
{
	skeys[key] = false;
}

void mousemove(int x, int y)
{
	if (x==window_size.x/2 && y==window_size.y/2)
		return;
	int dy = window_size.y/2-y;
	int dx = window_size.x/2-x;
	if (dy>30)
		dy = dx = 0;
	if (dx>30)
		dx = dy = 0;
	rot.x -= dy;
	if (rot.x < -85)
		rot.x = -85;
	if (rot.x > 85)
		rot.x = 85;
	rot.y -= dx;
	glutWarpPointer(window_size.x/2,window_size.y/2);
}