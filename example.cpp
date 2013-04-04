#include "example.h"

/**********************************************************
This function will set the color of a pixel.
**********************************************************/
void setPixel(const Point& pixel)
{ 
   //Check if point is in screen and viewport   
   if (pixel.x >= SCREEN_WIDTH || pixel.y >= SCREEN_HEIGHT || pixel.x < 0 || pixel.y < 0)
     return;
        
   int temp = ((pixel.y * SCREEN_WIDTH) + pixel.x) * 3;
   
   raster[ temp + 0 ] = pixel.color[0];
   raster[ temp + 1 ] = pixel.color[1];
   raster[ temp + 2 ] = pixel.color[2];

   return;
}

/**********************************************************
* This function will perform ray tracing.  It will fire
* a ray from the center of each pixel on the screen.
**********************************************************/
void ray_trace()
{
  return;
}












/**********************************************************
* This function will draw a simple sphere into world
* coordinate and then use ray tracing to generate
* the screen coordinantes.
**********************************************************/
void draw0()
{
  //Set up Scene
  currentScene.clear();

  //Add Objects
  double s[] = {500,500,100,0};
  Sphere a(s, 20);
  currentScene.addObj(a);  

  //Added a Light
  currentScene.addLight(Light(vector4(700,500,50,0)));


  //Set Camera
  double s1[] = {500,500,0,0};
  double s2[] = {0,0,1,0};
  currentScene.setCamera(Camera(s1,s2));
  
  //Ray_Trace
  ray_trace();
  
  return;
}



/**********************************************************
* Initialize the raster. //?Don't know what a raster is.
**********************************************************/
void initRaster()
{
   float fRasterSize = ((float) RASTER_SIZE); 
   for (int i = 0; i < RASTER_SIZE; i++)
      raster[i] = 0;
   return;
}

/**********************************************************
*
**********************************************************/
void init ()
{
   initRaster();
   glShadeModel(GL_SMOOTH);			// Enable Smooth Shading
   glClearDepth(1.0f);					// Depth Buffer Setup
   glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
   glDepthFunc(GL_LEQUAL);				// The Type Of Depth Testing To Do
   glEnable(GL_COLOR_MATERIAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**********************************************************
*
**********************************************************/
void display ( void )   // Create The Display Function
{
	 glClearColor(1,0,0,1);  // Set the clear color
   // Clear the screen to the clear color (i.e. if the clear color
   // is red, the screen turns red);
   glClear(GL_COLOR_BUFFER_BIT);

    // Save the old state so that you can set it back after you draw
    GLint oldmatrixmode;
    GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glGetIntegerv(GL_MATRIX_MODE,&oldmatrixmode);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    
    // Draw the array of pixels (This is where you draw the values
    // you have stored in the array 'raster')
    glRasterPos2f(-1,-1);
    glDrawPixels(SCREEN_HEIGHT,SCREEN_WIDTH,GL_RGB,GL_FLOAT,raster);
    
    //Set the state back to what it was
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glMatrixMode(oldmatrixmode);
    if (depthWasEnabled)
      glEnable(GL_DEPTH_TEST);

   glFlush();

	//If you are using glut, you will need to uncomment the following call:
	glutSwapBuffers();
}

/**********************************************************
*
**********************************************************/
void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
	glViewport( 0, 0, w, h );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/**********************************************************
*
**********************************************************/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) 
	{
		case 27:        // When Escape Is Pressed...
			exit ( 0 );  // Exit The Program
			break;
		case 49:
       draw0();
		   display();
		   break;
		default:
			break;
	}
}


/**********************************************************
*
**********************************************************/
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
  glutMainLoop();  // Initialize The Main Loop
  return 0;
}

