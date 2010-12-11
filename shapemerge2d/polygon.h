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
		bool operator==(const Polygon& o)const;
		std::string get_kind_str() const
		{
			switch(kind)
			{
			case HOLE: return "HOLE";
			case SOLID: return "SOLID";
			}
			throw std::runtime_error("Internal error in polygon");
		}
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
		    naive_area_calc();
		}
		const std::vector<Line2>& get_lines()const;
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
		std::vector<Line2> intersect_line(Line2 line) const;
		
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
        	printf("kind set to: %s\n",get_kind_str().c_str());
        }
	private:
		void naive_area_calc();
		Kind kind;
		const Shape* shape;
		int64_t doublearea;
		std::vector<Line2> lines;

	};


}












#endif
