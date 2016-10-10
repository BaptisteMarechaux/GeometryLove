#include "Math.h"

int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
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
	int p = l, q;
	do
	{
		hull.push_back(points[p]);
		q = (p + 1) % n;
		for (int i = 0; i < n; i++)
		{
			if (orientation(points[p], points[i], points[q]) == 2)
				q = i;
		}
		p = q;

	} while (p != l);  // While we don't come to first point

					   // Print Result
	for (int i = 0; i < hull.size(); i++)
		std::cout << "(" << hull[i].x << ", " << hull[i].y << ")\n";
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

	int smallerScalar = INFINITY;
	int smallerScalarProdIndex;

	do
	{
		
		for (int i = 0; i < n; i++)
		{
			//Trouver le produit scalaire
			float scalarProd = 0; //Fonction Produit scalaire

			smallerScalarProdIndex = i;
			//comparer avec les produits trouvés
			
			if (scalarProd == smallerScalarProdIndex)
			{
				smallerScalarProdIndex = 0;
			}
					
		}
		hull.push_back(points[smallerScalarProdIndex]);
		foundPoints++;
		
	} while (foundPoints < n);
	
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
