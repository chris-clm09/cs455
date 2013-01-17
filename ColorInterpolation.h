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
* This function will return the interpolated color
* based on a certin fraction.
* Source:
* http://harmoniccode.blogspot.com/2011/04/bilinear-
* color-interpolation.html
**********************************************************/
vector3 interpolateColor(vector3 COLOR1, vector3 COLOR2, float fraction)
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
   
   cout << "\ndR: " << DELTA_RED << " dG: "<< DELTA_GREEN << " dB: "<< DELTA_BLUE << endl;
   cout << "c1: " << COLOR1[0] << "," << COLOR1[1] << "," << COLOR1[2] << endl;
   cout << "c2: " << COLOR2[0] << "," << COLOR2[1] << "," << COLOR2[2] << endl;
   float red   = RED1   + (DELTA_RED   * fraction);
   float green = GREEN1 + (DELTA_GREEN * fraction);
   float blue  = BLUE1  + (DELTA_BLUE  * fraction);
   
   //cout << "r: "<<(int)(red) << " g: "<<(int)green << " b: "<<(int)blue << endl;
   
   
   red   = min(red,   1.0f);
   red   = max(red,   0.0f);
   green = min(green, 1.0f);
   green = max(green, 0.0f);
   blue  = min(blue,  1.0f);
   blue  = max(blue,  0.0f);
   
   red   *= 255;
   blue  *= 255;
   green *= 255;
   
   cout << "faction: " << fraction << " cnt: " << INT_TO_FLOAT_CONST << endl;
   //cout << "r: "<<(int)(red) << " g: "<<(int)green << " b: "<<(int)blue << endl;
   cout << "r: "<< red << " g: "<< green << " b: "<< blue << endl;
   return vector3(red, green, blue);        
}

