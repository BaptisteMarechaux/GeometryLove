#include "Math.h"

#include <algorithm>
#include "Edge.h"
#include <iterator>


#define _USE_MATH_DEFINES
#include <math.h>

int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

template <class TPoint>
glm::vec2 makeVector(TPoint p1, TPoint p2)
{
	int x = p2.x - p1.x;
	int y = p2.y - p1.y;
	return glm::vec2(x, y);
}

template <class TPoint>
int normVector(TPoint a, TPoint b)
{
	int x = pow(b.x - a.x, 2);
	int y = pow(b.y - a.y , 2);
	return sqrt(x + y);
}

int normVector(glm::vec2 vector)
{
	int x = pow(vector.x, 2);
	int y = pow(vector.y, 2);
	return sqrt(x + y);
}

double dotProduct(glm::vec2 vecA, glm::vec2 vecB)
{
	//return normVector(vecA) * normVector(vecB) * glm::cos (vecA, vecB);
	//return (p1.x * p2.x) + (p1.y * p2.y);
	return glm::dot(vecA, vecB);
}

template <class TPoint>
double angle(glm::vec2 vector, TPoint p1, TPoint p2)
{
	glm::vec2 vectorPoint = makeVector(p1, p2);
	int norm_vector_points = normVector(p1, p2);
	int norm_vector = normVector(vector);

	double dot = dotProduct(vectorPoint, vector);

	int determinant = vectorPoint.x * vector.y - vectorPoint.y * vector.x;
	double result = dot / (norm_vector_points * norm_vector);

	if (result < -1.0) result = -1.0;
	else if (result > 1.0) result = 1.0;
	double angle = acos(result);

	if (determinant == 0) // colinear
		return INFINITY;
	return angle * 180 / M_PI;
}

int isOnLine(Point &p1, Point &p2, Point &pointToCheck)
{
	float dot1 = dotProduct(makeVector(p1, p2), makeVector(p1, pointToCheck));
	float dot2 = dotProduct(makeVector(p1, p2), makeVector(p1, p2));

	if (dot1 < 0 || dot1 > dot2)
		return 0;
	else if (dot1 == 0)
		return 2;
	else if (dot1 == dot2)
		return 2;
	else if (dot1 > 0 && dot1 < dot2)
		return 1;
}


std::vector<Point2D> jarvisMarch(std::vector<Point2D> points)
{
	int n = points.size();
	std::vector<Point2D> hull;
	if (n < 3) return hull;

	//find start point - abs min / ord min
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i].x < points[l].x || (points[i].x == points[l].x && points[i].y < points[l].y))
			l = i;
	int current_index = l, next_index;

	int first_index = l;
	int i = first_index;
	int j = -1, inew = -1;

	glm::vec2 vectorDirector = makeVector(Point(0,0) , Point(0, 1));
	do
	{
		hull.push_back(points[i]);
		if (hull.size() > points.size())
		{
			std::cout << "ERROR stop function !" << std::endl;
			return hull;
		}
		if (i == 0)
			j = 1;
		else
			j = 0;

		double angle_min = angle(vectorDirector, points[i], points[j]);
		int lmax = normVector(points[i], points[j]);
		inew = j;
		for (j = inew + 1; j < n; j++)
		{
			if (j != i)
			{
				double current_angle = angle(vectorDirector, points[i], points[j]);
				if (angle_min > current_angle || (current_angle == angle_min && lmax < normVector(points[i], points[j])))
				{
					angle_min = current_angle;
					lmax = normVector(points[i], points[j]);
					inew = j;
				}
			}
		}
		vectorDirector = makeVector(points[i], points[inew]);
		i = inew;
		
	} while (i != first_index);

	return hull;
}


std::vector<Point2D> grahamScan(std::vector<Point2D> points)
{
	int n = points.size();
	int foundPoints = 0;
	std::vector<Point2D> hull;

	if (n < 3)
		return hull;

	Point2D bar = barycenter(points);

	int smallerDot = 999;
	int smallerDotProdIndex=0;
	float distanceToBar = 99999;

	do
	{
		smallerDot = 999;
		for (int i = 0; i < points.size(); i++)
		{
			//Trouver le produit scalaire
			glm::vec2 a = glm::vec2(1, 0); //x
			glm::vec2 b = glm::vec2(points[i].x-bar.x, points[i].y-bar.y); //segment to barycenter
			float dotProd = dotProduct( a, b );

			//comparer avec les produits trouvés
			if (dotProd <= smallerDot)
			{
				distanceToBar = glm::distance(glm::vec2(points[i].x, points[i].y), glm::vec2(bar.x, bar.y));
				smallerDotProdIndex = i;
				
			}
	
		}
		hull.push_back(points[smallerDotProdIndex]);
		points.erase(points.begin() + smallerDotProdIndex);
		foundPoints++;
		
	} while (foundPoints < n);

	hull = findAndSuppressConcavePoints(hull);
	
	return hull;
}

std::vector<Point2D> divideAndConquer(std::vector<Point2D> points)
{
	return std::vector<Point2D>();
}

Point2D barycenter(std::vector<Point2D> points)
{
	Point2D finalPoint(0, 0);
	int n = points.size();
	int x=0, y=0;

	for (int i = 0; i < n; i++)
	{
		x += points[i].x / n;
		y += points[i].y / n;
	}

	finalPoint = Point2D(x, y);

	return finalPoint;
}

//if (points.size() < 3)
//	return _edges;
//
//std::vector<Point> sortPoints = points;
//std::sort(sortPoints.begin(), sortPoints.end());
//
//for (int i = 0; i < sortPoints.size(); i++)
//	std::cout << sortPoints[i] << std::endl;

/*std::vector<Edge> subsets;
Triangle triangle = Triangle();
for (int i = 0; i < sortPoints.size(); i++)
{
if (sortPoints.size() % 2 != 0 && i + 3 == sortPoints.size())
{
triangle = Triangle(sortPoints[i], sortPoints[i + 1], sortPoints[i + 2]);
break;
}
else
{
subsets.push_back(Edge(sortPoints[i], sortPoints[i + 1]));
i++;
}
}

if (sortPoints.size() % 2 == 0)
{
for (int i = 0; i < subsets.size(); i++)
std::cout << "Subset " << i << " " << subsets[i] << std::endl;
}
else
{
for (int i = 0; i < subsets.size(); i++)
std::cout << "Subset " << i << " " << subsets[i] << std::endl;
std::cout << triangle << std::endl;
}*/

void select_close(float x, float y, int& select, const std::vector<Point2D> &points)
{
	double prevNorm = sqrt(pow(points[0].x - x, 2) + pow(points[0].y - y, 2));
	select = 0;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		double norm = sqrt(pow(points[i].x - x, 2) + pow(points[i].y - y, 2));
		if (norm < prevNorm)
		{
			prevNorm = norm;
			select = i;
		}
	}
	if (prevNorm > sqrt(25))
		select = -1;
}

std::vector<Point2D> findAndSuppressConcavePoints(std::vector<Point2D> points)
{
	Point2D initial;
	if (points.size() > 0)
		initial = Point2D(points[0].x, points[0].y);
	else
		return std::vector<Point2D>();
	Point2D pivot = Point2D(initial.x, initial.y);
	Point2D next = points.size() > 1 ? points[1] : initial;
	Point2D previous = points.size() > 2 ? points[2] : points.size() > 1 ? next : initial;
	bool go = false;
	do {
		if (isConvexPoint(pivot, previous, next))
		{
			previous = pivot;
			pivot = next;
			auto it = std::find(points.begin(), points.end(), pivot);
			auto a = std::prev(points.end());
			if (std::next(it, 1) != points.end())
			{
				next = *(std::next(it, 1));
			}
			else
			{
				next = points[0];
			}
			
			//Mettre a jour next et following
			go = true;
		}
		else
		{
			initial = previous;
			points.erase(std::remove(points.begin(), points.end(), pivot), points.end());
			next = previous;
			pivot = initial;
			go = false;
			//Mettre a jour next et following;
		}
	} while (pivot != initial || go == false);

	return points;
}

bool isConvexPoint(Point p, Point prevPoint, Point nextPoint)
{

	if (dotProduct(glm::vec2(prevPoint.x-p.x, prevPoint.y-p.y), glm::vec2(nextPoint.x-p.x, nextPoint.y-p.y)) >= 0)
		return true;
	return false;
}
