#include "Point.h"

Point::Point(float x, float y) : x(x), y(y){}
Point::Point() : x(0), y(0) {}

std::ostream& operator<<(std::ostream& os, const Point &point)
{
	return os << "X : " << point.x << "Y : " << point.y;
}

bool Point::operator<(const Point& point) const
{
	if (x < point.x || (x == point.x && y < point.y))
		return true;
	return false;
}

bool Point::operator == (const Point &point) const
{
	if (x == point.x && y == point.y)
		return true;
	else
		return false;
}