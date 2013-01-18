#include "main.h"
#include "img.h"
#include "sky.h"
#include <math.h>
#include <windows.h>

vector3d pos;
vector3d rot;
vector2i window_size;

int main(int argc, char** args)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0A );
	//GLUT
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("OpenGL Window\n");
	glutDisplayFunc(draw);
	glutIdleFunc(fps);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(skeydown);
	glutSpecialUpFunc(skeyup);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(mousemove);
	glutSetCursor(GLUT_CURSOR_NONE); 

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_BACK,GL_LINE);

	unsigned int width,height;
	unsigned int tex = loadTexture("sky.png",width,height);
	initSky(tex);

	printf("=Loading image...\n");
	color3ub* data = (color3ub*)readPNG("untitled.png",width,height);
	free(data);

	printf("Ready!\n");
	//Start
	glutMainLoop();
	return 0;
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

float r = 0;
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	testKeys();
	glRotated(rot.x,1,0,0);
	glRotated(rot.y,0,1,0);
	glTranslated(pos.x,pos.y,pos.z);
	drawSky();

	glTranslated(0,0,-5);
	glBegin(GL_QUADS);

	glTexCoord2d(0,0);
	glColor3f(0,0,0);
	glVertex3f(-1,-1,0);

	glTexCoord2d(1,0);
	glColor3f(0,0,1);
	glVertex3f(1,-1,0);

	glTexCoord2d(1,1);
	glColor3f(0,1,0);
	glVertex3f(1,1,0);

	glTexCoord2d(0,1);
	glColor3f(1,0,0);
	glVertex3f(-1,1,0);

	glEnd();

	glutSwapBuffers();
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