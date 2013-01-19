#pragma once
#include "main.h"

extern struct vector2i map_size;
extern unsigned int minimap_mask;
extern unsigned int minimap_size;
extern unsigned int mapVBO;
extern unsigned int minimap;
extern unsigned int floorTexture;
extern unsigned int map_vertex_count;
extern struct color3ub* map_data;

void initMap(const char* file);
void drawMap();
void drawMiniMap();
