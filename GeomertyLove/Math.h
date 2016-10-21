#include "Point.h"
#include "Triangle.h"
#include <vector>
#include "glm.hpp"

struct Triangulation
{
	std::vector<Point> sommets;
	std::vector<Edge> aretes;
	std::vector<Triangle> triangle;
};

int orientation(Point p, Point q, Point r);
std::vector<Point> jarvisMarch(std::vector<Point> points);
glm::vec2 makeVector(Point p1, Point p2);
int normVector(Point a, Point b);
int normVector(glm::vec2 vector);
double dotProduct(glm::vec2 vecA, glm::vec2 vecB);
double angle(glm::vec2 vector, Point p1, Point p2);
float sign(Point p1, Point p2, Point p3);

std::vector<Point> grahamScan(std::vector<Point> points);
std::vector<Point> divideAndConquer(std::vector<Point> points);
Point barycenter(std::vector<Point> points);

std::vector<Edge> add_triangulation(Triangulation &T, Point point);
std::vector<Point> findAndSuppressConcavePoints(std::vector<Point> points);
bool isConvexPoint(Point p, Point prevPoint, Point nextPoint);

void select_close(float x, float y, int& select, const std::vector<Point> &points);
