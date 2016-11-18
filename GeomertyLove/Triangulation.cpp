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

std::vector<Point2D> Triangulation::GetAllExtEdgesPoints()
{
	/*
	aretesExt.clear();
	for (int i = 0; i < aretes.size(); i++)
	{
		if ((aretes[i].t1 != NULL && aretes[i].t2 == NULL) || (aretes[i].t1 == NULL && aretes[i].t2 != NULL))
			aretesExt.push_back(aretes[i]);
	}
	*/
	
	std::vector<Point2D> returnPoints = std::vector<Point2D>();
	for(auto i=0;i<aretesExt.size();i++)
	{
		if ((aretesExt[i].t1 != NULL && aretesExt[i].t2 == NULL) || (aretesExt[i].t1 == NULL && aretesExt[i].t2 != NULL))
		{
			returnPoints.push_back(Point2D(aretesExt[i].p1.x, aretesExt[i].p1.y));
			returnPoints.push_back(Point2D(aretesExt[i].p2.x, aretesExt[i].p2.y));
		}
	}

	return returnPoints;
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
					//std::cout << "Pas colineaires" << std::endl;
					colineaire = false;
					break;
				}
			}
			//Point colineaire
			if (colineaire)
			{
				//std::cout << "Colineaires" << std::endl;

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
						//std::cout << "begin" << std::endl;
						aretes.push_back(Edge(point, sommets[indice]));
					}
					else
					{
						//std::cout << "end" << std::endl;
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
					//std::cout << "between " << ind1 << " and " << ind2 << std::endl;
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
			for (int i = 0; i < aretesExt.size(); i++)
			{
				if (checkVisibilityEdge(aretesExt[i], point))
					listeAreteTemp.push_back(aretesExt[i]);
			}
			//std::cout << "aretes vues " << listeAreteTemp.size() << std::endl;
		}

		//Cas B2
		int j = 0;
		while (listeAreteTemp.size() > 0)
		{
			Edge testEdge = listeAreteTemp[0];
		
			int triangleToRemove = -1;
			if (testEdge.t1 != NULL && testEdge.t1->circumCircleContains(point))
			{
				//std::cout << "Circum circle contains point for t1" << std::endl;
				for (int i = 0; i < triangles.size(); i++)
				{
					if (triangles[i] == *testEdge.t1)
						triangleToRemove = i;
				}
			}
			else if (testEdge.t2 != NULL && testEdge.t2->circumCircleContains(point))
			{
				//std::cout << "Circum circle contains point for t2" << std::endl;
				for (int i = 0; i < triangles.size(); i++)
				{
					if (triangles[i] == *testEdge.t2)
						triangleToRemove = i;
				}
			}

			if (triangleToRemove != -1)
			{
				
				if (testEdge == triangles[triangleToRemove].e1)
				{
					listeAreteTemp.push_back(triangles[triangleToRemove].e2);
					listeAreteTemp.push_back(triangles[triangleToRemove].e3);
				}
				if (testEdge == triangles[triangleToRemove].e2)
				{
					listeAreteTemp.push_back(triangles[triangleToRemove].e1);
					listeAreteTemp.push_back(triangles[triangleToRemove].e3);
				}
				if (testEdge == triangles[triangleToRemove].e3)
				{
					listeAreteTemp.push_back(triangles[triangleToRemove].e1);
					listeAreteTemp.push_back(triangles[triangleToRemove].e2);
				}

				triangles.erase(triangles.begin() + triangleToRemove);
				for (int i = 0; i < aretes.size(); i++)
				{
					if (aretes[i] == testEdge)
					{
						//std::cout << "suppress arete p1 " << aretes[i].p1 << " p2 " << aretes[i].p2 << std::endl;
						aretes.erase(aretes.begin() + i);
					}
				}
			}
			else
			{
				//std::cout << "No circum circle" << std::endl;
				aretes.push_back(Edge(testEdge.p1, point));
				aretes.push_back(Edge(testEdge.p2, point));

				triangles.push_back(Triangle(testEdge.p1, testEdge.p2, point));
				//triangles.push_back(Triangle(testEdge.p2, testEdge.p1, point));
			}
			listeAreteTemp.erase(listeAreteTemp.begin());
			j++;
			if (j > aretes.size() * 2)
			{
				std::cout << "error" << std::endl;
				break;
			}
		}
		sommets.push_back(point);
	}
		
	for (int i = 0; i < aretes.size(); i++)
	{
		aretes[i].t1 = NULL;
		aretes[i].t2 = NULL;

		for (int j = 0; j < triangles.size(); j++)
		{
			if (aretes[i] == triangles[j].e1 || aretes[i] == triangles[j].e2 || aretes[i] == triangles[j].e3)
			{
				Triangle *triangle = new Triangle(triangles[j]);
				if (aretes[i].t1 == NULL)
					aretes[i].t1 = triangle;
				else if (aretes[i].t2 == NULL && triangle != aretes[i].t1)
					aretes[i].t2 = triangle;

				if (aretes[i] == triangles[j].e1)
					aretes[i].n = triangles[j].e1.n;
				if (aretes[i] == triangles[j].e2)
					aretes[i].n = triangles[j].e2.n;
				if (aretes[i] == triangles[j].e3)
					aretes[i].n = triangles[j].e3.n;
			}
		}
	}

	aretesExt.clear();
	for (int i = 0; i < aretes.size(); i++)
	{
		if ((aretes[i].t1 != NULL && aretes[i].t2 == NULL) || (aretes[i].t1 == NULL && aretes[i].t2 != NULL))
			aretesExt.push_back(aretes[i]);
	}
	//for (int i = 0; i < sommets.size(); i++)
	//	std::cout << sommets[i] << std::endl;
	//std::cout << "Aretes exterieures " << aretesExt.size() << std::endl;
	//std::cout << std::endl;
}

void Triangulation::Delete(Point suppressedPoint)
{
	//Déroulé : 
	//Cas A : T ne contient aucun triangle/ les sommets sont colinéaires
		//
	//Cas B : T contient des triangles
		//Determiner quel triangle contient suppressedPoint et.. :
		//faire une liste LA1 d'arretes incidentes, liste LT de triangles incidents aux arretes de LA1 , liste LA2 d'arretes incidentes aux triangles mais non incidentes à supressedPoint
		//supprimer le tout sauf LA2
		//2 cas : 
		//Polygone fermé

		//Polygone ouvert
	auto incidentTrianglesInd = std::vector<int>();

	auto incidentTriangles = std::vector<Triangle>();
	auto incidentEdges = std::vector<Edge>();
	auto affectedEdges = std::vector<Edge>(); //Edges incidents aux triangles de "incidentTriangles" mais pas incidents à supressed point

	if (triangles.size() <= 0)
	{

	}
	else
	{
		//Determination des listes d'aretes et edges incidents
		for each(Triangle triangle in triangles)
		{
			if (suppressedPoint == triangle.p1 || suppressedPoint == triangle.p2 || suppressedPoint == triangle.p3)
			{
				incidentTriangles.push_back(triangle);
				
				if (suppressedPoint == triangle.e1.p1 || suppressedPoint == triangle.e1.p2)
					incidentEdges.push_back(triangle.e1);
				else
					affectedEdges.push_back(triangle.e1);

				if (suppressedPoint == triangle.e2.p1 || suppressedPoint == triangle.e2.p2)
					incidentEdges.push_back(triangle.e2);
				else
					incidentEdges.push_back(triangle.e2);

				if (suppressedPoint == triangle.e3.p1 || suppressedPoint == triangle.e3.p2)
					incidentEdges.push_back(triangle.e3);
				else
					affectedEdges.push_back(triangle.e3);

			}
		}
		//On supprime les triangles et edges incidents à suppressedPoint
		for (int i = 0; i < incidentTriangles.size(); i++)
		{
			for (int j = 0; j < triangles.size(); j++)
			{
				if(triangles[j] == incidentTriangles[i])
					triangles.erase(triangles.begin() + j);
			}
		}
		for (int i = 0; i < incidentEdges.size(); i++)
		{
			for (int j = 0; j < aretes.size(); j++)
			{
				if (aretes[j] == incidentEdges[i])
					aretes.erase(aretes.begin() + j);
			}
		}

		for (int i = 0; i < sommets.size(); i++)
		{
			if (sommets[i] == suppressedPoint)
				sommets.erase(sommets.begin() + i);
		}
	}
}

bool Triangulation::checkVisibilityEdge(Edge &edge, Point &point)
{
	int value = dotProduct(edge.n, makeVector(edge.p1, point));
	if (value < 0)
		return true;
	return false;
}

std::vector<Point2D> Triangulation::GetAllVisiblePoints(Point2D point)
{
	int nCuts = 0;
	std::vector<Point2D> extPoints = GetAllExtEdgesPoints();

	std::vector<Point2D> returnPoints = std::vector<Point2D>();
	for (auto i = 0; i < extPoints.size(); i++)
	{
		nCuts = 0;
		for (auto j = 0; j < aretesExt.size(); j++)
		{
			if (isCutting(glm::vec2(point.x, point.y), glm::vec2(extPoints[i].x, extPoints[i].y), glm::vec2(aretesExt[j].p1.x, aretesExt[j].p1.y), glm::vec2(aretesExt[j].p2.x, aretesExt[j].p2.y)))
			{
				nCuts++;
			}
		}
		if (nCuts <= 1)
		{
			returnPoints.push_back(extPoints[i]);
		}
		
	}
	return std::vector<Point2D>();
}

std::vector<Point2D> Triangulation::GetVoronoiPoints()
{
	auto centers = std::vector<Point2D>();
	auto internEdges = std::vector<Edge>();
	for each(Triangle triangle in triangles)
	{
		centers.push_back(triangle.getCircumCircleCenter());
	}
	//for each(Triangle triangle)
	return centers;
}
