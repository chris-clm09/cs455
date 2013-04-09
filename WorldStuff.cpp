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
	Camera(const vector4& p, const vector4 &d) : pos(p), dir(d), up(0,1,0,0) 
	{
		init();
		return;
	}
	
	Camera(double p[], double d[]) : pos(p[0], p[1], p[2], p[3]), 
									 dir(d[0], d[1], d[2], d[3]),
									 up(0,1,0,0) 
	{
		init();
		return;
	}

	void clear()
	{
		pos.set(0,0,0,0);
		dir.set(0,0,0,0);
	}

	void init()
	{
		vector3 dir3(dir[0],dir[1],dir[2]);
		vector3 up3(0,1,0);

		vector3 right3 = cross(dir3, up3);
		up3 = cross(right3, dir3);

		up[0] = up3[0];
		up[1] = up3[1];
		up[2] = up3[2];

		right[0] = -right3[0];		
		right[1] = -right3[1];
		right[2] = -right3[2];
	}

//private:
	vector4 pos;
	vector4 dir;
	vector4 up;
	vector4 right;	
};

#endif