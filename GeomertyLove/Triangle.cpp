#include "Triangle.h"
#include "Math.h"

Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) 
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	e1 = Edge(p1, p2);
	e2 = Edge(p2, p3);
	e3 = Edge(p3, p1);

	updateNormals();
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

Point2D Triangle::getCircumCircleCenter()
{
	float ab = (p1.x * p1.x) + (p1.y * p1.y);
	float cd = (p2.x * p2.x) + (p2.y * p2.y);
	float ef = (p3.x * p3.x) + (p3.y * p3.y);

	float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
	float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;
	return Point2D(circum_x, circum_y);
}

bool Triangle::containsPoint(const Point &point)
{
	if (dotProduct(makeVector(p1, point), e1.n) < 0.0f)
		return false;
	if (dotProduct(makeVector(p2, point), e2.n) < 0.0f)
		return false;
	if (dotProduct(makeVector(p3, point), e3.n) < 0.0f)
		return false;
	return true;
}

bool Triangle::isClockwise()
{
	float val1 = (p2.x - p1.x) * (p2.y + p1.y);
	float val2 = (p3.x - p2.x) * (p3.y + p2.y);
	float val3 = (p1.x - p3.x) * (p1.y + p3.y);

	if (val1 + val2 + val3 > 0)
		return true;
	else
		return false;
}

void Triangle::updateNormals()
{
	if (isClockwise())
	{
		glm::vec2 vector = makeVector(p1, p2);
		e1.n = glm::vec2(vector.y, -vector.x);

		vector = makeVector(p2, p3);
		e2.n = glm::vec2(vector.y, -vector.x);

		vector = makeVector(p3, p1);
		e3.n = glm::vec2(vector.y, -vector.x);
	}
	else
	{
		glm::vec2 vector = makeVector(p1, p2);
		e1.n = glm::vec2(-vector.y, vector.x);

		vector = makeVector(p2, p3);
		e2.n = glm::vec2(-vector.y, vector.x);

		vector = makeVector(p3, p1);
		e3.n = glm::vec2(-vector.y, vector.x);
	}
}
