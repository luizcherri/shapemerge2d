#include <vector>
#include <iostream>
#include <shapemerge2d/shapemerge2d.h>

using namespace shapemerge2d;

/*!
 * Here is a simple example of creating two square polygons,
 * side by side. The polygons are merged to one larger,
 * rectangular polygon.
 */
int main(int argc,char** argv)
{
	std::vector<Vertex> a_vertices;
	a_vertices.push_back(Vertex(0,0));
	a_vertices.push_back(Vertex(10,0));
	a_vertices.push_back(Vertex(10,10));
	a_vertices.push_back(Vertex(0,10));

	std::vector<Vertex> b_vertices;
	b_vertices.push_back(Vertex(10,0));
	b_vertices.push_back(Vertex(20,0));
	b_vertices.push_back(Vertex(20,10));
	b_vertices.push_back(Vertex(10,10));

	Polygon a(a_vertices);
	Polygon b(b_vertices);

	Shape ret=shape_union(a,b);

	std::cout<<ret<<std::endl;
	std::cout<<ret.get_polys()[0]<<std::endl;


}

