#pragma once

#include <gl/glew.h>
#include <gl/freeglut_std.h>
#include <gl/freeglut_ext.h>

#undef glGenBuffers
#undef glCreateBuffer
#undef glBindBuffer
#undef glDeleteBuffers

#define glGenBuffers glGenBuffersARB
#define glCreateBuffer glBindBufferARB
#define glBindBuffer glBindBufferARB
#define glDeleteBuffers glDeleteBuffersARB
