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

std::vector<Point> jarvisMarch(std::vector<Point> points)
{
	int n = points.size();
	std::vector<Point> hull;
	if (n < 3) return hull;
	

	// Start from leftmost point
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i].x < points[l].x)
			l = i;

	// moving counterclockwise
	int current_index = l, next_index;
	do
	{
		hull.push_back(points[current_index]);
		next_index = (current_index + 1) % n;
		for (int i = 0; i < n; i++)
		{
			if (orientation(points[current_index], points[i], points[next_index]) == 2)
				next_index = i;
		}
		current_index = next_index;

	} while (current_index != l);  // While we don't come to first point

					   // Print Result
	for (int i = 0; i < hull.size(); i++)
		std::cout << "(" << hull[i].x << ", " << hull[i].y << ")\n";
	return hull;
}