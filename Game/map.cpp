#include "map.h"

vector2i map_size;
unsigned int minimap_mask;
unsigned int minimap_size = 200;

unsigned int map_vertex_vbo;

unsigned int map_floor_vbo;
unsigned int map_floor_count;

unsigned int minimap;
unsigned int floorTexture;
color3ub* map_data;

void initMap(const char* file)
{
	unsigned int width, height;
	floorTexture = loadTexture("floor.png",width,height);
	map_data = (color3ub*)readPNG(file,width,height);
	map_size.x = width;
	map_size.y = height;

	float* vertexes = (float*)malloc(width*height*sizeof(float)*1024); //TODO: make this grab less when done
	unsigned int* floor_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*6); //Max
	int at_v = 0;
	int at_f = 0;
	float floor_height = 0;
	float ceiling_height = 3;
	int stride = 5;

	btTriangleMesh* meshI_f = new btTriangleMesh();

	for (unsigned int y=0;y<height;y++)
		for (unsigned int x=0;x<width;x++)
		{
			color3ub* pixel = map_data + (x) + width*y;
			if (*pixel==color3ub(76,76,76,255) || *pixel==color3ub(128,128,128,255))
			{
				if (*pixel==color3ub(76,76,76,255))
				{
					pos->setValue((float)x,1,(float)y);
				}
				int start = at_v/stride;

				vertexes[at_v++] = x-.5f;
				vertexes[at_v++] = floor_height;
				vertexes[at_v++] = y-.5f;
				vertexes[at_v++] = texswap(x,y,0,0,1,1);
				vertexes[at_v++] = texswap(x,y,0,1,1,0);

				vertexes[at_v++] = x-.5f;
				vertexes[at_v++] = floor_height;
				vertexes[at_v++] = y+.5f;
				vertexes[at_v++] = texswap(x,y,0,1,1,0);
				vertexes[at_v++] = texswap(x,y,1,1,0,0);

				vertexes[at_v++] = x+.5f;
				vertexes[at_v++] = floor_height;
				vertexes[at_v++] = y+.5f;
				vertexes[at_v++] = texswap(x,y,1,1,0,0);
				vertexes[at_v++] = texswap(x,y,1,0,0,1);

				vertexes[at_v++] = x+.5f;
				vertexes[at_v++] = floor_height;
				vertexes[at_v++] = y-.5f;
				vertexes[at_v++] = texswap(x,y,1,0,0,1);
				vertexes[at_v++] = texswap(x,y,0,0,1,1);

				floor_indices[at_f++] = start;
				floor_indices[at_f++] = start+1;
				floor_indices[at_f++] = start+2;
				floor_indices[at_f++] = start;
				floor_indices[at_f++] = start+2;
				floor_indices[at_f++] = start+3;

				start = at_v/stride;

				vertexes[at_v++] = x-.5f;
				vertexes[at_v++] = ceiling_height;
				vertexes[at_v++] = y-.5f;
				vertexes[at_v++] = texswap(x,y,0,0,1,1);
				vertexes[at_v++] = texswap(x,y,0,1,1,0);

				vertexes[at_v++] = x+.5f;
				vertexes[at_v++] = ceiling_height;
				vertexes[at_v++] = y-.5f;
				vertexes[at_v++] = texswap(x,y,1,0,0,1);
				vertexes[at_v++] = texswap(x,y,0,0,1,1);

				vertexes[at_v++] = x+.5f;
				vertexes[at_v++] = ceiling_height;
				vertexes[at_v++] = y+.5f;
				vertexes[at_v++] = texswap(x,y,1,1,0,0);
				vertexes[at_v++] = texswap(x,y,1,0,0,1);

				vertexes[at_v++] = x-.5f;
				vertexes[at_v++] = ceiling_height;
				vertexes[at_v++] = y+.5f;
				vertexes[at_v++] = texswap(x,y,0,1,1,0);
				vertexes[at_v++] = texswap(x,y,1,1,0,0);

				floor_indices[at_f++] = start;
				floor_indices[at_f++] = start+1;
				floor_indices[at_f++] = start+2;
				floor_indices[at_f++] = start;
				floor_indices[at_f++] = start+2;
				floor_indices[at_f++] = start+3;

				meshI_f->addTriangle(
					btVector3(x-.5f,floor_height,y-.5f),
					btVector3(x-.5f,floor_height,y+.5f),
					btVector3(x+.5f,floor_height,y+.5f),
					true
					);
				meshI_f->addTriangle(
					btVector3(x-.5f,floor_height,y-.5f),
					btVector3(x+.5f,floor_height,y+.5f),
					btVector3(x+.5f,floor_height,y-.5f),
					true
					);
			}
		}
		map_floor_count = at_f;
		int map_vertex_count = at_v/stride;

		glGenBuffers(1,&map_vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,map_vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,map_vertex_count * 5 * sizeof(float),vertexes,GL_STATIC_DRAW);
		glGenBuffers(1,&map_floor_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_floor_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_floor_count * sizeof(int),floor_indices,GL_STATIC_DRAW);

		btTriangleMeshShape* shape_f = new btBvhTriangleMeshShape(meshI_f,true);

		btDefaultMotionState* floorMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,floorMotionState,shape_f,btVector3(0,0,0));
        btRigidBody* floorRigidBody = new btRigidBody(groundRigidBodyCI);
		world->addRigidBody(floorRigidBody);

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
	glBindBuffer(GL_ARRAY_BUFFER,map_vertex_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_floor_vbo);

	glDisableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3,GL_FLOAT,sizeof(float)*5,0);
	glTexCoordPointer(2,GL_FLOAT,sizeof(float)*5,(void*)(sizeof(float)*3));
	glDrawElements(GL_TRIANGLES,map_floor_count,GL_UNSIGNED_INT,0);
}

void drawMiniMap()
{
	ortho();
	glPushMatrix();
	glLoadIdentity();

	int zoom = 10;
	double rady1 = toRad(-rot->getY()+45);
	double rady2 = toRad(-rot->getY()+135);
	double rady3 = toRad(-rot->getY()+225);
	double rady4 = toRad(-rot->getY()+315);
	double x = pos->getX()+.5f;
	double z = pos->getZ()+.5f;

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