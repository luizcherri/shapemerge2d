#ifndef BOOLEAN_OP_H_INCLUDED
#define BOOLEAN_OP_H_INCLUDED

#include <map>
#include "vertex.h"
#include "vector.h"
#include "polygon.h"
#include "shape.h"
#include "line2.h"

namespace shapemerge2d
{
#ifndef SWIG
struct SortAlongLine
{
	Vertex lstart;
	SortAlongLine(const Line2& pl) : lstart(pl.get_v1())
	{
	}
	bool operator()(const Vertex& a,const Vertex& b) const
	{
		int ta=lstart.taxidist(a);
		int tb=lstart.taxidist(b);
		return ta<tb;
	}
};
#endif
struct Edge;
struct Cell
{
#ifndef SWIG
	std::set<Edge*> edges;
	std::set<const Polygon*> cover;
	/**
	 * All neighbors, and for each neighbor, the set
	 * of edges by which that neighboring cell is adjacent
	 * to this cell.
	 */
	std::map<Cell*,std::set<Edge*> > neighbors;
#endif

	///Calculate the approximate center point of the
	///cell. This can only be used for heuristics,
	///since the center point is not really well-defined.
	///However, the calculation is at least deterministic.
	//Vertex approx_center() const;
	//Vertex leftmost_vertex() const;
	std::vector<Edge> dbg_get_edges();
};
struct Edge
{
#ifndef SWIG
	Vertex v1,v2;
	Rational line_k;
	bool line_is_vertical;
	std::set<const Polygon*> polys;
	Cell* side[2];
	Edge(Vertex v1,Vertex v2) : v1(v1),v2(v2)
	{
		side[0]=NULL;
		side[1]=NULL;
		line_is_vertical=0;
		line_k=0;
	}
#endif
	double get_k_approx()const{return line_k.numerator()/double(line_k.denominator());}
	bool get_is_vertical()const{return line_is_vertical;}
	Vertex get_v1()const{return v1;}
	Vertex get_v2()const{return v2;}
	Cell* get_cell(int s)
	{
		if (s<0 || s>=2) throw std::runtime_error("Bad side");
		return side[s];
	}
	Edge()
	{
		side[0]=NULL;
		side[1]=NULL;
		line_is_vertical=0;
		line_k=0;
	}
};
struct VertexPair
{
#ifndef SWIG
	Vertex v1,v2;
#endif
	Vertex get_v1()const{return v1;}
	Vertex get_v2()const{return v2;}
	VertexPair(const Vertex& pv1,const Vertex& pv2) : v1(pv1),v2(pv2){}
	bool operator<(const VertexPair& o) const
	{
		if (v1<o.v1) return true;
		if (o.v1<v1) return false;
		return v2<o.v2;
	}
};
class BooleanOp
{
public:
	BooleanOp();
	/**
	 * Extract all lines from the two shapes that we are to
	 * do a boolean operation upon. Each line is put into
	 * a vector, all_lines. Each polygon is put into the
	 * tagmap vector. The index into this vector is the 'tag'.
	 * The tag is stored with the lines, helping to keep
	 * track of which polygon the line belongs to.
	 * Note!
	 * FIXME: This tag-business isn't really used like it should, and
	 * might be unnecessary! Maybe fix sometime!
	 */

	void step1_add_lines(Shape* shape_a,Shape* shape_b);
	/**
	 * Find all intersections between the lines. This finds
	 * all vertexes in the graph of Vertexes and Edges that
	 * is to be created. vmap maps from vertex to lines,
	 * lmap from line to vertices. vertices is simply
	 * the set of all vertices.
	 */
	void step2_intersect_lines();
	std::vector<Line2> dbg_step2_get_split_lines();

	/**
	 * Create the 'edge' objects. Each edge has two
	 * vertices. It is built from all the lines which
	 * go between these vertices.
	 * pair2edge = map from pair of vertices to edge
	 * edgemap = map from vertex to all edges passing through it.
	 */
	void step3_create_edges();
	std::vector<Edge> dbg_step3_and_4_get_edges(); //also verify maps
	/**
	 * Eliminate vertices which are only visited by a single edge.
	 * Eliminate all edges which go to such a vertex.
	 * (Lines are kept, since we don't use them after this step anyway)
	 */
	void step4_eliminate_deadends(); //FIXME: Actually test this code.

	/**
	 * Go through all edges, and iterate around the holes
	 * and outlines of the graph. Follow the left side of an edge
	 * to the next vertex, there take the leftmost edge, and follow
	 * it along its left side to the next vertex, and so on, until
	 * you get back to the starting point. The thing you have encircled
	 * is a _cell_.
	 */
	void step5_create_cells();
	std::vector<Line2> dbg_step5_sort_edges(Vertex v,Line2 incoming,std::vector<Line2> sort,int side);
	std::vector<Cell> dbg_step5_get_cells();
	/**
	 * Determine which polygons cover each cell.
	 */
	void step6_determine_cell_cover();

	//Vertex leftmost_vertex() const;

public:
	//Shape addition();
private:
	void remove_edge(Line2* line);
private:

	std::vector<const Polygon*> tagmap;
	std::vector<Line2> all_lines; //also contains lines which were subsequently identified as dead-ends and removed

	//std::map<Vertex,std::set<Line2*> > vmap; //maybe remove? Not really needed outside of a few steps?
	std::map<Line2*,std::set<Vertex> > lmap; //maybe remove? Not really needed outside of a few steps?

	std::set<Vertex> vertices;
	std::map<VertexPair,Edge> pair2edge;
	std::map<Vertex,std::set<Edge*> > edgemap;

	/**
	 * Create a new Cell, and mark the 'side' side of the given edge as belonging
	 * to the new Cell.
	 * Then find the next edge of this cell, and mark it also, and move till a full
	 * circle has been created, marking all edges belonging to this Cell.
	 */
	void mark_side(int side,Edge* edge);

	/**
	 * Given 'inedge' as an incoming edge of vertex 'v', find the next edge leading
	 * out of v.
	 */
	Edge* get_out_edge_from_in_edge(Vertex v,Edge* inedge,int side);

	void recurse_determine_cover(Cell* curcell,std::set<const Polygon*> curpolys,std::set<Cell*>& visited);


};


//Shape boolean_add(const Shape& a,const Shape& b);


}







#endif
