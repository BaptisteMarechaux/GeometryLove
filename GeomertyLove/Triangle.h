#pragma once
#include "Point.h"
#include <vector>

class Triangle
{
	private:

	public:
		Point p1, p2, p3;
		Triangle(Point p1, Point p2, Point p3);
		Triangle();
		friend std::ostream& operator<<(std::ostream& os, const Triangle &triangle);
		bool circumCircleContains(const Point &v);
};