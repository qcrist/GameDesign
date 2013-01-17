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

int main(int argc, char **argv);
void draw(void);
void resize(int w, int h);
void fps();