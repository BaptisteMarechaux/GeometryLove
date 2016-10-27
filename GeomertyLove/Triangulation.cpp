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
			bool colineaire = false;
			for (int i = 0; i < sommets.size(); i++)
			{
				glm::vec2 vectorA = makeVector(sommets[i], point);
				glm::vec2 vectorB = makeVector(sommets[i], sommets[i + 1]);

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
			//Point 3.2 dans le poly
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
				sommets.push_back(point);
			}
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