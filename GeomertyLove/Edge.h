#pragma once

#ifndef Edge_H
#define Edge_H

#include "Point.h"
#include "glm.hpp"
class Triangle;

class Edge
{
	public:
		Edge(Point p1, Point p2);
		Edge();
		friend std::ostream& operator<<(std::ostream& os, const Edge &edge);
		void operator=(const Edge &edge);
		bool operator == (const Edge &edge) const;
		void isVisible(Point &point);
		glm::vec2  GetCenter();

		Point p1;
		Point p2;
		Triangle *t1;
		Triangle *t2;
		glm::vec2 n;
};

#endif