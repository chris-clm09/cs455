#ifndef clm_point
#define clm_point
typedef cml::vector4d vector4;

class Point
{
public:
   void init(int x, int y, double z, double w, 
             vector4 color, vector4 norm, vector4 light)
   {
      this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->color  = color;
		this->normal = norm;
		this->light  = light;
   }

	Point(int x, int y)
	{
	   init(x,y,0,1,vector4(0,0,0,0),vector4(0,0,0,0),vector4(0,0,0,0));
	}

	Point(int x, int y, double z, double w)
	{
	   init(x,y,z,w,vector4(0,0,0,0),vector4(0,0,0,0),vector4(0,0,0,0));
	}

	Point(int x, int y, double z, double w, vector4 color)
	{
	   init(x,y,z,w,color,vector4(0,0,0,0),vector4(0,0,0,0));	
	}

	Point(int x, int y, double z, double w, vector4 color, vector4 norm)
	{
	   init(x,y,z,w,color,norm,vector4(0,0,0,0));
	}

	Point(int x, int y, double z, double w, 
	      vector4 color, vector4 norm, vector4 light)
	{
	   init(x,y,z,w,color,norm,light);
	}

	bool operator<(const Point& rhs) { return x < rhs.x && y < rhs.y;}
	bool eq(const Point& p) {return x == p.x && y == p.y && z == p.z && w == p.w;}

	void set(int x, int y, double z, double w, 
	         vector4 color, vector4 norm, vector4 light) 
	{init(x,y,z,w,color, norm, light);}

	void clear() 
	{init(-1,-1,-1,-1,vector4(0,0,0,0),vector4(0,0,0,0),vector4(0,0,0,0));}

	vector4 getPoint() {return vector4(x,y,z,w);}

	int    x;
	int    y;
	double z;
	double w;
	vector4 color;
	vector4 normal;
	vector4 light;
};

struct less_key
{
bool operator() (const Point& struct1, const Point& struct2)
    {
        return (struct1.y < struct2.y);
    }
};

#endif
