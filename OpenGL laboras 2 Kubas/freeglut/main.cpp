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
 
#define X .525731112119133606 
#define Z .850650808352039932

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint   axis = 0;
//*---------------------

// angle of rotation for the camera direction
float angle = 0.0f;
float Xangle = 0.0f;
float Yangle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;

// XZ position of the camera
float x=0.0f, z=5.0f;

// the key states. These variables will be zero
//when no key is being presses
float XdeltaAngle = 0.0f;
float YdeltaAngle = 0.0f;
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
int yOrigin = -1;

//*------------------------
//skirti saugoti kiekvienam paveiksleliui
GLuint white_textureId;
GLuint red_textureId;
GLuint blue_textureId;
GLuint green_textureId;
GLuint yellow_textureId;
GLuint orange_textureId;
//*-----------------
//*-----------------
//kiekvieno kubelio startines koordinates.
//kiekviena kubelis pradedamas braizyti nuo front top left koordinates.
//cia surasyt kiekvieno kubelio tokia koordinate, nuo kurios bus atvaizduojamas kubelis
GLfloat CFTL[3] = {-2.0,2.0,2.0};
GLfloat CFTR[3] = {0.0,2.0,2.0};
GLfloat CFBL[3] = {-2.0,0.0,2.0};
GLfloat CFBR[3] = {0.0,0.0,2.0};

GLfloat CBTL[3] = {-2.0,2.0,0.0};
GLfloat CBTR[3] = {0.0,2.0,0.0};
GLfloat CBBL[3] = {-2.0,0.0,0.0};
GLfloat CBBR[3] = {0.0,0.0,0.0};
//*-----------------

//paimto kubo numeris, naudojamas atsekti i kuria puse sukti kuba
int Picked = 0;
int Rotation = 0;

GLfloat RotAngle=0.0;

void Copy(GLfloat *Arr1, GLfloat *Arr2, int length);
void drawCube(GLenum mode);


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
	Image* image = loadBMP("white.bmp");
    white_textureId = loadTexture(image);
    delete image;
 
        Image* image1 = loadBMP("red.bmp");
    red_textureId = loadTexture(image1);
    delete image1;
 
        Image* image2 = loadBMP("blue.bmp");
    blue_textureId = loadTexture(image2);
    delete image2;
 
        Image* image3 = loadBMP("orange.bmp");
    orange_textureId = loadTexture(image3);
    delete image3;
 
        Image* image4 = loadBMP("green.bmp");
    green_textureId = loadTexture(image4);
    delete image4;
 
        Image* image5 = loadBMP("yellow.bmp");
    yellow_textureId = loadTexture(image5);
    delete image5;
}

class kubas {
private:
	string vardas;
	char FrontW;
	char BackW;
	char TopW;
	char BottomW;
	char LeftW;
	char RightW;
	int index;
public:
	kubas::~kubas(){
		this->vardas= "";
		this->BackW=' ';
		this->FrontW=' ';
		this->TopW=' ';
		this->BottomW=' ';
		this->LeftW=' ';
		this->RightW=' ';
	}
	kubas::kubas(string Vardas){
		this->vardas = Vardas;
		this->CalculateIndex();
	}

	void kubas::CalculateIndex(){
		if(this->vardas.compare("FTL") == false){index=10;} else 
		if(this->vardas.compare("FTR") == false){index=20;;} else
		if(this->vardas.compare("FBL") == false){index=30;;} else
		if(this->vardas.compare("FBR") == false){index=40;;} else
		if(this->vardas.compare("BTL") == false){index=50;;} else
		if(this->vardas.compare("BTR") == false){index=60;;} else
		if(this->vardas.compare("BBL") == false){index=70;;} else
		if(this->vardas.compare("BBR") == false){index=80;;} else {
			index=100;
		}
	}

	void kubas::WallColors(char FrontW, char Backw, char TopW, char BottomW, char LeftW, char RightW){
		this->FrontW = FrontW;
		this->BackW = BackW;
		this->TopW = TopW;
		this->BottomW = BottomW;
		this->LeftW = LeftW;
		this->RightW = RightW;
	}

	void kubas::draw(GLenum mode){
		
		
		GLfloat StartPoint[3] = {0.0,0.0,0.0};
		if(this->vardas.compare("FTL") == false){Copy(StartPoint, CFTL, 3);} else 
		if(this->vardas.compare("FTR") == false){Copy(StartPoint, CFTR, 3);} else
		if(this->vardas.compare("FBL") == false){Copy(StartPoint, CFBL, 3);} else
		if(this->vardas.compare("FBR") == false){Copy(StartPoint, CFBR, 3);} else
		if(this->vardas.compare("BTL") == false){Copy(StartPoint, CBTL, 3);} else
		if(this->vardas.compare("BTR") == false){Copy(StartPoint, CBTR, 3);} else
		if(this->vardas.compare("BBL") == false){Copy(StartPoint, CBBL, 3);} else
		if(this->vardas.compare("BBR") == false){Copy(StartPoint, CBBR, 3);}

		GLuint spalva;

		

		spalva = red_textureId;
		switch(this->FrontW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}
		//Front wall
		
		if(mode == GL_SELECT) glLoadName(this->index);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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
		
		spalva = red_textureId;
		switch(this->BackW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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
		
		spalva = red_textureId;
		switch(this->TopW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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
		
		spalva = red_textureId;
		switch(this->BottomW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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
		
		spalva = red_textureId;
		switch(this->RightW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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
		
		//spalva = red_textureId;
		switch(this->LeftW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
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

	void kubas::ChangeWalls(const kubas *Kubas, int direction){
		if(direction == 1){
			this->FrontW=Kubas->BottomW;
			this->TopW=Kubas->FrontW;
			this->BackW=Kubas->TopW;
			this->BottomW=Kubas->BackW;
			this->RightW=Kubas->RightW;
			this->LeftW=Kubas->LeftW;

		} else if(direction == 2) {
			this->FrontW=Kubas->LeftW;
			this->TopW=Kubas->TopW;
			this->BackW=Kubas->RightW;
			this->BottomW=Kubas->BottomW;
			this->RightW=Kubas->FrontW;
			this->LeftW=Kubas->BackW;

		} else if (direction == 3) {
			this->FrontW=Kubas->TopW;
			this->TopW=Kubas->BackW;
			this->BackW=Kubas->BottomW;
			this->BottomW=Kubas->FrontW;
			this->RightW=Kubas->RightW;
			this->LeftW=Kubas->LeftW;
		} else if(direction == 4) {
			this->FrontW=Kubas->RightW;
			this->TopW=Kubas->TopW;
			this->BackW=Kubas->LeftW;
			this->BottomW=Kubas->BottomW;
			this->RightW=Kubas->BackW;
			this->LeftW=Kubas->FrontW;
		}
	}

	void kubas::CopyAll(const kubas *Kubas){
		this->BackW=Kubas->BackW;
		this->BottomW=Kubas->BottomW;
		this->FrontW=Kubas->FrontW;
		this->TopW=Kubas->TopW;
		this->LeftW=Kubas->LeftW;
		this->RightW=Kubas->RightW;
		this->vardas=Kubas->vardas;
	}
};

	kubas FTL("FTL");
	kubas FTR("FTR");
	kubas FBL("FBL");
	kubas FBR("FBR");
	kubas BTL("BTL");
	kubas BTR("BTR");
	kubas BBL("BBL");
	kubas BBR("BBR");


void InitCubes(){
	FTL.WallColors('Y','N','B','N','G','N');
	FTR.WallColors('Y','N','B','N','N','O');
	FBL.WallColors('Y','N','B','W','G','N');
	FBR.WallColors('Y','N','B','W','N','O');
	BTL.WallColors('N','R','B','N','G','N');
	BTR.WallColors('N','R','B','N','N','O');
	BBL.WallColors('N','R','B','W','G','N');
	BBR.WallColors('N','R','B','W','N','O');
}


void Copy(GLfloat *Arr1, GLfloat *Arr2, int length){
	if(length == 0) return;    
     *Arr1 = *Arr2;
     Copy(++Arr1, ++Arr2, length-1);
}

void mouseMove(int x, int y) { 	

         // this will only be true when the left button is down
         if (xOrigin >= 0) {

			// update deltaAngle
			XdeltaAngle = (x - xOrigin) * 0.1;
			YdeltaAngle = (y - yOrigin) * 0.1;

			// update camera's direction

			Xangle +=  XdeltaAngle;
			Yangle += YdeltaAngle;

			if (Xangle > 360.0 )
			Xangle -= 360.0;
			if(Xangle < -360.0)
				Xangle +=360;

			if (Yangle > 360.0 )
				Yangle -= 360.0;
			if (Yangle < 360.0 )
				Yangle += 360.0;

			xOrigin = x;
			yOrigin = y;
			glutPostRedisplay ();
		}
}

void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;
 
   printf ("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   cout<<"first "<<*(ptr+3)<<endl;
   if(*(ptr+3) < 100) {
		Picked = *(ptr+3);
   } else {
	   Picked = 0;
   }
   for (i = 0; i < hits; i++)
   { /*  for each hit  */
      names = *ptr;
	  
      ptr+=4;
      
   }
}

#define SIZE 1024

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_RIGHT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}


	//picking objects
	 GLuint selectBuf[SIZE];
   GLint hits;
   GLint viewport[4];
 
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && RotAngle == 0.0)
   {
   glGetIntegerv (GL_VIEWPORT, viewport);
 
   glSelectBuffer (SIZE, selectBuf);
   glRenderMode(GL_SELECT);
 
   glInitNames();
   glPushName(0);
 
   glMatrixMode (GL_PROJECTION);
   glPushMatrix ();
   glLoadIdentity ();
/* create 5x5 pixel picking region near cursor location */
   gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 1.0, 1.0, viewport);
   
   glOrtho (-8.0, 8.0, -8.0 * (GLfloat) 500 / (GLfloat) 500, 8.0 * (GLfloat) 500 / (GLfloat) 500, -100.0, 100.0);

   glMatrixMode (GL_PROJECTION);

	drawCube(GL_SELECT);
    
   glMatrixMode (GL_PROJECTION);
   
   glFlush ();
 glPopMatrix ();
   hits = glRenderMode (GL_RENDER);
   processHits (hits, selectBuf);
 
   glutPostRedisplay();

   glMatrixMode (GL_MODELVIEW);
   }
}

void drawCube(GLenum mode){
	glPushMatrix();
	glScalef(1.0,1.0,1.0);
	glRotatef (Xangle, 0.0, 1.0, 0.0);    /* x */
	glRotatef (Yangle, 1.0, 0.0, 0.0);    /* y */
	glScalef(1.0,1.0,1.0);
	LoadImages();
	
	

	if((Picked == 10 || Picked == 30) && Rotation == 1){
		glPushMatrix();
		glRotatef (RotAngle, 1.0, 0.0, 0.0);    /* x */
			FTL.draw(mode);
			FBL.draw(mode);
			BTL.draw(mode);
			BBL.draw(mode);
		glPopMatrix();

		FTR.draw(mode);
		FBR.draw(mode);
		BTR.draw(mode);
		BBR.draw(mode);

		if(RotAngle <= -90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTL);
			FTL.ChangeWalls(&FBL,1);
			FBL.ChangeWalls(&BBL,1);
			BBL.ChangeWalls(&BTL,1);
			BTL.ChangeWalls(&TARP,1);
		} else
		RotAngle-=5.0;
	}else if((Picked == 10 || Picked == 20) && Rotation == 2){
		glPushMatrix();
		glRotatef (RotAngle, 0.0, 1.0, 0.0);    /* y */
			FTL.draw(mode);
			BTL.draw(mode);
			FTR.draw(mode);
			BTR.draw(mode);
		glPopMatrix();

		FBL.draw(mode);
		BBL.draw(mode);
		FBR.draw(mode);
		BBR.draw(mode);

		if(RotAngle >= 90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTL);
			FTL.ChangeWalls(&BTL,2);
			BTL.ChangeWalls(&BTR,2);
			BTR.ChangeWalls(&FTR,2);
			FTR.ChangeWalls(&TARP,2);
		} else 
		RotAngle+=5.0;

	}else if((Picked == 10 || Picked == 30) && Rotation == 3){
		glPushMatrix();
		glRotatef (RotAngle, 1.0, 0.0, 0.0);    /* x */
			FTL.draw(mode);
			FBL.draw(mode);
			BTL.draw(mode);
			BBL.draw(mode);
		glPopMatrix();

		FTR.draw(mode);
		FBR.draw(mode);
		BTR.draw(mode);
		BBR.draw(mode);

		if(RotAngle >= 90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTL);
			FTL.ChangeWalls(&BTL,3);
			BTL.ChangeWalls(&BBL,3);
			BBL.ChangeWalls(&FBL,3);
			FBL.ChangeWalls(&TARP,3);
		} else 
		RotAngle+=5.0;
	}else if((Picked == 10 || Picked == 20) && Rotation == 4){
		glPushMatrix();
		glRotatef (RotAngle, 0.0, 1.0, 0.0);    /* y */
			FTL.draw(mode);
			BTL.draw(mode);
			FTR.draw(mode);
			BTR.draw(mode);
		glPopMatrix();

		FBL.draw(mode);
		BBL.draw(mode);
		FBR.draw(mode);
		BBR.draw(mode);

		if(RotAngle <= -90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTL);
			FTL.ChangeWalls(&FTR,4);
			FTR.ChangeWalls(&BTR,4);
			BTR.ChangeWalls(&BTL,4);
			BTL.ChangeWalls(&TARP,4);
		} else 
		RotAngle-=5.0;
	}else if((Picked == 20 || Picked == 40) && Rotation == 1){
		glPushMatrix();
		glRotatef (RotAngle, 1.0, 0.0, 0.0);    /* x */
			FTR.draw(mode);
			BTR.draw(mode);
			FBR.draw(mode);
			BBR.draw(mode);
		glPopMatrix();

		FTL.draw(mode);
		FBL.draw(mode);
		BTL.draw(mode);
		BBL.draw(mode);

		if(RotAngle <= -90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTR);
			FTR.ChangeWalls(&FBR,1);
			FBR.ChangeWalls(&BBR,1);
			BBR.ChangeWalls(&BTR,1);
			BTR.ChangeWalls(&TARP,1);
		} else 
		RotAngle-=5.0;
	}else if((Picked == 20 || Picked == 40) && Rotation == 3){
		glPushMatrix();
		glRotatef (RotAngle, 1.0, 0.0, 0.0);    /* x */
			FTR.draw(mode);
			BTR.draw(mode);
			FBR.draw(mode);
			BBR.draw(mode);
		glPopMatrix();

		FTL.draw(mode);
		FBL.draw(mode);
		BTL.draw(mode);
		BBL.draw(mode);

		if(RotAngle >= 90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FTR);
			FTR.ChangeWalls(&BTR,3);
			BTR.ChangeWalls(&BBR,3);
			BBR.ChangeWalls(&FBR,3);
			FBR.ChangeWalls(&TARP,3);
		} else 
		RotAngle+=5.0;
	}else if((Picked == 30 || Picked == 40) && Rotation == 2){
		glPushMatrix();
		glRotatef (RotAngle, 0.0, 1.0, 0.0);    /* y */
			FBL.draw(mode);
			BBL.draw(mode);
			FBR.draw(mode);
			BBR.draw(mode);
		glPopMatrix();

		FTL.draw(mode);
		BTL.draw(mode);
		FTR.draw(mode);
		BTR.draw(mode);

		if(RotAngle >= 90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FBL);
			FBL.ChangeWalls(&BBL,2);
			BBL.ChangeWalls(&BBR,2);
			BBR.ChangeWalls(&FBR,2);
			FBR.ChangeWalls(&TARP,2);
		} else 
		RotAngle+=5.0;

	}else if((Picked == 30 || Picked == 40) && Rotation == 4){
		glPushMatrix();
		glRotatef (RotAngle, 0.0, 1.0, 0.0);    /* y */
			FBL.draw(mode);
			BBL.draw(mode);
			FBR.draw(mode);
			BBR.draw(mode);
		glPopMatrix();

		FTL.draw(mode);
		BTL.draw(mode);
		FTR.draw(mode);
		BTR.draw(mode);

		if(RotAngle <= -90){
			RotAngle = 0;
			Picked = 0;
			Rotation = 0;
			kubas TARP("BTL");
			TARP.CopyAll(&FBL);
			FBL.ChangeWalls(&FBR,4);
			FBR.ChangeWalls(&BBR,4);
			BBR.ChangeWalls(&BBL,4);
			BBL.ChangeWalls(&TARP,4);
		} else 
		RotAngle-=5.0;
	}
	else {
		FTL.draw(mode);
		FTR.draw(mode);
		FBL.draw(mode);
		FBR.draw(mode);
		BTL.draw(mode);
		BTR.draw(mode);
		BBL.draw(mode);
		BBR.draw(mode);
	}
	glPopMatrix();
	glutPostRedisplay();
}

void display (void)
{
    /* display callback, clear frame buffer and z buffer, 
       rotate cube and draw, swap buffers */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
	
	
	drawCube(GL_RENDER);
	
	
    glFlush ();
    glutSwapBuffers ();
}

void myReshape (int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
        glOrtho (-8.0, 8.0, -8.0 * (GLfloat) h / (GLfloat) w, 
                             8.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho (-8.0 * (GLfloat) w / (GLfloat) h, 
                  8.0 * (GLfloat) w / (GLfloat) h, -8.0, 8.0, -10.0, 10.0);
    glMatrixMode (GL_MODELVIEW);
}

void Keyboard_keys(unsigned char key, int x, int y) 
{
	switch(key){
	case 'w' : {
			if(Picked !=0){
			Rotation = 1;
			}
		}
		break;
	case 'd' : {
			if(Picked !=0){
			Rotation = 2;
			}
		}
		break;
	case 's' : {
			if(Picked !=0){
			Rotation = 3;
			}
		}
		break;
	case 'a' : {
			if(Picked !=0){
			Rotation = 4;
			}
		}
		break;
	}
	
}

int main(int argc, char **argv) {
	InitCubes();
	 glutInit (&argc, argv);
	 
	  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutCreateWindow ("Color Cube");
    glutReshapeFunc (myReshape);
    glutDisplayFunc (display);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutKeyboardFunc( Keyboard_keys );

    glEnable (GL_DEPTH_TEST); /* Enable hidden-surface-removal */
 
    glutMainLoop ();
 
    return 0;
}

