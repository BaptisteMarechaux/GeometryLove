#include "Point.h"
#include <vector>

int orientation(Point p, Point q, Point r);
std::vector<Point> jarvisMarch(std::vector<Point> points);
std::vector<Point> grahamScan(std::vector<Point> points);
std::vector<Point> divideAndConquer(std::vector<Point> points);
Point barycenter(std::vector<Point> points);