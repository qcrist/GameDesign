#include "main.h"

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
	initListeners();
	glutSetCursor(GLUT_CURSOR_NONE); 

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_BACK,GL_LINE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//Load Sky
	initSky("sky.png");

	//Read map
	initMap("map.png");

	pos.y = 1;

	//Start
	glutMainLoop();

	return 0;
}


