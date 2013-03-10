#ifndef clm_project4
#define clm_project4

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
typedef cml::vector4d    vector4;
#include "ColorInterpolation.h"
#include "printGLMatrix.cpp"
#include "light.cpp"

//---------------------Forward Declarations----------------
void setPixel(const Point& pixel);
vector4 getPixelColor(int x, int y);
void initZBuffer();

/**********************************************************
* // GLOBAL CRAP
**********************************************************/
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH  = 640;
const int RASTER_SIZE   = SCREEN_HEIGHT * SCREEN_WIDTH * 3;
const int Z_BUFFER_SIZE = SCREEN_HEIGHT * SCREEN_WIDTH;

vector4 clearColor(0,0,0,0);
vector4 penColor(1,1,1,1);
vector4 normal(1,1,1,0);

float raster[RASTER_SIZE];
double zBuffer[SCREEN_WIDTH][SCREEN_HEIGHT];
GLenum glDrawMode;
vector<Point> savedPoints;
Point firstPt(-1,-1, -1, -1, vector4(-1,-1,-1,-1));
int lineWidth = 1;

int drawMode       = 0;
int mymode         = 0;
bool depth_test    = false;
bool color_test    = false;
bool material_test = false;

//-----------------------------------------------------
//Matricies Stacks
//Two stacks one for gl_modelview and gl_projection
GLenum matrixMode;
vector<matrix4> matrixStacks[2];
vector<matrix4> * currentMatrixStack;
matrix4 inverseTransOfModelView(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

vector4 viewport(0,0,0,0);//xmin, ymin, width, height
matrix4 identityMatrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

//-----------------------------------------------------
//Lights (Ligth0 == 16384)
Light lights[8];


//-------------------PRINT CRAP--------------------------------------
void pp(vector4 p){ cout << p[0] << "," << p[1] << "," << p[2] << endl;}
void pp(Point p) {cout << p.x << "," << p.y << endl;}

#endif
