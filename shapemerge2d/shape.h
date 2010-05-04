#ifndef SHAPE_MERGE_SHAPE_INCLUDED
#define SHAPE_MERGE_SHAPE_INCLUDED
#include <vector>
#include <string>
#include "polygon.h"

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
		Shape(const Shape&);//noncopyable (Polygons have pointers to their shapes)
	public:
		Shape* get_this(){return this;}
		const std::vector<Polygon>& get_polys();
		Shape();
		bool operator==(const Shape& o) const;
		Shape(const std::string& name,const Polygon poly);
		Shape(const std::string& name,const std::vector<Polygon>& poly);
		/**
		 * Shortcut to create a shape of only one polygon,
		 * that polygon being solid.
		 */
		Shape(const std::string& name,const std::vector<Vertex>& poly);
		const std::string& get_name()const{return name;}
		std::string __repr__()const;
		std::string dump() const;
	private:
		std::vector<Polygon> polys;
	};




}

#endif
