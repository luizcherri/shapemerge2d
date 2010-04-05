#ifndef SHAPE_MERGE_2D_INCLUDED
#define SHAPE_MERGE_2D_INCLUDED

#include "polygon.h"
#include "shape.h"
#include "vertex.h"
namespace shapemerge2d
{

	/**
	 * Boolean union operation between shapes a and b.
	 * Result will look like if
	 * both shapes were glued together on top of each
	 * other, and the seams made invisible, kind of.
	 * @return a + b
	 */
	Shape shape_union(Shape& a,Shape& b);

	/**
	 * Boolean difference operation between shapes a and b.
	 * Will return a shape that looks like a b-shaped
	 * bite has been taken out of the a-shape.
	 * @return a - b.
	 */
	Shape shape_difference(Shape& a,Shape& b);

}


#endif
