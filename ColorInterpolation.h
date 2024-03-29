#include <algorithm>
#include <math.h>
using namespace std;


/**********************************************************
* This function will find the distance between two points.
**********************************************************/
float dist(int x1, int y1, int x2, int y2)
{
   return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/**********************************************************
* This function will find the percentage cx and y are
* between s(start) and e(end).
**********************************************************/
float getFraction(int sx, int sy,
                  int ex, int ey,
                  int cx, int cy)
{
   float totalDist = dist(sx, sy, ex, ey);
   float curtDist  = dist(sx, sy, cx, cy);
   return curtDist / totalDist;
}


/**********************************************************
* This function will return the interpolated the z value.
**********************************************************/
double interpolateZ(Point one, Point two, float fraction)
{
   fraction = min(fraction, 1.0f);
   fraction = max(fraction, 0.0f);
   
   return (two.z - one.z)*fraction + one.z;
}

/**********************************************************
* This function will return the interpolated the z value.
**********************************************************/
vector4 interpolateLight(Point one, Point two, double fraction)
{
   vector4 oLight = one.light;
   vector4 tLight = two.light;

   return vector4(
      (tLight[0] - oLight[0]) * fraction + oLight[0],
      (tLight[1] - oLight[1]) * fraction + oLight[1],
      (tLight[2] - oLight[2]) * fraction + oLight[2],
      (tLight[3] - oLight[3]) * fraction + oLight[3]
      );;
}

/**********************************************************
* This funciton will interpolate normals.
**********************************************************/
vector4 interpolateNormal(Point one, Point two, double fraction)
{
   vector4 oNorm = one.normal;
   vector4 tNorm = two.normal;

   return vector4(
      (tNorm[0] - oNorm[0]) * fraction + oNorm[0],
      (tNorm[1] - oNorm[1]) * fraction + oNorm[1],
      (tNorm[2] - oNorm[2]) * fraction + oNorm[2],
      (tNorm[3] - oNorm[3]) * fraction + oNorm[3]
      );;
}

/**********************************************************
* This function will return the interpolated color
* based on a certin fraction.
* Source:
* http://harmoniccode.blogspot.com/2011/04/bilinear-
* color-interpolation.html
**********************************************************/
vector4 interpolateColor(vector4 COLOR1, vector4 COLOR2, float fraction)
{            
   const float INT_TO_FLOAT_CONST = 1.0f / 255.0f;
   
   fraction = min(fraction, 1.0f);
   fraction = max(fraction, 0.0f);
   
   float RED1   = COLOR1[0] * INT_TO_FLOAT_CONST;
   float GREEN1 = COLOR1[1] * INT_TO_FLOAT_CONST;
   float BLUE1  = COLOR1[2] * INT_TO_FLOAT_CONST;
   
   float RED2   = COLOR2[0] * INT_TO_FLOAT_CONST;
   float GREEN2 = COLOR2[1] * INT_TO_FLOAT_CONST;
   float BLUE2  = COLOR2[2] * INT_TO_FLOAT_CONST;
   
   float DELTA_RED   = RED2   - RED1;
   float DELTA_GREEN = GREEN2 - GREEN1;
   float DELTA_BLUE  = BLUE2  - BLUE1;
   
   float red   = RED1   + (DELTA_RED   * fraction);
   float green = GREEN1 + (DELTA_GREEN * fraction);
   float blue  = BLUE1  + (DELTA_BLUE  * fraction);
   
   red   = min(red,   1.0f);
   red   = max(red,   0.0f);
   green = min(green, 1.0f);
   green = max(green, 0.0f);
   blue  = min(blue,  1.0f);
   blue  = max(blue,  0.0f);
   
   red   *= 255;
   blue  *= 255;
   green *= 255;

   return vector4(red, green, blue, 1.0);        
}

