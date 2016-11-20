#include "Edge.h"

Edge::Edge(Point p1, Point p2) : p1(p1), p2(p2), t1(NULL), t2(NULL) {}
Edge::Edge() : p1(Point()), p2(Point()), t1(NULL), t2(NULL) {}

std::ostream& operator<<(std::ostream& os, const Edge &edge)
{
	return os << "Point 1 : " << edge.p1 << "Point 2 : " << edge.p2 << std::endl;
}

void Edge::operator=(const Edge &edge)
{
	p1 = edge.p1;
	p2 = edge.p2;
	t1 = edge.t1;
	t2 = edge.t2;
}

glm::vec2 Edge::GetCenter() const
{
	return glm::vec2((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

bool Edge::operator == (const Edge &edge) const
{
	if ((p1 == edge.p1 && p2 == edge.p2) || (p1 == edge.p2 && p2 == edge.p1))
		return true;
	else
		return false;
}

void Edge::SetTriangle(Triangle* triangle)
{
	if (t1 == NULL)
		t1 = triangle;
	else if (t2 == NULL && triangle != t1)
		t2 = triangle;
}

void Edge::UnsetTriangle(Triangle* triangle)
{
	if (t1 == triangle)
	{
		t1 = t2;
		t2 = NULL;
	}
	else if (t2 == triangle)
		t2 = NULL;
}