#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <cstdlib>
#include <fstream>
#include <cmath>

#include "imageloader.h"
using namespace std;

int mapWidth = 40;
int mapLength = 40;

int glWin; //used to destroy window

struct Maps {int x,z;} map; //Structure used to store map length and width

GLuint GroundTexture; //Ground image
GLuint WallTexture; //Ground image

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width, image->height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    return textureId;
}
void LoadImages(){
	Image* image = loadBMP("GroundTexture.bmp");
    GroundTexture = loadTexture(image);
    delete image;

	Image* image1 = loadBMP("WallTexture.bmp");
    WallTexture = loadTexture(image);
    delete image1;
}

void drawFloor(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, unsigned texture=GroundTexture)
{
    glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x1, y1, 0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x2, y1, 0.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x2,y2, 0.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x1, y2, 0.0);
 
	glEnd();
		glDisable(GL_TEXTURE_2D);

}

void drawWall(GLfloat x, GLfloat y, GLfloat z, unsigned texture=WallTexture){
		
		
		GLfloat StartPoint[3] = {x,y,z};

		//Front wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Back Wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Top wall

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		
		//Bottom Wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Right Wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Left Wall

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);
}

void display(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	
	LoadImages();

	gluLookAt(4,-4,2,30,-40,0,0,0,1);

	glPushMatrix();
	for(int i=0;i<map.x;i++)
		for(int j=0;j<map.z;j++){
			drawFloor(i*2,j*-2,i*2+2.0,j*(-2)-2.0);
		}
	glPopMatrix();


	glPushMatrix();
	for(int i=0;i<map.x;i++){
		drawWall(i*2.0,0.0,2.0);
		drawWall(i*2.0,(map.x-1)*-2.0,2.0);
	}

	for(int i=0;i<map.z;i++){
		drawWall(0.0,i*-2.0,2.0);
		drawWall(map.z*2.0,i*-2.0,2.0);
	}
	glPopMatrix();

	glutSwapBuffers();
}

void init(){
	map.x = mapWidth;
	map.z = mapLength;

}

void reshape( int w, int h )
{
    glViewport( 0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, (GLfloat)w/(GLfloat)h, 1.0, 120.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(0,25,25,0,0,0,0,1,0);
}

void Keyboard_keys(unsigned char key, int x, int y){
	
}

void halt(bool f);
int main(int argc, char **argv) {
	 glutInit (&argc, argv);
	 
	  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glWin = glutCreateWindow ("Maze");
	init();

    glutReshapeFunc (reshape);
    glutDisplayFunc (display);

	glutKeyboardFunc( Keyboard_keys );

    glEnable (GL_DEPTH_TEST); /* Enable hidden-surface-removal */
 
    glutMainLoop ();
 
    return 0;
}

void halt(bool f)
{
        glutDestroyWindow(glWin);
        exit(0);
}
