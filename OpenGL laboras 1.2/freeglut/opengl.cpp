#include <iostream>
#include <stdlib.h> 
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

int n = 5;
float pi = 3.14159265356;
float rotx = 10.0, roty = -10.0;
float tet = 0;
float fi = 0;
float tetMax = 5*pi;
float stepsize = 0.1;
bool buttonPressed[3]; 

float a = 0.1;
float b = 0.1;
float R = 1;

float sukti_x = 0.0;
float sukti_y = 0.0;
float zoom = 1.0;  
float tx = 0.0, ty = 0.0;  

int lastx = 0, lasty = 0;

void key(unsigned char  keys, int x, int y)
{
	if (keys == GLUT_KEY_RIGHT)          sukti_x += 5;
	else if (keys == GLUT_KEY_LEFT)      sukti_x -= 5;
	else if (keys == GLUT_KEY_UP)        sukti_y += 5;
	else if (keys == GLUT_KEY_DOWN)      sukti_y -= 5;
	else if (keys==27) exit(0);
	/*
  glutPostRedisplay();
  if (keys == GLUT_KEY_RIGHT)
    sukti_x += 5;
  else if (keys == GLUT_KEY_LEFT)
    sukti_x -= 5;
  else if (keys == GLUT_KEY_UP)
    sukti_y += 5;
  else if (keys == GLUT_KEY_DOWN)
    sukti_y -= 5;
  else if (keys == 27)
	  exit(0);
	glRotatef(sukti_x, 1.0, 0.0, 1.0 );
    glRotatef(sukti_y, 0.0, 1.0, 1.0 );*/
    glutPostRedisplay(); 
};
static void plottriangle (float x1, float y1, float z1,float x2, float y2, float z2,float x3, float y3, float z3)
{
	glBegin(GL_TRIANGLES);

	float x1n = x1*x1+y1*y1+z1*z1;
	float x2n = x2*x2+y2*y2+z2*z2;
	float x3n = x3*x3+y3*y3+z3*z3;
	glColor3f(x1/x1n,y1/x1n,z1/x1n);

	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x3, y3,z3);

	glEnd;
}

static float seashellx(float tet, float fi)
{

	float x;
	x = (a*tet+ R*sin(fi))*cos(tet);
	return x;
}
static float seashelly(float tet, float fi)
{

	float y;
	y = (a*tet+ R*sin(fi))*sin(tet);
	return y;
}

static float seashellz(float tet, float fi)
{
	float z;
	z = b*tet + R*(1-cos(fi));
	return z;
}

void cbMouseClick( int button, int state, int x, int y )
{
    switch ( button )
    {
	case GLUT_LEFT_BUTTON:
	    buttonPressed[0] = ( state == GLUT_DOWN );
	    break;
	case GLUT_MIDDLE_BUTTON:
	    buttonPressed[1] = ( state == GLUT_DOWN );
	    break;
	case GLUT_RIGHT_BUTTON:
	    buttonPressed[2] = ( state == GLUT_DOWN );
	    break;
    }
    lastx = x;
    lasty = y;
}

void cbMouseMove( int x, int y )
{
    const int dx = x - lastx;
    const int dy = y - lasty;
    lastx = x;
    lasty = y;

    if ( buttonPressed[0] )
    {
	rotx += (float) 0.5 * dy;
	roty += (float) 0.5 * dx;
    }
    else if ( buttonPressed[1] )
    {
	zoom -= (float) 0.05 *  dy;
    }
    else if ( buttonPressed[2] )
    {
	tx += (float) 0.01 * dx;
	ty -= (float) 0.01 * dy;
    }
    glutPostRedisplay();
}

static void display(void)
{
	glLoadIdentity();

	gluLookAt( 0, 0, 5,   
	           0.0, 0.0, 0.0,   
	           0.0, 1.0, 0.0 );
	GLfloat light_position[] = { 5.0, 5.0, 3.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    glRotatef( sukti_x, 1.0, 0.0, 0.0 );
    glRotatef( sukti_y, 0.0, 1.0, 0.0 );

	glTranslatef( 0.0, 0.0, -zoom );
    glTranslatef( tx, ty, 0.0 );
    glRotatef( rotx, 1.0, 0.0, 0.0 );
    glRotatef( roty, 0.0, 1.0, 0.0 );
	for(float tet = 0; tet < tetMax; tet+=stepsize){
  for(float fi = 0; fi < pi; fi+=stepsize){
    plottriangle(seashellx(tet, fi), seashelly(tet, fi), seashellz(tet, fi), seashellx(tet+stepsize, fi), seashelly(tet+stepsize, fi), seashellz(tet+stepsize, fi),seashellx(tet+stepsize, fi+stepsize), seashelly(tet+stepsize, fi+stepsize), seashellz(tet+stepsize, fi+stepsize));
    plottriangle(seashellx(tet, fi), seashelly(tet, fi), seashellz(tet, fi),seashellx(tet, fi+stepsize), seashelly(tet, fi+stepsize), seashellz(tet, fi+stepsize),seashellx(tet+stepsize, fi+stepsize), seashelly(tet+stepsize, fi+stepsize), seashellz(tet+stepsize, fi+stepsize));

  }
}

    glFlush();
    glutSwapBuffers();
}

void init()
{

    glEnable( GL_CULL_FACE );
 
}

static void idle(void)
{
    glutPostRedisplay();
}

static void resize(int width, int height)
{
    const float ar = 5*((float) width / (float) height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ar, ar, -5.0, 5.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

int main( int argc, char *argv[] )
{
    // Initialize GLUT and register callback functions

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 400, 400 );

    glutCreateWindow( "Seashell" );
	glutFullScreen();

	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutReshapeFunc(resize);
	glutMouseFunc( cbMouseClick );
    glutMotionFunc( cbMouseMove );

	glFlush();
    glutMainLoop();
	return 0;
}