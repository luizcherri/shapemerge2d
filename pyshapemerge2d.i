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
#include "shapemerge2d/line2.h" 
using namespace shapemerge2d;
%}

%include "exception.i"
%include "std_string.i"
%include "std_vector.i"
%include "shapemerge2d/vertex.h" 
%include "shapemerge2d/vector.h" 
%include "shapemerge2d/line2.h" 

%rename(__eq__) Vector::operator==;
%rename(__eq__) Vertex::operator==;
%rename(__add__) Vector::operator+;
%rename(__sub__) Vector::operator-;
%rename(__sub__) Vertex::operator-;

namespace std {
   %template(lvector) vector<shapemerge2d::Line2>;
   %template(vvector) vector<shapemerge2d::Vertex>;
}



%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}
