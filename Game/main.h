#pragma once

#include <gl/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <time.h>
#include <windows.h>
#define sleep(ms) Sleep(ms);

#include <stdio.h>

#undef glGenBuffers
#undef glCreateBuffer
#undef glBindBuffer
#undef glDeleteBuffers

#define glGenBuffers glGenBuffersARB
#define glCreateBuffer glBindBufferARB
#define glBindBuffer glBindBufferARB
#define glDeleteBuffers glDeleteBuffersARB

#define toRad(n) (n*0.01745329251994329576923690768489);

int main(int argc, char **argv);
void draw(void);
void resize(int w, int h);
void mousemove(int x, int y);
void fps();

typedef struct
{
	double x,y,z; 
}vector3d;

typedef struct
{
	int x,y;
}vector2i;

extern char keys[];
extern char skeys[]; //<-- Max value of special key

extern vector2i window_size;

void keydown(unsigned char key,int,int);
void keyup(unsigned char key,int,int);
void skeydown(int key,int,int);
void skeyup(int key,int,int);
