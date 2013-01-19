#pragma once
#include "main.h"

#define toRad(n) ((n)*0.01745329251994329576923690768489)
#define texswap(x,y,a,b,c,d) (float)(x%2?(y%2?a:b):(y%2?c:d))

void projection();
void ortho();

struct vector3d
{
	double x,y,z;
};
typedef vector3d vector3d;

struct vector2i
{
	int x,y;
};
typedef vector2i vector2i;

struct color3ub
{
	unsigned char r,g,b,a;
	bool operator ==(color3ub&);
	color3ub(unsigned char r = 0,unsigned char g = 0,unsigned char b = 0,unsigned char a = 0);
};
typedef color3ub color3ub;
