#pragma once
#include "main.h"

extern struct vector2i map_size;
extern struct color3ub* map_data;
extern unsigned int minimap_mask;

void initMap(const char* file);
void drawMap();
void drawMiniMap();
void clearMap();


void upQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty);
void downQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty);
void backQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty);
void frontQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty);
void rightQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty);
void leftQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty);
