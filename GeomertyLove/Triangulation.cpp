#include "Triangulation.h"
#include "Math.h"
#include <algorithm>

Triangulation::Triangulation()
{
}

void Triangulation::Reset()
{
	aretes.clear();
	sommets.clear();
	triangles.clear();
}

const std::list<Edge>& Triangulation::GetAretes()
{
	return aretes;
}
const std::vector<Point>& Triangulation::GetSommets()
{
	return sommets;
}
const std::list<Triangle>& Triangulation::GetTriangles()
{
	return triangles;
}

void Triangulation::GetAllExtEdgesPoints(std::vector<Point2D> &returnPoints)
{
	returnPoints.clear();
	for(auto i=0;i<aretesExt.size();i++)
	{
		if ((aretesExt[i]->T1() != NULL && aretesExt[i]->T2() == NULL) || (aretesExt[i]->T1() == NULL && aretesExt[i]->T2() != NULL))
		{
			returnPoints.push_back(Point2D(aretesExt[i]->p1.x, aretesExt[i]->p1.y));
			returnPoints.push_back(Point2D(aretesExt[i]->p2.x, aretesExt[i]->p2.y));
		}
	}
}

void Triangulation::Add(Point2D point2D)
{
	Point point = (Point)point2D;
	bool find = false;
	find = std::find(sommets.begin(), sommets.end(), point) != sommets.end();
	if (find)
		return;

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
					colineaire = false;
					break;
				}
			}
			//Point colineaire
			if (colineaire)
			{
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
						aretes.push_back(Edge(point, sommets[indice]));
					else
						aretes.push_back(Edge(sommets[indice], point));
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
				}
			}
			//Point pas colineaire
			else
			{
				int oldSizeAretes = aretes.size();
				for (int i = 0; i < sommets.size(); i++)
				{
					Edge newEdge = Edge(sommets[i], point);
					aretes.push_back(newEdge);
					//sommets[i].e = new Edge(sommets[i], point);
				}

				std::list<Edge>::iterator it = aretes.begin();
				for (int i = 0; i < oldSizeAretes; i++)
				{
					Point s1 = it->p1;
					Point s2 = it->p2;

					Edge* e2 = &(*std::find(aretes.begin(), aretes.end(), Edge(s2, point)));
					Edge* e3 = &(*std::find(aretes.begin(), aretes.end(), Edge(point, s1)));

					triangles.push_back(Triangle(&(*it), e2, e3, s1, s2, point));
					triangles.back().SetEgdeRefs();

					++it;
				}
			}
			sommets.push_back(point);
		}
	}
	//Cas B (T contient des triangles)
	else
	{
		std::vector<Edge*> listeAreteTemp;

		bool triangleToRemoveFound = false;
		std::list<Triangle>::iterator triangleToRemove;
		std::list<Triangle>::iterator it;
		for (it = triangles.begin(); it != triangles.end(); ++it)
		{
			if (it->containsPoint(point))
			{
				triangleToRemove = it;
				triangleToRemoveFound = true;
				break;
			}
		}
		//Cas B1-1
		if (triangleToRemoveFound != false)
		{
			listeAreteTemp.push_back(triangleToRemove->E1());
			listeAreteTemp.push_back(triangleToRemove->E2());
			listeAreteTemp.push_back(triangleToRemove->E3());

			triangleToRemove->UnsetEgdeRefs();
			triangles.erase(triangleToRemove);
		}
		//Cas B1-2
		else
		{
			for (int i = 0; i < aretesExt.size(); i++)
			{
				if (checkVisibilityEdge(*aretesExt[i], point))
					listeAreteTemp.push_back(aretesExt[i]);
			}
		}

		//Cas B2
		int j = 0;
		while (listeAreteTemp.size() > 0)
		{
			Edge* testEdge = listeAreteTemp[0];
			triangleToRemoveFound = false;
			if (testEdge->T1() != NULL && testEdge->T1()->circumCircleContains(point))
			{
				for (it = triangles.begin(); it != triangles.end(); ++it)
				{
					if (&(*it) == testEdge->T1())
					{
						triangleToRemove = it;
						triangleToRemoveFound = true;
						break;
					}
						
				}
			}
			else if (testEdge->T2() != NULL && !testEdge->T2()->circumCircleContains(point))
			{
				for (it = triangles.begin(); it != triangles.end(); ++it)
				{
					if (&(*it) == testEdge->T2())
					{
						triangleToRemove = it;
						triangleToRemoveFound = true;
						break;
					}
				}
			}

			if (triangleToRemoveFound != false)
			{
				if (testEdge == triangleToRemove->E1())
				{
					listeAreteTemp.push_back(triangleToRemove->E2());
					listeAreteTemp.push_back(triangleToRemove->E3());
				}
				if (testEdge == triangleToRemove->E2())
				{
					listeAreteTemp.push_back(triangleToRemove->E1());
					listeAreteTemp.push_back(triangleToRemove->E3());
				}
				if (testEdge == triangleToRemove->E3())
				{
					listeAreteTemp.push_back(triangleToRemove->E1());
					listeAreteTemp.push_back(triangleToRemove->E2());
				}

				triangleToRemove->UnsetEgdeRefs();
				triangles.erase(triangleToRemove);

				aretes.erase(std::find(aretes.begin(), aretes.end(), *testEdge));
			}
			else
			{
				std::list<Edge>::iterator e2, e3;

				Edge newEdge3(testEdge->p1, point);
				e3 = std::find(aretes.begin(), aretes.end(), newEdge3);
				if (e3 == aretes.end())
				{
					aretes.push_back(newEdge3);
					e3 = --aretes.end();
				}
				Edge newEdge2(testEdge->p2, point);
				e2 = std::find(aretes.begin(), aretes.end(), newEdge2);
				if (e2 == aretes.end())
				{
					aretes.push_back(newEdge2);
					e2 = --aretes.end();
				}

				triangles.push_back(Triangle(testEdge, &(*e2), &(*e3), testEdge->p1, testEdge->p2, point));
				triangles.back().SetEgdeRefs();
			}
			listeAreteTemp.erase(listeAreteTemp.begin());
		}
		sommets.push_back(point);
	}
		
	aretesExt.clear();

	std::list<Edge>::iterator it = aretes.begin();
	for (; it != aretes.end(); ++it)
	{
		if ((it->T1() != NULL && it->T2() == NULL) || (it->T1() == NULL && it->T2() != NULL))
			aretesExt.push_back(&(*it));
	}
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
	std::vector<Triangle*> incidentTriangles;
	std::vector<Edge*> incidentEdges;
	std::vector<Edge*> affectedEdges; //Edges incidents aux triangles de "incidentTriangles" mais pas incidents à supressed point : La2

	if (triangles.size() <= 0)
	{

	}
	else
	{
		//Determination des listes d'aretes et edges incidents
		std::list<Triangle>::iterator it = triangles.begin();
		for (; it != triangles.end(); ++it)
		{
			if (suppressedPoint == it->P1() || suppressedPoint == it->P2() || suppressedPoint == it->P3())
			{
				incidentTriangles.push_back(&(*(it)));

				if (suppressedPoint == it->E1()->p1 || suppressedPoint == it->E1()->p2)
					incidentEdges.push_back(&(*(it)->E1()));
				else
					affectedEdges.push_back(&(*(it)->E1()));

				if (suppressedPoint == it->E2()->p1 || suppressedPoint == it->E2()->p2)
					incidentEdges.push_back(&(*(it)->E2()));
				else
					affectedEdges.push_back(&(*(it)->E2()));

				if (suppressedPoint == it->E3()->p1 || suppressedPoint == it->E3()->p2)
					incidentEdges.push_back(&(*(it)->E3()));
				else
					affectedEdges.push_back(&(*(it)->E3()));
			}
		}

		for (int i = 0; i < incidentTriangles.size(); i++)
		{
			std::list<Triangle>::iterator triangleToRemove;
			triangleToRemove = std::find(triangles.begin(), triangles.end(), *incidentTriangles[i]);
			if (triangleToRemove != triangles.end())
			{
				triangleToRemove->UnsetEgdeRefs();
				triangles.erase(triangleToRemove);
			}
		}

		for (int i = 0; i < incidentEdges.size(); i++)
		{
			std::list<Edge>::iterator edgeToRemove;
			edgeToRemove = std::find(aretes.begin(), aretes.end(), *incidentEdges[i]);
			if (edgeToRemove != aretes.end())
				aretes.erase(edgeToRemove);
		}

		for (int i = 0; i < sommets.size(); i++)
		{
			if (sommets[i] == suppressedPoint)
				sommets.erase(sommets.begin() + i);
		}

	
		
		bool isClosedPolygon = true; //On peut simplifier en enlevant cette variable
		int checked = 0;
		//On va calculer le nombre d'occurences de chaque point
		//Si un seul point n'apparait qu'une seule fois dans la liste, alors la liste est ouverte
		for (unsigned int i = 0; i < affectedEdges.size(); i++)
		{
			checked = 1;
			for (unsigned int j = 0; j < affectedEdges.size(); j++)
			{
				if (affectedEdges[i] != affectedEdges[j])
				{
					if (affectedEdges[i]->p1 == affectedEdges[j]->p1 || affectedEdges[i]->p1 == affectedEdges[j]->p2)
						checked++;
				}
				
			}
			if (checked < 2)
			{
				isClosedPolygon = false;
				break;
			}
			checked = 1;
			for (unsigned int j = 0; j < affectedEdges.size(); j++)
			{
				if (affectedEdges[i] != affectedEdges[j])
				{
					if (affectedEdges[i]->p2 == affectedEdges[j]->p2 || affectedEdges[i]->p2 == affectedEdges[j]->p1)
						checked++;
				}
			}
			if (checked < 2)
			{
				isClosedPolygon = false;
				break;
			}
		}
		
		std::cout << "Closed polygon " << isClosedPolygon << std::endl;

		if (isClosedPolygon) //Cas du polygone fermé
		{
			//Sort Edges in La2
			std::vector<Edge*> affectedEdgesSort;
			affectedEdgesSort.push_back(affectedEdges[0]);
			sortEdges(affectedEdges, affectedEdgesSort, 0);

			auto sIndex=0;
			
			Edge* edgeRef = affectedEdgesSort[0];
			int i = 0;
			while (affectedEdgesSort.size() > 3)
			{
				Point p1 = affectedEdgesSort[i]->p1;
				Point p2 = affectedEdgesSort[i + 1]->p1;

				Edge *third = new Edge(affectedEdgesSort[i + 1]->p2, affectedEdgesSort[i]->p1);
				Point p3 = third->p1;

				Triangle t = Triangle(affectedEdgesSort[i], affectedEdgesSort[i + 1], third, p1, p2, p3);
				std::vector<Point> pointsFromEdges;
				getAllPointsFromListOriented(pointsFromEdges, affectedEdgesSort);

				bool canAddTriangle = true;
				for (int i = 0; i < pointsFromEdges.size(); i++)
				{
					if(pointsFromEdges[i] != t.E1()->p1 && pointsFromEdges[i] != t.E1()->p2
						&& pointsFromEdges[i] != t.E2()->p1 && pointsFromEdges[i] != t.E2()->p2 
						&& t.circumCircleContains(pointsFromEdges[i]))
						canAddTriangle = false;
				}
		
				if (canAddTriangle)
				{
					triangles.push_back(Triangle(affectedEdgesSort[i], affectedEdgesSort[i + 1], third, p1, p2, p3));
					triangles.back().SetEgdeRefs();

					std::list<Edge>::iterator e;
					Edge newEdge(p3, p1);
					e = std::find(aretes.begin(), aretes.end(), newEdge);
					if (e == aretes.end())
					{
						aretes.push_back(newEdge);
						e = --aretes.end();
					}

					affectedEdgesSort.erase(std::find(affectedEdgesSort.begin(), affectedEdgesSort.end(), affectedEdgesSort[i]));
					affectedEdgesSort.erase(std::find(affectedEdgesSort.begin(), affectedEdgesSort.end(), affectedEdgesSort[i]));

					affectedEdgesSort.push_back(third);
					i = 0;
				}
				i++;
			}
			
			//verify if list already sort
			if (affectedEdgesSort[0]->p2 == affectedEdgesSort[1]->p1 &&  affectedEdgesSort[1]->p2 == affectedEdgesSort[2]->p1 && affectedEdgesSort[2]->p2 == affectedEdgesSort[0]->p1)
				std::cout << "already sort";
			else
			{
				affectedEdges = affectedEdgesSort;
				affectedEdgesSort.clear();
				affectedEdgesSort.push_back(affectedEdges[2]);
				sortEdges(affectedEdges, affectedEdgesSort, 2);
				std::cout << "not sort";
			}

			//il devrait rester 3 edges
			triangles.push_back(Triangle(affectedEdgesSort[0], affectedEdgesSort[1], affectedEdgesSort[2], affectedEdgesSort[0]->p1, affectedEdgesSort[1]->p1, affectedEdgesSort[2]->p1));
			triangles.back().SetEgdeRefs();
		}
		else
		{
			//Sort Edges in La2
			std::vector<Edge*> affectedEdgesSort;

			std::vector<Point> pointsFromEdges;
			int indexFirstEdge = -1;
			getAllUniquePointsFromList(pointsFromEdges, affectedEdges, indexFirstEdge);

			affectedEdgesSort.push_back(affectedEdges[indexFirstEdge]);
			sortEdges(affectedEdges, affectedEdgesSort, indexFirstEdge);

			//Polygone Ouvert
			std::cout << "Open Polygon" << std::endl;
			Edge* edgeRef = affectedEdgesSort[0];
			int i = 0;
			bool canAddTriangle = true;
			while(affectedEdgesSort.size() >= 2 && canAddTriangle)
			{
				
				Point p1 = affectedEdgesSort[i]->p1;
				Point p2 = affectedEdgesSort[i + 1]->p1;

				Edge *third = new Edge(affectedEdgesSort[i + 1]->p2, affectedEdgesSort[i]->p1);
				Point p3 = third->p1;

				Triangle t = Triangle(affectedEdgesSort[i], affectedEdgesSort[i + 1], third, p1, p2, p3);
				std::vector<Point> pointsFromEdges;
				getAllPointsFromListOriented(pointsFromEdges, affectedEdgesSort);

				bool canAddTriangle = true;
				for (int i = 0; i < pointsFromEdges.size(); i++)
				{
					if (pointsFromEdges[i] != p1 
						&& pointsFromEdges[i] != p2 
						&& pointsFromEdges[i] != p3
						&& t.circumCircleContains(pointsFromEdges[i]))
						canAddTriangle = false;
				}

				if (!isConvexPoint(p2, p1, p3))
				{
					canAddTriangle = false;
				}

				if (canAddTriangle)
				{
					triangles.push_back(Triangle(affectedEdgesSort[i], affectedEdgesSort[i + 1], third, p1, p2, p3));
					triangles.back().SetEgdeRefs();

					std::list<Edge>::iterator e;
					Edge newEdge(p3, p1);
					e = std::find(aretes.begin(), aretes.end(), newEdge);
					if (e == aretes.end())
					{
						aretes.push_back(newEdge);
						e = --aretes.end();
					}

					affectedEdgesSort.erase(std::find(affectedEdgesSort.begin(), affectedEdgesSort.end(), affectedEdgesSort[i]));
					affectedEdgesSort.erase(std::find(affectedEdgesSort.begin(), affectedEdgesSort.end(), affectedEdgesSort[i]));

					affectedEdgesSort.push_back(third);
					i = 0;
				}
				i++;
				if (i >= affectedEdgesSort.size())
					i = 0;

			}
			
		}
	}
	aretesExt.clear();

	std::list<Edge>::iterator it = aretes.begin();
	for (; it != aretes.end(); ++it)
	{
		if ((it->T1() != NULL && it->T2() == NULL) || (it->T1() == NULL && it->T2() != NULL))
			aretesExt.push_back(&(*it));
	}
	std::cout << "Suppression Finie" << std::endl;
	
}

bool Triangulation::checkVisibilityEdge(Edge &edge, Point &point)
{
	int value = dotProduct(edge.T1()->getNormal(&edge), makeVector(edge.p1, point));
	if (value < 0)
		return true;
	return false;
}

void Triangulation::getAllPointsFromListOriented(std::vector<Point>& pointsFind, std::vector<Edge*>& edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		pointsFind.push_back(edges[i]->p1);
	}
}

void Triangulation::getAllUniquePointsFromList(std::vector<Point>& pointsFind, std::vector<Edge*>& edges, int& indexEdge)
{
	for (int i = 0; i < edges.size(); i++)
	{
		if (std::find(pointsFind.begin(), pointsFind.end(), edges[i]->p1) == pointsFind.end())
			pointsFind.push_back(edges[i]->p1);
		else
			pointsFind.erase(std::find(pointsFind.begin(), pointsFind.end(), edges[i]->p1));
		if (std::find(pointsFind.begin(), pointsFind.end(), edges[i]->p2) == pointsFind.end())
			pointsFind.push_back(edges[i]->p2);
		else
			pointsFind.erase(std::find(pointsFind.begin(), pointsFind.end(), edges[i]->p2));
	}

	for (int i = 0; i < edges.size(); i++)
	{
		if (pointsFind[0] == edges[i]->p1)
			indexEdge = i;
		if (pointsFind[0] == edges[i]->p2)
		{
			indexEdge = i;
			Point s1 = edges[i]->p1;
			Point s2 = edges[i]->p2;
			edges[i]->p1 = s2;
			edges[i]->p2 = s1;
		}
	}
}

void Triangulation::sortEdges(std::vector<Edge*>& edgesToSort, std::vector<Edge*>& edgesSort, int index)
{
	Edge *edgeRef = edgesToSort[index];
	for (int i = 0; i < edgesToSort.size(); i++)
	{
		if (edgeRef != edgesToSort[i])
		{
			if (edgeRef->p2 == edgesToSort[i]->p1)
			{
				edgeRef = edgesToSort[i];
				edgesSort.push_back(edgesToSort[i]);
				i = - 1;
			}
			else if (edgeRef->p2 == edgesToSort[i]->p2)
			{

				edgeRef = edgesToSort[i];
				Point s1 = edgesToSort[i]->p1;
				Point s2 = edgesToSort[i]->p2;

				edgesToSort[i]->p1 = s2;
				edgesToSort[i]->p2 = s1;

				edgesSort.push_back(edgesToSort[i]);
				i = - 1;
			}
		}
		if (edgesSort.size() == edgesToSort.size())
			break;
	}
}

void Triangulation::GetVoronoi(std::vector<Point2D> &voronoi)
{
	voronoi.clear();

	std::list<Edge>::iterator it = aretes.begin();
	for (; it != aretes.end(); ++it)
	{
		//Cas d'une arête interne
		if (it->T1() != NULL && it->T2() != NULL)
		{
			Point2D center1 = it->T1()->getCircumCircleCenter();
			voronoi.push_back(center1);
			Point2D center2 = it->T2()->getCircumCircleCenter();
			voronoi.push_back(center2);
		}
		//Cas d'une arête externe
		else if (it->T1() != NULL && it->T2() == NULL || it->T1() == NULL && it->T2() != NULL)
		{
			Triangle *triangle;
			if (it->T1() != NULL)
				triangle = it->T1();
			else
				triangle = it->T2();

			//on push le premier point (centre triangle)
			Point2D center1;
			center1 = triangle->getCircumCircleCenter();
			voronoi.push_back(center1);

			//calcul normal
			glm::vec2 normal = triangle->getNormal(&(*it));
		
			Point2D center2;
			glm::vec2 normalized = glm::normalize(normal);
			normalized *= -3000;
			normalized = it->GetCenter() + normalized;
			center2 = Point2D(normalized.x, normalized.y);

			voronoi.push_back(center2);
		}
	}
}

void Triangulation::GetNormalsTriangle(std::vector<glm::vec2> &centers, std::vector<glm::vec2>&normals)
{
	std::list<Triangle>::iterator it = triangles.begin();
	for (; it != triangles.end(); ++it)
	{
		normals.push_back(it->N1());
		centers.push_back(it->E1()->GetCenter());

		normals.push_back(it->N2());
		centers.push_back(it->E2()->GetCenter());

		normals.push_back(it->N3());
		centers.push_back(it->E3()->GetCenter());
	}
}
