//---------------------------------------
// Program: curve.cpp
// Purpose: To display lines and curves
// Author:  John Gauch
// Date:    Spring 2013
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global constants
#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500
#define LINE_COUNT 1000
#define LINE 0
#define CURVE 1
#define BEZIER 2
#define SPLINE 3

// Global variables 
int count = -1;
int mode = LINE;
float X[LINE_COUNT];
float Y[LINE_COUNT];
int linesize = 0;

GLUnurbsObj *theNurb;
//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);

   glShadeModel (GL_FLAT);
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Change drawing mode
   if ((key == 'l') || (key == 'L'))
      mode = LINE;
   else if ((key == 's') || (key == 'S'))
      mode = SPLINE;

   switch(key){
	case 43 : {
			
			linesize++;
		}
		break;
	case 45 : {
		if(linesize >0)
			linesize--;
		}
		break;
	}
   
   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN); //1/5
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);//1/5

   // Handle mouse down
   if (state == GLUT_DOWN)
   {

	   if( button==GLUT_RIGHT_BUTTON )
		{
			count--;
		} 
		else if( button==GLUT_LEFT_BUTTON )
		{
			count++; 
			X[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
			Y[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
		}
	}
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   X[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   Y[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   std::cout<<x<<" "<<y<<" "<<X[count]<<" "<<Y[count]<<std::endl;
   glutPostRedisplay();
}


//---------------------------------------
// Draw B-spline curve
//---------------------------------------
void draw_spline()
{
    GLfloat ctlpoints[4][3] = {{-.75, -.75, 0.0},
	{-.5, .75, 0.0}, {.5, .75, 0.0}, {.75, -.75, 0.0}};
    GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

	int skaitliukas = -1;
 
    for(int i=0;i<count;i++){

		skaitliukas++;
		ctlpoints[skaitliukas][0]=X[i];
		ctlpoints[skaitliukas][1]=Y[i];
		ctlpoints[skaitliukas][2]=0.0;

		if(skaitliukas>=3){
			gluBeginCurve(theNurb);
			gluNurbsCurve(theNurb,
				8, knots,
				3,
				&ctlpoints[0][0],
				4,
				GL_MAP1_VERTEX_3);
			gluEndCurve(theNurb);
			skaitliukas=0;
			ctlpoints[skaitliukas][0]=X[i];
			ctlpoints[skaitliukas][1]=Y[i];
			ctlpoints[skaitliukas][2]=0.0;
		}
	}
    glFlush();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw lines
   if (mode == LINE)
   {
      // Draw control points
      glColor3f(1.0, 1.0, 1.0);
      glPointSize(3);
      glBegin(GL_POINTS);
      for (int i=0; i<=count; i++)
         glVertex2f(X[i], Y[i]);
      glEnd();

      // Connect control points
      glLineWidth(1);
      glBegin(GL_LINE_STRIP);
      for (int i=0; i<=count; i++)
         glVertex2f(X[i], Y[i]);
      glEnd();
   }

   // Draw B-spline curves
   if (mode == SPLINE)
   {
      glColor3f(0.0, 1.0, 0.0);
      glLineWidth(linesize);
         draw_spline();
   }

   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Curve");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
  
   init();
   printf("Keyboard commands:\n");
   printf("   'l' - draw lines\n");
   printf("   's' - draw B-spline curves\n");
   glutMainLoop();
   return 0;
}