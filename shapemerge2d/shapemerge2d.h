#ifndef SHAPE_MERGE_2D_INCLUDED
#define SHAPE_MERGE_2D_INCLUDED

#include "vertex.h"
#include "line.h"
#include "polygon.h"
#include "shape.h"
/**
 * The entire shapemerge2d package has all its symbols declared
 * in the shapemerge2d namespace, to avoid clashes with other
 * packages.
 *
 */
namespace shapemerge2d
{
	/**
	 * \mainpage
	 *
	 * \section Introduction
	 * shapemerge2d is a software package which calculates the union
	 * of polygon shapes. That is, given two overlapping polygons,
	 * return a new larger polygon, which covers the two original polygons.
	 *
	 * Note that when merging two concave polygons, the merged polygon may
	 * have holes in it (not our fault, it's just the way the universe works).
	 * Such shapes (polygons with possible holes) are represented by Shape
	 * objects.
	 *
	 * Shape objects are made up of several Polygon objects. Each Polygon
	 * consists of multiple Line objects. Each Line has a start and end point,
	 * represented by one Vertex object each.
	 *
	 * \section Getting Started
	 * See this example: \ref example1.cpp
	 *
	 *
	 */

	/**
	 * Boolean union operation between shapes a and b.
	 * Result will look like if
	 * both shapes were glued together on top of each
	 * other, and the seams made invisible, kind of.
	 *
	 * Each of the two shapes can consist of multiple
	 * polygons. The polygons within each shape are not
	 * allowed to overlap or intersect. If you aren't
	 * sure if they do, put them into separate shapes,
	 * and merge together to one shape, before merging
	 * the final result.
	 *
	 * @return a union b
	 */
	Shape shape_union(const Shape& a,const Shape& b);

	/**
	 * Boolean union operation between polygons a and b.
	 * Result will look like if
	 * both polygons were glued together on top of each
	 * other, and the seams made invisible, kind of.
	 *
	 * Note that the output may have holes, if the polygons
	 * are concave. That is why the return type is not a
	 * polygon.
	 *
	 * @return a union b
	 */
	Shape shape_union(const Polygon& a,const Polygon& b);

    
	/**
	 * Boolean subtraction operation between polygons a and b.
	 * Result will look like if
	 * polygon b took a bite out of polygon a.
	 * If a and b do not overlap, the result will be a unmodified.
     * The area covered by b will not be covered by polygon in the output.
	 *
	 * Note that the output may have holes, b cuts a into two 
     * (or more) halves.
	 *
	 * @return a subtract b
	 */
	Shape shape_subtraction(const Polygon& a,const Polygon& b);

	/**
	 * Boolean subtraction operation between shapes a and b.
	 * Result will look like if
	 * shape b took a bite out of shape a.
	 * If a and b do not overlap, the result will be a unmodified.
     * The area covered by b will not be covered by shape in the output.
	 *
	 * Note that the output may have holes, b cuts a into two 
     * (or more) halves.
	 *
	 * @return a subtract b
	 */
	Shape shape_subtraction(const Shape& a,const Shape& b);

	/**
	* Tidy up a polygon. Any loops are eliminated, and new
	* polygons are created instead.
	*/
	Shape tidy_up_polygon(const Polygon& a);
	/**
	* Tidy up all polygons in a shape. Any loops are eliminated, and new
	* polygons are created instead.
	*/
	Shape tidy_up_polygons(const Shape& a);


}


#endif
