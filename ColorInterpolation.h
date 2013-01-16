#include <math>

vector3 interpolateColor(vector3 COLOR1, vector3 COLOR2, float fraction)
{            
   const float INT_TO_FLOAT_CONST = 1.0 / 255.0;
   fraction = Math.min(fraction, 1.0);
   fraction = Math.max(fraction, 0.0);
   
   float RED1   = COLOR1[0] * INT_TO_FLOAT_CONST;
   float GREEN1 = COLOR1[1] * INT_TO_FLOAT_CONST;
   float BLUE1  = COLOR1[3] * INT_TO_FLOAT_CONST;
   
   float RED2 = COLOR2.getRed() * INT_TO_FLOAT_CONST;
   float GREEN2 = COLOR2.getGreen() * INT_TO_FLOAT_CONST;
   float BLUE2 = COLOR2.getBlue() * INT_TO_FLOAT_CONST;
   
   final float DELTA_RED   = RED2 - RED1;
   final float DELTA_GREEN = GREEN2 - GREEN1;
   final float DELTA_BLUE  = BLUE2 - BLUE1;
   
   float red   = RED1   + (DELTA_RED * fraction);
   float green = GREEN1 + (DELTA_GREEN * fraction);
   float blue  = BLUE1  + (DELTA_BLUE * fraction);
   
   red   = Math.min(red,   1.0);
   red   = Math.max(red,   0.0);
   green = Math.min(green, 1.0);
   green = Math.max(green, 0.0);
   blue  = Math.min(blue,  1.0);
   blue  = Math.max(blue,  0.0);
   
   return vertex3(red, green, blue);        
}

