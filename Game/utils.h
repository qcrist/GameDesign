#pragma once
#include "main.h"

#define toRad(n) ((n)*0.01745329251994329576923690768489)
#define texswap(x,y,a,b,c,d) (float)(x%2?(y%2?a:b):(y%2?c:d))

void projection();
void ortho();

struct vector3d{
	double x,y,z; 
};
typedef vector3d vector3d;

typedef struct
{
	int x,y;
}vector2i;

extern vector3d pos;
extern vector3d rot;
extern vector2i window_size;