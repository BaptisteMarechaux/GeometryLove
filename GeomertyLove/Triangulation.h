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
		bool checkVisibilityEdge(Edge &edge, Point &point);
	public:
		Triangulation();
		void Add(Point2D point2D);
		void Delete(Point2D point2D);
		std::vector<Edge> GetAretes();
		std::vector<Point> GetSommets();
		std::vector<Triangle> GetTriangles();
};

