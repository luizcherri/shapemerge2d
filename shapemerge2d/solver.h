#ifndef SHAPE_MERGE_SOLVER_INCLUDED
#define SHAPE_MERGE_SOLVER_INCLUDED

#ifndef SHAPE_MERGE_INTERNAL
#error You should not include this file directly. Include shapemerge2d.h instead!
#endif

class Operation
{
public:
	enum Resolution
	{
		DELETE,
		KEEP_HOLE,
		KEEP_SOLID
	};
	virtual Resolution resolve(const Cell& cell)=0;
};

/*
4: For each cell, determine which shapes it belongs to
5a: Union rule: Is non-hole in any shape -> is non-hole
5b: Subtraction rule: Is non-hole in first shape, and is hole in second shape -> is non-hole
6: Mark the cells appropriately. Remove cells whose resolution is DELETE.

*/
class Solver
{
	Operation& operation;
	ShapeSoup& soup;
public:
	Solver(Operation& op,ShapeSoup& soup);
};
/*
7: Merge all cells which share edges. Can be done iteratively.
8: Sanity checking - free holes? Intersecting polys? (none of which should happen)
*/
class Merger
{
	ShapeSoup& soup;
public:

	Shape merged();
};


#endif

