#ifndef SHAPE_MERGE_POLYGON_INCLUDED
#define SHAPE_MERGE_POLYGON_INCLUDED
#include <vector>
#include "line2.h"
namespace shapemerge2d
{

	/**
	 * A polygon is defined by a set of points, and a type.
	 * The set of points define a space, by connecting lines
	 * between the points, in order. There is always an implicit
	 * line from the last point to the first point. The space
	 * inside all of these lines, is the area of the polygon.
	 * None of the lines my intersect each other. There are
	 * as many lines as there are vertices. Each vertex
	 * is connected to exactly two vertices.
	 * All vertices must be at unique positions (in fact, this
	 * is given by the definition of a vertex in this program).
	 *
	 * Summary:
	 * Polygons consist of a set of vertices connected by lines.
	 * The lines must not cross each other (except at the vertices of course).
	 * There is a line between the last vertex and the first vertex,
	 * guaranteeing that all polygons are always closed.
	 */
	class Shape;
	class Polygon
	{
	public:
		enum Kind
		{
			HOLE,
			SOLID
		};
		void set_shape(const Shape* shape)
		{
			this->shape=shape;
		}
		std::string get_kind_str() const
		{
			switch(kind)
			{
			case HOLE: return "HOLE";
			case SOLID: return "SOLID";
			}
			throw std::runtime_error("Internal error in polygon");
		}
		/**
		 * Caller must ensure that polygon edges do not cross each other.
		 * Edges may coincide, but not actually cross. (Consider a
		 * very very flat triangle, then segment each of the three edges
		 * into many small edges. Some of these small edges will coincide.)
		 */
		Polygon() : kind(SOLID),shape(NULL)
		{
		}
		Polygon(const std::vector<Vertex>& vs,Kind pkind=SOLID,Shape* pshape=NULL) :
			kind(pkind),
			shape(pshape)
		{
			if (vs.size()<3) throw std::runtime_error("A polygon must have at least 3 vertices");
			for(int i=0;i<(int)vs.size();++i)
			{
				int j=i+1;
				if (j==(int)vs.size()) j=0;
				lines.push_back(Line2(vs[i],vs[j]));
			}
		}
		const std::vector<Line2>& get_lines()const;
		/**
		 * Return the vertex with the lowest x coordinate, and if several have
		 * the same, the one with the lowest y coodinate.
		 */
		Vertex lower_left_vertex() const;
		const Shape* get_shape()const{return shape;}
	private:
		Kind kind;
		const Shape* shape;
		std::vector<Line2> lines;

	};


}












#endif
