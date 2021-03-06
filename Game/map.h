#pragma once
#include "main.h"
#include <vector>

extern struct vector2i map_size;
extern struct color3ub* map_data;
extern unsigned int minimap_mask;
extern unsigned int minimap;
extern unsigned int key_count;
extern int zoom;

void initMap(int level);
void drawMap();
void drawMiniMap();
void clearMap();
void destroyMap();

extern unsigned int map_key_texture;

static const char* maps[] = {"map0.png","map1.png","map2.png","map3.png","map4.png","map5.png","map6.png","map7.png","map8.png","map9.png","map10.png","map11.png","map12.png"};
static const char* maps_by[] = {"Quintin","Brian","Quintin","Blake","Tripp","Quintin","Brian","Tripp","Blake","Quintin","Brian","Tripp","Brian"};


void upQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty);
void downQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty);
void backQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty);
void frontQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty);
void rightQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty);
void leftQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty);

#include <BulletDynamics\Dynamics\btRigidBody.h> //Not really sure why I have to include it again...

struct obj{
	btRigidBody* body;
	btVector3 pos;
};

extern std::vector<obj*>* map_keys;
extern std::vector<obj*>* map_doors;