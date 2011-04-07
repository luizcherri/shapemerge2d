#ifndef SHAPE_MERGE_POLYGON_INCLUDED
#define SHAPE_MERGE_POLYGON_INCLUDED
#include <vector>
#include "line.h"
namespace shapemerge2d
{

	class Shape;
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
		/**
		 * Polygons can be of two types - HOLE or SOLID.
		 * A SOLID polygon is simply a normal polygon.
		 * A HOLE is a polygon which is in fact a hole inside
		 * some other bigger polygon. HOLEs can never exist
		 * by themselves. Some polygon-operation libraries
		 * represent holes by clockwise polygons and SOLIDs
		 * by counter-clockwise polygons. Shapemerge2d is
		 * explicit about this instead.
		 */
		enum Kind
		{
			HOLE,
			SOLID
		};
		/**
		 * Each Polygon can belong to a Shape (it always should if
		 * it belongs to a Shape.). Adding a polygon to a Shape
		 * makes the Shape automatically call this function.
		 * So you don't need to call this yourself in normal situations.
		 */
		void set_shape(const Shape* shape)
		{
			this->shape=shape;
		}
		/**
		 * Compare two polygons. This is a slower operation than you
		 * might expect, since it cares about wether or not the polygons
		 * are equivalent, not if they have the exact same representation.
		 * What this means is that even if they don't have the same
		 * start vertex, as long as they have the same vertices, they
		 * are identical. They must also have the same kind (HOLE or SOLID)
		 * to be equal.
		 * NOTE: If one of the polygons is counter-clockwise, and the other
		 * is clockwise, they are considered different. This is slightly
		 * inconsistent, and might change in the future.
		 */
		bool operator==(const Polygon& o)const;
		/**
		 * Get a string-representation of the kind (HOLE/SOLID) of this polygon.
		 */
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
		 * Get a string representation of the polygon.
		 */
		std::string __repr__() const;
		/**
		 * Caller must ensure that polygon edges do not cross each other.
		 * Edges may coincide, but not actually cross. (Consider a
		 * very very flat triangle, then segment each of the three edges
		 * into many small edges. Some of these small edges will coincide.)
		 */
		Polygon() : kind(SOLID),shape(NULL)
		{
		}
		/**
		 *
		 */
		Polygon(const std::vector<Vertex>& vs,Kind pkind=SOLID,Shape* pshape=NULL);
		void reverse();
		const std::vector<Line>& get_lines()const;
		std::vector<Vertex> get_vertices()const;
		/**
		 * Return the vertex with the lowest x coordinate, and if several have
		 * the same, the one with the lowest y coodinate.
		 */
		Vertex lower_left_vertex() const;
		const Shape* get_shape()const{return shape;}
		
		/**
		 * Polygons should not double back on themselves and intersect. However,
		 * saying that two non-neighbor edges cannot intersect is too strict.
		 * Consider a long tapering V, ending in a single point. The two edges
		 * ending at the point will be touching for quite some distances. Now
		 * consider what happens if one of these edges are split into several 
		 * small edges. They will now be overlapping non-neighbor edges.
		 * 
		 * However, there should at least not be more than one area encompassed
		 * by the boundary of the polygon. There should be no 'loops', encircled
		 * by edges, where the edges touches or intersects at the beginning of the loop.
		 * (or rather, there should only be one such loop - the entire polygon).
		 * TODO: Rephrase the definition of a loop above, to make it understandable.
		 *		 
		 * This routine removes all loops, leaving one arbitrarily chosen loop, if
		 * multiple loops exist.		 
		 */
		Polygon remove_loops() const;

        /**
         * Return a set of lines, so that they are all contained within
         * the polygon, and part of the original line 'line'.
         * Only works on loop-free (non-self-overlapping), ccw polygons.
         */		
		std::vector<Line> intersect_line(Line line) const;
		
		/**
		 * Calculate area of polygon with a simple method.
		 * Only works for sane polygons. The returned
		 * area is only an approximation (although probably
		 * quite good).
		 */
		float calc_area() const;
		
		/**
		 * Returns a naive approximation of the area of the polygon,
		 * in units of half pixels! Divide this by two to get an approximation of
		 * real area.
		 */
		int64_t naive_double_area() const;
		/**
		 * Returns a naive approximation of the area of the polygon.
		 */
		int64_t naive_area() const;

        /** Is the given vertex inside this polygon */
        bool is_inside(Vertex v) const;		

		/**
		 *
		 * Return true if the polgyon has its edges in
		 * counter-clockwise order.
		 *
		 * Nominal polygons have their vertices in a
		 * counter clockwise order.		 
		 * This routine requires that the polygon
		 * contain no repeated vertices. Two adjacent
		 * vertices must not be identical.
		 */
		bool is_ccw()const;

		void merge_straight_sections();
        Kind get_kind() const
        {
            return kind;
        }
        void set_kind(Kind pkind)
        {
        	kind=pkind;
        	///printf("kind set to: %s\n",get_kind_str().c_str());
        }

        /**
         * Convenience function, equivalent to:
         * shape_union(Shape("temp",*this),other)
         */
        Shape merge(const Shape& other) const;
        /**
         * Convenience function, equivalent to:
         * shape_union(Shape("a",*this),Shape("b",other))
         */
        Shape merge(const Polygon& other) const;

        std::vector<Vertex> first_entrance(const Line& line);

	private:
		void naive_area_calc();
		Kind kind;
		const Shape* shape;
		int64_t doublearea;
		std::vector<Line> lines;
		long reserved;

	};


}
#ifndef SWIG
inline std::ostream& operator<<(std::ostream& os,const shapemerge2d::Polygon& x)
{
	os<<x.__repr__();
	return os;
}
#endif













#endif
