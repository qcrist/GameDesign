#include "main.h"

btKinematicCharacterController* character;
btDiscreteDynamicsWorld* world;
int level = 0;
unsigned int start_screen;
unsigned int start_screen_width;
unsigned int start_screen_height;

void start_draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	ortho();
	int needwidth = window_size.y*start_screen_width/start_screen_height;
	int offset = needwidth-window_size.x;
	offset/=2;
	glBindTexture(GL_TEXTURE_2D,start_screen);
	glTranslated(-offset,0,0);
	glBegin(GL_QUADS);
	glColor4d(1,1,1,1);
	glTexCoord2d(1,0);
	glVertex3d(0,0,0);
	glTexCoord2d(0,0);
	glVertex3d(needwidth,0,0);
	glTexCoord2d(0,1);
	glVertex3d(needwidth,window_size.y,0);
	glTexCoord2d(1,1);
	glVertex3d(0,window_size.y,0);
	glEnd();
	projection();
	glutSwapBuffers();
}

void start_resize(int w, int h)
{
	resize(w,h);
	start_draw();
}

void start_keydown(unsigned char key, int,int)
{
	if (key == ' ')
	{
		glutSetCursor(GLUT_CURSOR_NONE); 
		glutDisplayFunc(pause_draw);
		glutIdleFunc(NULL);
		glutReshapeFunc(pause_resize);
		glutKeyboardFunc(pause_keydown);
		glutPassiveMotionFunc(NULL);
		glutSetCursor(GLUT_CURSOR_INHERIT);
		pause_draw();
	}
}

int main(int argc, char** args)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0A );
	//GLUT
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("OpenGL Window\n");
	initListeners();
//	glutSetCursor(GLUT_CURSOR_NONE); 

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_BACK,GL_LINE);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutDisplayFunc(start_draw);
	glutIdleFunc(NULL);
	glutReshapeFunc(start_resize);
	glutKeyboardFunc(start_keydown);
	glutPassiveMotionFunc(NULL);
	start_screen = loadTexture("start.png",start_screen_width,start_screen_height);

	//glEnable (GL_FOG); //enable the fog

	glFogi (GL_FOG_MODE, GL_EXP2); 
	GLfloat fogColor[4] = {0, 0.0, 0.0, 1.0};
	glFogfv (GL_FOG_COLOR, fogColor); 
	glFogf (GL_FOG_DENSITY, .3f);
	glHint (GL_FOG_HINT, GL_FASTEST); 

	//Physics
	setupWorld();

	//Load Sky
	initSky("sky.png");

	//Read map
	initMap(level);

	//Start
	glutMainLoop();

	return 0;
}


void setupWorld()
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	//stepheight .1
	btPairCachingGhostObject* obj = new btPairCachingGhostObject();
	obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	btCapsuleShape* shape = new btCapsuleShape(.3f,.4f);
	obj->setCollisionShape(shape);
	character = new btKinematicCharacterController(obj,shape,.1f);
	obj->setWorldTransform(btTransform::getIdentity());

	world = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	world->addCollisionObject(character->getGhostObject(),btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	world->addAction(character);

	pos = &obj->getWorldTransform().getOrigin();
	rot = new btVector3(0,0,0);

	world->setGravity(btVector3(0,-15,0));
	character->setGravity(-world->getGravity().y());
	character->setJumpSpeed(5);
	character->setUseGhostSweepTest(false);
}
