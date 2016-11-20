#pragma once

#ifndef Triangle_H
#define Triangle_H

#include "Point.h"
#include <vector>
#include "Edge.h"
#include "glm.hpp"

class Triangle
{
	private:
		Edge *e1, *e2, *e3;
		Point p1, p2, p3;
		glm::vec2 n1, n2, n3;

	public:
		Edge* E1() { return e1; }
		Edge* E2() { return e2; }
		Edge* E3() { return e3; }

		glm::vec2 N1() { return n1; }
		glm::vec2 N2() { return n2; }
		glm::vec2 N3() { return n3; }

		const Point& P1() const { return p1; }
		const Point& P2() const { return p2; }
		const Point& P3() const { return p3; }

		Triangle(Edge* e1, Edge* e2, Edge* e3,
				 const Point& p1, const Point& p2, const Point& p3);

		glm::vec2 getNormal(Edge *e);

		void SetEgdeRefs();
		void UnsetEgdeRefs();

		friend std::ostream& operator<<(std::ostream& os, const Triangle &triangle);
		bool operator == (const Triangle &triangle) const;
		bool circumCircleContains(const Point &v) const;
		Point2D getCircumCircleCenter() const;
		bool containsPoint(const Point &point) const;
		bool isClockwise() const;
		void updateNormals();
};

#endif