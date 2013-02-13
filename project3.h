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

typedef cml::matrix44d_c matrix4;
typedef cml::vector4d vector4;
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

int drawMode    = 0;
int mymode      = 0;
bool depth_test = false;

//-----------------------------------------------------
//Matricies Stacks
//Two stacks one for gl_modelview and gl_projection
vector<matrix4> matrixStacks[2];
vector<matrix4> * currentMatrixStack;

vector4 viewport(0,0,0,0);
matrix4 identityMatrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

//-------------------PRINT CRAP--------------------------------------
void pp(vector4 p){ cout << p[0] << "," << p[1] << "," << p[2] << endl;}
void pp(Point p) {cout << p.x << "," << p.y << endl;}

