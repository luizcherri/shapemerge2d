# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.40
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pyshapemerge2d', [dirname(__file__)])
        except ImportError:
            import _pyshapemerge2d
            return _pyshapemerge2d
        if fp is not None:
            try:
                _mod = imp.load_module('_pyshapemerge2d', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _pyshapemerge2d = swig_import_helper()
    del swig_import_helper
else:
    import _pyshapemerge2d
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class shapemerge2d_SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, shapemerge2d_SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, shapemerge2d_SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyshapemerge2d.delete_shapemerge2d_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_value(self)
    def incr(self, n = 1): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_incr(self, n)
    def decr(self, n = 1): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_decr(self, n)
    def distance(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_equal(self, *args)
    def copy(self): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_copy(self)
    def next(self): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_next(self)
    def __next__(self): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___next__(self)
    def previous(self): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_previous(self)
    def advance(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _pyshapemerge2d.shapemerge2d_SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
shapemerge2d_SwigPyIterator_swigregister = _pyshapemerge2d.shapemerge2d_SwigPyIterator_swigregister
shapemerge2d_SwigPyIterator_swigregister(shapemerge2d_SwigPyIterator)

class Vertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vertex, name)
    __swig_setmethods__["x"] = _pyshapemerge2d.Vertex_x_set
    __swig_getmethods__["x"] = _pyshapemerge2d.Vertex_x_get
    if _newclass:x = _swig_property(_pyshapemerge2d.Vertex_x_get, _pyshapemerge2d.Vertex_x_set)
    __swig_setmethods__["y"] = _pyshapemerge2d.Vertex_y_set
    __swig_getmethods__["y"] = _pyshapemerge2d.Vertex_y_get
    if _newclass:y = _swig_property(_pyshapemerge2d.Vertex_y_get, _pyshapemerge2d.Vertex_y_set)
    def get_x(self): return _pyshapemerge2d.Vertex_get_x(self)
    def get_y(self): return _pyshapemerge2d.Vertex_get_y(self)
    __swig_destroy__ = _pyshapemerge2d.delete_Vertex
    __del__ = lambda self : None;
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_Vertex(*args)
        try: self.this.append(this)
        except: self.this = this
    def taxidist(self, *args): return _pyshapemerge2d.Vertex_taxidist(self, *args)
    def __sub__(self, *args): return _pyshapemerge2d.Vertex___sub__(self, *args)
    def __eq__(self, *args): return _pyshapemerge2d.Vertex___eq__(self, *args)
    def __ne__(self, *args): return _pyshapemerge2d.Vertex___ne__(self, *args)
    def __lt__(self, *args): return _pyshapemerge2d.Vertex___lt__(self, *args)
    def __hash__(self): return _pyshapemerge2d.Vertex___hash__(self)
    def __repr__(self): return _pyshapemerge2d.Vertex___repr__(self)
Vertex_swigregister = _pyshapemerge2d.Vertex_swigregister
Vertex_swigregister(Vertex)

class Vector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector, name)
    __swig_setmethods__["x"] = _pyshapemerge2d.Vector_x_set
    __swig_getmethods__["x"] = _pyshapemerge2d.Vector_x_get
    if _newclass:x = _swig_property(_pyshapemerge2d.Vector_x_get, _pyshapemerge2d.Vector_x_set)
    __swig_setmethods__["y"] = _pyshapemerge2d.Vector_y_set
    __swig_getmethods__["y"] = _pyshapemerge2d.Vector_y_get
    if _newclass:y = _swig_property(_pyshapemerge2d.Vector_y_get, _pyshapemerge2d.Vector_y_set)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_Vector(*args)
        try: self.this.append(this)
        except: self.this = this
    def get_x(self): return _pyshapemerge2d.Vector_get_x(self)
    def get_y(self): return _pyshapemerge2d.Vector_get_y(self)
    __swig_destroy__ = _pyshapemerge2d.delete_Vector
    __del__ = lambda self : None;
    def __sub__(self, *args): return _pyshapemerge2d.Vector___sub__(self, *args)
    def __add__(self, *args): return _pyshapemerge2d.Vector___add__(self, *args)
    def __eq__(self, *args): return _pyshapemerge2d.Vector___eq__(self, *args)
    def scalarprod(self, *args): return _pyshapemerge2d.Vector_scalarprod(self, *args)
    def __repr__(self): return _pyshapemerge2d.Vector___repr__(self)
    def taxilength(self): return _pyshapemerge2d.Vector_taxilength(self)
    def maxnorm(self): return _pyshapemerge2d.Vector_maxnorm(self)
    def approxlength(self): return _pyshapemerge2d.Vector_approxlength(self)
Vector_swigregister = _pyshapemerge2d.Vector_swigregister
Vector_swigregister(Vector)

class DbgFloatVertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DbgFloatVertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DbgFloatVertex, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _pyshapemerge2d.new_DbgFloatVertex()
        try: self.this.append(this)
        except: self.this = this
    def get_x(self): return _pyshapemerge2d.DbgFloatVertex_get_x(self)
    def get_y(self): return _pyshapemerge2d.DbgFloatVertex_get_y(self)
    __swig_destroy__ = _pyshapemerge2d.delete_DbgFloatVertex
    __del__ = lambda self : None;
DbgFloatVertex_swigregister = _pyshapemerge2d.DbgFloatVertex_swigregister
DbgFloatVertex_swigregister(DbgFloatVertex)


def ratfloor(*args):
  return _pyshapemerge2d.ratfloor(*args)
ratfloor = _pyshapemerge2d.ratfloor
class Line2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Line2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Line2, name)
    def get_v1(self): return _pyshapemerge2d.Line2_get_v1(self)
    def get_v2(self): return _pyshapemerge2d.Line2_get_v2(self)
    def get_tags(self): return _pyshapemerge2d.Line2_get_tags(self)
    def __repr__(self): return _pyshapemerge2d.Line2___repr__(self)
    def reversed(self): return _pyshapemerge2d.Line2_reversed(self)
    def approx_dist(self, *args): return _pyshapemerge2d.Line2_approx_dist(self, *args)
    def approx_closest(self, *args): return _pyshapemerge2d.Line2_approx_closest(self, *args)
    def is_vertical(self): return _pyshapemerge2d.Line2_is_vertical(self)
    def get_k(self): return _pyshapemerge2d.Line2_get_k(self)
    def get_m(self): return _pyshapemerge2d.Line2_get_m(self)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_Line2(*args)
        try: self.this.append(this)
        except: self.this = this
    def __eq__(self, *args): return _pyshapemerge2d.Line2___eq__(self, *args)
    def __ne__(self, *args): return _pyshapemerge2d.Line2___ne__(self, *args)
    def __lt__(self, *args): return _pyshapemerge2d.Line2___lt__(self, *args)
    def add_tag(self, *args): return _pyshapemerge2d.Line2_add_tag(self, *args)
    def add_tags(self, *args): return _pyshapemerge2d.Line2_add_tags(self, *args)
    def __hash__(self): return _pyshapemerge2d.Line2___hash__(self)
    def taxilen(self): return _pyshapemerge2d.Line2_taxilen(self)
    def slow_all_vertices(self): return _pyshapemerge2d.Line2_slow_all_vertices(self)
    def side_of_extrapolated_line(self, *args): return _pyshapemerge2d.Line2_side_of_extrapolated_line(self, *args)
    def dbg_point_on_line(self, *args): return _pyshapemerge2d.Line2_dbg_point_on_line(self, *args)
    def dbg_fv1(self): return _pyshapemerge2d.Line2_dbg_fv1(self)
    def dbg_fv2(self): return _pyshapemerge2d.Line2_dbg_fv2(self)
    def get_x1_inexact(self): return _pyshapemerge2d.Line2_get_x1_inexact(self)
    def get_x2_inexact(self): return _pyshapemerge2d.Line2_get_x2_inexact(self)
    def get_y1_inexact(self): return _pyshapemerge2d.Line2_get_y1_inexact(self)
    def get_y2_inexact(self): return _pyshapemerge2d.Line2_get_y2_inexact(self)
    def split(self, *args): return _pyshapemerge2d.Line2_split(self, *args)
    def split3(self, *args): return _pyshapemerge2d.Line2_split3(self, *args)
    def is_on_line(self, *args): return _pyshapemerge2d.Line2_is_on_line(self, *args)
    def get_yrange(self, *args): return _pyshapemerge2d.Line2_get_yrange(self, *args)
    def intersection_points(self, *args): return _pyshapemerge2d.Line2_intersection_points(self, *args)
    def intersect(self, *args): return _pyshapemerge2d.Line2_intersect(self, *args)
    def intersect2(self, *args): return _pyshapemerge2d.Line2_intersect2(self, *args)
    __swig_destroy__ = _pyshapemerge2d.delete_Line2
    __del__ = lambda self : None;
Line2_swigregister = _pyshapemerge2d.Line2_swigregister
Line2_swigregister(Line2)

class Polygon(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Polygon, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Polygon, name)
    HOLE = _pyshapemerge2d.Polygon_HOLE
    SOLID = _pyshapemerge2d.Polygon_SOLID
    def set_shape(self, *args): return _pyshapemerge2d.Polygon_set_shape(self, *args)
    def __eq__(self, *args): return _pyshapemerge2d.Polygon___eq__(self, *args)
    def get_kind_str(self): return _pyshapemerge2d.Polygon_get_kind_str(self)
    def __repr__(self): return _pyshapemerge2d.Polygon___repr__(self)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_Polygon(*args)
        try: self.this.append(this)
        except: self.this = this
    def get_lines(self): return _pyshapemerge2d.Polygon_get_lines(self)
    def lower_left_vertex(self): return _pyshapemerge2d.Polygon_lower_left_vertex(self)
    def get_shape(self): return _pyshapemerge2d.Polygon_get_shape(self)
    def remove_loops(self): return _pyshapemerge2d.Polygon_remove_loops(self)
    def intersect_line(self, *args): return _pyshapemerge2d.Polygon_intersect_line(self, *args)
    def calc_area(self): return _pyshapemerge2d.Polygon_calc_area(self)
    def is_inside(self, *args): return _pyshapemerge2d.Polygon_is_inside(self, *args)
    def is_ccw(self): return _pyshapemerge2d.Polygon_is_ccw(self)
    def merge_straight_sections(self): return _pyshapemerge2d.Polygon_merge_straight_sections(self)
    __swig_destroy__ = _pyshapemerge2d.delete_Polygon
    __del__ = lambda self : None;
Polygon_swigregister = _pyshapemerge2d.Polygon_swigregister
Polygon_swigregister(Polygon)

class Shape(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Shape, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Shape, name)
    def get_this(self): return _pyshapemerge2d.Shape_get_this(self)
    def get_polys(self): return _pyshapemerge2d.Shape_get_polys(self)
    def __eq__(self, *args): return _pyshapemerge2d.Shape___eq__(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_Shape(*args)
        try: self.this.append(this)
        except: self.this = this
    def get_name(self): return _pyshapemerge2d.Shape_get_name(self)
    def __repr__(self): return _pyshapemerge2d.Shape___repr__(self)
    def dump(self): return _pyshapemerge2d.Shape_dump(self)
    __swig_destroy__ = _pyshapemerge2d.delete_Shape
    __del__ = lambda self : None;
Shape_swigregister = _pyshapemerge2d.Shape_swigregister
Shape_swigregister(Shape)

HOLE = _pyshapemerge2d.HOLE
SOLID = _pyshapemerge2d.SOLID
VOID = _pyshapemerge2d.VOID
UNCLASSIFIED = _pyshapemerge2d.UNCLASSIFIED
class Cell(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Cell, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Cell, name)
    def __init__(self): 
        this = _pyshapemerge2d.new_Cell()
        try: self.this.append(this)
        except: self.this = this
    def get_merged_poly(self): return _pyshapemerge2d.Cell_get_merged_poly(self)
    def get_leftmost(self): return _pyshapemerge2d.Cell_get_leftmost(self)
    def dbg_get_edges(self): return _pyshapemerge2d.Cell_dbg_get_edges(self)
    def get_shapes(self): return _pyshapemerge2d.Cell_get_shapes(self)
    def get_neighbors(self): return _pyshapemerge2d.Cell_get_neighbors(self)
    def get_classification(self): return _pyshapemerge2d.Cell_get_classification(self)
    def __repr__(self): return _pyshapemerge2d.Cell___repr__(self)
    __swig_destroy__ = _pyshapemerge2d.delete_Cell
    __del__ = lambda self : None;
Cell_swigregister = _pyshapemerge2d.Cell_swigregister
Cell_swigregister(Cell)

class Edge(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Edge, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Edge, name)
    def __repr__(self): return _pyshapemerge2d.Edge___repr__(self)
    def get_k_approx(self): return _pyshapemerge2d.Edge_get_k_approx(self)
    def get_is_vertical(self): return _pyshapemerge2d.Edge_get_is_vertical(self)
    def get_v1(self): return _pyshapemerge2d.Edge_get_v1(self)
    def get_v2(self): return _pyshapemerge2d.Edge_get_v2(self)
    def get_leftmost(self): return _pyshapemerge2d.Edge_get_leftmost(self)
    def get_cell(self, *args): return _pyshapemerge2d.Edge_get_cell(self, *args)
    def __init__(self): 
        this = _pyshapemerge2d.new_Edge()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pyshapemerge2d.delete_Edge
    __del__ = lambda self : None;
Edge_swigregister = _pyshapemerge2d.Edge_swigregister
Edge_swigregister(Edge)

class VertexPair(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VertexPair, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VertexPair, name)
    __repr__ = _swig_repr
    def get_v1(self): return _pyshapemerge2d.VertexPair_get_v1(self)
    def get_v2(self): return _pyshapemerge2d.VertexPair_get_v2(self)
    def smallest(self): return _pyshapemerge2d.VertexPair_smallest(self)
    def largest(self): return _pyshapemerge2d.VertexPair_largest(self)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_VertexPair(*args)
        try: self.this.append(this)
        except: self.this = this
    def __lt__(self, *args): return _pyshapemerge2d.VertexPair___lt__(self, *args)
    __swig_destroy__ = _pyshapemerge2d.delete_VertexPair
    __del__ = lambda self : None;
VertexPair_swigregister = _pyshapemerge2d.VertexPair_swigregister
VertexPair_swigregister(VertexPair)

class BooleanOpStrategy(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, BooleanOpStrategy, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, BooleanOpStrategy, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def evaluate(self, *args): return _pyshapemerge2d.BooleanOpStrategy_evaluate(self, *args)
    __swig_destroy__ = _pyshapemerge2d.delete_BooleanOpStrategy
    __del__ = lambda self : None;
BooleanOpStrategy_swigregister = _pyshapemerge2d.BooleanOpStrategy_swigregister
BooleanOpStrategy_swigregister(BooleanOpStrategy)

class BooleanOrStrategy(BooleanOpStrategy):
    __swig_setmethods__ = {}
    for _s in [BooleanOpStrategy]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, BooleanOrStrategy, name, value)
    __swig_getmethods__ = {}
    for _s in [BooleanOpStrategy]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, BooleanOrStrategy, name)
    __repr__ = _swig_repr
    def evaluate(self, *args): return _pyshapemerge2d.BooleanOrStrategy_evaluate(self, *args)
    def __init__(self): 
        this = _pyshapemerge2d.new_BooleanOrStrategy()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pyshapemerge2d.delete_BooleanOrStrategy
    __del__ = lambda self : None;
BooleanOrStrategy_swigregister = _pyshapemerge2d.BooleanOrStrategy_swigregister
BooleanOrStrategy_swigregister(BooleanOrStrategy)

class BooleanOp(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, BooleanOp, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, BooleanOp, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _pyshapemerge2d.new_BooleanOp()
        try: self.this.append(this)
        except: self.this = this
    def step1_add_lines(self, *args): return _pyshapemerge2d.BooleanOp_step1_add_lines(self, *args)
    def step2_intersect_lines(self): return _pyshapemerge2d.BooleanOp_step2_intersect_lines(self)
    def dbg_step2_get_split_lines(self): return _pyshapemerge2d.BooleanOp_dbg_step2_get_split_lines(self)
    def step3_create_edges(self): return _pyshapemerge2d.BooleanOp_step3_create_edges(self)
    def dbg_step3_and_4_get_edges(self): return _pyshapemerge2d.BooleanOp_dbg_step3_and_4_get_edges(self)
    def step4_eliminate_deadends(self): return _pyshapemerge2d.BooleanOp_step4_eliminate_deadends(self)
    def step5_create_cells(self): return _pyshapemerge2d.BooleanOp_step5_create_cells(self)
    def dbg_step5_sort_edges(self, *args): return _pyshapemerge2d.BooleanOp_dbg_step5_sort_edges(self, *args)
    def dbg_step5_get_cells(self): return _pyshapemerge2d.BooleanOp_dbg_step5_get_cells(self)
    def step6_determine_cell_cover(self): return _pyshapemerge2d.BooleanOp_step6_determine_cell_cover(self)
    def step7_classify_cells(self, *args): return _pyshapemerge2d.BooleanOp_step7_classify_cells(self, *args)
    def step8_merge_cells(self): return _pyshapemerge2d.BooleanOp_step8_merge_cells(self)
    def step9_calc_result(self): return _pyshapemerge2d.BooleanOp_step9_calc_result(self)
    def step9_get_result(self): return _pyshapemerge2d.BooleanOp_step9_get_result(self)
    __swig_destroy__ = _pyshapemerge2d.delete_BooleanOp
    __del__ = lambda self : None;
BooleanOp_swigregister = _pyshapemerge2d.BooleanOp_swigregister
BooleanOp_swigregister(BooleanOp)

class lvector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, lvector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, lvector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.lvector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.lvector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.lvector___bool__(self)
    def __len__(self): return _pyshapemerge2d.lvector___len__(self)
    def pop(self): return _pyshapemerge2d.lvector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.lvector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.lvector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.lvector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.lvector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.lvector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.lvector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.lvector_append(self, *args)
    def empty(self): return _pyshapemerge2d.lvector_empty(self)
    def size(self): return _pyshapemerge2d.lvector_size(self)
    def clear(self): return _pyshapemerge2d.lvector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.lvector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.lvector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.lvector_begin(self)
    def end(self): return _pyshapemerge2d.lvector_end(self)
    def rbegin(self): return _pyshapemerge2d.lvector_rbegin(self)
    def rend(self): return _pyshapemerge2d.lvector_rend(self)
    def pop_back(self): return _pyshapemerge2d.lvector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.lvector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_lvector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.lvector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.lvector_front(self)
    def back(self): return _pyshapemerge2d.lvector_back(self)
    def assign(self, *args): return _pyshapemerge2d.lvector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.lvector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.lvector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.lvector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.lvector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_lvector
    __del__ = lambda self : None;
lvector_swigregister = _pyshapemerge2d.lvector_swigregister
lvector_swigregister(lvector)

class pvector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, pvector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, pvector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.pvector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.pvector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.pvector___bool__(self)
    def __len__(self): return _pyshapemerge2d.pvector___len__(self)
    def pop(self): return _pyshapemerge2d.pvector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.pvector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.pvector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.pvector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.pvector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.pvector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.pvector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.pvector_append(self, *args)
    def empty(self): return _pyshapemerge2d.pvector_empty(self)
    def size(self): return _pyshapemerge2d.pvector_size(self)
    def clear(self): return _pyshapemerge2d.pvector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.pvector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.pvector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.pvector_begin(self)
    def end(self): return _pyshapemerge2d.pvector_end(self)
    def rbegin(self): return _pyshapemerge2d.pvector_rbegin(self)
    def rend(self): return _pyshapemerge2d.pvector_rend(self)
    def pop_back(self): return _pyshapemerge2d.pvector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.pvector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_pvector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.pvector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.pvector_front(self)
    def back(self): return _pyshapemerge2d.pvector_back(self)
    def assign(self, *args): return _pyshapemerge2d.pvector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.pvector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.pvector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.pvector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.pvector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_pvector
    __del__ = lambda self : None;
pvector_swigregister = _pyshapemerge2d.pvector_swigregister
pvector_swigregister(pvector)

class vvector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, vvector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, vvector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.vvector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.vvector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.vvector___bool__(self)
    def __len__(self): return _pyshapemerge2d.vvector___len__(self)
    def pop(self): return _pyshapemerge2d.vvector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.vvector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.vvector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.vvector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.vvector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.vvector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.vvector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.vvector_append(self, *args)
    def empty(self): return _pyshapemerge2d.vvector_empty(self)
    def size(self): return _pyshapemerge2d.vvector_size(self)
    def clear(self): return _pyshapemerge2d.vvector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.vvector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.vvector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.vvector_begin(self)
    def end(self): return _pyshapemerge2d.vvector_end(self)
    def rbegin(self): return _pyshapemerge2d.vvector_rbegin(self)
    def rend(self): return _pyshapemerge2d.vvector_rend(self)
    def pop_back(self): return _pyshapemerge2d.vvector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.vvector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_vvector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.vvector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.vvector_front(self)
    def back(self): return _pyshapemerge2d.vvector_back(self)
    def assign(self, *args): return _pyshapemerge2d.vvector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.vvector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.vvector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.vvector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.vvector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_vvector
    __del__ = lambda self : None;
vvector_swigregister = _pyshapemerge2d.vvector_swigregister
vvector_swigregister(vvector)

class evector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, evector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, evector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.evector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.evector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.evector___bool__(self)
    def __len__(self): return _pyshapemerge2d.evector___len__(self)
    def pop(self): return _pyshapemerge2d.evector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.evector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.evector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.evector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.evector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.evector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.evector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.evector_append(self, *args)
    def empty(self): return _pyshapemerge2d.evector_empty(self)
    def size(self): return _pyshapemerge2d.evector_size(self)
    def clear(self): return _pyshapemerge2d.evector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.evector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.evector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.evector_begin(self)
    def end(self): return _pyshapemerge2d.evector_end(self)
    def rbegin(self): return _pyshapemerge2d.evector_rbegin(self)
    def rend(self): return _pyshapemerge2d.evector_rend(self)
    def pop_back(self): return _pyshapemerge2d.evector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.evector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_evector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.evector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.evector_front(self)
    def back(self): return _pyshapemerge2d.evector_back(self)
    def assign(self, *args): return _pyshapemerge2d.evector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.evector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.evector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.evector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.evector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_evector
    __del__ = lambda self : None;
evector_swigregister = _pyshapemerge2d.evector_swigregister
evector_swigregister(evector)

class cvector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, cvector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, cvector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.cvector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.cvector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.cvector___bool__(self)
    def __len__(self): return _pyshapemerge2d.cvector___len__(self)
    def pop(self): return _pyshapemerge2d.cvector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.cvector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.cvector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.cvector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.cvector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.cvector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.cvector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.cvector_append(self, *args)
    def empty(self): return _pyshapemerge2d.cvector_empty(self)
    def size(self): return _pyshapemerge2d.cvector_size(self)
    def clear(self): return _pyshapemerge2d.cvector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.cvector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.cvector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.cvector_begin(self)
    def end(self): return _pyshapemerge2d.cvector_end(self)
    def rbegin(self): return _pyshapemerge2d.cvector_rbegin(self)
    def rend(self): return _pyshapemerge2d.cvector_rend(self)
    def pop_back(self): return _pyshapemerge2d.cvector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.cvector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_cvector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.cvector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.cvector_front(self)
    def back(self): return _pyshapemerge2d.cvector_back(self)
    def assign(self, *args): return _pyshapemerge2d.cvector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.cvector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.cvector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.cvector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.cvector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_cvector
    __del__ = lambda self : None;
cvector_swigregister = _pyshapemerge2d.cvector_swigregister
cvector_swigregister(cvector)

class svector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, svector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, svector, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyshapemerge2d.svector_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyshapemerge2d.svector___nonzero__(self)
    def __bool__(self): return _pyshapemerge2d.svector___bool__(self)
    def __len__(self): return _pyshapemerge2d.svector___len__(self)
    def pop(self): return _pyshapemerge2d.svector_pop(self)
    def __getslice__(self, *args): return _pyshapemerge2d.svector___getslice__(self, *args)
    def __setslice__(self, *args): return _pyshapemerge2d.svector___setslice__(self, *args)
    def __delslice__(self, *args): return _pyshapemerge2d.svector___delslice__(self, *args)
    def __delitem__(self, *args): return _pyshapemerge2d.svector___delitem__(self, *args)
    def __getitem__(self, *args): return _pyshapemerge2d.svector___getitem__(self, *args)
    def __setitem__(self, *args): return _pyshapemerge2d.svector___setitem__(self, *args)
    def append(self, *args): return _pyshapemerge2d.svector_append(self, *args)
    def empty(self): return _pyshapemerge2d.svector_empty(self)
    def size(self): return _pyshapemerge2d.svector_size(self)
    def clear(self): return _pyshapemerge2d.svector_clear(self)
    def swap(self, *args): return _pyshapemerge2d.svector_swap(self, *args)
    def get_allocator(self): return _pyshapemerge2d.svector_get_allocator(self)
    def begin(self): return _pyshapemerge2d.svector_begin(self)
    def end(self): return _pyshapemerge2d.svector_end(self)
    def rbegin(self): return _pyshapemerge2d.svector_rbegin(self)
    def rend(self): return _pyshapemerge2d.svector_rend(self)
    def pop_back(self): return _pyshapemerge2d.svector_pop_back(self)
    def erase(self, *args): return _pyshapemerge2d.svector_erase(self, *args)
    def __init__(self, *args): 
        this = _pyshapemerge2d.new_svector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyshapemerge2d.svector_push_back(self, *args)
    def front(self): return _pyshapemerge2d.svector_front(self)
    def back(self): return _pyshapemerge2d.svector_back(self)
    def assign(self, *args): return _pyshapemerge2d.svector_assign(self, *args)
    def resize(self, *args): return _pyshapemerge2d.svector_resize(self, *args)
    def insert(self, *args): return _pyshapemerge2d.svector_insert(self, *args)
    def reserve(self, *args): return _pyshapemerge2d.svector_reserve(self, *args)
    def capacity(self): return _pyshapemerge2d.svector_capacity(self)
    __swig_destroy__ = _pyshapemerge2d.delete_svector
    __del__ = lambda self : None;
svector_swigregister = _pyshapemerge2d.svector_swigregister
svector_swigregister(svector)



