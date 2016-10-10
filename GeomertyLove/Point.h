#pragma once

#include <iostream>
#include <ostream>

class Point
{
	private:
	
	public:
		float x;
		float y;
		Point(float x, float y);
		Point();

		friend std::ostream& operator<<(std::ostream& os, const Point &point);
		bool operator<(const Point& point) const;
};