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
	public:
		Shape(const Shape&);
		/**
		 * Create a shape consisting of the given polygons.
		 * The name can be anything, and is mostly usable for debugging.
		 */
		Shape(const std::string& name,const std::vector<Polygon>& poly);
		/**
		 * Create a shape containing only the given polygon.
		 * The name can be anything, and is mostly usable for debugging.
		 */
		Shape(const std::string& name,const Polygon& poly);
		/**
		 * Shortcut to create a shape of only one polygon,
		 * that polygon being solid.
		 * The name can be anything, and is mostly usable for debugging.
		 */
		Shape(const std::string& name,const std::vector<Vertex>& poly);
		/**
		 * Inside the Shape object is a vector of polygons.
		 * This function returns that vector.
		 */
		const std::vector<Polygon>& get_polys()const;
		/**
		 * Inside the Shape object is a vector of polygons.
		 * This function returns that vector.
		 * You may update this vector.
		 */
		std::vector<Polygon>& get_polys();
		bool operator==(const Shape& o) const;
		const std::string& get_name()const{return name;}
		/**
		 * IMPLEMENTATION DETAIL
		 * Give a short string representation of this object.
		 * This is used to get a string representation of this
		 * object in the Python-based test bench.
		 */
		std::string __repr__()const;
		/**
		 * IMPLEMENTATION DETAIL
		 * Print a lot of information about the polygon,
		 * usable for debugging.
		 */
		std::string dump() const;
		/**
		 * IMPLEMENTATION DETAIL
		 * Remove a specific polygon from the Shape.
		 * Removes the polygon whose index is idx
		 * in the vector returned by get_polys().
		 */
        void remove_polygon_by_idx(int idx);
        /**
         * Convenience function, equivalent to:
         * shape_union(*this,other)
         */
        Shape merge(const Shape& other) const;
        /**
         * Convenience function, equivalent to:
         * shape_union(*this,Shape("temp",other))
         */
        Shape merge(const Polygon& other) const;
	private:
		std::string name;
		std::vector<Polygon> polys;
	};
}
#ifndef SWIG

inline std::ostream& operator<<(std::ostream& os,const shapemerge2d::Shape& shape)
{
	os<<shape.__repr__();
	return os;
}
#endif

#endif
