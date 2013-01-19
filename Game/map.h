#pragma once
#include "main.h"

extern struct vector2i map_size;
extern struct color3ub* map_data;

void initMap(const char* file);
void drawMap();
void drawMiniMap();
