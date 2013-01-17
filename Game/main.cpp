#include "main.h"
#include "img.h"
#include <math.h>

vector3d pos;
vector3d rot;
vector2i window_size;

int main(int argc, char** args)
{
	//GLUT
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("OpenGL Window");
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

	int width,height;
	png_texture_load("untitled.png",&width,&height);

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
	gluPerspective(45,(float)w/h,.1,1000);
	glMatrixMode(GL_MODELVIEW);
	window_size.x = w;
	window_size.y = h;
}

float r = 0;
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotated(rot.x,1,0,0);
	glRotated(rot.y,0,1,0);
	glTranslated(pos.x,pos.y,pos.z);

	float yrad = toRad(rot.y);
	float xrad = toRad(rot.x);

	if (keys['w'])
	{
		pos.z += cos(yrad)/10;
		pos.x -= sin(yrad)/10;
	}
	if (keys['s'])
	{
		pos.z -= cos(yrad)/10;
		pos.x += sin(yrad)/10;
	}
	if (keys['a'])
	{
		pos.z += sin(yrad)/10;
		pos.x += cos(yrad)/10;
	}
	if (keys['d'])
	{
		pos.z -= sin(yrad)/10;
		pos.x -= cos(yrad)/10;
	}
	if (skeys[GLUT_KEY_END])
		exit(0);


	glTranslated(0,0,-5);
	//glRotated(r+=2,0,1,0);
	//glRotated(r*1.7,1,0,0);
	//glRotated(r*1.5,0,0,1);
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