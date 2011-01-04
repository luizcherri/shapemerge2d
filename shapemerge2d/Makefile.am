lib_LTLIBRARIES = libshapemerge2d.la
libshapemerge2d_la_SOURCES=boolean_op.cpp polygon.cpp shapemerge2d.cpp vertex.cpp line.cpp shapemerge2d shape.cpp vector.cpp
libshapemerge2d_la_CXXFLAGS = -Wall -Werror -std=c++0x

include ../VERSION
SHAPEMERGE2D_LIBRARY_VERSION=$(VERSION)
libshapemerge2d_la_LDFLAGS = -version-info $(SHAPEMERGE2D_LIBRARY_VERSION)

