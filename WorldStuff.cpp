#ifndef clm_worldStuff
#define clm_worldStuff

/************************************************************
* Represents a ray in the world.  Given by a starting point
* and a direction.
************************************************************/
class Ray
{
public:
	Ray(){};
	Ray(const vector4 &p, const vector4 &d) : pos(p), dir(d) {return;}

//private:
	vector4 pos;
	vector4 dir;
};

/************************************************************
* Represents a point light in the world.  Represented by
* a position.
************************************************************/
class Light
{
public:
	Light(){};
	Light(const vector4 &p,const vector4 &d,const vector4 &a,const vector4 &s) 
	: pos(p),  deffuseColor(d), ambientColor(a), specColor(s){return;}

//private:
	vector4 pos;
	vector4 ambientColor;
	vector4 deffuseColor;
	vector4 specColor;
};

/************************************************************
* Reprecents the camera in the world.
************************************************************/
class Camera
{
public:
	Camera(){};
	Camera(const vector4& p, const vector4 &d) : pos(p), dir(d) {return;}
	Camera(double p[], double d[]) : pos(p[0], p[1], p[2], p[3]), 
									 dir(d[0], d[1], d[2], d[3]) {return;}

	void clear()
	{
		pos.set(0,0,0,0);
		dir.set(0,0,0,0);
	}

//private:
	vector4 pos;
	vector4 dir;	
};

#endif