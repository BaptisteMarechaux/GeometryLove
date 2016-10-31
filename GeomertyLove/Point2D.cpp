#include "Point2D.h"

Point2D::Point2D(float x, float y) : x(x), y(y) {}
Point2D::Point2D() : x(0), y(0) {}

std::ostream& operator<<(std::ostream& os, const Point2D &point)
{
	return os << "X : " << point.x << "Y : " << point.y;
}

bool Point2D::operator<(const Point2D& point) const
{
	if (x < point.x || (x == point.x && y < point.y))
		return true;
	return false;
}

bool Point2D::operator==(const Point2D & point) const
{
	return (x == point.x || y == point.y);
}

bool Point2D::operator!=(const Point2D & point) const
{
	return (x != point.x || y != point.y);
}
