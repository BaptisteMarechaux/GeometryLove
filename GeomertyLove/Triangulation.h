#pragma once
#include "Point.h"
#include "Triangle.h"
#include <vector>

class Triangulation
{
	private:
		std::vector<Point> sommets;
		std::vector<Edge> aretes;
		std::vector<Triangle> triangles;
	public:
		Triangulation();
		void Add(Point point);
		void Delete(Point point);
};

