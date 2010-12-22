#ifndef SHAPE_MERGE_2D_INCLUDED
#define SHAPE_MERGE_2D_INCLUDED

#include "vertex.h"
#include "polygon.h"
#include "shape.h"
namespace shapemerge2d
{

	/**
	 * Boolean union operation between shapes a and b.
	 * Result will look like if
	 * both shapes were glued together on top of each
	 * other, and the seams made invisible, kind of.
	 *
	 * Each of the two shapes can consist of multiple
	 * polygons. The polygon within each shape are not
	 * allowed to overlap or intersect. If you aren't
	 * sure if they do, put them into separate shapes,
	 * and merge together to one shape, before merging
	 * the final result.
	 *
	 * @return a + b
	 */
	Shape shape_union(Shape& a,Shape& b);


}


#endif
