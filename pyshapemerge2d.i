%module pyshapemerge2d
#ifndef SwigPyIterator
/*Work around SWIG-bug 1863647*/
#define SwigPyIterator shapemerge2d_SwigPyIterator
#endif

%{
#include <exception>
#include <string>
#include "shapemerge2d/vertex.h" 
#include "shapemerge2d/vector.h" 
#include "shapemerge2d/line.h" 
#include "shapemerge2d/polygon.h" 
#include "shapemerge2d/shape.h" 
#include "shapemerge2d/boolean_op.h" 
#include "shapemerge2d/shapemerge2d.h" 
using namespace shapemerge2d;
%}

%include "exception.i"
%include "std_string.i"
%include "std_vector.i"
%include "shapemerge2d/vertex.h" 
%include "shapemerge2d/vector.h" 
%include "shapemerge2d/line.h" 
%include "shapemerge2d/polygon.h" 
%include "shapemerge2d/shape.h" 
%include "shapemerge2d/boolean_op.h" 
%include "shapemerge2d/shapemerge2d.h" 

%rename(__eq__) Line::operator==;
%rename(__eq__) Vector::operator==;
%rename(__eq__) Vertex::operator==;
%rename(__add__) Vector::operator+;
%rename(__sub__) Vector::operator-;
%rename(__sub__) Vertex::operator-;

namespace std {
   %template(lvector) vector<shapemerge2d::Line>;
   %template(pvector) vector<shapemerge2d::Polygon>;
   %template(vvector) vector<shapemerge2d::Vertex>;
   %template(evector) vector<shapemerge2d::Edge>;
   %template(cvector) vector<shapemerge2d::Cell>;
   %template(svector) vector<std::string>;
}



%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}
