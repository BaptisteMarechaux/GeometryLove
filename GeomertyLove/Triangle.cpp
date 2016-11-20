#include "Triangle.h"
#include "Math.h"

Triangle::Triangle(Edge* e1, Edge* e2, Edge* e3,
				   const Point& p1, const Point& p2, const Point& p3)
	: e1(e1), e2(e2), e3(e3),
	p1(p1), p2(p2), p3(p3)
{
	updateNormals();
}

void Triangle::SetEgdeRefs()
{
	e1->SetTriangle(this);
	e2->SetTriangle(this);
	e3->SetTriangle(this);
}

void Triangle::UnsetEgdeRefs()
{
	e1->UnsetTriangle(this);
	e2->UnsetTriangle(this);
	e3->UnsetTriangle(this);
}

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

bool Triangle::circumCircleContains(const Point &v) const
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

Point2D Triangle::getCircumCircleCenter() const
{
	float ab = (p1.x * p1.x) + (p1.y * p1.y);
	float cd = (p2.x * p2.x) + (p2.y * p2.y);
	float ef = (p3.x * p3.x) + (p3.y * p3.y);

	float circum_x = (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) / (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) + p3.x * (p2.y - p1.y)) / 2.f;
	float circum_y = (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) / (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) + p3.y * (p2.x - p1.x)) / 2.f;
	return Point2D(circum_x, circum_y);
}

bool Triangle::containsPoint(const Point &point) const
{
	if (dotProduct(makeVector(p1, point), n1) < 0.0f)
		return false;
	if (dotProduct(makeVector(p2, point), n2) < 0.0f)
		return false;
	if (dotProduct(makeVector(p3, point), n3) < 0.0f)
		return false;
	return true;
}

bool Triangle::isClockwise() const
{
	float val1 = (p2.x - p1.x) * (p2.y + p1.y);
	float val2 = (p3.x - p2.x) * (p3.y + p2.y);
	float val3 = (p1.x - p3.x) * (p1.y + p3.y);

	if (val1 + val2 + val3 > 0)
		return true;
	else
		return false;
}

glm::vec2 Triangle::getNormal(Edge *e)
{
	if (e1 == e)
		return n1;
	else if (e2 == e)
		return n2;
	else if (e3 == e)
		return n3;
}
void Triangle::updateNormals()
{
	if (isClockwise())
	{
		glm::vec2 vector = makeVector(p1, p2);
		n1 = glm::vec2(vector.y, -vector.x);

		vector = makeVector(p2, p3);
		n2 = glm::vec2(vector.y, -vector.x);

		vector = makeVector(p3, p1);
		n3 = glm::vec2(vector.y, -vector.x);
	}
	else
	{
		glm::vec2 vector = makeVector(p1, p2);
		n1 = glm::vec2(-vector.y, vector.x);

		vector = makeVector(p2, p3);
		n2 = glm::vec2(-vector.y, vector.x);

		vector = makeVector(p3, p1);
		n3 = glm::vec2(-vector.y, vector.x);
	}
}
