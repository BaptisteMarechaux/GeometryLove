#include "Edge.h"

Edge::Edge(Point p1, Point p2) : p1(p1), p2(p2) {}
Edge::Edge() : p1(Point()), p2(Point()) {}

std::ostream& operator<<(std::ostream& os, const Edge &edge)
{
	return os << "Point 1 : " << edge.p1 << "Point 2 : " << edge.p2 << std::endl;
}

void Edge::operator=(const Edge &edge)
{
	p1 = edge.p1;
	p2 = edge.p2;
}

bool Edge::operator == (const Edge &edge) const
{
	if (p1 == edge.p1 && p2 == edge.p2)
		return true;
	else
		return false;
}