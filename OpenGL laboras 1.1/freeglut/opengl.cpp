#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
 
#define X .525731112119133606 
#define Z .850650808352039932

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint   axis = 0;

int divi = 0;
 
 
GLfloat colors[][3] = {
    {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
    {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0},
    {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}
};

static GLfloat vdata[12][3] = {    
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};
static GLuint tindices[20][3] = { 
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
 
void normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}
 
void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r) {
    if (div<=0) {
        glNormal3fv(a); glVertex3f(a[0]*r, a[1]*r, a[2]*r); glColor3fv (colors[1]);
        glNormal3fv(b); glVertex3f(b[0]*r, b[1]*r, b[2]*r); glColor3fv (colors[2]);
        glNormal3fv(c); glVertex3f(c[0]*r, c[1]*r, c[2]*r); glColor3fv (colors[4]);
    } else {
        GLfloat ab[3], ac[3], bc[3];
        for (int i=0;i<3;i++) {
            ab[i]=(a[i]+b[i])/2;
            ac[i]=(a[i]+c[i])/2;
            bc[i]=(b[i]+c[i])/2;
        }
        normalize(ab); normalize(ac); normalize(bc);
        drawtri(a, ab, ac, div-1, r);
        drawtri(b, bc, ab, div-1, r);
        drawtri(c, ac, bc, div-1, r);
        drawtri(ab, bc, ac, div-1, r);  
    }  
}
 
void drawsphere(int ndiv, float radius=1.0) {
    glBegin(GL_TRIANGLES);
    for (int i=0;i<20;i++)
        drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], divi, radius);
    glEnd();
}

void mouse (int btn, int state, int x, int y)
{
    char *sAxis [] = { "X-axis", "Y-axis", "Z-axis" };
 
    /* mouse callback, selects an axis about which to rotate */
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        axis = (++axis) % 3;
        printf ("Rotate about %s\n", sAxis[axis]);

		
    }
}

void display (void)
{
    /* display callback, clear frame buffer and z buffer, 
       rotate cube and draw, swap buffers */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    glRotatef (theta[0], 1.0, 0.0, 0.0);    /* x */
    glRotatef (theta[1], 0.0, 1.0, 0.0);    /* y */
    glRotatef (theta[2], 0.0, 0.0, 1.0);    /* z */
	
    drawsphere (2, 1.1);
 
    glFlush ();
    glutSwapBuffers ();
}

void spinCube (void)
{
    /* Idle callback, rotate cube 0.2 degrees about selected axis */
    theta[axis] += 0.8;
    if (theta[axis] > 360.0 )
        theta[axis] -= 360.0;
    glutPostRedisplay ();
}

/*void process_Normal_Keys(unsigned char key, int x, int y) 
{
	switch(key){
	case 43 : {
			if(rekursija < 5){
				int length = cubes.size();

				for(int i=0;i < length;i++ ){
				if(cubes[i].rekursija == rekursija){
					dividecube(i);
				}
		
				}
				rekursija++;
			}
		}
		break;
	case 45 : {
			if(rekursija>0){
				int length = cubes.size();
				rekursija--;
				for(int i=0;i < cubes.size();i++ ){
					if(cubes[i].rekursija > rekursija){
						cubes.erase(cubes.begin()+i);
					}
				}
			}
		}    //algirdas.maknickas@vgtu.lt
		break;
	}

}
*/

void myReshape (int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
        glOrtho (-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w, 
                             2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho (-2.0 * (GLfloat) w / (GLfloat) h, 
                  2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode (GL_MODELVIEW);
}

void Keyboard_keys(unsigned char key, int x, int y) 
{
	switch(key){
	case 43 : {
			
			divi++;
		}
		break;
	case 45 : {
			if(divi>0)
			divi--;
		}
		break;
	}

}

int main(int argc, char **argv) {

	 glutInit (&argc, argv);

	  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutCreateWindow ("Color Cube");
    glutReshapeFunc (myReshape);
    glutDisplayFunc (display);
    glutIdleFunc (spinCube);
    glutMouseFunc (mouse);

	glutKeyboardFunc( Keyboard_keys );

    glEnable (GL_DEPTH_TEST); /* Enable hidden-surface-removal */
 
    glutMainLoop ();
	 //std::cout<<"df";
 
    return 0;
}