#include "Triangle.h"

Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}
Triangle::Triangle() : p1(Point()), p2(Point()), p3(Point()) {}

std::ostream& operator<<(std::ostream& os, const Triangle &triangle)
{
	return os << "Point 1 : " << triangle.p1 << "Point 2 : " << triangle.p2 << "Point 3" << triangle.p3 << std::endl;
}

bool Triangle::circumCircleContains(const Point &v)
{
	float ab = (p1.x * p1.x) + (p1.y * p1.y);
	float cd = (p2.x * p2.x) + (p2.y * p2.y);
	float ef = (p3.x * p3.x) + (p3.y * p3.y);

	float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
	float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;
	float circum_radius = sqrtf(((p1.x - circum_x) * (p1.x - circum_x)) + ((p1.y - circum_y) * (p1.y - circum_y)));

	float dist = sqrtf(((v.x - circum_x) * (v.x - circum_x)) + ((v.y - circum_y) * (v.y - circum_y)));
	return dist <= circum_radius;
}