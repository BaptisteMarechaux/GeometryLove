#pragma once
#include "Point.h"
#include <vector>
#include "Edge.h"

class Triangle
{
	private:

	public:
		Edge e1, e2, e3;
		Point p1, p2, p3;
		Triangle(Point p1, Point p2, Point p3);
		Triangle();
		friend std::ostream& operator<<(std::ostream& os, const Triangle &triangle);
		bool operator == (const Triangle &triangle) const;
		bool circumCircleContains(const Point &v);
		bool containsPoint(const Point &point);
};