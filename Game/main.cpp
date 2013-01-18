#include "main.h"
#include "img.h"
#include "sky.h"
#include <math.h>
#include <windows.h>

vector3d pos;
vector3d rot;
vector2i window_size;

vector2i map_size;
unsigned int minimap_mask;
unsigned int minimap_size = 200;
unsigned int mapVBO;
unsigned int minimap;
color3ub* map_data;


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

void initMap(const char* file)
{
	unsigned int width, height;
	map_data = (color3ub*)readPNG("map.png",width,height);
	map_size.x = width;
	map_size.y = height;

	float* vbo = (float*)malloc(width*height*1024);
	int at = 0;
	glGenBuffers(1,&mapVBO);
	glBindBuffer(GL_ARRAY_BUFFER,mapVBO);
	int h=0;

	for (unsigned int y=0;y<height;y++)
		for (unsigned int x=0;x<width;x++)
		{
			color3ub* pixel = map_data + (x) + width*y;
			vbo[at++] = x-.5;
			vbo[at++] = h;
			vbo[at++] = y-.5;
			vbo[at++] = pixel->r/255.f;
			vbo[at++] = pixel->g/255.f;
			vbo[at++] = pixel->b/255.f;

			vbo[at++] = x-.5;
			vbo[at++] = h;
			vbo[at++] = y+.5;
			vbo[at++] = pixel->r/255.f;
			vbo[at++] = pixel->g/255.f;
			vbo[at++] = pixel->b/255.f;

			vbo[at++] = x+.5;
			vbo[at++] = h;
			vbo[at++] = y+.5;
			vbo[at++] = pixel->r/255.f;
			vbo[at++] = pixel->g/255.f;
			vbo[at++] = pixel->b/255.f;

			vbo[at++] = x+.5;
			vbo[at++] = h;
			vbo[at++] = y-.5;
			vbo[at++] = pixel->r/255.f;
			vbo[at++] = pixel->g/255.f;
			vbo[at++] = pixel->b/255.f;

		}
		glBufferData(GL_ARRAY_BUFFER,width*height*1024,vbo,GL_STATIC_DRAW);


		glGenTextures(1, &minimap_mask);
		glBindTexture(GL_TEXTURE_2D, minimap_mask);
		void* clear = malloc(height*width*4);
		memset(clear,0,height*width*4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, clear);
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,clear);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER); 
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);

		glGenTextures(1, &minimap);
		glBindTexture(GL_TEXTURE_2D, minimap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, map_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER); 
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
}

void drawMap()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindBuffer(GL_ARRAY_BUFFER,mapVBO);
	glVertexPointer(3,GL_FLOAT,sizeof(float)*6,0);
	glColorPointer(3,GL_FLOAT,sizeof(float)*6,(void*)(sizeof(float)*3));
	glDrawArrays(GL_QUADS,0,map_size.x*map_size.y*4);
}

void drawMiniMap()
{
	ortho();
	glLoadIdentity();

	int zoom = 20;
	double rady1 = toRad(-rot.y+45);
	double rady2 = toRad(-rot.y+135);
	double rady3 = toRad(-rot.y+225);
	double rady4 = toRad(-rot.y+315);
	float x = -pos.x+.5;
	float z = -pos.z+.5;

	glBindTexture(GL_TEXTURE_2D,0);

	glBegin(GL_QUADS);
	glColor3f(.2f,.2f,.2f);
	glVertex3f(0,0,0);
	glVertex3f(minimap_size,0,0);
	glVertex3f(minimap_size,minimap_size,0);
	glVertex3f(0,minimap_size,0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,minimap);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);

	glTexCoord2d((x+zoom*cos(rady1))/map_size.x,(z-zoom*sin(rady1))/map_size.y);
	glVertex2f(0,0);

	glTexCoord2d((x+zoom*cos(rady2))/map_size.x,(z-zoom*sin(rady2))/map_size.y);
	glVertex2f(minimap_size,0);

	glTexCoord2d((x+zoom*cos(rady3))/map_size.x,(z-zoom*sin(rady3))/map_size.y);
	glVertex2f(minimap_size,minimap_size);

	glTexCoord2d((x+zoom*cos(rady4))/map_size.x,(z-zoom*sin(rady4))/map_size.y);
	glVertex2f(0,minimap_size);

	glEnd();

	glBindTexture(GL_TEXTURE_2D,minimap_mask);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);

	glTexCoord2d((x+zoom*cos(rady1))/map_size.x,(z-zoom*sin(rady1))/map_size.y);
	glVertex2f(0,0);

	glTexCoord2d((x+zoom*cos(rady2))/map_size.x,(z-zoom*sin(rady2))/map_size.y);
	glVertex2f(minimap_size,0);

	glTexCoord2d((x+zoom*cos(rady3))/map_size.x,(z-zoom*sin(rady3))/map_size.y);
	glVertex2f(minimap_size,minimap_size);

	glTexCoord2d((x+zoom*cos(rady4))/map_size.x,(z-zoom*sin(rady4))/map_size.y);
	glVertex2f(0,minimap_size);

	glEnd();

	glBindTexture(GL_TEXTURE_2D,0);
	glPushMatrix();
	glBegin(GL_LINES);
	glColor4f(1,1,1,.2);
	glVertex2f(minimap_size,minimap_size/2);
	glVertex2f(0,minimap_size/2);
	glVertex2f(minimap_size/2,0);
	glVertex2f(minimap_size/2,minimap_size);
	glEnd();
	glPopMatrix();

	glBegin(GL_LINE_LOOP);
	glColor3d(.5,.5,.5);
	glVertex3f(0,0,-1);
	glVertex3f(minimap_size,0,0);
	glVertex3f(minimap_size,minimap_size,0);
	glVertex3f(0,minimap_size,0);
	glEnd();

	projection();
}

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
	printf("Ready!\n");
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
	glTranslated(pos.x,-pos.y,pos.z);
	//drawSky();
	drawMap();
	drawMiniMap();
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