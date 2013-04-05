#ifndef clm_sceneObj
#define clm_sceneObj

#include "WorldStuff.cpp"

/************************************************************
* Represents a sphere in 3d space.
************************************************************/
class Sphere
{
public:
	Sphere():radius(0), pos(0,0,0,0), color(1,1,1,0) {return;}
	
	Sphere(double p[], 
			double radius, 
			const vector4 &c, 
			const vector4 &sc=vector4(1,1,1,0),
		   	double shinyness=1, 
		   	double reflectivity=1)
	:pos(p[0],p[1],p[2],p[3]),
	radius(radius),
	color(c),
	specColor(sc),
	shinyness(shinyness),
	reflectivity(reflectivity)
	{return;}


	/************************************************************
	* Return true if a ray intersects this sphere. It also
	* updates d to the distance from the ray to the hit.
	************************************************************/
	bool rayHitMeCloserThanD(const Ray &r, double &d)
	{
	 	vector4 dist = r.pos - pos;

		double  B = 2 * dot(r.dir,dist);
		vector4 s = r.pos + r.dir * d - pos;
		double  C = dot(dist, dist) - dot(s,s);
		double  D = (B * B) - (4 * C);
		
		if (D < 0.0f) return false;
		
		double t0 = (- B - sqrtf(D)) / 2.0;
		double t1 = (- B + sqrtf(D)) / 2.0;
		
		bool retvalue = false;
		if ((t0 > 0.1f ) && (t0 < d))
		{
			d = t0;
			retvalue = true;
		}
		if ((t1 > 0.1f ) && (t1 < d))
		{
			d = t1;
			retvalue = true;
		}
		return retvalue;
	}

//private:
	vector4 pos;
	vector4 color;
	vector4 specColor;
	double reflectivity;
	double shinyness;
	double radius;
};

#endif