#include "Triangle.h"
#include "Math.h"

Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) 
{
	p1 = p1;
	p2 = p2;
	p3 = p3;

	e1 = Edge(p1, p2);
	e2 = Edge(p2, p3);
	e3 = Edge(p3, p1);

	glm::vec2 nTemp1, nTemp2;

	glm::vec2 vector = makeVector(p1, p2);
	nTemp1 = glm::vec2(-vector.y, vector.x);
	nTemp2 = glm::vec2(vector.y, -vector.x);
	
	if (dotProduct(nTemp1, makeVector(p1, p3)))
		n1 = nTemp1;
	else
		n1 = nTemp2;

	vector = makeVector(p2, p3);
	nTemp1 = glm::vec2(-vector.y, vector.x);
	nTemp2 = glm::vec2(vector.y, -vector.x);

	if (dotProduct(nTemp1, makeVector(p2, p1)))
		n2 = nTemp1;
	else
		n2 = nTemp2;


	vector = makeVector(p3, p1);
	nTemp1 = glm::vec2(-vector.y, vector.x);
	nTemp2 = glm::vec2(vector.y, -vector.x);

	if (dotProduct(nTemp1, makeVector(p3, p2)))
		n3 = nTemp1;
	else
		n3 = nTemp2;

	std::cout << n1.x << " " << n1.y << std::endl;
	std::cout << n2.x << " " << n2.y << std::endl;
	std::cout << n3.x << " " << n3.y << std::endl;
}

Triangle::Triangle() : p1(Point()), p2(Point()), p3(Point()), e1(Edge()), e2(Edge()), e3(Edge()) {}

std::ostream& operator<<(std::ostream& os, const Triangle &triangle)
{
	return os << "Triangle : " << "Point 1 : " << triangle.p1 << " Point 2 : " << triangle.p2 << " Point 3" << triangle.p3 << std::endl;
}

bool Triangle::operator == (const Triangle &triangle) const
{
	return	(p1 == triangle.p1 || p1 == triangle.p2 || p1 == triangle.p3) &&
		(p2 == triangle.p1 || p2 == triangle.p2 || p2 == triangle.p3) &&
		(p3 == triangle.p1 || p3 == triangle.p2 || p3 == triangle.p3);
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

bool Triangle::containsPoint(const Point &point)
{
	bool b1, b2, b3;

	b1 = sign(point, p1, p2) < 0.0f;
	b2 = sign(point, p2, p3) < 0.0f;
	b3 = sign(point, p3, p1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}
