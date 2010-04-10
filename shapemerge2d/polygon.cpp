#include "polygon.h"
#include "line2.h"
namespace shapemerge2d
{


const std::vector<Line2>& Polygon::get_lines() const
{
	return lines;
}
Vertex Polygon::lower_left_vertex() const
{
	if (lines.size()==0) throw std::runtime_error("Polygon has no edges!");
	Vertex most=lines[0].get_v1();
	BOOST_FOREACH(const Line2& l,lines)
	{
		Vertex vs[2]={l.get_v1(),l.get_v2()};
		for(int i=0;i<2;++i)
		{
			Vertex& v=vs[i];
			if (v.x<most.x) v=most;
			if (v.x==most.x && v.y<most.y) v=most;
		}
	}
	return most;
}


}
