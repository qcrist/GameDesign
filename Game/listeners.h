#pragma once
#include "main.h"
#include <time.h>
#include <windows.h>
#define sleep(ms) Sleep(ms);
#include "math.h"

extern char keys[];
extern char skeys[];
extern class btVector3* pos;
extern class btVector3* rot;
extern struct vector2i window_size;

void initListeners();
void draw();
void fps();
void resize(int w, int h);
void keydown(unsigned char key,int,int);
void keyup(unsigned char key,int,int);
void skeydown(int key,int,int);
void skeyup(int key,int,int);
void mousemove(int x, int y);
void testKeys();