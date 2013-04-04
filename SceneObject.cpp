#ifndef clm_sceneObj
#define clm_sceneObj

#include "WorldStuff.cpp"


/************************************************************
* Represents a sphere in 3d space.
************************************************************/
class Sphere
{
public:
	Sphere():radius(0), pos(0,0,0,0) {return;}
	Sphere(double p[], double radius):pos(p[0],p[1],p[2],p[3]),radius(radius){return;}

	/************************************************************
	* Return true if a ray intersects this sphere. It also
	* updates t to the distance from the ray to the hit.
	************************************************************/
	bool rayHitMe(const Ray &r, double &t)
	{
		return false;
	}

//private:
	vector4 pos;
	double radius;
};

#endif