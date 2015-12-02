#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include<time.h>
 #include<conio.h>
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "imageloader.h"
using namespace std;

const int sMax=6, direction_parts=36;
int cur_direction=0;
double distance=4.0;

int mapWidth = 40;
int mapLength = 40;

float theta = 0.45;
float phi;


const float M_PI = 3.14159265;

float eyeX;
float eyeY;

int glWin; //used to destroy window

struct Maps {int x[40],z[40];
int width, length;
} map; //Structure used to store map length and width

GLuint GroundTexture; //Ground image
GLuint WallTexture; //Ground image

float	lightAmb [] = { 0.03, 0.03, 0.03 };
float	lightDif [] = { 0.95, 0.95, 0.95 };
float	lightPos [] = { 40,  40,  40};


struct Player	//
{
    int x,y,z;	//player koordinates on map
    int dx,dy;	//what koordinates actualy render uses
    bool isGo;	//is player going. used to animate movement
} player, ex;		//created also ex, when we reac other object, game will be over.
	
float C_Distance; //came distance to object


#define MAX 61  // 30 * 2 + 1
#define CELL 900  // 30 * 30
#define WALL 1
#define PATH 0

int maze[MAX][MAX];

void halt(bool f);

float pulse = 0.5;
float pulseDx = 0.01;


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

void animate()
{
    if ((player.x == ex.x)&&(player.y==ex.y)){
        halt(true);
    };

    if (player.isGo==true){
        if (player.dx>0)	player.dx+=1; else
        if (player.dy>0)	player.dy+=1; else
        if (player.dx<0)	player.dx-=1; else
        if (player.dy<0)	player.dy-=1;

        if ((player.dx>=sMax)||(player.dy>=sMax)){
            player.isGo=false;
            if (player.dx>0)	player.x+=1;
            if (player.dy>0)	player.y+=1;
            player.dx=0; player.dy=0;
        }else
        if ((player.dx<=-sMax)||(player.dy<=-sMax)){
            player.isGo=false;
            if (player.dx<0)	player.x-=1;
            if (player.dy<0)	player.y-=1;
            player.dx=0; player.dy=0;
        }
    }
    glutPostRedisplay();
}

void display(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	//eyeX = player.x*2.0+1 + 10*cos(phi)*sin(theta);
	//eyeY = -player.y*2.0-1 - 10*sin(phi)*sin(theta);
	//gluLookAt(eyeX,eyeY,C_Distance,player.x*2.0+1,-player.y*2.0-1,1,0,0,1);


	gluLookAt(player.x*2+(2.0*player.dx/sMax)+1+8*cos(cur_direction*M_PI/18),
			  -player.y*2-(2.0*player.dy/sMax)-1-8*sin(cur_direction*M_PI/18),
			  player.z+C_Distance,
              player.x*2+(2.0*player.dx/sMax)+1,
			  -player.y*2-(2.0*player.dy/sMax)-1,
			  1,
              0,0,1);

	
	glPushMatrix();

	 for(int a = 0; a < MAX; a++)
     {
         for(int b = 0; b < MAX; b++)
         {
			 drawFloor(a*2,b*-2,a*2+2.0,b*(-2)-2.0);
             if(maze[a][b] == WALL)
                 drawWall(a*2.0,b*-2.0,2.0);

         }
     }
	 glPopMatrix();

	 glPushMatrix(); 
		glTranslatef (ex.x*2+1,-ex.y*2.0-1, 1.0);
		glColor3d(1,0,1);
		glutSolidSphere(1.0,100,100);
		glColor3d(1,1,1);
    glPopMatrix();

	glClearColor(0,0,0,1);

	 if(pulse>0.95){
		 pulseDx=-pulseDx;
	 } else if(pulse<0.5){
		 pulseDx=-pulseDx;
	 }
	 pulse+=pulseDx;

	glPushMatrix(); 
	glTranslatef (player.x*2.0+1+(2.0*player.dx/sMax),-player.y*2.0-1-(2.0*player.dy/sMax), 1.0);

		glEnable ( GL_LIGHT0);
		glEnable ( GL_LIGHTING );
		glColorMaterial ( GL_FRONT_AND_BACK, GL_DIFFUSE) ;
		
		glEnable ( GL_COLOR_MATERIAL ) ;

		//vidine sfera
		glColor4f(pulse,1.0f,1.0f,0.9f);
		glutSolidSphere(pulse,100,100);

		//permatoma isorine sfera
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
		glColor4f(1-pulse,pulse-0.2,pulse-0.5,0.5);
		glutSolidSphere(1.0,100,100);
		glDisable (GL_BLEND);

		glDisable( GL_LIGHTING);
		glColor3d(1,1,1);
    glPopMatrix();

			glClearColor( 0.0, 0.0, 0.0, 1.0 );

	glutSwapBuffers();
}

void init(){
	
	ex.x=39; ex.y=39;
	player.x=1; player.y=1; player.isGo=false;
	C_Distance = 3;

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_TEXTURE_2D );
 

    //glEnable(GL_COLOR_MATERIAL);
}

void reshape( int w, int h )
{
    glViewport( 0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 40.0, (GLfloat)w/(GLfloat)h, 1.0, 180.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(0,25,25,0,0,0,0,1,0);
}

void Keyboard_keys(unsigned char key, int x, int y)
{
	if ((key=='w' || key=='W') && (C_Distance<150.0) ) C_Distance+=0.55;
    if ((key=='s' || key=='S') && (C_Distance>1) ) C_Distance-=0.55;
	
    if ( key=='d' || key=='D' ) { cur_direction--; if (cur_direction<0) cur_direction+=direction_parts;}
    if ( key=='a' || key=='A' ) { cur_direction++; if (cur_direction==direction_parts) cur_direction=0;}

	glutPostRedisplay ();
}

const int c_move[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
const int c_move_key[4] = {GLUT_KEY_UP, GLUT_KEY_LEFT, GLUT_KEY_DOWN, GLUT_KEY_RIGHT };

bool good_move(int y, int x){
    return (0<=y && 0<=x && y<mapWidth*2 && x<mapWidth*2 && maze[y][x]!=1);
}

void keys( int key, int x, int y)
{
    if (player.isGo) return;
    int dir=int(((direction_parts-cur_direction-1)/double(direction_parts))*4.+0.5);

    for (int i=0; i<4; i++)
        if ( key == c_move_key[i] ) {
			int tarp = (dir+i)%4;
            int newx=player.x+c_move[tarp][0];
            int newy=player.y+c_move[tarp][1];
            if (good_move(newx,newy)) {
                player.isGo=true;
                player.dx+=c_move[tarp][0];
                player.dy+=c_move[tarp][1];
            }
        }
}


void init_maze();
void maze_generator(int indeks, int backtrack_x[CELL], int bactrack_y[CELL], int x, int y, int n, int visited);
int is_closed(int x, int y);

int main(int argc, char **argv) {
	srand((unsigned)time(NULL));
 
    int indeks = 0;

    int backtrack_x[CELL];
    int backtrack_y[CELL];
 
    init_maze();
 
    backtrack_x[indeks] = 1;
    backtrack_y[indeks] = 1;
	maze_generator(indeks, backtrack_x, backtrack_y, 1, 1, 30, 1);


	 glutInit (&argc, argv);
	 
	  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize (500, 500);
    glWin = glutCreateWindow ("Maze");
	init();

    glutReshapeFunc (reshape);
	LoadImages();
    glutDisplayFunc (display);

	glutKeyboardFunc( Keyboard_keys );
	    glutSpecialFunc(keys);
    glutIdleFunc(animate);

    glEnable (GL_DEPTH_TEST); /* Enable hidden-surface-removal */
 
    glutMainLoop ();
    return 0;
}

void halt(bool f)
{
        glutDestroyWindow(glWin);
        exit(0);
}


void init_maze()
{
     for(int a = 0; a < MAX; a++)
     {
         for(int b = 0; b < MAX; b++)
         {
             if(a % 2 == 0 || b % 2 == 0)
                 maze[a][b] = 1;
             else
                 maze[a][b] = PATH;
         }
     }
}
 
void maze_generator(int indeks, int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int n, int visited)
{
    if(visited < n * n)
    {
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];
 
        int x_next;
        int y_next;
 
        if(x - 2 > 0 && is_closed( x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }
 
        if(y - 2 > 0 && is_closed( x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }
 
        if(y + 2 < n * 2 + 1 && is_closed( x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;
 
        }
 
        if(x + 2 < n * 2 + 1 && is_closed( x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }
 
        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[indeks];
            y_next = backtrack_y[indeks];
            indeks--;
        }
 
        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            indeks++;
            backtrack_x[indeks] = x_next;
            backtrack_y[indeks] = y_next;
 
            int rstep = step[random];
 
            if(rstep == 1)
                maze[x_next+1][y_next] = PATH;
            else if(rstep == 2)
                maze[x_next][y_next + 1] = PATH;
            else if(rstep == 3)
                maze[x_next][y_next - 1] = PATH;
            else if(rstep == 4)
                maze[x_next - 1][y_next] = PATH;
            visited++;
        }
 
        maze_generator(indeks, backtrack_x, backtrack_y, x_next, y_next, n, visited);
    }
}
 
 
int is_closed(int x, int y)
{
    if(maze[x - 1][y]  == WALL
       && maze[x][y - 1] == WALL
       && maze[x][y + 1] == WALL
       && maze[x + 1][y] == WALL
    )
        return 1;
 
    return 0;
}