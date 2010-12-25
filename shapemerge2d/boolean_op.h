#ifndef BOOLEAN_OP_H_INCLUDED
#define BOOLEAN_OP_H_INCLUDED

#include <map>
#include "vertex.h"
#include "vector.h"
#include "polygon.h"
#include "shape.h"
#include "line.h"

namespace shapemerge2d
{
#ifndef SWIG
struct SortAlongLine
{
	Vertex lstart;
	SortAlongLine(const Line& pl) : lstart(pl.get_v1())
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
enum BooleanUpResult
{
	HOLE,
	SOLID,
	VOID, //The stuff outside of all the polygons, beyond the borders of the polygon-populated world
	UNCLASSIFIED
};
struct Cell;
struct ComponentInfo
{
	Cell* leader;//encompasser of its component
	ComponentInfo()
	{
		leader=NULL;
	}
};
const char* bur_tostr(BooleanUpResult);
struct Edge;
struct Cell
{
#ifndef SWIG
	std::set<Edge*> edges;
	std::set<const Polygon*> cover;
	bool cover_determined;
	/**
	 * All neighbors, and for each neighbor, the set
	 * of edges by which that neighboring cell is adjacent
	 * to this cell.
	 */
	std::map<Cell*,std::set<Edge*> > neighbors;
	BooleanUpResult classification;
	int merged_poly;

	int component; //which connected component does this belong to?
	bool is_leader;
	int startside;
	Polygon* polygon;
	std::vector<Vertex> vertices;
	Cell* parent;
	std::vector<Cell*> children;
#endif

	Cell(){cover_determined=false;classification=UNCLASSIFIED;merged_poly=-1;polygon=0;component=-1;parent=NULL;is_leader=false;startside=-1;}
	///Calculate the approximate center point of the
	///cell. This can only be used for heuristics,
	///since the center point is not really well-defined.
	///However, the calculation is at least deterministic.
	//Vertex approx_center() const;
	//Vertex leftmost_vertex() const;

	/**
	 * Return true if this cell is an "outside" cell, one which
	 * has its startside on the outside of the area it envelopes.
	 * This is logically equivalent to it being a component leader.
	 * However, it is actually this property that is used to
	 * calculate the component leader and set the "is_leader" property.
	 */
	bool is_enveloping()const;

	int get_merged_poly(){return merged_poly;}
	Vertex get_leftmost();
	std::vector<Edge> dbg_get_edges();
	std::vector<std::string> get_shapes();
	std::vector<Cell> get_neighbors();
	std::string get_classification();
	std::string __repr__() const;
};
class SortCellsOnArea
{
public:
	bool operator()(const Cell* a,const Cell* b) const
	{
		return std::abs(a->polygon->naive_double_area())<std::abs(b->polygon->naive_double_area());
	}
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
	std::string __repr__() const;
	double get_k_approx()const{return line_k.numerator()/double(line_k.denominator());}
	bool get_is_vertical()const{return line_is_vertical;}
	Vertex get_v1()const{return v1;}
	Vertex get_v2()const{return v2;}
	Vertex get_leftmost()const;
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
	inline Vertex smallest()const{
		if (v1.x<v2.x) return v1;
		if (v1.x>v2.x) return v2;
		if (v1.y<v2.y) return v1;
		return v2;
	}
	inline Vertex largest()const{
		if (v1.x<v2.x) return v2;
		if (v1.x>v2.x) return v1;
		if (v1.y<v2.y) return v2;
		return v1;
	}
	VertexPair(const Vertex& pv1,const Vertex& pv2) : v1(pv1),v2(pv2){}
	bool operator<(const VertexPair& o) const
	{
		Vertex a=smallest();
		Vertex b=largest();
		Vertex oa=o.smallest();
		Vertex ob=o.largest();
		if (a<oa) return true;
		if (oa<a) return false;
		return b<ob;
	}
};
struct BooleanOpStrategy
{
	virtual void init(Shape* a,Shape* b)=0;
	virtual BooleanUpResult evaluate(const Cell& cell)=0;
};
struct BooleanOrStrategy : public BooleanOpStrategy
{
	Shape* shape_a;
	Shape* shape_b;
	BooleanOrStrategy(){shape_a=shape_b=0;}
	virtual void init(Shape* a,Shape* b);
	virtual BooleanUpResult evaluate(const Cell& cell);

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
	std::vector<Line> dbg_step2_get_split_lines();

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
	void step5_create_cells(); //TODO: Must be able to determine if a cell is within another cell (maybe separate step)
	std::vector<Line> dbg_step5_sort_edges(Vertex v,Line incoming,std::vector<Line> sort,int side);
	std::vector<Cell> dbg_step5_get_cells();
	
	void step5b_determine_cell_hierarchy();

	/**
	 * Determine which polygons cover each cell.
	 */
	void step6_determine_cell_cover();

	//Vertex leftmost_vertex() const;

	void step7_classify_cells(BooleanOpStrategy* strat);

	/**
	 * Merge adjacent cells with the same classification.
	 */
	void step8_merge_cells();


	/**
	 * Create resulting Shapes. This is almost the last step, only some
	 * cleanup of cells enclosed within other cells remains.
	 */
	void step9_calc_result();

	/**
	 * Merge cells where one cell is entirely inside another, without
	 * touching it - but only if the two cells have the same classification.
	 * I.e, if a hole is containing another hole, the inner hole can be removed.
	 * If a solid contains another solid, the inner one can be removed.
	 */
	///void step10_eliminate_enclosed_cells();
	

    /**
     * Just returned the computed result
     */
	Shape* step11_get_result();

public:
	//Shape addition();
private:
	void remove_edge(Line* line);
private:

	Shape* shape_a;
	Shape* shape_b;
	std::vector<const Polygon*> tagmap;
	std::vector<Line> all_lines; //also contains lines which were subsequently identified as dead-ends and removed
	std::vector<ComponentInfo> components;
	//std::map<Vertex,std::set<Line*> > vmap; //maybe remove? Not really needed outside of a few steps?
	std::map<Line*,std::set<Vertex> > lmap; //maybe remove? Not really needed outside of a few steps?

	std::set<Vertex> vertices;
	std::map<VertexPair,Edge> pair2edge;
	std::map<Vertex,std::set<Edge*> > edgemap;
	std::vector<Cell*> cells;
	int num_merged_polys;
	Shape* result;
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
