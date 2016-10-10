#include "Math.h"
#include <math.h>

int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

Point makeVector(Point p1, Point p2)
{
	int x = p2.x - p1.x;
	int y = p2.y - p1.y;
	return Point(x, y);
}

int normVector(Point p1, Point p2)
{
	int x = pow(p2.x - p1.x, 2);
	int y = pow(p2.y - p1.y , 2);
	return sqrt(x + y);
}

int normVector(Point vector)
{
	int x = pow(vector.x, 2);
	int y = pow(vector.y, 2);
	return sqrt(x + y);
}

double angle(Point vector, Point p1, Point p2)
{
	Point vectorPoint = makeVector(p1, p2);
	int norm_vector_points = normVector(p1, p2);
	int norm_vector = normVector(vector);

	return acos((vectorPoint.x * vector.x + vectorPoint.y * vector.y) / (norm_vector_points * norm_vector));
}

std::vector<Point> jarvisMarch(std::vector<Point> points)
{
	int n = points.size();
	std::vector<Point> hull;
	if (n < 3) return hull;

	//find start point - abs min / ord min
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i].x < points[l].x || (points[i].x == points[l].x && points[i].y < points[l].y))
			l = i;
	int current_index = l, next_index;

	//do while not return to start point
	do
	{
		hull.push_back(points[current_index]);
		next_index = (current_index + 1) % n;
		for (int i = 0; i < n; i++)
		{
			//counterclockwise
			if (orientation(points[current_index], points[i], points[next_index]) == 2)
				next_index = i;
		}
		current_index = next_index;

	} while (current_index != l); 
	return hull;
}

std::vector<Point> grahamScan(std::vector<Point> points)
{
	int n = points.size();
	std::vector<Point> hull;

	if (n < 3)
		return hull;

	return hull;
}

std::vector<Point> divideAndConquer(std::vector<Point> points)
{
	return std::vector<Point>();
}

Point barycenter(std::vector<Point> points)
{
	Point finalPoint(0, 0);
	int n = points.size();
	int x, y;

	for (int i = 0; i < n; i++)
	{
		x += points[i].x / n;
		y += points[i].y / n;
	}

	finalPoint = Point(x, y);

	return finalPoint;
}
