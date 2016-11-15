#include "Point.h"
#include "Point2D.h"
#include "Triangle.h"
#include <vector>
#include "glm.hpp"



int orientation(Point p, Point q, Point r);

template <class TPoint>
glm::vec2 makeVector(TPoint p1, TPoint p2);

template <class TPoint>
int normVector(TPoint a, TPoint b);
int normVector(glm::vec2 vector);

double dotProduct(glm::vec2 vecA, glm::vec2 vecB);

template <class TPoint>
double angle(glm::vec2 vector, TPoint p1, TPoint p2);
template <class TPoint>
double fullAngle(glm::vec2 vector, TPoint p1, TPoint p2);

int isOnLine(Point &p1, Point &p2, Point &pointToCheck);

std::vector<Point2D> jarvisMarch(std::vector<Point2D> points);
std::vector<Point2D> grahamScan(std::vector<Point2D> points);
std::vector<Point2D> divideAndConquer(std::vector<Point2D> points);
Point2D barycenter(std::vector<Point2D> points);

std::vector<Point2D> findAndSuppressConcavePoints(std::vector<Point2D> points);
bool isConvexPoint(Point p, Point prevPoint, Point nextPoint);

void select_close(float x, float y, int& select, const std::vector<Point2D> &points);
