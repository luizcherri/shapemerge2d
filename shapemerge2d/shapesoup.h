#ifndef SHAPE_MERGE_SHAPE_SOUP_INCLUDED
#define SHAPE_MERGE_SHAPE_SOUP_INCLUDED

#ifndef SHAPE_MERGE_INTERNAL
#error You should not include this file directly. Include shapemerge2d.h instead!
#endif

/**
 * A cell is a piece of space with no edges in it,
 * after the a-priori edge merging has happened, but
 * before meaningless inner edges have been removed.
 * The beauty of the cells is that they are rather
 * easy to compute - just find all intersections,
 * between polygons, and put vertices at the intersections,
 * then find all loops by picking a vertex, and then
 * keeping left for every intersection, until you get
 * back to that vertex. Those vertices visited comprise
 * a cell. There is a 1-to-1 mapping between nodes
 * and vertices.
 *
 * Two cells may share edges.
 *
 * Each cell has three or more edges.
 *
 * Cells can be both convex and concave (something
 * which goes for everything in this program).
 *
 * A set of cells, plus their edges and nodes,
 * constitutes a ShapeSoup. By removing
 * edges and cells and marking remaining cells
 * as holes/not holes, the resultat shape
 * is created.
 */
struct Cell
{
	/**
	 * The paint-fill algorithm is used to
	 * mark each cell with all shapes it is inside.
	 */
	std::vector<Shape*> shapes;
};
/**
 * An edge is a piece of the wall of a cell.
 * An edge can be adjacent to one or two cells.
 * An edge which is adjacent to only one cell
 * is an outer edge.
 */
struct Edge
{
	Cell* left_cell;
	Cell* right_cell;

	/**
	 * This edge was originally a boundary to
	 * all these polygons. To the left of this edge,
	 * was the polygon inside. To the right of this
	 * edge, was outside the given polygon.
	 */
	std::vector<Polygon*> left_polys;
	/**
	 * This edge was originally a boundary to
	 * all these polygons. To the right of this edge,
	 * was the polygon inside. To the left of this
	 * edge, was outside the given polygon.
	 */
	std::vector<Polygon*> right_polys;
	/**
	 * Start vertex of this edge
	 */
	Vertex2d v1;
	/**
	 * End vertex of this edge.
	 */
	Vertex2d v2;
};
/**
 * An EdgeEnd represents the joining of an
 * edge to a Node. Each edge has exactly
 * two EdgeEnds. One in each end.
 */
struct EdgeEnd
{
	Edge* edge;
	double angle;
};
/**
 * A Node represents an intersection of
 * two or more Edges. There is a one-to-one
 * mapping between vertices and nodes, for
 * the ShapeSoup.
 */
struct Node
{
	Vertex2d v;
	std::vector<EdgeEnd> edges;
};

/**
1: Find all shape intersections. These plus the original vertices are the
   nodes. Record which edges need to be split, and where the split will be.
2: Split edges according to recorded instructions.
3: Find all ShapeSoup edges. Note that edge direction has no meaning for
   ShapeSoup edges.
4: Find all cells. Each edge can belong to at most two cells.
   After finding a cell, mark the relevant half of its edges
   with the cell pointer. No other cell can use that half.
5: Use paint-fill algorithm to mark all cells according to which shapes
   they are inside. Note that every cell is completely inside or outside
   any of the original polygons, and thus any of the original shapes.
   Take each shape in turn. Find any edge which is marked as being
   the edge of this shape. Recursively mark all adjacent cells,
   stopping only when would otherwise have traversed across the
   boundary of the shape. Note that the shape area is still bounded
   by edges that either are identical to an original polygon edge of the
   shape, or which are the result of splitting original polygon edges. Note
   that the original polygon edge can have been merged with any number of
   polygons edges. It is still marked with the original poly as well.

*/
class ShapeSoup
{
	std::vector<Node> nodes;
	std::vector<Edge> edges;
	std::vector<Cell> cells;
public:

};

#endif
