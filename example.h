#ifndef clm_example
#define clm_example

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <iostream>
#include "cml/cml.h"
#include <vector>

typedef cml::matrix44d_c matrix4;
typedef cml::vector4d    vector4;

#include "point.h"
#include "Scene.cpp"
using namespace std;

/**********************************************************
* // GLOBAL CRAP
**********************************************************/
const int SCREEN_HEIGHT = 640;
const int SCREEN_WIDTH  = 480;
const int RASTER_SIZE = SCREEN_HEIGHT * SCREEN_WIDTH * 3;
float raster[RASTER_SIZE];
int drawMode = 0;

Scene currentScene;

#endif
