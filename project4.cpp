#include "project4.h"

/**********************************************************
A simple rounding function.
**********************************************************/
template <class T>
int round(T n) {return (int) (n + .5);}

/**********************************************************
This function will set a pixel in a line.
**********************************************************/
Point setLinePixel(Point one, Point two, double cX, double cY)
{
   double fraction = getFraction(one.x, one.y, 
                                two.x, two.y, 
                                cX,    cY);
                                       
   vector4 theColor = interpolateColor(one.color,
                                       two.color,
                                       fraction);

   double cZ = interpolateZ(one, two, fraction);
   

   Point pixel(cX, cY, cZ, 0, theColor);
   
   setPixel(pixel);      
   
   return pixel;
}                  


/**********************************************************
* /// Line Function ///
* This function will draw a line between the two points.
**********************************************************/
vector<Point> drawLine(Point one, Point two)
{
   Point savedOne(one);
   Point savedTwo(two);
   
	int dX = two.x - one.x;
	int dY = two.y - one.y;
	double slope     = (double)dY / (double)dX;
	double intersept = two.y - slope * two.x;
	vector<Point> theLine;
	theLine.push_back(one);
	theLine.push_back(two);
	
	
	if (dX == 0)
	{
		int change = (one.y > two.y ? -1 : 1);
		while (one.y != two.y)
		{
			one.y += change;
			theLine.push_back(setLinePixel(savedOne, savedTwo, one.x, one.y));
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(savedOne, savedTwo, one.x+i, one.y);
			   setLinePixel(savedOne, savedTwo, one.x-i, one.y);
			}      
		}
		return theLine;	
	}
	else if (dY == 0)
	{
		int change = (one.x > two.x ? -1 : 1);
		while (one.x != two.x)
		{
			one.x += change;
			theLine.push_back(setLinePixel(savedOne, savedTwo, one.x, one.y));
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(savedOne, savedTwo, one.x, one.y+i);
			   setLinePixel(savedOne, savedTwo, one.x, one.y-i);
			}
		}
		return theLine;	
	}
	
	if (abs(dX) > abs(dY))
	{
		int change = (one.x > two.x ? -1 : 1);
		unsigned int newY; 
		while (one.x != two.x)
		{
			one.x += change;
			newY = (unsigned int) (ceil(slope * one.x + intersept-.5));
			theLine.push_back(setLinePixel(savedOne, savedTwo, one.x, newY));
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(savedOne, savedTwo, one.x+i, newY);
   			setLinePixel(savedOne, savedTwo, one.x-i, newY);
   	   }
		}	
	}
	else
	{
		int change = (one.y > two.y ? -1 : 1);
		unsigned int newX;
		while (one.y != two.y)
		{
			one.y += change;
			newX = (unsigned int)(ceil((1.0 / slope) * (one.y - intersept) -.5));
			theLine.push_back(setLinePixel(savedOne, savedTwo, newX, one.y));
			for (int i = 0; i <= lineWidth/2; i++)
			{
			   setLinePixel(savedOne, savedTwo, newX+i, one.y);
   			setLinePixel(savedOne, savedTwo, newX-i, one.y);
   	   }
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
      Point min = getXMin(points);
      Point max = getXMax(points);
      drawLine(min,max);
      removeYBack(points);   
   }
   return;
}

/**********************************************************
Sets the clear color.
**********************************************************/
void clm_glClearColor(double r, double g, double b, double a)
{
   glClearColor(r, g, b, a);
   clearColor.set(r, g, b, a);
   return;
}

/**********************************************************
This function will set the color of a pixel.
**********************************************************/
void setPixel(const Point& pixel)
{ 
   //Check if point is in screen and viewport   
   if (pixel.x >= SCREEN_WIDTH || pixel.y >= SCREEN_HEIGHT 
     || pixel.x < 0 || pixel.y < 0
     || pixel.x >= viewport[0] + viewport[2] || pixel.x < viewport[0]
     || pixel.y >= viewport[1] + viewport[3] || pixel.y < viewport[1]
     || (depth_test && (pixel.z >= zBuffer[pixel.x][pixel.y]
                       /*|| pixel.z < -1 || pixel.z > 1*/)))
     return;
   
        
   int temp = ((pixel.y * SCREEN_WIDTH) + pixel.x) * 3;
   raster[ temp + 0 ] = pixel.color[0];
   raster[ temp + 1 ] = pixel.color[1];
   raster[ temp + 2 ] = pixel.color[2];
   
   if (depth_test)
      zBuffer[pixel.x][pixel.y]  = pixel.z;
        
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
void fillRasterWColor(vector4 color, int x_start = 0, int x_max = SCREEN_WIDTH 
                                   , int y_start = 0, int y_max = SCREEN_HEIGHT)
{
   bool oldTest = depth_test;
   depth_test   = false;
   
   for (int x = x_start; x < x_max; x++)
      for (int y = y_start; y < y_max; y++)
         setPixel(Point(x, y, 0, 1, color));
         
   depth_test = oldTest;      
   return;
}

/**********************************************************
Clears the entire screen to the clear color.
**********************************************************/
void clm_glClear(GLint bit)
{
   glClear(bit);
   
   if (bit & GL_DEPTH_BUFFER_BIT)
      initZBuffer();
  
   fillRasterWColor(clearColor, viewport[0], viewport[2],
                                viewport[1], viewport[3]);
               
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
   firstPt.clear();
   return;
}

/**********************************************************
This function will save the given point and return if 
the desired number of points has been reached.
**********************************************************/
bool saveAndReachedPoints(int num, int x, int y, double z, double w)
{
   if (savedPoints.size() < num)
   {  //save
      savedPoints.push_back(Point(x,y,z,w,penColor));
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
void drawStrip(int x, int y, double z, double w)
{
   if (savedPoints.size() == 0)
      saveAndReachedPoints(2, x, y, z, w);
   else
   {
      saveAndReachedPoints(2,x,y,z,w);
      drawLine(savedPoints[0], savedPoints[1]);
               
      savedPoints.erase(savedPoints.begin());
   }     
   return;
}

/**********************************************************
* Set's raster pixels given current drawing mode.
**********************************************************/
void drawVertex(int x, int y, double z, double w)
{
   if (glDrawMode == GL_POINTS) 
      setPixel(Point(x, y, z, w, penColor));
   else if (glDrawMode == GL_LINES)
   {
      int numPointsInLine = 2;
      if (saveAndReachedPoints(numPointsInLine, x, y, z, w))
      {
         drawLine(savedPoints[0], savedPoints[1]);
         savedPoints.clear();
      }
	}
	else if (glDrawMode == GL_TRIANGLES)
	{
	   int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y, z, w))
      {
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0], savedPoints[1]);
         temp   = drawLine(savedPoints[1], savedPoints[2]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2], savedPoints[0]);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.clear();
      }
	}
   //Draws one line segment for every call to glVertex2i except the first; 
   //vertices n and n + 1 define line segment n.
   else if (glDrawMode == GL_LINE_STRIP)
   {
      drawStrip(x,y,z,w);
   }
   //Exactly like GL_LINE_STRIP except one additional line is draw between 
   //the first and last calls to glVertex2i when glEnd is called.
   else if (glDrawMode == GL_LINE_LOOP)
   {
      if (firstPt.eq(Point(-1,-1,-1,-1)))
         firstPt.set(x,y,z,w,penColor);
      drawStrip(x,y,z,w);
   }
   else if (glDrawMode == GL_TRIANGLE_STRIP)//Draws a connected group of triangles.
   {
      int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y, z, w))
      {
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0], savedPoints[1]);
         temp   = drawLine(savedPoints[1], savedPoints[2]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2], savedPoints[0]);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin());
      }
   }
   else if (glDrawMode == GL_TRIANGLE_FAN || 
            glDrawMode == GL_POLYGON)//Draws a connected group of triangles. 
   {
      int numPointsInTri = 3;
      if (saveAndReachedPoints(numPointsInTri, x, y, z, w))
      {
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0], savedPoints[1]);
         temp   = drawLine(savedPoints[1], savedPoints[2]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2], savedPoints[0]);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin()+1);
      }
   }
   else if (glDrawMode == GL_QUADS)
   {
      int numPointsInTri = 4;
      if (saveAndReachedPoints(numPointsInTri, x, y, z, w))
      {
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0], savedPoints[1]);
         temp   = drawLine(savedPoints[1], savedPoints[2]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2], savedPoints[3]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[3], savedPoints[0]);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.clear();
      }
   }
   else if (glDrawMode == GL_QUAD_STRIP)
   {
      int numPointsInTri = 4;
      if (saveAndReachedPoints(numPointsInTri, x, y, z, w))
      {
         vector<Point> points;
         vector<Point> temp;
         
         points = drawLine(savedPoints[0], savedPoints[1]);
         temp   = drawLine(savedPoints[1], savedPoints[3]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[3], savedPoints[2]);
         points.insert(points.end(), temp.begin(), temp.end());
         temp   = drawLine(savedPoints[2], savedPoints[0]);
         points.insert(points.end(), temp.begin(), temp.end());
                 
         fill(points);
         
         savedPoints.erase(savedPoints.begin());
         savedPoints.erase(savedPoints.begin());
      }
   }
   else
	{
	   cout << "Holly Crap!" << endl;
	   exit(0);
	}
   return;
}

/**********************************************************
There must be one glEnd for every glBegin. glVertex only 
works between Begin & End pairs.
**********************************************************/
void clm_glEnd()
{
   glEnd();
   
   if (glDrawMode == GL_LINE_LOOP)
      drawStrip(firstPt.x, firstPt.y, firstPt.z, firstPt.w);
   
   savedPoints.clear();
   firstPt.clear();
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
* This function will generate a new matrix based off of
* an array.
**********************************************************/
matrix4 createMatrix(const double* m)
{  
   return matrix4(m[0], m[4], m[8],  m[12],
                  m[1], m[5], m[9],  m[13],
                  m[2], m[6], m[10], m[14],
                  m[3], m[7], m[11], m[15]);
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

   switch (mode)
   {
      case GL_MODELVIEW:
         currentMatrixStack = &(matrixStacks[0]);         
         break;
      case GL_PROJECTION:
         currentMatrixStack = &(matrixStacks[1]);
         break;
      default:
         cout << "Warning: clm_glMatrixMode Unknow mode type.\n";
         break;
   }
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
   viewport.set(x,y,width,height);
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
   matrix4 m = currentMatrixStack->back();
   currentMatrixStack->push_back(m);
   return;
}

/**********************************************************
* glPopMatrix pops the current matrix stack, replacing the 
* current matrix with the one below it on the stack.
**********************************************************/
void clm_glPopMatrix()
{
   glPopMatrix();
   if (currentMatrixStack->size() > 1)
      currentMatrixStack->pop_back();
   else
      cout << "Warning: You tried to pop the matrix stack w/ Size == 1\n";
      
   return;
}

/**********************************************************
* Specifies a four vector point.
**********************************************************/
void clm4f(double x, double y, double z=0.0, double w=1.0)
{
   vector4 v(x,y,z,w);
   
   //  Projection                ModelView
   v = matrixStacks[1].back() * matrixStacks[0].back() * v;
   
   //Divid by W
   v /= v[3];
   
   //Size to your viewport. Basically, add 1 to all x and y coordinates, 
   //and then divide by two. This will put all your points between 0,0 and 
   //1,1. Then, multiply your x coordinate by the viewport width and add the 
   //x viewport min. Do the same to your y coordinate with the height and y 
   //viewport min.
   v[0] = ((v[0]+1)/2.0) * viewport[2] + viewport[0];
   v[1] = ((v[1]+1)/2.0) * viewport[3] + viewport[1];
        
   drawVertex(round(v[0]), round(v[1]), v[2], v[3]);
   
   return;
}

/**********************************************************
* Specifies a four vector point.
**********************************************************/
void clm_glVertex4f(double x, double y, double z=0.0, double w=1.0)
{
   glVertex4f(x, y, z, w);
   clm4f(x,y,z,w);   
   return;
}

/**********************************************************
* Calls flVertex4f with z=0.
**********************************************************/
void clm_glVertex3f(double x, double y, double z=0) 
{clm_glVertex4f(x,y,z);}

/**********************************************************
* Rounds x and y and calls Vertex2i
**********************************************************/
void clm_glVertex2f(double x, double y)
{
   glVertex2f(x, y);
   clm4f(x, y);
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
   clm_glVertex4f(x,y);
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
   currentMatrixStack->push_back(identityMatrix);
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
   currentMatrixStack->push_back(createMatrix(m));
   return;
}

/**********************************************************
* multiplies the current matrix with the one specified using
* m, and replaces the current matrix with the product.
**********************************************************/
void multMatrix(const matrix4& m)
{
   (*currentMatrixStack)[currentMatrixStack->size() - 1] *= m;
   return;
}

/**********************************************************
* Prints the current matrix.
**********************************************************/
void printCurrentMatrix()
{
   cout << (*currentMatrixStack)[currentMatrixStack->size() - 1] << endl;
   return;
}

/**********************************************************
* multiplies the current matrix with the one specified using
* m, and replaces the current matrix with the product.
**********************************************************/
void clm_glMultMatrixd(const double * m)
{
   glMultMatrixd(m);
   multMatrix(createMatrix(m));
   return;
}

/**********************************************************
* Enable
**********************************************************/
void clm_glEnable(GLenum mask)
{ 
   glEnable(mask);
   
   if (mask & GL_DEPTH_TEST)
      depth_test = true;
   
   return;
}

/**********************************************************
* Disable
**********************************************************/
void clm_glDisable(GLenum mask)
{
   glDisable(mask);
   
   if (mask & GL_DEPTH_TEST || mask == GL_DEPTH_TEST)
      depth_test = false;
   
   return;
}

/**********************************************************
*  multiply the current matrix by a rotation matrix
**********************************************************/
void clm_glRotatef(double angle, double x, double y, double z)
{
   glRotatef(angle,x,y,z);
   
   double s = sin(angle*M_PI/180.0);
   double c = cos(angle*M_PI/180.0);
   
   matrix4 m((x*x*(1-c))+c,     (x*y*(1-c))-z*s,   (x*z*(1-c))+y*s,     0,
             y*x*(1-c)+z*s,     y*y*(1-c)+c,       y*z*(1-c)-x*s,       0,
             x*z*(1-c)-y*s,     y*z*(1-c)+x*s,     z*z*(1-c)+c,         0,
                  0,                    0,                0,            1);

   multMatrix(m);
   return;
}

/**********************************************************
*  multiply the current matrix by a translation matrix
**********************************************************/
void clm_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
   glTranslatef(x,y,z);
   
   matrix4 m(1, 0, 0, x,
             0, 1, 0, y,
             0, 0, 1, z,
             0, 0, 0, 1);
             
   multMatrix(m);
   return;
}

/**********************************************************
* multiply the current matrix by a general scaling matrix
**********************************************************/
void clm_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
   glScalef(x,y,z);

   matrix4 m(x, 0, 0, 0,
             0, y, 0, 0, 
             0, 0, z, 0,
             0, 0, 0, 1);

   multMatrix(m);
   return;
}

/**********************************************************
* multiply the current matrix with an orthographic matrix
**********************************************************/
void clm_glOrtho(GLdouble left, GLdouble right, GLdouble bottom, 
                 GLdouble top,  GLdouble zNear, GLdouble zFar)
{
   glOrtho(left, right, bottom, top, zNear, zFar);
   
   double tx = - (right + left)   / (right - left);
   double ty = - (top   + bottom) / (top   - bottom);
   double tz = - (zFar  + zNear)  / (zFar  - zNear);

   matrix4 m( 2/(right-left), 0, 0, tx,
              0, 2/(top-bottom), 0, ty,
              0, 0, -2/(zFar-zNear),tz,
              0, 0, 0, 1);
   
   multMatrix(m);
   return;
}

/**********************************************************
scale such that the point (cx,cy,cz) does not move.
**********************************************************/
void clm_fixedScale(double sx, double sy, double sz, 
                    double cx, double cy, double cz)
{
   matrix4 M(sx, 0, 0, cx - cx * sx,
             0, sy, 0, cy - cy * sy, 
             0, 0, sz, cz,
             0, 0, 0,  1);
   
   glMultMatrixd(M.data());
   multMatrix(M);
   return;
}

/**********************************************************
shearing, where x moves sxy * y, etc.
**********************************************************/
void clm_shear(double sxy, double sxz, double syx, 
               double syz, double szx, double szy)
{
   matrix4 M(1, sxy, sxz, 0,
             syx, 1, syz, 0,
             szx, szy, 1, 0,
             0, 0, 0, 1);
             
   glMultMatrixd(M.data());
   multMatrix(M);
   return;
}


void clm_fullRotate(double angle, double x, double y, double z, 
                                double bx, double by, double bz)
{
   double s = sin(angle*M_PI/180.0);
   double c = cos(angle*M_PI/180.0);
   
   matrix4 m((x*x*(1-c))+c,     (x*y*(1-c))-z*s,   (x*z*(1-c))+y*s,     0,
             y*x*(1-c)+z*s,     y*y*(1-c)+c,       y*z*(1-c)-x*s,       0,
             x*z*(1-c)-y*s,     y*z*(1-c)+x*s,     z*z*(1-c)+c,         0,
                  0,                    0,                0,            1);

   matrix4 t(1,0,0,-bx,
             0,1,0,-by,
             0,0,1,-bz,
             0,0,0,1);   
   matrix4 tb(1,0,0, bx,
              0,1,0, by,
              0,0,1, bz,
              0,0,0, 1);
             
   matrix4 M = tb*m*t;//TODO Precalculate this.
   
   glMultMatrixd(M.data());
   
   multMatrix(M);

   return;
}

/**********************************************************
* glFrustum - multiply the current matrix by a perspective 
*             matrix
* 
* glFrustum describes a perspective matrix that  produces  
* a  perspective projection.   The  current  matrix  
* (see glMatrixMode) is multiplied by this  matrix.
**********************************************************/
void clm_glFrustum(GLdouble left, GLdouble right, 
                   GLdouble bottom, GLdouble top,
                   GLdouble zNear, GLdouble zFar )
{
   //glFrustum(left, right, bottom, top, zNear, zFar);
   
   
   double A =       (right + left) / (right - left);
   double B =       (top + bottom) / (top - bottom);
   double C =     - (zFar + zNear) / (zFar - zNear);
   double D = - (2 * zFar * zNear) / (zFar - zNear);

   matrix4 m(2 * zNear/ (right - left), 0, A, 0,
             0, 2*zNear/(top-bottom),      B, 0,
             0, 0,  C, D,
             0, 0, -1, 0);
   
   glMultMatrixd(m.data());
   multMatrix(m);
   
   return;
}                       

/**********************************************************
* gluPerspective - set up a perspective projection matrix
* 
* gluPerspective specifies a viewing frustum into  the  
* world  coordinate system.   In  general,  the aspect ratio 
* in gluPerspective should match the aspect ratio of the 
* associated viewport.
* See man page.
**********************************************************/
void clm_gluPerspective(GLdouble fovy,  GLdouble aspect,
                        GLdouble zNear, GLdouble zFar )
{
   gluPerspective(fovy, aspect, zNear, zFar);
   
   double angle = (fovy/2.0)*(M_PI/180.0);
   double f  = cos(angle)/sin(angle);
 
   matrix4 m(f/aspect,      0,              0,              0,
            0,              f,              0,              0,
            0,              0, zFar+zNear/(zNear-zFar), 2*zFar*zNear/(zNear-zFar),
            0,              0,              -1,             0);
     
   multMatrix(m);

   return;
}







/**********************************************************
* Draw will call all of my byu functions, which will
* draw what shapes desired.. etc.
**********************************************************/
void draw()
{
   clm_glClearColor(0,0,0,1);
   clm_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   switch (drawMode)
   {
      case 0:
         //Fulstrum
         clm_glMatrixMode(GL_PROJECTION);
         clm_glLoadIdentity();
         clm_glFrustum(-0.1,0.1, -0.1*480/640.0,0.1*480/640.0, 0.1,10);
         clm_glMatrixMode(GL_MODELVIEW);
         clm_glLoadIdentity();

         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0,0,1);
           clm_glVertex3f(-0.4,-0.6,-1);
           clm_glVertex3f(0.4,-0.6,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(-0.4,-0.1,-1);
           clm_glVertex3f(-0.4,-0.6,-1);
           clm_glColor3f(1,0,1);
           clm_glVertex3f(-0.4,-0.1,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(0.3,0,-2);
           clm_glVertex3f(0.3,0,-2);
           clm_glVertex3f(-0.3,0,-2);
           clm_glVertex3f(-0.4,-0.1,-1);
         clm_glEnd();	
         break;
      case 1: 
         //Perspective
         clm_glMatrixMode(GL_PROJECTION);
         clm_glLoadIdentity();
         clm_gluPerspective(90, double(640)/480.0, .1, 10);
         clm_glMatrixMode(GL_MODELVIEW);
         clm_glLoadIdentity();

         clm_glBegin(GL_TRIANGLES);
           clm_glColor3f(0,0,1);
           clm_glVertex3f(-0.4,-0.6,-1);
           clm_glVertex3f(0.4,-0.6,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(-0.4,-0.1,-1);
           clm_glVertex3f(-0.4,-0.6,-1);
           clm_glColor3f(1,0,1);
           clm_glVertex3f(-0.4,-0.1,-1);
           clm_glVertex3f(0.4,-0.1,-1);
           clm_glVertex3f(0.3,0,-2);
           clm_glVertex3f(0.3,0,-2);
           clm_glVertex3f(-0.3,0,-2);
           clm_glVertex3f(-0.4,-0.1,-1);
         clm_glEnd();
         break;
      case 2:

         break;   
      case 3:

         break;
      case 4:

         break;
      case 5:

         break;
      case 6:
         //
         break;
      case 7:
         //
         break;
      case 8:
         //
         break;
      case 9:
         //
         break;   
      case 10:
      //
         break;
      case 11:
      //
         break;
      case 12:
          //
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
      clm_glDisable(GL_DEPTH_TEST);
      glGetIntegerv(GL_MATRIX_MODE,&oldmatrixmode);
      clm_glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
      clm_glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
      
      // Draw the array of pixels (This is where you draw the values
      // you have stored in the array 'raster')
      glRasterPos2f(-1,-1);
      glDrawPixels(SCREEN_WIDTH,SCREEN_HEIGHT,GL_RGB,GL_FLOAT,raster);
      
      //Set the state back to what it was
      glPopMatrix();
      clm_glMatrixMode(GL_MODELVIEW); glPopMatrix();
      clm_glMatrixMode(oldmatrixmode);
      if (depthWasEnabled)
        clm_glEnable(GL_DEPTH_TEST);
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
* Initialize the Z Buffer to 1's.
**********************************************************/
void initZBuffer()
{
   for (int x = 0; x < SCREEN_WIDTH; x++)
      for (int y = 0; y < SCREEN_HEIGHT; y++)
         zBuffer[x][y] = 1;
         
   return;
}

/**********************************************************
* Initalizes clm_'s matrix stacks.
**********************************************************/
void initMatrixStacks()
{
   clm_glMatrixMode(GL_PROJECTION);
	clm_glLoadIdentity();
	clm_glMatrixMode(GL_MODELVIEW);
	clm_glLoadIdentity();
   return;
}

/**********************************************************
* Initial GL and clm_GL 
**********************************************************/
void init ()
{
   initRaster();
   initZBuffer();
   initMatrixStacks();
   glShadeModel(GL_SMOOTH);			// Enable Smooth Shading
   glClearDepth(1.0f);					// Depth Buffer Setup
   clm_glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
   glDepthFunc(GL_LEQUAL);				// The Type Of Depth Testing To Do
   clm_glEnable(GL_COLOR_MATERIAL);
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
	clm_glViewport( 0, 0, w, h );
	clm_glMatrixMode(GL_PROJECTION);
	clm_glLoadIdentity();
	clm_glMatrixMode(GL_MODELVIEW);
	clm_glLoadIdentity();
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
      drawMode = (drawMode+1)%13;
      display();
      break;
    case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
      if ((drawMode=drawMode-1) < 0)drawMode=12;
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
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode
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

