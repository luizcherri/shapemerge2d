#include "shape.h"
#include <sstream>
namespace shapemerge2d
{

Shape::Shape(const std::string& name,const std::vector<Vertex>& poly) : name(name)
{
	polys.push_back(Polygon(poly,Polygon::SOLID,this));
}
std::string Shape::__repr__()const
{
	std::ostringstream s;
	s<<"Shape(\""<<name<<"\", "<<polys.size()<< " polygons)";
	return s.str();
}
Shape::Shape(const Shape& o) : name(o.name),polys(o.polys)
{
	BOOST_FOREACH(Polygon& po,polys)
		po.set_shape(this);
}
std::string Shape::dump() const
{
	std::ostringstream s;
	s<<"Shape(\""<<name<<"\","<<"\n";
	for(size_t i=0;i<polys.size();++i)
	{
		s<<polys[i].__repr__()<<"\n";
	}
	s<<")";
	return s.str();
}

bool Shape::operator==(const Shape& o) const
{
	if (polys.size()!=o.polys.size()) return false;
	std::vector<bool> matched(o.polys.size(),false);
	for(size_t i=0;i<polys.size();++i)
	{
		bool found=false;
		for(size_t j=0;j<o.polys.size();++j)
		{
			if (matched[j]) continue;
			if (polys[i]==o.polys[j])
			{
				matched[j]=true;
				found=true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

Shape::Shape(const std::string& name,const Polygon& poly) : name(name)
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
const std::vector<Polygon>& Shape::get_polys() const
{
	return polys;
}
std::vector<Polygon>& Shape::get_polys()
{
    return polys;
}
void Shape::remove_polygon_by_idx(int idx)
{
    if (idx<0 || (size_t)idx>=polys.size())
        throw std::runtime_error("Bad polygon number");
    polys.erase(polys.begin()+idx);
}









}
