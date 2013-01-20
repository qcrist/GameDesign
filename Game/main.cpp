#include "main.h"

btKinematicCharacterController* character;
btDiscreteDynamicsWorld* world;
int level = 0;

int main(int argc, char** args)
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0A );
	//GLUT
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("OpenGL Window\n");
	initListeners();
	glutSetCursor(GLUT_CURSOR_NONE); 

	//GLEW
	glewInit();

	//OpenGL
	glClearColor(.1f,.1f,.1f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	//	glPolygonMode(GL_BACK,GL_LINE);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable (GL_FOG); //enable the fog

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
