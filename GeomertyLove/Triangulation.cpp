#include "Triangulation.h"
#include "Math.h"

Triangulation::Triangulation()
{
}

void Triangulation::Add(Point point)
{
	std::vector<Triangle> _triangles;
	std::vector<Edge> _edges;

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
				glm::vec2 vectorA = makeVector(sommets[i], point);
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
					//T.sommets[i].e = newEdge;
				}

				for (int i = 0; i < aretes.size(); i++)
				{
					Point s1 = aretes[i].p1;
					Point s2 = aretes[i].p2;

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

		}
	}
	//return _edges;
}

void Delete(Point point)
{

}