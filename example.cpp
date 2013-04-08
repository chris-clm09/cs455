#include "example.h"

inline bool isOutOfScreen(int x, int y)
{
  return (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT || x < 0 || y < 0);
}

/**********************************************************
This function will set the color of a pixel.
**********************************************************/
void setPixel(int x, int y, const vector4 & color)
{ 
   //Check if point is in screen and viewport   
   if (isOutOfScreen(x,y)) return;
        
   int temp = ((y * SCREEN_WIDTH) + x) * 3;
   
   raster[ temp + 0 ] = color[0];
   raster[ temp + 1 ] = color[1];
   raster[ temp + 2 ] = color[2];

   //cout << x << " " << y << " " << color << endl;

   return;
}

/**********************************************************
This function will generate the color of a pixel based on
light and texture.
**********************************************************/
vector4 elementTimes(const vector4 &one, const vector4& two)
{
   return vector4(
      one[0] * two[0],
      one[1] * two[1],
      one[2] * two[2],
      one[3] * two[3]
      );
}

/**********************************************************
* Returns the index of the closest object that was hit by
* the given ray.
**********************************************************/
inline int getIndexOfClosestHit(const Ray& r, double &d)
{
  int indexHitSphere = -1;
  d = 20000.0;

  for (int i = 0; i < currentScene.sceneObjects.size(); i++)
  {
    if (currentScene.sceneObjects[i].rayHitMeCloserThanD(r, d))
    { 
      //cout << "Hit Somthing!\n"; 
      indexHitSphere = i;
    }
  } 
  return indexHitSphere;
}


/**********************************************************
**********************************************************/
inline vector4 calHitPoint(const Ray& r, const double & d)
{
  return r.pos + d * r.dir;
}

/**********************************************************
**********************************************************/
inline bool inShadow(const Ray& lightRay, double d)
{
  for (int i = 0; i < currentScene.sceneObjects.size() ; ++i)
    if (currentScene.sceneObjects[i].rayHitMeCloserThanD(lightRay, d))
      return true;

  return false;
}

/**********************************************************
* This function will shoot a ray into the scene and 
* generate the color for that pixel based off of any
* objects it hits in the scene.
**********************************************************/
vector4 shootRay(const Ray& r, vector4 color=vector4(0,0,0,0), int level=0, double coef=1.0)
{
  //cout << "level: " << level << endl;
  if (level > 3) return color;

  //Hit something
  double d;
  int indexClosestHitSphere = getIndexOfClosestHit(r, d);
  if (indexClosestHitSphere == -1) return color;
  
  //Calc Point Hit
  vector4 hitPoint = calHitPoint(r, d);

  //Calc Normal at Hit
  vector4 hitPntNormal = hitPoint - currentScene.sceneObjects[indexClosestHitSphere].pos;
  hitPntNormal = hitPntNormal.normalize();

  //Validate Closest Hit
  if (hitPntNormal == vector4(0,0,0,0)) {cout << "NormZero.\n";return color;}

  //Calc Color
  Ray lightRay;
  lightRay.pos = hitPoint;

  vector4 light(0,0,0,0);
  for (int i = 0; i < currentScene.lights.size(); i++)
  {
    Light currentLight = currentScene.lights[i];

    lightRay.dir = currentLight.pos - hitPoint;
    double lightDist = dot(lightRay.dir,lightRay.dir);
    lightRay.dir = lightRay.dir.normalize();
    
    if (!inShadow(lightRay, lightDist))
      light += max(0.0, (double)cml::dot(hitPntNormal, lightRay.dir)) *
               coef * 
               currentLight.deffuseColor;
  }
  vector4 objColor = elementTimes(currentScene.sceneObjects[indexClosestHitSphere].color, light);
  color += objColor;

  //Shoot Next Ray
  Ray nextRay;
  coef *= currentScene.sceneObjects[indexClosestHitSphere].reflectivity;
  double reflet = 2.0 * dot(r.dir, hitPntNormal);
  nextRay.pos = hitPoint;
  nextRay.dir = r.dir - reflet * hitPntNormal;

  return shootRay(nextRay, color, level+1, coef);
}

/**********************************************************
* This function will perform ray tracing.  It will fire
* a ray from the center of each pixel on the screen.
**********************************************************/
void ray_trace()
{
  //Walk through Each Point
  for (int x = 0; x < SCREEN_WIDTH; x++)
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
      vector4 rayPos((currentScene.camera.pos[0] - SCREEN_WIDTH  / 2.0) + x,
                     (currentScene.camera.pos[1] - SCREEN_HEIGHT / 2.0) + y,
                     currentScene.camera.pos[2]  + 1,
                     0);

      //cout << rayPos << endl;
      vector4 rayDir(0,0,1,0);
      //vector4 rayDir = (rayPos - currentScene.camera.pos).normalize();

      Ray r(rayPos, rayDir);

      vector4 color = shootRay(r);

      setPixel(x,y,color);
    }
  return;
}






/**********************************************************
* This function will draw a simple sphere into world
* coordinate and then use ray tracing to generate
* the screen coordinantes.
**********************************************************/
void draw0()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  //Add Objects
  double s[] = {500,500,200,0};
  Sphere a(s, 100, vector4(1,0,0,0));
  currentScene.addObj(a);  

  //Added a Light
  currentScene.addLight(Light(vector4(600,400,0,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1)));


  //Set Camera
  double s1[] = {500,500,0,0};
  double s2[] = {0,0,1,0};
  currentScene.setCamera(Camera(s1,s2));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE0" << endl;

  return;
}

/**********************************************************
* This function will draw a simple sphere into world
* coordinate and then use ray tracing to generate
* the screen coordinantes.
**********************************************************/
void draw1()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1.5;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,i,0,0), .5);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,i,0), .5);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(i,0,i,0), .5);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1)));

//Added a Light
  currentScene.addLight(Light(vector4(640,240,0,0),
                              vector4(.09,.09,.09,0),
                              vector4(.1,.1,.1,1)));

  //Set Camera
  double cpos[] = {250,200,0,0};
  double cdir[] = {0,0,1,0};
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE1" << endl;

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
   glEnable(GL_POINT_SMOOTH);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**********************************************************
*
**********************************************************/
void display ( void )   // Create The Display Function
{
//	 glClearColor(1,0,0,1);  // Set the clear color
   // Clear the screen to the clear color (i.e. if the clear color
   // is red, the screen turns red);
 //  glClear(GL_COLOR_BUFFER_BIT);

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
    glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_FLOAT, raster);
    
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
    case 50:
      draw1();
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
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); // This is the window size
  glutCreateWindow("CLM"); // Window Title
  init();
  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  draw0();
  display();
  glutMainLoop();  // Initialize The Main Loop
  return 0;
}

