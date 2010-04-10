#include "shape.h"
namespace shapemerge2d
{

Shape::Shape()
{

}
Shape::Shape(const std::string& name,const Polygon poly) : name(name)
{
	polys.push_back(poly);
	polys.back().set_shape(this);
}
Shape::Shape(const std::string& name,const std::vector<Polygon>& polys) :
	name(name),polys(polys)
{
	BOOST_FOREACH(auto& p,this->polys)
		p.set_shape(this);

}
const std::vector<Polygon>& Shape::get_polys()
{
	return polys;
}














}
