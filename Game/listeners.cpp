#include "listeners.h"

btVector3* pos;
btVector3* rot;
vector2i window_size;
float mat[16];

void testForCollisions()
{
	 btManifoldArray   manifoldArray;
	 btBroadphasePairArray& pairArray = character->getGhostObject()->getOverlappingPairCache()->getOverlappingPairArray();
      int numPairs = pairArray.size();

      for (int i=0;i<numPairs;i++)
      {
         manifoldArray.clear();

         const btBroadphasePair& pair = pairArray[i];

		 //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
		 btBroadphasePair* collisionPair = world->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
		 if (!collisionPair)
			 continue;

		 if (collisionPair->m_algorithm)
			 collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		 for (int j=0;j<manifoldArray.size();j++)
		 {
			 btPersistentManifold* manifold = manifoldArray[j];
			 const btCollisionObject* obA = (manifold->getBody0());
			 const btCollisionObject* obB = (manifold->getBody1());
			 for (int p=0;p<manifold->getNumContacts();p++)
			 {
				 const btManifoldPoint&pt = manifold->getContactPoint(p);
				 btCollisionObject* other = NULL;
				 if (obA == character->getGhostObject())
					 other = (btCollisionObject*)obB;
				 else if (obB == character->getGhostObject())
					 other = (btCollisionObject*)obA;
				 if (other == NULL) continue;
				 if (other->getUserPointer()==NULL) continue;
				 int x = other->getWorldTransform().getOrigin().x();
				 int y = other->getWorldTransform().getOrigin().z();
				 for (int i=0;i<map_keys->size();i++)
				 {
					 if (map_keys->operator[](i)==other->getUserPointer())
					 {
						 color3ub c(128,128,128,255);
						 glBindTexture(GL_TEXTURE_2D,minimap);
						 glTexSubImage2D(GL_TEXTURE_2D,0,x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&c);
						 map_keys->erase(map_keys->begin()+i);
						 world->removeCollisionObject(other);
						key_count ++;
						printf("Key + 1 :D\n");
						return;
					}
				}
				if (key_count>0)
				{
					for (int i=0;i<map_doors->size();i++)
					{
						if (map_doors->operator[](i)==other->getUserPointer())
						{
							color3ub c(128,128,128,255);
							glBindTexture(GL_TEXTURE_2D,minimap);
							glTexSubImage2D(GL_TEXTURE_2D,0,x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&c);
							key_count --;
							map_doors->erase(map_doors->begin()+i);
							world->removeCollisionObject(other);
							printf("Door - 1 :D\n");
							return;
						}
					}
				}
				if (other->getUserPointer() == (void*)1)
				{
					destroyMap();
					setupWorld();
					level++;
					if (level >= sizeof(maps)/sizeof(const char*))
					{
						printf("WIN!");
						exit(5);
					}
					initMap(level);
				}
				return;
			 }
		 }
	  }
}


float r = 0;
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	testForCollisions();
	clearMap();
	testKeys();
	glRotatef(rot->getX(),1,0,0);
	glRotatef(rot->getY(),0,1,0);
	glTranslated(-pos->getX(),-pos->getY()-.5,-pos->getZ());

	drawSky();
	drawMap();
	drawMiniMap();
	glutSwapBuffers();
}

void initListeners()
{
	glutDisplayFunc(draw);
	glutIdleFunc(fps);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(skeydown);
	glutSpecialUpFunc(skeyup);
	glutMotionFunc(mousemove);
	glutPassiveMotionFunc(mousemove);
}

void testKeys()
{
	float yrad = toRadf(rot->getY());
	float xrad = toRadf(rot->getY());
	float cosy = cosf(yrad);
	float siny = sinf(yrad);

	float dx = 0;
	float dz = 0;
	if (keys['s'])
	{
		dz += cosy/10;
		dx -= siny/10;
	}
	if (keys['w'])
	{
		dz -= cosy/10;
		dx += siny/10;
	}
	if (keys['d'])
	{
		dz += siny/10;
		dx += cosy/10;
	}
	if (keys['a'])
	{
		dz -= siny/10;
		dx -= cosy/10;
	}
	if (skeys[GLUT_KEY_END])
		exit(0);

	character->setWalkDirection(btVector3(dx,0,dz));

	/*
	if (dx<0)
	if (pos.x+dx<-.5)
	pos.x = -.5;
	else
	pos.x += dx;
	else
	if (pos.x+dx>map_size.x-.5)
	pos.x = map_size.x-.5;
	else
	pos.x += dx;

	if (dz<0)
	if (pos.z+dz<-.5)
	pos.z = -.5;
	else
	pos.z += dz;
	else
	if (pos.z+dz>map_size.y-.5)
	pos.z = map_size.y-.5;
	else
	pos.z += dz;*/
}

long next_frame;
long last_frame;
int fps_limit = 60;
void fps()
{
	long now = clock();
	if (now>=next_frame)
	{
		float spent = (now-last_frame)/1000.f;
		next_frame = now + CLOCKS_PER_SEC/fps_limit;
		float current_fps = CLOCKS_PER_SEC/(float)(now-last_frame);
		last_frame = clock();
		char buf[50];
		sprintf(buf,"FPS: %f",current_fps);
		glutSetWindowTitle(buf);
		world->stepSimulation(spent);
		draw();
	}else{
		if (next_frame-now > 10)
		{
			sleep(10);
		}
		else 
		{
			sleep(next_frame-now);
		}
	}
}

void resize(int w, int h)
{
	if (h == 0)h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,(float)w/h,.1,5000);
	glMatrixMode(GL_MODELVIEW);
	window_size.x = w;
	window_size.y = h;
}


char keys[256];
char skeys[256]; //<-- Max value of special key

void keydown(unsigned char key,int,int)
{
	keys[key] = true;
	if (key == ' ' && character->canJump() && character->onGround())
		character->jump();
	if (key == '+' && zoom>5)
		zoom -= 5;
	if (key == '-')
		zoom += 5;
}

void keyup(unsigned char key,int,int)
{
	keys[key] = false;
}

void skeydown(int key,int,int)
{
	skeys[key] = true;
}

void skeyup(int key,int,int)
{
	skeys[key] = false;
}

void mousemove(int x, int y)
{
	if (x==window_size.x/2 && y==window_size.y/2)
		return;
	int dy = window_size.y/2-y;
	int dx = window_size.x/2-x;
	if (dy>30)
		dy = dx = 0;
	if (dx>30)
		dx = dy = 0;
	rot->setX(rot->getX()-dy);
	if (rot->getX() < -85)
		rot->setX(-85);
	if (rot->getX() > 85)
		rot->setX(85);
	rot->setY(rot->getY()-dx);
	//	rot.y -= dx;
	glutWarpPointer(window_size.x/2,window_size.y/2);
}