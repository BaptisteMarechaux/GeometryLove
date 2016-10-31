#pragma once

#ifndef Point2D_H
#define Point2D_H

#include <iostream>
#include <ostream>

class Point2D
{
public:
	float x;
	float y;
	Point2D(float x, float y);
	Point2D();

	friend std::ostream& operator<<(std::ostream& os, const Point2D &point);
	bool operator<(const Point2D& point) const;
	bool operator == (const Point2D &point) const;
	bool operator != (const Point2D& point) const;
};

#endif