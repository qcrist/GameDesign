#include "map.h"

vector2i map_size;
unsigned int minimap_mask;
unsigned int minimap_size = 200;

unsigned int map_vertex_vbo;

unsigned int map_floor_vbo;
unsigned int map_floor_count;
unsigned int map_floor_texture;

unsigned int map_exit_vbo;
unsigned int map_exit_count;
unsigned int map_exit_texture;

unsigned int map_wall_vbo;
unsigned int map_wall_count;
unsigned int map_wall_texture;

unsigned int map_key_vbo;
unsigned int map_key_count;
unsigned int map_key_texture;

unsigned int map_door_vbo;
unsigned int map_door_count;
unsigned int map_door_texture;

unsigned int minimap;
color3ub* map_data;

void* clear;

#define STRIDE 5

void initMap(const char* file)
{
	unsigned int width, height;
	map_floor_texture = loadTexture("floor.png",width,height);
	map_wall_texture = loadTexture("wall.png",width,height);
	map_exit_texture = loadTexture("exit.png",width,height);
	map_key_texture = loadTexture("key.png",width,height);
	map_door_texture = loadTexture("door.png",width,height);
	map_data = (color3ub*)readPNG(file,width,height);
	map_size.x = width;
	map_size.y = height;

	float* vertexes = (float*)malloc(width*height*sizeof(float)*1024); //TODO: make this grab less when done
	unsigned int* floor_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*12); //Max
	unsigned int* wall_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*24); //Max?
	unsigned int* exit_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*24); //Max?
	unsigned int* key_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*24); //Max?
	unsigned int* door_indices = (unsigned int*)malloc(width*height*sizeof(unsigned int)*24); //Max?
	int at_v = 0;
	int at_f = 0;
	int at_w = 0;
	int at_e = 0;
	int at_k = 0;
	int at_d = 0;
	float floor_height = 0;
	float ceiling_height = 3;
	float middle_height =(ceiling_height-floor_height)/2;

	btBoxShape* wall= new btBoxShape(btVector3(.5,middle_height,.5));
	btBoxShape* box= new btBoxShape(btVector3(.5,.5,.5));
	for (unsigned int y=0;y<height;y++)
		for (unsigned int x=0;x<width;x++)
		{
			color3ub* pixel = map_data + (x) + width*y;
			if (*pixel==color3ub(50,100,200,255) || *pixel==color3ub(128,128,128,255))
			{
				if (*pixel==color3ub(50,100,200,255))
				{
					pos->setValue((float)x,1,(float)y);
				}
				upQuadHelper(vertexes,floor_indices,at_v,at_f,x,floor_height,y,.5,.5,x,y,1,1);
				downQuadHelper(vertexes,floor_indices,at_v,at_f,x,ceiling_height,y,.5,.5,x,y,1,1);
			}else if (*pixel==color3ub(20,20,20,255))
			{
				btDefaultMotionState* wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3((float)x,middle_height,(float)y)));
				btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0,wallMotionState,wall,btVector3(0,0,0));
				btRigidBody* body = new btRigidBody(rigidBodyCI);
				world->addRigidBody(body);

				if ((int)y+1<map_size.y)
					if (!(map_data[x + width*(y+1)]==color3ub(20,20,20,255)))
						if (map_data[x + width*(y+1)]==color3ub(0,255,33,255))
							backQuadHelper(vertexes,exit_indices,at_v,at_e,x,middle_height,y+.5,.5,1.5,x,y,1,3);
						else
							backQuadHelper(vertexes,wall_indices,at_v,at_w,x,middle_height,y+.5,.5,1.5,x,y,1,3);
				if ((int)y-1>0)
					if (!(map_data[x + width*(y-1)]==color3ub(20,20,20,255)))
						if (map_data[x + width*(y-1)]==color3ub(0,255,33,255))
							frontQuadHelper(vertexes,exit_indices,at_v,at_e,x,middle_height,y-.5,.5,1.5,x,y,1,3);
						else
							frontQuadHelper(vertexes,wall_indices,at_v,at_w,x,middle_height,y-.5,.5,1.5,x,y,1,3);
				if ((int)x+1<map_size.x)
					if (!(map_data[x+1 + width*y]==color3ub(20,20,20,255)))
						if (map_data[x+1 + width*y]==color3ub(0,255,33,255))
					rightQuadHelper(vertexes,exit_indices,at_v,at_e,x+.5,middle_height,y,1.5,.5,x,y,1,3);
						else
					rightQuadHelper(vertexes,wall_indices,at_v,at_w,x+.5,middle_height,y,1.5,.5,x,y,1,3);
				if ((int)x-1>0)
					if (!(map_data[x-1 + width*y]==color3ub(20,20,20,255)))
						if (map_data[x-1 + width*y]==color3ub(0,255,33,255))
					leftQuadHelper(vertexes,exit_indices,at_v,at_e,	x-.5,middle_height,y,1.5,.5,x,y,1,3);
						else
					leftQuadHelper(vertexes,wall_indices,at_v,at_w,x-.5,middle_height,y,1.5,.5,x,y,1,3);
			}else if (*pixel==color3ub(0,255,33,255))
			{
				upQuadHelper(vertexes,exit_indices,at_v,at_e,x,floor_height,y,.5,.5,x,y,1,1);
				downQuadHelper(vertexes,floor_indices,at_v,at_f,x,ceiling_height,y,.5,.5,x,y,1,1);
			}else if (*pixel==color3ub(255,216,0,255))
			{
				if ((int)y+1<map_size.y)
					if (!(map_data[x + width*(y+1)]==color3ub(20,20,20,255)))
						backQuadHelper(vertexes,key_indices,at_v,at_k,x,.5,y+.5,.5,.5,x,y,1,1);
				if ((int)y-1>0)
					if (!(map_data[x + width*(y-1)]==color3ub(20,20,20,255)))
						frontQuadHelper(vertexes,key_indices,at_v,at_k,x,.5,y-.5,.5,.5,x,y,1,1);
				if ((int)x+1<map_size.x)
					if (!(map_data[x+1 + width*y]==color3ub(20,20,20,255)))
						rightQuadHelper(vertexes,key_indices,at_v,at_k,x+.5,.5,y,.5,.5,x,y,1,1);
				if ((int)x-1>0)
					if (!(map_data[x-1 + width*y]==color3ub(20,20,20,255)))
						leftQuadHelper(vertexes,key_indices,at_v,at_k,x-.5,.5,y,.5,.5,x,y,1,1);
				upQuadHelper(vertexes,key_indices,at_v,at_k,x,1,y,.5,.5,x,y,1,1);
				downQuadHelper(vertexes,floor_indices,at_v,at_f,x,ceiling_height,y,.5,.5,x,y,1,1);

				btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3((float)x,.5,(float)y)));
				btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0,boxMotionState,box,btVector3(0,0,0));
				btRigidBody* body = new btRigidBody(rigidBodyCI);
				world->addRigidBody(body);
			}else if (*pixel==color3ub(127,51,0,255))
			{
				btDefaultMotionState* wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3((float)x,middle_height,(float)y)));
				btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0,wallMotionState,wall,btVector3(0,0,0));
				btRigidBody* body = new btRigidBody(rigidBodyCI);
				world->addRigidBody(body);

				if ((int)y+1<map_size.y)
					if (!(map_data[x + width*(y+1)]==color3ub(20,20,20,255)))
						backQuadHelper(vertexes,door_indices,at_v,at_d,x,middle_height,y+.5,.5,1.5,x,y,1,3);
				if ((int)y-1>0)
					if (!(map_data[x + width*(y-1)]==color3ub(20,20,20,255)))
						frontQuadHelper(vertexes,door_indices,at_v,at_d,x,middle_height,y-.5,.5,1.5,x,y,1,3);
				if ((int)x+1<map_size.x)
					if (!(map_data[x+1 + width*y]==color3ub(20,20,20,255)))
						rightQuadHelper(vertexes,door_indices,at_v,at_d,x+.5,middle_height,y,1.5,.5,x,y,1,3);
				if ((int)x-1>0)
					if (!(map_data[x-1 + width*y]==color3ub(20,20,20,255)))
						leftQuadHelper(vertexes,door_indices,at_v,at_d,x-.5,middle_height,y,1.5,.5,x,y,1,3);
			}
		}
		map_floor_count = at_f;
		map_wall_count = at_w;
		map_exit_count = at_e;
		map_key_count = at_k;
		map_door_count = at_d;
		int map_vertex_count = at_v/STRIDE;

		glGenBuffers(1,&map_vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,map_vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,map_vertex_count * 5 * sizeof(float),vertexes,GL_STATIC_DRAW);
		glGenBuffers(1,&map_floor_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_floor_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_floor_count * sizeof(int),floor_indices,GL_STATIC_DRAW);
		glGenBuffers(1,&map_wall_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_wall_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_wall_count * sizeof(int),wall_indices,GL_STATIC_DRAW);
		glGenBuffers(1,&map_exit_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_exit_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_exit_count * sizeof(int),exit_indices,GL_STATIC_DRAW);
		glGenBuffers(1,&map_key_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_key_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_key_count * sizeof(int),key_indices,GL_STATIC_DRAW);
		glGenBuffers(1,&map_door_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_door_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,map_door_count * sizeof(int),door_indices,GL_STATIC_DRAW);

		free(vertexes);
		free(floor_indices);
		free(wall_indices);
		free(exit_indices);
		free(key_indices);
		free(door_indices);

		glGenTextures(1, &minimap_mask);
		glBindTexture(GL_TEXTURE_2D, minimap_mask);
		clear = malloc(height*width*4);
		memset(clear,0,height*width*4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, clear);
//		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,clear);
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

		btCollisionShape* floor = new btStaticPlaneShape(btVector3(0,1,0),0);
		btDefaultMotionState* floorMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,floorMotionState,floor,btVector3(0,0,0));
		btRigidBody* floorBody = new btRigidBody(groundRigidBodyCI);
		world->addRigidBody(floorBody);

		/*
		btCollisionShape* xpos = new btStaticPlaneShape(btVector3(1,0,0),-.5);
		btCollisionShape* xneg = new btStaticPlaneShape(btVector3(-1,0,0),-(map_size.x-.5f));
		btCollisionShape* zpos = new btStaticPlaneShape(btVector3(0,0,1),-.5);
		btCollisionShape* zneg = new btStaticPlaneShape(btVector3(0,0,-1),-(map_size.y-.5f));
		btDefaultMotionState* xposms = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btDefaultMotionState* xnegms = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btDefaultMotionState* zposms = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btDefaultMotionState* znegms = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
		btRigidBody::btRigidBodyConstructionInfo xposCI(0,xposms,xpos,btVector3(0,0,0));
		btRigidBody::btRigidBodyConstructionInfo xnegCI(0,xnegms,xneg,btVector3(0,0,0));
		btRigidBody::btRigidBodyConstructionInfo zposCI(0,zposms,zpos,btVector3(0,0,0));
		btRigidBody::btRigidBodyConstructionInfo znegCI(0,znegms,zneg,btVector3(0,0,0));
		btRigidBody* xposBody = new btRigidBody(xposCI);
		btRigidBody* xnegBody = new btRigidBody(xnegCI);
		btRigidBody* zposBody = new btRigidBody(zposCI);
		btRigidBody* znegBody = new btRigidBody(znegCI);
		world->addRigidBody(xposBody);
		world->addRigidBody(xnegBody);
		world->addRigidBody(zposBody);
		world->addRigidBody(znegBody);
		*/
}

void clearMap()
{
	int x = (int)(pos->getX()+.5);
	int y = (int)(pos->getZ()+.5);

	if (x<1) x=1;
	if (y<1) y=1;
	if (x+1>map_size.x) x = map_size.x-1;
	if (y+1>map_size.y) y = map_size.y-1;
	glBindTexture(GL_TEXTURE_2D,minimap_mask);
	glTexSubImage2D(GL_TEXTURE_2D,0,x-1,y-1,3,3,GL_RGBA,GL_UNSIGNED_BYTE,clear);
}

void drawMap()
{
	glColor3f(1,1,1);
	glBindBuffer(GL_ARRAY_BUFFER,map_vertex_vbo);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(float)*5,0);
	glTexCoordPointer(2,GL_FLOAT,sizeof(float)*5,(void*)(sizeof(float)*3));

	glBindTexture(GL_TEXTURE_2D,map_floor_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_floor_vbo);
	glDrawElements(GL_TRIANGLES,map_floor_count,GL_UNSIGNED_INT,0);

	glBindTexture(GL_TEXTURE_2D,map_wall_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_wall_vbo);
	glDrawElements(GL_TRIANGLES,map_wall_count,GL_UNSIGNED_INT,0);

	glBindTexture(GL_TEXTURE_2D,map_exit_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_exit_vbo);
	glDrawElements(GL_TRIANGLES,map_exit_count,GL_UNSIGNED_INT,0);

	glBindTexture(GL_TEXTURE_2D,map_key_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_key_vbo);
	glDrawElements(GL_TRIANGLES,map_key_count,GL_UNSIGNED_INT,0);

	glBindTexture(GL_TEXTURE_2D,map_door_texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,map_door_vbo);
	glDrawElements(GL_TRIANGLES,map_door_count,GL_UNSIGNED_INT,0);
}

void drawMiniMap()
{
	ortho();
	glPushMatrix();
	glLoadIdentity();

	int zoom = 15;
	double rady1 = toRad(-rot->getY()+45);
	double rady2 = toRad(-rot->getY()+135);
	double rady3 = toRad(-rot->getY()+225);
	double rady4 = toRad(-rot->getY()+315);
	double x = pos->getX()+.5f;
	double z = pos->getZ()+.5f;

	glBindTexture(GL_TEXTURE_2D,0);

	glBegin(GL_QUADS);
	glColor4d(0,0,0,.8);
	glTexCoord2d(1,1);
	glVertex3d(0,0,0);
	glTexCoord2d(0,1);
	glVertex3d(minimap_size,0,0);
	glTexCoord2d(0,0);
	glVertex3d(minimap_size,minimap_size,0);
	glTexCoord2d(1,0);
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

void upQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty)
{
	int start = at_v/STRIDE;
	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,tx);
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,ty,0);

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,ty,0,0);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,tx,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,ty);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,tx);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,ty);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}

void downQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dz,unsigned int tsx,unsigned int tsy,float tx,float ty)
{
	int start = at_v/STRIDE;

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,tx);
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,ty,0);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,tx);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,ty);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,tx,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,ty);

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,ty,0,0);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}

void backQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty)
{
	int start = at_v/STRIDE;

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,ty,0);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,tx);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,tx,ty,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,tx);

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,tx,0,0);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}

void frontQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dx, float dy,unsigned int tsx,unsigned int tsy,float tx, float ty)
{
	int start = at_v/STRIDE;

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,ty,0);

	vertexes[at_v++] = x-dx;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,tx,0,0);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,tx,ty,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,tx);

	vertexes[at_v++] = x+dx;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,tx);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}

void rightQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty)
{
	int start = at_v/STRIDE;

	vertexes[at_v++] = x;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,ty,0);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,tx,0,0);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,ty,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,tx);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,tx);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}

void leftQuadHelper(float*& vertexes, unsigned int*& indices, int& at_v, int& at_i,float x, float y,float z, float dy, float dz,unsigned int tsx,unsigned int tsy,float tx, float ty)
{
	int start = at_v/STRIDE;

	vertexes[at_v++] = x;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,0,tx,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,tx,ty,0);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y-dy;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,0,0,ty);
	vertexes[at_v++] = texswap(tsx,tsy,0,0,ty,tx);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z+dz;
	vertexes[at_v++] = texswap(tsx,tsy,tx,ty,0,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,0,0,tx);

	vertexes[at_v++] = x;
	vertexes[at_v++] = y+dy;
	vertexes[at_v++] = z-dz;
	vertexes[at_v++] = texswap(tsx,tsy,0,ty,tx,0);
	vertexes[at_v++] = texswap(tsx,tsy,ty,tx,0,0);

	indices[at_i++] = start;
	indices[at_i++] = start+1;
	indices[at_i++] = start+2;
	indices[at_i++] = start;
	indices[at_i++] = start+2;
	indices[at_i++] = start+3;
}
