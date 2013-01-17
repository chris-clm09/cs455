class Point
{
public:
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	
	bool eq(const Point& p)
	{ return x == p.x && y == p.y; }
	
	int x;
	int y;
};
