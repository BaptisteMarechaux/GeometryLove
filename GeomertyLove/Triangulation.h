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
		std::vector<Edge> aretesExt;

		Triangulation();
		void Reset();
		void Add(Point2D point2D);
		void Delete(Point point2D);
		std::vector<Edge> GetAretes();
		std::vector<Point> GetSommets();
		std::vector<Triangle> GetTriangles();
		std::vector<Point2D> GetAllExtEdgesPoints();
		std::vector<Point2D> GetAllVisiblePoints(Point2D point);
		std::vector<Point2D> GetVoronoiPoints();
		void GetNormalsTriangle(std::vector<glm::vec2> &centers, std::vector<glm::vec2>&normals);
};

