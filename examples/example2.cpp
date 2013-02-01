
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <iostream>
using namespace std;

int drawMode = 0;

void init ()
{
   glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glClearDepth(1.0f);						// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);					// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);						// The Type Of Depth Testing To Do
   glEnable ( GL_COLOR_MATERIAL );
   glEnable( GL_POINT_SMOOTH );
   //glPointSize( 6.0 );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display ( void )   // Create The Display Function
{
	glClearColor(0.0, 0.0, 0.0, 1.0);				// Black Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();					// Reset The Current Modelview Matrix
	glColor3f(0,0,1);
  if (drawMode == 0){
    glColor3f(1,1,0);
    glTranslatef(-1.5,0.0,-6.0);					// Move Left 1.5 Units And Into The Screen 6.0
    glBegin(GL_POINTS);						// Drawing Using Points
      glVertex3f(0.0f, 1.0f, 0.0f);
      glVertex3f(-1.0f,-1.0f, 0.0f);
      glVertex3f(1.0f,-1.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(2.0f,-1.0f, 0.0f);
      glVertex3f( 1.0,-1.0, 0.0);
      glVertex3f( 1.0, 1.0, 0.0);
    glEnd();
  }
  else if (drawMode == 1){
    glTranslatef(0.0,0.0,-6.0);					// Move Left 1.5 Units And Into The Screen 6.0
    glBegin(GL_LINES);						// Drawing Using Lines
      glColor3f(1,1,0);
      glVertex3f( 0.0, 1.0, 0.0);
      glVertex3f(-1.0,-1.0, 0.0);
      glColor3f(1,0,1);
      glVertex3f( 1.0,-1.0, 0.0);
      glVertex3f( 1.0, 1.0, 0.0);
    glEnd();
  }
  else if (drawMode == 2){
    glTranslatef(-1.5,0.0,-6.0);					// Move Left 1.5 Units And Into The Screen 6.0
    glBegin(GL_TRIANGLES);						// Drawing Using Triangles
      glVertex3f( 0.0, 1.0, 0.0);
      glVertex3f(-1.0,-1.0, 0.0);
      glVertex3f( 1.0,-1.0, 0.0);
    glEnd();
  }
  else if (drawMode == 3){
    glColor3f(1,0,0);
    glTranslatef(1.5,0.0,-6.0);						// Move Right 1.5 Units And Into The Screen 6.0
    glBegin(GL_QUADS);							// Draw A Quad
      glVertex3f(-1.0, 1.0, 1.0);
      glColor3f(1,0,0.5);
      glVertex3f( 1.0, 1.0, -4.0);
      glColor3f(1,0.5,0);
      glVertex3f( 1.0,-1.0, 0.0);
      glColor3f(.5,0,0.2);
      glVertex3f(-1.0,-1.0, 0.0);
    glEnd();
  }
  else if (drawMode == 4){
    glColor3f(0,1,0);
    glTranslatef(0,0.0,-4.0);						// Move Into The Screen 4.0
    glBegin(GL_POLYGON);							// Draw A Polygon
      glVertex3f(-2.0, 1.0, 0.0);
      glVertex3f(1.0, 1.0, 1.0);
      glVertex3f(1.0, -2.0, 1.0);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(-1.0, -1.0, -10.0);
    glEnd();
  }
  else{
    glClearColor(0.5, 0.5, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  glFlush();


	glutSwapBuffers ( );
	// Swap The Buffers To Not Be Left With A Clear Screen
}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  if ( h==0 )  // Calculate The Aspect Ratio Of The Window
     gluPerspective ( 80, ( float ) w, 1.0, 5000.0 );
  else
     gluPerspective ( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0,640, 0,480, -1,1); // only for project 2; delete thereafter!
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {
		case 27:        // When Escape Is Pressed...
			exit ( 0 );   // Exit The Program
			break;
		default:
			break;
	}
}

void arrow_keys ( int a_keys, int x, int y )  // Create Special Function (required for arrow keys)
{
  switch ( a_keys ) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
      drawMode = (drawMode+1)%5;
      display();
      break;
    case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
      if ((drawMode=drawMode-1) < 0)drawMode=4;
      display();
      break;
    default:
      break;
  }
}

int main ( int argc, char** argv )   // Create Main Function
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
  glutInitWindowSize(640, 480); // This is the window size
  glutCreateWindow("OpenGL Example Program"); // Window Title
  init();
  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);
  glutMainLoop();  // Initialize The Main Loop
  return 0;
}

