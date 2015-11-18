/*==========================================================*/
/* ColorCube.c                                              */
/*==========================================================*/
/*                                                          */
/* Rotating cube with color interpolation                   */
/*                                                          */
/* E. Angel, Interactive Computer Graphics                  */
/* A Top-Down Approach with OpenGL, Third Edition           */
/* Addison-Wesley Longman, 2003                             */
/*                                                          */
/* Updated:  Bary W Pollack; March 2005                     */
/*                                                          */
/* Demonstration of use of homogeneous coordinate           */
/* transformations and simple data structure for            */
/* representing the cube from Chapter 4                     */
/*                                                          */
/* Colors are assigned to the vertices                      */
/* Left mouse button control direction of rotation          */
/*Egzaminas 4 balai is praktiniu uzsiemimu. 6 is egzo. egze bus teorja, 2 balai teorijoje is praktines uzduoteles.
Savarankiska uzduotis- */
/*==========================================================*/
 
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint   axis = 0;

/***************************************************************************
  OBJ Loading
 ***************************************************************************/
 
class Model_OBJ
{
  public:
    Model_OBJ();
    float* calculateNormal(float* coord1,float* coord2,float* coord3 );
    int Load(char *filename);	// Loads the model
    void Draw();			// Draws the model on the screen
    void Release();			// Release the model
    float* normals;			// Stores the normals
    float* Faces_Triangles;		// Stores the triangles
    float* vertexBuffer;		// Stores the points which make the object
    long TotalConnectedPoints;		// Stores the total number of connected verteces
    long TotalConnectedTriangles;	// Stores the total number of connected triangles
};
 
 
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
using namespace std;
 
Model_OBJ::Model_OBJ()
{
   this->TotalConnectedTriangles = 0;
   this->TotalConnectedPoints = 0;
}
 
float* Model_OBJ::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
   /* calculate Vector1 and Vector2 */
   float va[3], vb[3], vr[3], val;
   va[0] = coord1[0] - coord2[0];
   va[1] = coord1[1] - coord2[1];
   va[2] = coord1[2] - coord2[2];
 
   vb[0] = coord1[0] - coord3[0];
   vb[1] = coord1[1] - coord3[1];
   vb[2] = coord1[2] - coord3[2];
 
   /* cross product */
   vr[0] = va[1] * vb[2] - vb[1] * va[2];
   vr[1] = vb[0] * va[2] - va[0] * vb[2];
   vr[2] = va[0] * vb[1] - vb[0] * va[1];
 
   /* normalization factor */
   val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
 
   float norm[3];
   norm[0] = vr[0]/val;
   norm[1] = vr[1]/val;
   norm[2] = vr[2]/val;
 
   return norm;
}
 
 
int Model_OBJ::Load(char* filename)
{
   string line;
   ifstream objFile (filename);
   if (objFile.is_open())					// If obj file is open, continue
   {
      objFile.seekg (0, ios::end);				// Go to end of the file,
      long fileSize = objFile.tellg();				// get file size
      objFile.seekg (0, ios::beg);				// we'll use this to register memory for our 3d model
 
      vertexBuffer = (float*) malloc (fileSize);		// Allocate memory for the verteces
      Faces_Triangles = (float*) malloc(fileSize*sizeof(float));// Allocate memory for the triangles
      normals  = (float*) malloc(fileSize*sizeof(float));	// Allocate memory for the normals
 
      int triangle_index = 0;					// Set triangle index to zero
      int normal_index = 0;					// Set normal index to zero
 
      while (! objFile.eof() )					// Start reading file data
      {
         getline(objFile,line);				// Get line from file
 
         if (line.c_str()[0] == 'v')				// The first character is a v: on this line is a vertex stored.
         {
            line[0] = ' ';					// Set first character to 0. This will allow us to use sscanf
 
            sscanf(line.c_str(),"%f %f %f ",			// Read floats from the line: v X Y Z
               &vertexBuffer[TotalConnectedPoints],
               &vertexBuffer[TotalConnectedPoints+1],
               &vertexBuffer[TotalConnectedPoints+2]);
 
               TotalConnectedPoints += POINTS_PER_VERTEX;	// Add 3 to the total connected points
         }
         if (line.c_str()[0] == 'f')				// The first character is an 'f': on this line is a point stored
         {
            line[0] = ' ';					// Set first character to 0. This will allow us to use sscanf
 
            int vertexNumber[4] = { 0, 0, 0 };
            sscanf(line.c_str(),"%i%i%i",			// Read integers from the line:  f 1 2 3
               &vertexNumber[0],				// First point of our triangle. This is an
               &vertexNumber[1],				// pointer to our vertexBuffer list
               &vertexNumber[2] );				// each point represents an X,Y,Z.
 
               vertexNumber[0] -= 1;				// OBJ file starts counting from 1
               vertexNumber[1] -= 1;				// OBJ file starts counting from 1
               vertexNumber[2] -= 1;				// OBJ file starts counting from 1
 
 
            /********************************************************************
             * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z).
             * The vertexBuffer contains all verteces
             * The triangles will be created using the verteces we read previously
            */
 
            int tCounter = 0;
            int co = 3;
            for (int i = 0; i < POINTS_PER_VERTEX; i++)
            {
               Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[co*vertexNumber[i] ];
               Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[co*vertexNumber[i]+1 ];
               Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[co*vertexNumber[i]+2 ];
               tCounter += POINTS_PER_VERTEX;
               /*cout << vertexNumber[i] << " "<< vertexBuffer[co*vertexNumber[i]] << " "
                    << vertexBuffer[co*vertexNumber[i]+1] << " "
                    << vertexBuffer[co*vertexNumber[i]+2] << endl;*/
 
             }
 
             /*********************************************************************
              * Calculate all normals, used for lighting
             */
             float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],
                                 Faces_Triangles[triangle_index+2]};
             float coord2[3] = { Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],
                                 Faces_Triangles[triangle_index+5]};
             float coord3[3] = { Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],
                                 Faces_Triangles[triangle_index+8]};
             const float *norm = this->calculateNormal( coord1, coord2, coord3 );
 
             tCounter = 0;
             for (int i = 0; i < POINTS_PER_VERTEX; i++)
             {
                normals[normal_index + tCounter ] = norm[0];
                normals[normal_index + tCounter +1] = norm[1];
                normals[normal_index + tCounter +2] = norm[2];
                tCounter += POINTS_PER_VERTEX;
              }
 
              triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
              normal_index += TOTAL_FLOATS_IN_TRIANGLE;
              TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;
           }
        }
	objFile.close();						// Close OBJ file
    }
    else
    {
        cout << "Unable to open file";
    }
    return 0;
}
 
void Model_OBJ::Release()
{
    free(this->Faces_Triangles);
    free(this->normals);
    free(this->vertexBuffer);
}
 
void Model_OBJ::Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);			// Enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);			// Enable normal arrays
	glEnableClientState(GL_COLOR_ARRAY);			// Enable normal arrays
    glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);		// Vertex Pointer to triangle array
    glNormalPointer(GL_FLOAT, 0, normals);			// Normal pointer to normal array
	glColorPointer(3,GL_FLOAT,	0,Faces_Triangles);
    glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);	// Draw the triangles
    glDisableClientState(GL_VERTEX_ARRAY);			// Disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);			// Disable normal arrays
}
 
/*==========================================================*/
 
Model_OBJ a;
void display (void)
{
    /* display callback, clear frame buffer and z buffer, 
       rotate cube and draw, swap buffers */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    glRotatef (theta[0], 1.0, 0.0, 0.0);    /* x */
    glRotatef (theta[1], 0.0, 1.0, 0.0);    /* y */
    glRotatef (theta[2], 0.0, 0.0, 1.0);    /* z */
 
    

	//glBegin(GL_TRIANGLES);

	a.Draw();
	//glEnd();
	

    glFlush ();
    glutSwapBuffers ();
}
 
/*==========================================================*/
 
void spinCube (void)
{
    /* Idle callback, rotate cube 0.2 degrees about selected axis */
    theta[axis] += 0.8;
    if (theta[axis] > 360.0 )
        theta[axis] -= 360.0;
    glutPostRedisplay ();
}
 
/*==========================================================*/
 
void mouse (int btn, int state, int x, int y)
{
    char *sAxis [] = { "X-axis", "Y-axis", "Z-axis" };
 
    /* mouse callback, selects an axis about which to rotate */
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        axis = (++axis) % 3;
        printf ("Rotate about %s\n",  
sAxis[axis]);

		
    }
}
void process_Normal_Keys(unsigned char key, int x, int y) 
{
	switch(key){
	case 43 : {
			
		}
		break;
	case 45 : {
			
		}
		break;
	}

}

/*==========================================================*/
 
void myReshape (int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
        glOrtho (-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w, 
                             4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho (-2.0 * (GLfloat) w / (GLfloat) h, 
                  2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode (GL_MODELVIEW);
}
 
 
/*==========================================================*/
 
int main (int argc, char **argv)
{
    glutInit (&argc, argv);

	
 
    printf ("\nPress left mouse button to change rotation axis\n\n");
    /* need both double buffering and z buffer */
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutCreateWindow ("Color Cube");
    glutReshapeFunc (myReshape);
    glutDisplayFunc (display);
    glutIdleFunc (spinCube);
    glutMouseFunc (mouse);

	glutKeyboardFunc( process_Normal_Keys );

    glEnable (GL_DEPTH_TEST); /* Enable hidden-surface-removal */
	a.Load("test10.obj");
    glutMainLoop ();
	a.Release();
    return 0;
}
 
/*==========================================================*/