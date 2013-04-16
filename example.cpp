#include "example.h"

inline bool isOutOfScreen(int x, int y)
{
  return (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT || x < 0 || y < 0);
}

vector4 black(0,0,0,0);
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

   if (color != black)
    display();

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
    if (currentScene.sceneObjects[i].rayHitMeCloserThanD(r, d))
      indexHitSphere = i;

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
vector4 shootRay(const Ray& r, int level=0, double coef=1.0)
{
  vector4 color(0,0,0,0);

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
  vector4 specColor(0,0,0,0);
  for (int i = 0; i < currentScene.lights.size(); i++)
  {
    Light currentLight = currentScene.lights[i];

    lightRay.dir = currentLight.pos - hitPoint;
    double lightDist = dot(lightRay.dir,lightRay.dir);
    lightRay.dir = lightRay.dir.normalize();
    double lightProjection = cml::dot(hitPntNormal, lightRay.dir);

    if (!inShadow(lightRay, lightDist))
    {
      if (lightProjection > 0)
      {
        //Calc Light
        light += lightProjection * coef * currentLight.deffuseColor;

        //Calc Spec
        vector4 halfway = (lightRay.dir + vector4(0,0,1,0)).normalize();
            
        specColor += max(0.0, 
                         pow(cml::dot(hitPntNormal, halfway), 
                             currentScene.sceneObjects[indexClosestHitSphere].shinyness)) 
                  * coef
                  * elementTimes(currentScene.sceneObjects[indexClosestHitSphere].specColor, 
                                 currentLight.specColor);  
      }
    }
  }
  
  //Shoot Next Ray
  Ray nextRay;
  coef *= currentScene.sceneObjects[indexClosestHitSphere].reflectivity;
  double reflet = 2.0 * dot(r.dir, hitPntNormal);
  nextRay.pos = hitPoint;
  nextRay.dir = r.dir - reflet * hitPntNormal;

  return elementTimes(currentScene.sceneObjects[indexClosestHitSphere].color, 
                      light + coef * shootRay(nextRay, level+1, coef)) 
         + coef*specColor;
}


/**********************************************************
* Returns a ray in world coordinates that will represent
* the given x,y screen coordinate.
**********************************************************/
Ray ComputeCameraRay(int i, int j) 
{
  double normalized_i = (i / (double)SCREEN_WIDTH)  - 0.5;
  double normalized_j = (j / (double)SCREEN_HEIGHT) - 0.5;

  vector4 image_point = normalized_i * currentScene.camera.right +
                        normalized_j * currentScene.camera.up +
                        currentScene.camera.pos + currentScene.camera.dir;

  vector4 ray_direction = (image_point - currentScene.camera.pos).normalize();

  return Ray(currentScene.camera.pos, ray_direction);
}

/**********************************************************
* Shoot and averages multiple rays through a single pixcel.
**********************************************************/
vector4 antiAlias(int x, int y)
{
  vector4 color(0,0,0,0);
  double sampleRatio = .25L;

  for (double fragmentx = double(x); fragmentx < x + 1.0L; fragmentx += 0.5L)
    for (double fragmenty = double(y); fragmenty < y + 1.0L; fragmenty += 0.5L)
      color += sampleRatio * shootRay(ComputeCameraRay(fragmentx, fragmenty));

  return color;
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
      setPixel(x,y,antiAlias(x,y));

  return;
}




/////////////////////////////////////////////////////////////
//Import Tests//
#include "tests.cpp"
/////////////////////////////////////////////////////////////





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
    // Save the old state so that you can set it back after you draw
    //GLint oldmatrixmode;
    //GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    //glGetIntegerv(GL_MATRIX_MODE,&oldmatrixmode);
    //glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    //glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    
    // Draw the array of pixels (This is where you draw the values
    // you have stored in the array 'raster')
    glRasterPos2f(-1,-1);
    glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_FLOAT, raster);
    
    //Set the state back to what it was
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW); glPopMatrix();
    //glMatrixMode(oldmatrixmode);
    //if (depthWasEnabled)
    //  glEnable(GL_DEPTH_TEST);

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
    case 51:
      draw2();
      display();
      break;
    case 52:
      draw3();
      display();
      break;
    case 53:
      draw4();
      display();
      break;
    case 54:
      draw5();
      display();
      break;
    case 55:
      draw6();
      display();
      break;
    case 56:
      draw7();
      display();
      break;
    case 57:
      draw8();
      display();
      break;
    case 48:
      draw9();
      display();
      break;
    default:
    cout << (int)key << endl;
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

