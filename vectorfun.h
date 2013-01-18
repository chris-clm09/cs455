#include <vector>
#include "point.h"

using namespace std;

/***************************************************************
This function will find a vetor's back y's min x.
***************************************************************/
int getXMin(vector<Point> pts)
{
   int min = pts.back().x;
   
   for (int i = pts.size() - 1; i >= 0 && pts[i].y == pts.back().y; i--)
      if (pts[i].x < min)
         min = pts[i].x;

   return min;
}

/***************************************************************
This function will find a vetor's back y's max x.
***************************************************************/
int getXMax(vector<Point> pts)
{
   int max = pts.back().y;
   
   for (int i = pts.size() - 1; i >= 0 && pts[i].y == pts.back().y; i--)
      if (pts[i].x > max)
         max = pts[i].x;
   
   return max;
}

/***************************************************************
This function will remove the y's that match back.
***************************************************************/
void removeYBack(vector<Point>& pts)
{
   vector<Point>::iterator it = pts.begin();
   while ((*it).y != pts.back().y)
      it++;
   pts.erase(it, pts.end());
   return;
}
