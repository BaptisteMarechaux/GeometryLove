#include "Triangulation.h"
#include "Math.h"

Triangulation::Triangulation()
{
}

void Triangulation::Reset()
{
	aretes.clear();
	sommets.clear();
	triangles.clear();
}

std::vector<Edge> Triangulation::GetAretes()
{
	return aretes;
}
std::vector<Point> Triangulation::GetSommets()
{
	return sommets;
}
std::vector<Triangle> Triangulation::GetTriangles()
{
	return triangles;
}

void Triangulation::Add(Point2D point2D)
{
	std::vector<Triangle> _triangles;
	std::vector<Edge> _edges;


	Point point = (Point)point2D;
	//Cas A (T ne contient pas de triangles)
	if (triangles.size() == 0)
	{
		if (sommets.size() == 0)
			sommets.push_back(point);

		else if (sommets.size() == 1)
		{
			sommets.push_back(point);
			aretes.push_back(Edge(sommets[0], point));
		}
		else
		{
			bool colineaire = true;
			//parcours de tout les sommets / si pas colineaire break
			for (int i = 0; i < sommets.size() - 1; i++)
			{
				glm::vec2 vectorA = makeVector(sommets[i], (Point)point);
				glm::vec2 vectorB = makeVector(sommets[i], sommets[i + 1]);

				if (vectorA.x * vectorB.y != vectorA.y * vectorB.x)
				{
					std::cout << "Pas colineaires" << std::endl;
					colineaire = false;
					break;
				}
			}
			//Point colineaire
			if (colineaire)
			{
				std::cout << "Colineaires" << std::endl;

				glm::vec2 longVector = makeVector(sommets[0], sommets[1]);
				int indice1 = 0, indice2 = 1;
				for (int i = 0; i < sommets.size(); i++)
				{
					for (int j = 0; j < sommets.size(); j++)
					{
						if (i != j && normVector(makeVector(sommets[i], sommets[j])) > normVector(longVector))
						{
							longVector = makeVector(sommets[i], sommets[j]);
							indice1 = i;
							indice2 = j;
						}
					}
				}

				int indice = 0;
				glm::vec2 shortVectorToPoint = makeVector(sommets[0], point);
				for (int i = 1; i < sommets.size(); i++)
				{
					if (normVector(makeVector(sommets[i], point)) < normVector(shortVectorToPoint))
					{
						shortVectorToPoint = makeVector(sommets[i], point);
						indice = i;
					}
				}
				//not on segment
				if (isOnLine(sommets[indice1], sommets[indice2], point) == 0)
				{
					if (sommets[indice].x > point.x || (sommets[indice].x == point.x && sommets[indice].y > point.y))
					{
						std::cout << "begin" << std::endl;
						aretes.push_back(Edge(point, sommets[indice]));
					}
					else
					{
						std::cout << "end" << std::endl;
						aretes.push_back(Edge(sommets[indice], point));
					}
				}
				//on segment
				else if(isOnLine(sommets[indice1], sommets[indice2], point) == 1)
				{
					int norm, ind1, ind2;
					if (indice == 0)
					{
						norm = normVector(makeVector(sommets[indice], sommets[1]));
						ind1 = indice; ind2 = 1;
					}
					else
					{
						norm = normVector(makeVector(sommets[indice], sommets[0]));
						ind1 = indice; ind2 = 0;
					}
					for (int i = 0; i < sommets.size(); i++)
					{
						for (int j = 0; j < sommets.size(); j++)
						{
							if (i != j && (i == indice || j == indice) && isOnLine(sommets[i], sommets[j], point) == 1)
							{
								if (norm > normVector(makeVector(sommets[i], sommets[j])))
								{
									norm = normVector(makeVector(sommets[i], sommets[j]));
									ind1 = i; ind2 = j;
								}
							}
						}
					}
					std::cout << "between " << ind1 << " and " << ind2 << std::endl;
				}
			}
			//Point pas colineaire
			else
			{
				std::vector<Edge> listeAreteTemp;
				for (int i = 0; i < sommets.size(); i++)
				{
					Edge newEdge = Edge(sommets[i], point);
					listeAreteTemp.push_back(newEdge);
					sommets[i].e = new Edge(sommets[i], point);
				}

				for (int i = 0; i < aretes.size(); i++)
				{
					Point s1 = aretes[i].p1;
					Point s2 = aretes[i].p2;

					//aretes[i].t1 = new Triangle(s1, s2, point);
					triangles.push_back(Triangle(s1, s2, point));
				}

				for (int i = 0; i < listeAreteTemp.size(); i++)
				{
					aretes.push_back(listeAreteTemp[i]);
				}
			}
			sommets.push_back(point);
		}
	}
	//Cas B (T contient des triangles)
	else
	{
		std::vector<Edge> listeAreteTemp;
		int triangleToremove = -1;
		for (int i = 0; i < triangles.size(); i++)
		{
			if (triangles[i].containsPoint(point))
			{
				triangleToremove = i;
				break;

			}
		}
		//Cas B1-1
		if (triangleToremove != -1)
		{
			listeAreteTemp.push_back(triangles[triangleToremove].e1);
			listeAreteTemp.push_back(triangles[triangleToremove].e2);
			listeAreteTemp.push_back(triangles[triangleToremove].e3);

			triangles.erase(triangles.begin() + triangleToremove);
		}
		//Cas B1-2
		else
		{
			for (int i = 0; i < triangles.size(); i++)
			{
				if (checkVisibilityEdge(triangles[i].e1, point))
					listeAreteTemp.push_back(triangles[i].e1);
				if (checkVisibilityEdge(triangles[i].e2, point))
					listeAreteTemp.push_back(triangles[i].e2);
				if (checkVisibilityEdge(triangles[i].e3, point))
					listeAreteTemp.push_back(triangles[i].e3);
			}
			std::cout << "aretes vues " << listeAreteTemp.size() << std::endl;
		}

		//Cas B2
		int j = 0;
		while (listeAreteTemp.size() > 0)
		{
			Edge testEdge = listeAreteTemp[j];
			if (testEdge.t1 || testEdge.t2)
			{
				int triangleToRemove = -1;
				if (testEdge.t1 != NULL && testEdge.t1->circumCircleContains(point))
				{
					std::cout << "Circum circle contains point for t1" << std::endl;
					for (int i = 0; i < triangles.size(); i++)
					{
						if (triangles[i] == *testEdge.t1)
							triangleToRemove = i;
					}
				}
				else if (testEdge.t2 != NULL && testEdge.t2->circumCircleContains(point))
				{
					std::cout << "Circum circle contains point for t2" << std::endl;
					for (int i = 0; i < triangles.size(); i++)
					{
						if (triangles[i] == *testEdge.t2)
							triangleToRemove = i;
					}
				}

				if (triangleToRemove != -1)
				{
					listeAreteTemp.push_back(triangles[triangleToRemove].e2);
					listeAreteTemp.push_back(triangles[triangleToRemove].e3);

					triangles.erase(triangles.begin() + triangleToRemove);
					for (int i = 0; i < aretes.size(); i++)
					{
						if (aretes[i] == testEdge)
							aretes.erase(aretes.begin() + i);
					}
				}
			}
			else
			{
				aretes.push_back(Edge(testEdge.p1, point));
				aretes.push_back(Edge(testEdge.p2, point));

				triangles.push_back(Triangle(testEdge.p1, testEdge.p2, point));
				//triangles.push_back(Triangle(testEdge.p2, testEdge.p1, point));
			}
			listeAreteTemp.erase(listeAreteTemp.begin());
			j++;
		}
		sommets.push_back(point);
	}

	//marquage des triangles d'incidence
	for (int i = 0; i < triangles.size(); i++)
	{
		Triangle *triangle = new Triangle(triangles[i]);
		if (triangles[i].e1.t1 == NULL)
			triangles[i].e1.t1 = triangle;
		else if (triangles[i].e1.t2 == NULL && triangles[i].e1.t1 != triangle)
			triangles[i].e1.t2 = triangle;

		if (triangles[i].e2.t1 == NULL)
			triangles[i].e2.t1 = triangle;
		else if (triangles[i].e2.t2 == NULL && triangles[i].e1.t1 != triangle)
			triangles[i].e2.t2 = triangle;

		if (triangles[i].e3.t1 == NULL)
			triangles[i].e3.t1 = triangle;
		else if (triangles[i].e3.t2 == NULL && triangles[i].e1.t1 != triangle)
			triangles[i].e3.t2 = triangle;
	}
	//return _edges;
}

void Delete(Point point2D)
{

}

bool Triangulation::checkVisibilityEdge(Edge &edge, Point &point)
{
	if ((edge.t1 == NULL && edge.t2 != NULL) || (edge.t1 != NULL && edge.t2 == NULL))
	{
		if (dotProduct(edge.n, makeVector(edge.p1, point)) < 0)
			return true;
	}
	return false;
}