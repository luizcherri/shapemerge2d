#include "shapemerge2d/vertex.h"
#include "shapemerge2d/vector.h"
#include <algorithm>

namespace shapemerge2d
{

	bool Vertex::operator==(const Vertex& o) const
	{
		return x==o.x && y==o.y;
	}
	bool Vertex::operator!=(const Vertex& o) const
	{
		return x!=o.x || y!=o.y;
	}
	Vertex::Vertex(int px,int py) : x(px),y(py)
	{
	}
	Vertex::Vertex() : x(0),y(0)
	{
	}
	Vector Vertex::operator-(const Vertex& o) const
	{
		return Vector(x-o.x,y-o.y);
	}
	Vertex origin;

}
