#pragma once

#ifndef Point_H
#define Point_H

#include <iostream>
#include <ostream>

class Point
{
	public:
		float x;
		float y;
		Point(float x, float y);
		Point();

		friend std::ostream& operator<<(std::ostream& os, const Point &point);
		bool operator<(const Point& point) const;
		bool operator == (const Point &point) const;
		bool operator != (const Point& point) const;
};

#endif