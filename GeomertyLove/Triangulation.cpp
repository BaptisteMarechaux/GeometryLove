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

		for (int i = 0; i < incidentTriangles.size(); i++)
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

		bool isClosedPolygon = true;
		bool checked = true;
		for (unsigned int i = 0; i < affectedEdges.size();i++)
		{
			checked = false;
			for (unsigned int j = 0; j < affectedEdges.size(); j++)
			{
				if (!(affectedEdges[i] == affectedEdges[j]))
				{
					if (affectedEdges[i]->p1 == affectedEdges[j]->p1 || affectedEdges[i]->p1 == affectedEdges[j]->p2 || affectedEdges[i]->p2 == affectedEdges[j]->p1 || affectedEdges[i]->p2 == affectedEdges[j]->p2)
					{
						checked = true;
					}
				}
			}
			isClosedPolygon = checked;
			//if (checked == false)
			//{
			//	isClosedPolygon = false;
			//}
		}
		
		if (isClosedPolygon) //Cas du polygone fermé
		{
			auto sIndex=0;
			
			while (affectedEdges.size() > 3)
			{
				bool canAddTriangle=true;
				Edge *First, *Second, *Third; //Edges constituant un triangle à ajouter à la liste
				Point p1, p2, p3; //points consituant un triangle à ajouter à la liste
				for (auto i = 0; i < affectedEdges.size(); i++)
				{
					First = affectedEdges[i];
					p1 = affectedEdges[i]->p1;
					for (auto j = 0; j < affectedEdges.size(); j++)
					{
						if (affectedEdges[i]->p2 == affectedEdges[j]->p1) //on vérifie si on trouve un sommet convexe incident
						{
							Second = affectedEdges[j];
							Third = new Edge(affectedEdges[j]->p2, affectedEdges[i]->p1);
							p2 = affectedEdges[j]->p1;
							p3 = affectedEdges[j]->p2;
							break;
						}
						if (affectedEdges[i]->p2 == affectedEdges[j]->p2)
						{
							Second = affectedEdges[j];
							Third = new Edge(affectedEdges[j]->p1, affectedEdges[i]->p1);
							p2 = affectedEdges[j]->p2;
							p3 = affectedEdges[j]->p1;
							break;
						}
					}
					Triangle t = Triangle(First, Second, Third, p1, p2, p3);
					for (auto j = 0; j < affectedEdges.size(); j++)
					{
						if (
							affectedEdges[i]->p1 != t.P1() && affectedEdges[i]->p1 != t.P2() && affectedEdges[i]->p1 != t.P3() &&
							affectedEdges[i]->p2 != t.P1() && affectedEdges[i]->p2 != t.P2() && affectedEdges[i]->p2 != t.P3()
							)
						{
							if (t.circumCircleContains(affectedEdges[i]->p1) || t.circumCircleContains(affectedEdges[i]->p2))
							{
								canAddTriangle = false;
							}
						}
					}
				}
				if (canAddTriangle)
				{
					triangles.push_back(Triangle(First, Second, Third, p1, p2, p3));
					aretes.push_back(*First);
					aretes.push_back(*Second);
					aretes.push_back(*Third);
					sommets.push_back(p1);
					sommets.push_back(p2);
					sommets.push_back(p3);
					affectedEdges.erase(std::find(affectedEdges.begin(), affectedEdges.end(), First));
					affectedEdges.erase(std::find(affectedEdges.begin(), affectedEdges.end(), Second));
					affectedEdges.push_back(Third);
				}
			}
		
			//inversion du dernier edge
			Point s1 = affectedEdges[2]->p1;
			Point s2 = affectedEdges[2]->p2;

			affectedEdges[2]->p1 = s2;
			affectedEdges[2]->p2 = s1;

			//il devrait rester 3 edges
			triangles.push_back(Triangle(affectedEdges[0], affectedEdges[1], affectedEdges[2], affectedEdges[0]->p1, affectedEdges[1]->p1, affectedEdges[2]->p1));
			triangles.back().SetEgdeRefs();
		}
		else
		{
			
		}
	}
}

bool Triangulation::checkVisibilityEdge(Edge &edge, Point &point)
{
	int value = dotProduct(edge.T1()->getNormal(&edge), makeVector(edge.p1, point));
	if (value < 0)
		return true;
	return false;
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
