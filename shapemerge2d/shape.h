#ifndef SHAPE_MERGE_SHAPE_INCLUDED
#define SHAPE_MERGE_SHAPE_INCLUDED
#include <vector>


namespace shapemerge2d
{


	/**
	 * A shape is simply a set of polygons.
	 * The does not have to be coherent. A shape
	 * may, for instance, be comprised of two non-overlapping
	 * rectangles, spaced some distance apart.
	 *
	 * The polygons of the shape may not overlap.
	 * They may touch at vertices, but may not touch
	 * along edges.
	 *
	 * The polygons can be of two varieties:
	 *  * Holes
	 *  * Solids
	 * Holes are only allowed inside solids.
	 * It is possible to have solids inside holes, as long
	 * as those holes are all themselves inside other solids.
	 */
	class Shape
	{
		std::string name;
		std::vector<Polygon> polys;
	public:
		const std::vector<Polygon>& get_polys();
		Shape(const std::string& name,const Polygon poly);
		Shape(const std::string& name,const std::vector<Polygon>& poly);
		Shape boolean_union(const Shape& o);

	private:
		std::vector<Polygon> polys;
	};




}

#endif
