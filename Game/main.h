#pragma once

#include "gl.h"
#include "img.h"
#include "sky.h"
#include "listeners.h"
#include "map.h"
#include "utils.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics\Character\btKinematicCharacterController.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

extern btKinematicCharacterController* character;
extern btDiscreteDynamicsWorld* world;

int main(int argc, char **argv);
void setupWorld();