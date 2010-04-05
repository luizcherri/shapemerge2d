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
	class Polygon
	{
	public:
		enum Kind
		{
			HOLE,
			SOLID
		};
		/**
		 * If the given vertex sequence would give rise to a polygon
		 * with overlapping lines, extra vertices need to be introduced
		 * to normalize the polygon.
		 */
		Polygon(const std::vector<Vertex>& vs,Kind pkind,Shape* pshape=NULL) :
			vertices(vs),
			kind(pkind),
			shape(pshape)
		{
		}
		const std::vector<Line>& get_lines();
		/**
		 * Return the vertex with the lowest x coordinate, and if several have
		 * the same, the one with the lowest y coodinate.
		 */
		Vertex lower_left_vertex() const;
	private:
		/**
		 *All polygons should be counterclockwise.
		 */
		void assert_direction();
		Kind kind;
		Shape* shape;
		std::vector<Line2> vertices;

	};


}












#endif
