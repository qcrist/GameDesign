#include "map.h"

vector2i map_size;
unsigned int minimap_mask;
unsigned int minimap_size = 200;
unsigned int mapVBO;
unsigned int minimap;
unsigned int floorTexture;
unsigned int map_vertex_count;
color3ub* map_data;

void initMap(const char* file)
{
	unsigned int width, height;
	floorTexture = loadTexture("floor.png",width,height);
	map_data = (color3ub*)readPNG(file,width,height);
	map_size.x = width;
	map_size.y = height;

	float* vbo = (float*)malloc(width*height*1024);
	int at = 0;
	glGenBuffers(1,&mapVBO);
	glBindBuffer(GL_ARRAY_BUFFER,mapVBO);
	float h=0;
	float h2=3;
	for (unsigned int y=0;y<height;y++)
		for (unsigned int x=0;x<width;x++)
		{
			color3ub* pixel = map_data + (x) + width*y;
			if (*pixel==color3ub(76,76,76,255) || *pixel==color3ub(128,128,128,255))
			{
				if (*pixel==color3ub(76,76,76,255))
				{
					pos.x = x;
					pos.z = y;
				}
				vbo[at++] = x-.5f;
				vbo[at++] = h;
				vbo[at++] = y-.5f;
				vbo[at++] = texswap(x,y,0,0,1,1);
				vbo[at++] = texswap(x,y,0,1,1,0);

				vbo[at++] = x-.5f;
				vbo[at++] = h;
				vbo[at++] = y+.5f;
				vbo[at++] = texswap(x,y,0,1,1,0);
				vbo[at++] = texswap(x,y,1,1,0,0);

				vbo[at++] = x+.5f;
				vbo[at++] = h;
				vbo[at++] = y+.5f;
				vbo[at++] = texswap(x,y,1,1,0,0);
				vbo[at++] = texswap(x,y,1,0,0,1);

				vbo[at++] = x+.5f;
				vbo[at++] = h;
				vbo[at++] = y-.5f;
				vbo[at++] = texswap(x,y,1,0,0,1);
				vbo[at++] = texswap(x,y,0,0,1,1);

				vbo[at++] = x-.5f;
				vbo[at++] = h2;
				vbo[at++] = y-.5f;
				vbo[at++] = texswap(x,y,0,0,1,1);
				vbo[at++] = texswap(x,y,0,1,1,0);

				vbo[at++] = x+.5f;
				vbo[at++] = h2;
				vbo[at++] = y-.5f;
				vbo[at++] = texswap(x,y,1,0,0,1);
				vbo[at++] = texswap(x,y,0,0,1,1);

				vbo[at++] = x+.5f;
				vbo[at++] = h2;
				vbo[at++] = y+.5f;
				vbo[at++] = texswap(x,y,1,1,0,0);
				vbo[at++] = texswap(x,y,1,0,0,1);

				vbo[at++] = x-.5f;
				vbo[at++] = h2;
				vbo[at++] = y+.5f;
				vbo[at++] = texswap(x,y,0,1,1,0);
				vbo[at++] = texswap(x,y,1,1,0,0);
			}
		}
		map_vertex_count = at/5;
		glBufferData(GL_ARRAY_BUFFER,map_vertex_count * 5 * sizeof(float),vbo,GL_STATIC_DRAW);


		glGenTextures(1, &minimap_mask);
		glBindTexture(GL_TEXTURE_2D, minimap_mask);
		void* clear = malloc(height*width*4);
		memset(clear,0,height*width*4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, clear);
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,clear);
		free(clear);
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
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,floorTexture);
	glBindBuffer(GL_ARRAY_BUFFER,mapVBO);

	glDisableClientState(GL_COLOR_ARRAY);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3,GL_FLOAT,sizeof(float)*5,0);
	glTexCoordPointer(2,GL_FLOAT,sizeof(float)*5,(void*)(sizeof(float)*3));
	glDrawArrays(GL_QUADS,0,map_vertex_count);
}

void drawMiniMap()
{
	ortho();
	glPushMatrix();
	glLoadIdentity();

	int zoom = 10;
	double rady1 = toRad(-rot.y+45);
	double rady2 = toRad(-rot.y+135);
	double rady3 = toRad(-rot.y+225);
	double rady4 = toRad(-rot.y+315);
	double x = pos.x+.5f;
	double z = pos.z+.5f;

	glBindTexture(GL_TEXTURE_2D,0);

	glBegin(GL_QUADS);
	glColor3f(.2f,.2f,.2f);
	glVertex3d(0,0,0);
	glVertex3d(minimap_size,0,0);
	glVertex3d(minimap_size,minimap_size,0);
	glVertex3d(0,minimap_size,0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,minimap);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);

	glTexCoord2d((x+zoom*cos(rady1))/map_size.x,(z-zoom*sin(rady1))/map_size.y);
	glVertex2d(0,0);

	glTexCoord2d((x+zoom*cos(rady2))/map_size.x,(z-zoom*sin(rady2))/map_size.y);
	glVertex2d(minimap_size,0);

	glTexCoord2d((x+zoom*cos(rady3))/map_size.x,(z-zoom*sin(rady3))/map_size.y);
	glVertex2d(minimap_size,minimap_size);

	glTexCoord2d((x+zoom*cos(rady4))/map_size.x,(z-zoom*sin(rady4))/map_size.y);
	glVertex2d(0,minimap_size);

	glEnd();

	glBindTexture(GL_TEXTURE_2D,minimap_mask);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);

	glTexCoord2d((x+zoom*cos(rady1))/map_size.x,(z-zoom*sin(rady1))/map_size.y);
	glVertex2d(0,0);

	glTexCoord2d((x+zoom*cos(rady2))/map_size.x,(z-zoom*sin(rady2))/map_size.y);
	glVertex2d(minimap_size,0);

	glTexCoord2d((x+zoom*cos(rady3))/map_size.x,(z-zoom*sin(rady3))/map_size.y);
	glVertex2d(minimap_size,minimap_size);

	glTexCoord2d((x+zoom*cos(rady4))/map_size.x,(z-zoom*sin(rady4))/map_size.y);
	glVertex2d(0,minimap_size);

	glEnd();

	glBindTexture(GL_TEXTURE_2D,0);
	glBegin(GL_LINES);
	glColor4d(1,1,1,.2);
	glVertex2d(minimap_size,minimap_size/2);
	glVertex2d(0,minimap_size/2);
	glVertex2d(minimap_size/2,0);
	glVertex2d(minimap_size/2,minimap_size);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3d(.5,.5,.5);
	glVertex3f(0,0,-1);
	glVertex3d(minimap_size,0,0);
	glVertex3d(minimap_size,minimap_size,0);
	glVertex3d(0,minimap_size,0);
	glEnd();

	glPopMatrix();
	projection();
}