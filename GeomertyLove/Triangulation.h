#pragma once
#include "Point.h"
#include "Triangle.h"
#include <vector>
#include <list>

class Triangulation
{
	private:
		std::vector<Edge*> aretesExt;

		std::vector<Point> sommets;
		std::list<Edge>  aretes;
		std::list<Triangle> triangles;

		void connectEdgesToNewPoint(Point point, std::vector<Edge*> listeAreteTemp, bool triangleToRemoveFound, std::list<Triangle>::iterator triangleToRemove);

	public:
		Triangulation();

		void Reset();
		void Add(Point2D point2D);
		void Delete(Point point2D);
		bool checkVisibilityEdge(Edge &edge, Point &point);

		void Triangulation::GetVoronoi(std::vector<Point2D> &voronoi);


		//Debug
		const std::list<Edge> & GetAretes();
		const std::vector<Point>& GetSommets();
		const std::list<Triangle>& GetTriangles();

		void GetNormalsTriangle(std::vector<glm::vec2> &centers, std::vector<glm::vec2>&normals);
		void GetAllExtEdgesPoints(std::vector<Point2D> &returnPoints);
};

