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
		glm::vec2  GetCenter() const;
		void SetTriangle(Triangle* triangle);
		void UnsetTriangle(Triangle* triangle);
		
		Triangle* T1() { return t1; }
		Triangle* T2() { return t2; }

		Point p1;
		Point p2;

	private:
		Triangle *t1;
		Triangle *t2;
};

#endif