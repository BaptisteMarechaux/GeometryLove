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

glm::vec2 makeVector(Point p1, Point p2)
{
	int x = p2.x - p1.x;
	int y = p2.y - p1.y;
	return glm::vec2(x, y);
}

int normVector(Point a, Point b)
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

double angle(glm::vec2 vector, Point p1, Point p2)
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

std::vector<Edge> add_triangulation(Triangulation &T, Point point)
{
	std::vector<Triangle> _triangles;
	std::vector<Edge> _edges;


	if (T.triangle.size() == 0)
	{
		if (T.sommets.size() == 0)
			T.sommets.push_back(point);

		else if (T.sommets.size() == 1)
		{
			T.sommets.push_back(point);
			T.aretes.push_back(Edge(T.sommets[0], point));
		}
		else
		{
			bool colineaire = false;
			for (int i = 0; i < T.sommets.size(); i++)
			{
				glm::vec2 vectorA = makeVector(T.sommets[i], point);
				glm::vec2 vectorB = makeVector(T.sommets[i], T.sommets[i + 1]);
				
				if (vectorA.x * vectorB.y != vectorA.y * vectorB.x)
				{
					std::cout << "Pas colineaires" << std::endl;
					colineaire = false;
					break;
				}
				/*if (vectorA.x * vectorB.y == vectorA.y * vectorB.x)
				{
					std::cout << "Colineaires" << std::endl;
					std::cout << normVector(vectorA) << std::endl;
					std::cout << normVector(vectorB) << std::endl;

					if (dotProduct(vectorA, vectorB) < 0)
					{
						std::cout << "begin" << std::endl;
						T.aretes.push_back(Edge(point, T.sommets[i]));
					}
					else
					{
						if (normVector(vectorA) > normVector(vectorB))
						{
							std::cout << "end" << std::endl;
							T.aretes.push_back(Edge(T.sommets[i + 1], point));
						}
						else
						{
							std::cout << "between" << std::endl;
							T.aretes[0] = Edge(T.sommets[i], point);
							T.aretes.push_back(Edge(point, T.sommets[i + 1]));
						}
					}*/

				}
				if (colineaire)
				{
				}
				else
				{
					std::vector<Edge> listeAreteTemp;
					for (int i = 0; i < T.sommets.size(); i++)
					{
						listeAreteTemp.push_back(Edge(T.sommets[i], point));
					}
					T.sommets.push_back(point);
				}
				
			}
			
		}
	return _edges;
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

void select_close(float x, float y, int& select, const std::vector<Point> &points)
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

std::vector<Point> findAndSuppressConcavePoints(std::vector<Point> points)
{
	Point initial;
	if (points.size() > 0)
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
