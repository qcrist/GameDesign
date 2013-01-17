#include "main.h"
#include "img.h"

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

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_BACK,GL_LINE);

	int width,height;
	png_texture_load("Untitled.png",&width,&height);

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
}

int r = 0;
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0,0,-5);
	//glRotatef(r++,0,1,0);
	//glRotatef(r*2,1,0,0);
	//glRotatef(r*3,0,0,1);
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