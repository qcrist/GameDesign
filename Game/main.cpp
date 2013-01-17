#include "main.h"

int main(int argc, char** args)
{
	//GLUT
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("OpenGL Window");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_BACK,GL_LINE);

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

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}
