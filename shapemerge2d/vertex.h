#ifndef SHAPE_MERGE_VERTEX_INCLUDED
#define SHAPE_MERGE_VERTEX_INCLUDED
#include <sstream>
#include <algorithm>
namespace shapemerge2d
{
	class Vector;
	/**
	 * A point in 2D space. Also, the vertex's position is
	 * it's identity. In this program,
	 * if two vertices have the same position, they are by
	 * definition the same vertex.
	 */
	struct Vertex
	{
		int x,y;
		Vertex(int x,int y);
		int get_x()const{return x;}
		int get_y()const{return y;}
		~Vertex(){}
		Vertex();
		int taxidist(const Vertex& o) const
		{
			return std::abs(x-o.x)+std::abs(y-o.y);
		}
		Vector operator-(const Vertex& o) const;
		bool operator==(const Vertex& o) const;
		bool operator!=(const Vertex& o) const;
		bool operator<(const Vertex& o) const
		{
			if (x<o.x) return true;
			if (x>o.x) return false;
			return y<o.y;
		}
		int __hash__() const
		{
			return x+17*y;
		}
		std::string __repr__() const
		{
			std::ostringstream os;
			os<<"Vertex("<<x<<","<<y<<")";
			return os.str();
		}
	};
//	extern Vertex origin;
}

#endif
