#pragma once

#include <gl/glew.h>
#include <GL/glut.h>

#undef glGenBuffers
#undef glCreateBuffer
#undef glBindBuffer
#undef glDeleteBuffers

#define glGenBuffers glGenBuffersARB
#define glCreateBuffer glBindBufferARB
#define glBindBuffer glBindBufferARB
#define glDeleteBuffers glDeleteBuffersARB