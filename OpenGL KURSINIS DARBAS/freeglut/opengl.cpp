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

float theta;
float phi;

float eyeX;
float eyeY;

int glWin; //used to destroy window

struct Maps {int x,z;} map; //Structure used to store map length and width

GLuint GroundTexture; //Ground image
GLuint WallTexture; //Ground image

float	lightAmb [] = { 0.03, 0.03, 0.03 };
float	lightDif [] = { 0.95, 0.95, 0.95 };
float	lightPos [] = { 40,  40,  40};


struct Player	//
{
    int x,y,z;	//player koordinates on map
    int dx,dz;	//what koordinates actualy render uses
    bool isGo;	//is player going. used to animate movement
} player, ex;		//created also ex, when we reac other object, game will be over.
	
float C_Distance; //came distance to object
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

	eyeX = player.x*2.0+1 + 8*cos(phi)*sin(theta);
	eyeY = player.y*2.0-1 - 8*sin(phi)*sin(theta);
	
	
	gluLookAt(eyeX,eyeY,C_Distance,player.x*2.0+1,-player.y*2.0-1,1,0,0,1);

	


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
		for(int j=2;j<map.z-2;j+=2)
		for(int i=2;i<map.x-2;i+=2){
			drawWall(i*2.0,j*-2.0,2.0);
		
		}

		for(int i=0;i<map.z;i++){
			drawWall(0.0,i*-2.0,2.0);
			drawWall(map.z*2.0,i*-2.0,2.0);
		}
	glPopMatrix();

	glPushMatrix(); 
		glTranslatef (player.x*2.0+1,-player.y*2.0-1, 1.0);
		glColor3d(0,0,1);
		glutSolidSphere(1.0,100,100);
		glColor3d(1,1,1);
    glPopMatrix(); 

	glutSwapBuffers();
}

void init(){
	map.x = mapWidth;
	map.z = mapLength;
	player.x=1; player.y=1; player.isGo=false;
	C_Distance = 3;

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_TEXTURE_2D );
   
    glPixelStorei ( GL_PACK_ALIGNMENT, 1 );
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    glShadeModel (GL_SMOOTH);
    glLightfv    ( GL_LIGHT0, GL_AMBIENT,  lightAmb );
    glLightfv    ( GL_LIGHT0, GL_DIFFUSE,  lightDif );
    //glLightfv    ( GL_LIGHT0, GL_POSITION, lightPos );
    glEnable ( GL_LIGHT0 );
    glEnable ( GL_LIGHTING );
    glEnable(GL_COLOR_MATERIAL);
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
	if ((key=='z' || key=='Z') && (C_Distance<58.0) ) C_Distance+=0.25;
    if ((key=='x' || key=='X') && (C_Distance>1) ) C_Distance-=0.25;
	if (key=='w' || key=='W') player.x+=1;
	if (key=='s' || key =='S') player.x-=1;
	if (key=='q' || key=='Q') {theta+=0.1;phi += 0.1;}
	if (key=='e' || key =='E') {phi -= 0.1; theta-=0.1;}
	glutPostRedisplay ();
}

void halt(bool f);
int main(int argc, char **argv) {
	 glutInit (&argc, argv);
	 
	  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glWin = glutCreateWindow ("Maze");
	init();

    glutReshapeFunc (reshape);
	LoadImages();
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
