#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <iostream>
#include "cml/cml.h"
#include <vector>
#include "point.h"
#include "vectorfun.h"
using namespace std;

typedef cml::matrix44f_c matrix4;
typedef cml::vector4f vector4;
#include "ColorInterpolation.h"

//---------------------Forward Declarations----------------
void setPixel(int x, int y, double r, double g, double b);
vector4 getPixelColor(int x, int y);

/**********************************************************
* // GLOBAL CRAP
**********************************************************/
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH  = 640;
const int RASTER_SIZE   = SCREEN_HEIGHT * SCREEN_WIDTH * 3;

vector4 clearColor(0,0,0,0);
vector4 penColor(0,0,0,0);
float raster[RASTER_SIZE];
GLenum glDrawMode;
vector<Point> savedPoints;
Point firstPt(-1,-1);
int lineWidth = 1;

int drawMode = 0;
int mymode   = 0;



//-------------------PRINT CRAP--------------------------------------
void pp(vector4 p){ cout << p[0] << "," << p[1] << "," << p[2] << endl;}
void pp(Point p) {cout << p.x << "," << p.y << endl;}



/**********************************************************
A simple rounding function.
**********************************************************/
template <class T>
int round(T n) {return (int) (n + .5);}

/**********************************************************
This function will set a pixel in a line.
**********************************************************/
void setLinePixel(int xOne, int yOne, int xTwo, int yTwo,
                  float cX, float cY)
{
   vector4 theColor = interpolateColor( getPixelColor(xOne,yOne),
                                        getPixelColor(xTwo,yTwo),
                                        getFraction(xOne, yOne,
                                                    xTwo, yTwo,
                                                    cX, cY));
   setPixel(cX, cY, 
	         theColor[0], theColor[1], theColor[2]);      
   return;
}                  


/**********************************************************
* /// Line Function ///
* This function will draw a line between the two points.
**********************************************************/
vector<Point> drawLine(int xOne, int yOne, int xTwo, int yTwo)
{
   int pts[] = { xOne, yOne, xTwo, yTwo };
	int dX = xTwo - xOne;
	int dY = yTwo - yOne;
	float slope     = (float)dY / (float)dX;
	float intersept = yTwo - slope * xTwo;
	vector<Point> theLine;
	
	if (dX == 0)
	{
		int change = (yOne > yTwo ? -1 : 1);
		while (yOne != yTwo)
		{
			yOne += change;
			setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne, yOne);
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne+i, yOne);
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne-i, yOne);
			}      
			theLine.push_back(Point(xOne,yOne));
		}
		return theLine;	
	}
	else if (dY == 0)
	{
		int change = (xOne > xTwo ? -1 : 1);
		while (xOne != xTwo)
		{
			xOne += change;
			setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne, yOne);
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne, yOne+i);
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne, yOne-i);
			}
			theLine.push_back(Point(xOne,yOne));
		}
		return theLine;	
	}
	
	if (abs(dX) > abs(dY))
	{
		int change = (xOne > xTwo ? -1 : 1);
		unsigned int newY; 
		while (xOne != xTwo)
		{
			xOne += change;
			newY = (unsigned int) (ceil(slope * xOne + intersept-.5));
			setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne, newY);
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne+i, newY);
   			setLinePixel(pts[0], pts[1], pts[2], pts[3], xOne-i, newY);
   	   }
			theLine.push_back(Point(xOne, newY));
		}	
	}
	else
	{
		int change = (yOne > yTwo ? -1 : 1);
		unsigned int newX;
		while (yOne != yTwo)
		{
			yOne += change;
			newX = (unsigned int)(ceil((1.0 / slope) * (yOne - intersept) -.5));
			setLinePixel(pts[0], pts[1], pts[2], pts[3], newX, yOne);
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(pts[0], pts[1], pts[2], pts[3], newX+i, yOne);
   			setLinePixel(pts[0], pts[1], pts[2], pts[3], newX-i, yOne);
   	   }
			theLine.push_back(Point(newX,yOne));
		}
	}
	return theLine;
}


/**********************************************************
This function will fill in the area between two lines
with interpolated color.
**********************************************************/
void fill(vector<Point> points)
{
   sort(points.begin(), points.end(), less_key());
   
   while (points.size() > 0)
   {
      int min = getXMin(points);
      int max = getXMax(points);
      drawLine(min, points.back().y, max, points.back().y);
      removeYBack(points);   
   }
   return;
}

/**********************************************************
Sets the clear color.
**********************************************************/
void clm_glClearColor(float r, float g, float b, float a)
{
   glClearColor(r, g, b, a);
   clearColor.set(r, g, b, a);
   return;
}

/**********************************************************
This function will set the color of a pixel.
**********************************************************/
void setPixel(int x, int y, double r, double g, double b)
{
   int temp = ((y * SCREEN_WIDTH) + x) * 3;
   raster[ temp + 0 ] = r;
   raster[ temp + 1 ] = g;
   raster[ temp + 2 ] = b;   
   return;
}

/**********************************************************
This function will return the color of a pixel.
**********************************************************/
vector4 getPixelColor(int x, int y)
{
   int temp = ((y * SCREEN_WIDTH) + x) * 3;
   return vector4(raster[ temp + 0 ],
                  raster[ temp + 1 ],
                  raster[ temp + 2 ],
                  1.0); 
}

/**********************************************************
Fill raster via color.
**********************************************************/
void fillRasterWColor(vector4 color)
{
   for (int x = 0; x < SCREEN_WIDTH; x++)
      for (int y = 0; y < SCREEN_HEIGHT; y++)
         setPixel(x, y, color[0], color[1], color[2]);
         
   return;
}

/**********************************************************
Clears the entire screen to the clear color.
**********************************************************/
void clm_glClear(GLint bit)
{
   glClear(bit);
  
   // Clear the Raster
   if (bit != 0)
      fillRasterWColor(clearColor);
   else
      //TODO:: Clear Viewport only.
                
   return;
}

/**********************************************************
This tells you how to interpret points, as will be 
explained more below.
// with parameters GL_POINTS, GL_LINES, and GL_TRIANGLES
**********************************************************/
void clm_glBegin(GLenum eVar)
{
   glBegin(eVar);
   glDrawMode = eVar;
   firstPt.set(-1,-1);
   return;
}

/**********************************************************
This function will save the given point and return if 
the desired number of points has been reached.
**********************************************************/
bool saveAndReachedPoints(int num, int x, int y)
{
   if (savedPoints.size() < num)
   {  //save
      savedPoints.push_back(Point(x,y));
      return savedPoints.size() == num;
   }
   else 
   {        
      cout << "Error: Invalid number of saved Points.\n";	   
      exit(0);
   }
}


/**********************************************************
Draws a strip between two points.
**********************************************************/
void drawStrip(int x, int y)
{
   if (savedPoints.size() == 0)
   {
      setPixel(x, y, penColor[0], penColor[1], penColor[2]);
      saveAndReachedPoints(2, x, y);
   }
   else
   {
      setPixel(x, y, penColor[0], penColor[1], penColor[2]);
      saveAndReachedPoints(2,x,y);
      drawLine(savedPoints[0].x, savedPoints[0].y,
               savedPoints[1].x, savedPoints[1].y);
               
      savedPoints.erase(savedPoints.begin());
   }     
   return;
}


/**********************************************************
glVertex specifies a point for drawing, though how it is 
drawn depends on the mode specified by glBegin. 
glVertex2i(x,y) specifies the 4-vector point (x,y,0,1).
//, plus possibly other glVertex calls
**********************************************************/
void clm_glVertex2i(int x, int y)
{
   glVertex2i(x,y);
   
   if (glDrawMode == GL_POINTS) 
      setPixel(x, y, penColor[0], penColor[1], penColor[2]);
   else if (glDrawMode == GL_LINES)
   {
      int numPointsInLine = 2;
      if (saveAndReachedPoints(numPointsInLine, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         savedPoints.clear();
      }
      else if (savedPoints.size() == 1)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
	}
	else if (glDrawMode == GL_TRIANGLES)
	{
	   int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         temp   = drawLine(savedPoints[1].x, savedPoints[1].y,
                  savedPoints[2].x, savedPoints[2].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2].x, savedPoints[2].y,
                  savedPoints[0].x, savedPoints[0].y);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.clear();
      }
      else if (savedPoints.size() == 1 ||
               savedPoints.size() == 2)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         	    
	}
   //Draws one line segment for every call to glVertex2i except the first; 
   //vertices n and n + 1 define line segment n.
   else if (glDrawMode == GL_LINE_STRIP)
   {
      drawStrip(x,y);
   }
   //Exactly like GL_LINE_STRIP except one additional line is draw between 
   //the first and last calls to glVertex2i when glEnd is called.
   else if (glDrawMode == GL_LINE_LOOP)
   {
      if (firstPt.eq(Point(-1,-1)))
         firstPt.set(x,y);
      drawStrip(x,y);
   }
   else if (glDrawMode == GL_TRIANGLE_STRIP)//Draws a connected group of triangles.
   {
      int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         temp   = drawLine(savedPoints[1].x, savedPoints[1].y,
                  savedPoints[2].x, savedPoints[2].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2].x, savedPoints[2].y,
                  savedPoints[0].x, savedPoints[0].y);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin());
      }
      else if (savedPoints.size() == 1 ||
               savedPoints.size() == 2)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
   }
   else if (glDrawMode == GL_TRIANGLE_FAN || 
            glDrawMode == GL_POLYGON)//Draws a connected group of triangles. 
   {
      int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         temp   = drawLine(savedPoints[1].x, savedPoints[1].y,
                  savedPoints[2].x, savedPoints[2].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2].x, savedPoints[2].y,
                  savedPoints[0].x, savedPoints[0].y);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin()+1);
      }
      else if (savedPoints.size() == 1 ||
               savedPoints.size() == 2)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
   }
   else if (glDrawMode == GL_QUADS)
   {
      int numPointsInTri = 4;
      if (saveAndReachedPoints(numPointsInTri, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         temp   = drawLine(savedPoints[1].x, savedPoints[1].y,
                  savedPoints[2].x, savedPoints[2].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2].x, savedPoints[2].y,
                  savedPoints[3].x, savedPoints[3].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[3].x, savedPoints[3].y,
                  savedPoints[0].x, savedPoints[0].y);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.clear();
      }
      else if (savedPoints.size() == 1 ||
               savedPoints.size() == 2 || 
               savedPoints.size() == 3)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
   }
   else if (glDrawMode == GL_QUAD_STRIP)
   {
      int numPointsInTri = 4;
      if (saveAndReachedPoints(numPointsInTri, x, y))
      {
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
         
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0].x, savedPoints[0].y,
                  savedPoints[1].x, savedPoints[1].y);
         temp   = drawLine(savedPoints[1].x, savedPoints[1].y,
                  savedPoints[3].x, savedPoints[3].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[3].x, savedPoints[3].y,
                  savedPoints[2].x, savedPoints[2].y);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2].x, savedPoints[2].y,
                  savedPoints[0].x, savedPoints[0].y);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin());
         savedPoints.erase(savedPoints.begin());
      }
      else if (savedPoints.size() == 1 ||
               savedPoints.size() == 2 || 
               savedPoints.size() == 3)
         setPixel(x, y, penColor[0], penColor[1], penColor[2]);
   }
   else
	{
	   cout << "Holly Crap!" << endl;
	   exit(0);
	}
   
   return;
}

/**********************************************************
Rounds x and y and calls Vertex2i
**********************************************************/
void clm_glVertex2f(double x, double y)
{
   glVertex2f(x, y);
   
   //TODO:: Do something smart ...
   clm_glVertex2i(x, y);
}


/**********************************************************
There must be one glEnd for every glBegin. glVertex only 
works between Begin & End pairs.
**********************************************************/
void clm_glEnd()
{
   glEnd();
   
   if (glDrawMode == GL_LINE_LOOP)
      drawStrip(firstPt.x, firstPt.y);
   
   savedPoints.clear();
   firstPt.set(-1,-1);
   return;
}

/**********************************************************
There is one current color (a 4-vector) at all times in 
OpenGL. Initially, it is (1,1,1,1) (white). Calling 
glColor3f(r,g,b) sets it to (r,g,b,1). 
**********************************************************/
void clm_glColor3f(double r, double g, double b)
{
   glColor3f(r, g, b);
   penColor.set(r, g, b, 1.0);
   return;
}


/**********************************************************
This function will set the pen width.
**********************************************************/
void clm_glLineWidth(int lwidth)
{
   glLineWidth(lwidth);
   lineWidth = lwidth;
   return;
}





/**********************************************************
* glMatrixMode(GLenum)
* Rather than separate calls to change different matrices, 
* OpenGL has only one set of matrix modification calls 
* which change whichever matrix was last specified by a 
* call to glMatrixMode.
* Modes == GL_MODELVIEW and GL_PROJECTION
**********************************************************/
void clm_glMatrixMode(GLenum mode)
{
   glMatrixMode(mode);
   
   
   return;
}

/**********************************************************
* glViewprot(x,y,width,height)
* Specifies the active viewport—that is, the rectangle of 
* pixels OpenGL should render to.
**********************************************************/
void clm_glViewport(int x , int y, int width, int height)
{
   glViewport(x, y, width, height);
   
   return;
}

/**********************************************************
* glPushMatrix  pushes  the current matrix stack down by one, 
* duplicating the current matrix.  That is, after a 
* glPushMatrix call, the matrix  on top of the stack is 
* identical to the one below it.
**********************************************************/
void clm_glPushMatrix()
{
   glPushMatrix();
   
   return;
}

/**********************************************************
* glPopMatrix pops the current matrix stack, replacing the 
* current matrix with the one below it on the stack.
**********************************************************/
void clm_glPopMatrix()
{
   glPopMatrix();
   
   return ;
}

/**********************************************************
* Calls flVertex4f with z=0.
**********************************************************/
void clm_glVertex3f(float x, float y, float z) 
{glVertex4f(x,y,z,1);}

/**********************************************************
* Specifies a four vector point.
**********************************************************/
void clm_glVertex4f(float x, float y, float z, float w)
{
   glVertex4f(x, y, z, w);
   
   return;
}

/**********************************************************
* glLoadIdentity replaces the current matrix with the 
* identity matrix. It is semantically equivalent to calling 
* glLoadMatrix with the identity matrix.
**********************************************************/
void clm_glLoadIdentity()
{
   glLoadIdentity();
   
   return;
}

/**********************************************************
* replaces  the  current matrix with the one whose elements
* are specified by m.  The current matrix is the projection 
* matrix,  modelview  matrix, or texture matrix, depending 
* on the current matrix mode (see glMatrixMode).
**********************************************************/
void clm_glLoadMatrixd(const double * m)
{
   glLoadMatrixd(m);
   
   return;
}

/**********************************************************
* multiplies the current matrix with the one specified using
* m, and replaces the current matrix with the product.
**********************************************************/
void clm_glMultMatrixd(const double * m)
{
   glMultMatrixd(m);
   
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glEnable(...)
{ 
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glDisable(...)
{
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glRotatef(...)
{
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glTranslatef(...)
{
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glScalef(...)
{
   return;
}

/**********************************************************
*
**********************************************************/
void clm_glOrtho(...)
{
   return;
}




#include "matrixTestCase.h"
/**********************************************************
* Draw will call all of my byu functions, which will
* draw what shapes desired.. etc.
**********************************************************/
void draw()
{
   clm_glClearColor(0,0,0,1);
   clm_glClear(GL_COLOR_BUFFER_BIT);
   clm_glClear(GL_DEPTH_BUFFER_BIT);
   
   switch (drawMode)
   {
      case 0:
         clm_glViewport(0,0,320,240);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0,1,0);
           clm_glVertex3f(-1,0,0);
           clm_glVertex3f(0,-0.8,0);
           clm_glVertex3f(0.5,0.8,0);
         clm_glEnd();
         clm_glViewport(320,240,320,240);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0,0,1);
           clm_glVertex3f(-1,0.8,0);
           clm_glVertex3f(0.1,-0.8,0);
           clm_glVertex3f(0.5,0.8,0);
         clm_glEnd();
         //Restore your viewport to the whole screen
         clm_glViewport(0,0,640,480);   
         break;
      case 1: 
         clm_glBegin(GL_TRIANGLES);
         clm_glColor3f(1,0,0);
            clm_glVertex4f(-1,0.2,0,1);
            clm_glVertex4f(0,0.8,0,1);
            clm_glVertex4f(1,0.2,0,1);
         clm_glColor3f(1,0,1);
            clm_glVertex4f(-1,-0.8,0,2);
            clm_glVertex4f(0,-0.2,0,2);
            clm_glVertex4f(1,-0.8,0,2);
         clm_glEnd();
         break;
      case 2:
         clm_glEnable(GL_DEPTH_TEST);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0,1,1);
           clm_glVertex3f(-0.5,0.2,0.5);
           clm_glVertex3f(0,-0.5,0);
           clm_glVertex3f(0.5,0.2,-0.5);
           clm_glColor3f(1,1,0);
           clm_glVertex3f(-0.5,-0.2,-0.5);
           clm_glVertex3f(0,0.5,0);
           clm_glVertex3f(0.5,-0.2,0.5);
         clm_glEnd();
         clm_glDisable(GL_DEPTH_TEST);
         break;   
      case 3:
         clm_glEnable(GL_DEPTH_TEST);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.5,1,1);
           clm_glVertex3f(0.5,0,0);
           clm_glVertex3f(0,0.5,-2);
           clm_glVertex3f(0,-0.5,2);
         clm_glEnd();
         clm_glViewport(50,50,200,400);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(1,1,0.5);
           clm_glVertex3f(-1.4,-1.2,-0.5);
           clm_glVertex3f(0,1.2,0);
           clm_glVertex3f(1.5,-0.2,0.5);
         clm_glEnd();
         //Restore your viewport to the whole screen
         clm_glViewport(0,0,640,480);
         clm_glDisable(GL_DEPTH_TEST);
         break;
      case 4:
      {
         double translate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, -1.2,0.3,0,1};
         double rotate[16] = {cos(M_PI/2),sin(M_PI/2),0,0, 
                              -sin(M_PI/2),cos(M_PI/2),0,0, 
                              0,0,1,0, 0,0,0,1};
         clm_glLoadIdentity();
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.5,0.2,1);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         clm_glLoadMatrixd(translate);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.5,0.8,0.2);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         clm_glLoadIdentity();
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.2,0.6,1);
           clm_glVertex3f(0.5,-0.4,0);
           clm_glVertex3f(0.8,-0.4,0);
           clm_glVertex3f(0.65,-0.7,0);
         clm_glEnd();
         clm_glLoadMatrixd(rotate);
         clm_glMultMatrixd(translate);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.9,0.2,0.4);
           clm_glVertex3f(0.5,-0.4,0);
           clm_glVertex3f(0.8,-0.4,0);
           clm_glVertex3f(0.65,-0.7,0);
         clm_glEnd();
         clm_glLoadIdentity();
         }
         break;
      case 5:
         matrixTest();      
         break;
      case 6:
         //glRotate:
         clm_glLoadIdentity();
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.5,0.2,1);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         clm_glRotatef(90,0,0,1);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.1,0.2,1);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         break;
      case 7:
         //glTranslate:
         clm_glLoadIdentity();
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.33,0.77,0);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         clm_glTranslatef(-1,-1,0);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.77,0.2,0.3);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         break;
      case 8:
         //glScale:
         clm_glLoadIdentity();
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.9,0.5,1);
           clm_glVertex3f(0.5,0.4,0);
           clm_glVertex3f(0.8,0.4,0);
           clm_glVertex3f(0.65,0.9,0);
         clm_glEnd();
         clm_glScalef(0.8,0.7,1);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0.8,0.7,0);
           clm_glVertex3f(0.5,0.1,0);
           clm_glVertex3f(0.8,0.1,0);
           clm_glVertex3f(0.65,0.4,0);
         clm_glEnd();
         break;
      case 9:
         //glOrtho:
         clm_glLoadIdentity();
         clm_glOrtho(0,640,0,480,-1,1);
         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(1,0,0);
           clm_glVertex3f(300,300,0);
           clm_glColor3f(0,1,0);
           clm_glVertex3f(600,300,0);
           clm_glColor3f(0,0,1);
           clm_glVertex3f(450,410,0);
           clm_glColor3f(1,1,0);
           clm_glVertex3f(100,400,0);
           clm_glColor3f(0,1,1);
           clm_glVertex3f(70,60,0);
           clm_glColor3f(1,0,1);
           clm_glVertex3f(350,100,0);
         clm_glEnd();
         break;   
      case 10:
         break;
      default:
         cout << "Unknown drawMode! I'm bailing!" << endl;
         exit(0);
         break;         
   }
   return;
}

/**********************************************************
* This function will draw my raster on top of 
* OpenGL's result.
**********************************************************/
void mydraw()
{
   if (mymode == 2) 
   {
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
      glDrawPixels(SCREEN_WIDTH,SCREEN_HEIGHT,GL_RGB,GL_FLOAT,raster);
      
      //Set the state back to what it was
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW); glPopMatrix();
      glMatrixMode(oldmatrixmode);
      if (depthWasEnabled)
        glEnable(GL_DEPTH_TEST);
   }
   return;
}

/**********************************************************
* Initialize the raster. //?Don't know what a raster is.
**********************************************************/
void initRaster()
{
   float fRasterSize = ((float) RASTER_SIZE); 
   for (int i = 0; i < RASTER_SIZE; i++)
      raster[i] = i / fRasterSize;
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
   //glEnable( GL_POINT_SMOOTH );
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**********************************************************
* This function will display all of the desired images.
**********************************************************/
void display ( void )   // Create The Display Function
{
   draw();
   mydraw();
   glFlush();
	glutSwapBuffers();
	return;
}

/**********************************************************
*
**********************************************************/
// Create The Reshape Function (the viewport)
void reshape ( int w, int h )   
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
// Create Keyboard Function
void keyboard ( unsigned char key, int x, int y )  
{
	switch ( key ) 
	{
		case 27:        // When Escape Is Pressed...
			exit ( 0 );  // Exit The Program
			break;
		case 49:
		   mymode = 1;
		   display();
		   break;
		case 50:
		   mymode = 2;
		   display();
		   break;
		default:
			break;
	}
}

/**********************************************************
*
**********************************************************/
// Create Special Function (required for arrow keys)
void arrow_keys ( int a_keys, int x, int y )  
{
  switch ( a_keys ) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
      drawMode = (drawMode+1)%11;
      display();
      break;
    case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
      if ((drawMode=drawMode-1) < 0)drawMode=10;
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
  glutCreateWindow("CLM/OpenGL Graphics Program"); // Window Title
  init();
  glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(arrow_keys);
  glutMainLoop();  // Initialize The Main Loop
  return 0;
}

