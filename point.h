#ifndef clm_point
#define clm_point

class Point
{
public:
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	
	bool operator<(const Point& rhs)
	{
	   return x < rhs.x && y < rhs.y;
	}
	
	bool eq(const Point& p)
	{ return x == p.x && y == p.y; }
	
	int x;
	int y;
};

struct less_key
{
bool operator() (const Point& struct1, const Point& struct2)
    {
        return (struct1.y < struct2.y);
    }
};

#endif
