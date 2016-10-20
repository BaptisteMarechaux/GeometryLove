#include "Point.h"
#include "Triangle.h"
#include <vector>

int orientation(Point p, Point q, Point r);
std::vector<Point> jarvisMarch(std::vector<Point> points);
Point makeVector(Point p1, Point p2); 
int normVector(Point p1, Point p2);
int normVector(Point vector);
int dotProduct(Point p1, Point p2);
double angle(Point vector, Point p1, Point p2);

std::vector<Point> grahamScan(std::vector<Point> points);
std::vector<Point> divideAndConquer(std::vector<Point> points);
Point barycenter(std::vector<Point> points);
std::vector<Triangle> triangulation(std::vector<Point> points);
std::vector<Point> findAndSuppressConcavePoints(std::vector<Point> points);
bool isConvexPoint(Point p, Point prevPoint, Point nextPoint);
