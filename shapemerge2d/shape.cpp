#include "shape.h"
namespace shapemerge2d
{

Shape::Shape()
{

}
Shape::Shape(const std::string& name,const Polygon poly) : name(name)
{
	polys.push_back(poly);
}
Shape::Shape(const std::string& name,const std::vector<Polygon>& polys) :
	name(name),polys(polys)
{

}
const std::vector<Polygon>& Shape::get_polys()
{
	return polys;
}














}
