#include "Point.h"
#include <vector>

int orientation(Point p, Point q, Point r);
std::vector<Point> jarvisMarch(std::vector<Point> points);

Point makeVector(Point p1, Point p2); 

int normVector(Point p1, Point p2);