#include "shapemerge2d.h"
#include "boolean_op.h"
namespace shapemerge2d
{

Shape shape_union(Shape& shape_a,Shape& shape_b)
{
    BooleanOp bo;
    bo.step1_add_lines(&shape_a,&shape_b);
    bo.step2_intersect_lines();

    bo.step3_create_edges();
    bo.step4_eliminate_deadends();
    bo.step5_create_cells();
    bo.step5b_determine_cell_hierarchy();
    bo.step6_determine_cell_cover();
    BooleanOrStrategy strat;
    bo.step7_classify_cells(&strat);
    bo.step8_merge_cells();
    bo.step9_calc_result();
    Shape ret=*bo.step11_get_result();
    return ret;
}
}
