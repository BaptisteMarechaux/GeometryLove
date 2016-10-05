#include "Point.h"

Point::Point(float x, float y) : x(x), y(y){}
Point::Point() : x(0), y(0) {}
void Point::print()
{
	std::cout << "X : " << x << " Y : " << y << std::endl;
}