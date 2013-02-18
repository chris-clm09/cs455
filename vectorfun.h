#include <vector>
#include "point.h"

using namespace std;

/***************************************************************
This function will find a vetor's back y's min x.
***************************************************************/
Point getXMin(vector<Point> pts)
{
   Point minPoint = pts.back();
   
   for (int i = pts.size() - 1; i >= 0 && pts[i].y == pts.back().y; i--)
      if (pts[i].x < minPoint.x)
         minPoint = pts[i];

   return minPoint;
}

/***************************************************************
This function will find a vetor's back y's max x.
***************************************************************/
Point getXMax(vector<Point> pts)
{
   Point maxPoint = pts.back();
   
   for (int i = pts.size() - 1; i >= 0 && pts[i].y == pts.back().y; i--)
      if (pts[i].x > maxPoint.x)
         maxPoint = pts[i];
   
   return maxPoint;
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
