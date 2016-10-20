#include "Math.h"
#include <math.h>
#include <algorithm>

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

int dotProduct(Point p1, Point p2)
{
	return (p1.x * p2.x) + (p1.y * p2.y);
}

double angle(Point vector, Point p1, Point p2)
{
	Point vectorPoint = makeVector(p1, p2);
	int norm_vector_points = normVector(p1, p2);
	int norm_vector = normVector(vector);

	return acos(dotProduct(vectorPoint, vector) / (norm_vector_points * norm_vector));
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
	int foundPoints = 0;
	std::vector<Point> hull;

	if (n < 3)
		return hull;

	Point bar = barycenter(points);

	int smallerDot = INFINITY;
	int smallerDotProdIndex;

	do
	{
		
		for (int i = 0; i < n; i++)
		{
			//Trouver le produit scalaire
			float dotProd = 0; //Fonction Produit scalaire

			smallerDotProdIndex = i;
			//comparer avec les produits trouvés
			
			if (dotProd == smallerDotProdIndex)
			{
				smallerDotProdIndex = 0;
			}
					
		}
		hull.push_back(points[smallerDotProdIndex]);
		foundPoints++;
		
	} while (foundPoints < n);

	hull = findAndSuppressConcavePoints(hull);
	
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

std::vector<Triangle> triangulation(std::vector<Point> points)
{
	std::vector<Point> sortPoints = points;
	std::vector<Triangle> result;

	std::sort(sortPoints.begin(), sortPoints.end());

	for (int i = 0; i < sortPoints.size(); i++)
		std::cout << sortPoints[i] << std::endl;

	return result;
}

std::vector<Point> findAndSuppressConcavePoints(std::vector<Point> points)
{
	Point initial;
	if (points.size > 0)
		initial = Point(points[0].x, points[0].y);
	else
		return std::vector<Point>();
	Point pivot = Point(initial.x, initial.y);
	Point next = points.size() > 1 ? points[1] : initial;
	Point previous = points.size() > 2 ? points[2] : points.size() > 1 ? next : initial;
	bool go = false;
	do {
		if (isConvexPoint(pivot, previous, next))
		{
			previous = pivot;
			pivot = next;
			//next = ???;
			//Mettre a jour next et following
			go = true;
		}
		else
		{
			points.erase(std::remove(points.begin(), points.end(), pivot), points.end());
			next = pivot;
			pivot = previous;
			go = false;
			//Mettre a jour next et following;
		}
	} while (pivot != initial && go == true);

	return std::vector<Point>();
}

bool isConvexPoint(Point p, Point prevPoint, Point nextPoint)
{
	return false;
}
