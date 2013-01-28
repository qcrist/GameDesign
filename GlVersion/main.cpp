#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#pragma comment(lib,"freeglut.lib")
#pragma comment(lib,"glew.lib")

int main(int argc, char** args)
{
glutInit(&argc,args);
glutInitWindowSize(1,1);
glutInitWindowPosition(-10,-10);
glutCreateWindow("");
glutHideWindow();
glewInit();
int n;
glGetIntegerv(GL_NUM_EXTENSIONS, &n); 
for (int i=0; i<n; i++) 
{ 
  const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
  printf("%s\n", extension); 
}
system("pause");
return 0;
}