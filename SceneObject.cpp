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
	 	vector4 dist = pos - r.pos;
		double B = dot(r.dir,dist);
		double D = B * B - dot(dist,dist) + radius * radius;
		
		if (D < 0.0f) return false;
		
		double t0 = B - sqrtf(D);
		double t1 = B + sqrtf(D);
		
		bool retvalue = false;
		if ((t0 > 0.1f ) && (t0 < t))
		{
			t = t0;
			retvalue = true;
		}
		if ((t1 > 0.1f ) && (t1 < t))
		{
			t = t1;
			retvalue = true;
		}
		return retvalue;
	}

//private:
	vector4 pos;
	double radius;
};

#endif