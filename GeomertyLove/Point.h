#pragma once

#ifndef Point_H
#define Point_H

#include <iostream>
#include <ostream>
#include "Point2D.h"
class Edge;

class Point
{
	public:
		float x;
		float y;
		Edge *e;
		Point(float x, float y);
		Point();
		Point(const Point2D& point);

		friend std::ostream& operator<<(std::ostream& os, const Point &point);
		bool operator<(const Point& point) const;
		bool operator == (const Point &point) const;
		bool operator != (const Point& point) const;
};

#endif