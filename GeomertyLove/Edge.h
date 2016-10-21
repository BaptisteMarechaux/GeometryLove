#pragma once

#ifndef Edge_H
#define Edge_H

#include "Point.h"

class Edge
{
	public:
		Edge(Point p1, Point p2);
		Edge();
		friend std::ostream& operator<<(std::ostream& os, const Edge &edge);
		void operator=(const Edge &edge);
		bool operator == (const Edge &edge) const;
		Point p1;
		Point p2;
};

#endif