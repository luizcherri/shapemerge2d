#include "polygon.h"
#include "line2.h"
#include "math.h"
#include "limits.h"

namespace shapemerge2d
{


const std::vector<Line2>& Polygon::get_lines() const
{
	return lines;
}
bool Polygon::is_ccw()const
{
	double turnsum=0;
	for(size_t i=0;i<lines.size();++i)
	{
		size_t nexti=(i+1)%lines.size();
		Vertex v1=lines[i].get_v1();
		Vertex v2=lines[nexti].get_v1();
		Vertex v3=lines[nexti].get_v2();
		assert(v1!=v2);
		assert(v2!=v3);
		assert(v2==lines[i].get_v2());
		int dx1=v2.x-v1.x;
		int dy1=v2.y-v1.y;
		int dx2=v3.x-v2.x;
		int dy2=v3.y-v2.y;
		double a1=atan2(dy1,dx1);
		double a2=atan2(dy2,dx2);
		double turn=a2-a1;
		if (turn<M_PI)
			turn+=2*M_PI;
		if (turn>M_PI)
			turn-=2*M_PI;
		//printf("Turn: %f\n",turn);
		turnsum+=turn;
	}
	//printf("Turnsum: %f\n",turnsum);
	return turnsum>0;
}

std::string Polygon::__repr__() const
{
	std::ostringstream s;
	s<<"Polygon("<<get_kind_str()<<",\n";
	BOOST_FOREACH(const Line2& l,lines)
	{
		s<<l.__repr__()<<"\n";
	}
	s<<")";
	return s.str();
}
void Polygon::merge_straight_sections()
{
	if (lines.size()<=1) return;
	size_t i=0;
	for(;;)
	{
		if (i>=lines.size()) //can be >, if last item is removed
			break;
		int nexti=(i+1)%lines.size();
		Line2& one=lines[i];
		Line2& two=lines[nexti];
		assert(one.get_v2()==two.get_v1());
		if (
				(one.get_k()==two.get_k() &&
				 one.get_m()==two.get_m() &&
				 !one.is_vertical() && !two.is_vertical())
				 ||
				(one.is_vertical() && two.is_vertical() &&
				one.get_v1().x==two.get_v1().x)
			)
		{
			one=Line2(one.get_v1(),two.get_v2(),one.get_k(),one.get_m());
			lines.erase(lines.begin()+nexti);
		}
		else
		{
			++i;
		}
	}
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
struct ISect
{
    bool entering; //if false => is exiting
    Vertex where;
};

/*
std::vector<Line2> Polygon::intersect_line(Line2 line)
{
    for(size_t i=0;i<lines.size();++i)
	{
	    Line2& a=lines[i];
	    std::vector<Line2> isect=line.intersect(a);
	    
        Line2& start_a=isect[1];
	    
	}
}
*/
enum poly_sides
{
    start_side,
    other_side
};
bool Polygon::is_inside(Vertex& v)
{
    for(size_t i=0;i<lines.size();++i)
    {
        Line2& a=lines[i];
        if (a.is_on_line(v))
            return true; //vertex falls on an edge, by definition inside.
    }
    Line2 exiter=Line2(cur,Vertex(INT_MIN,v.y));
    for(size_t i=0;i<lines.size();++i)
    {
        Line2& a=lines[i];
        exiter.intersect(exiter);
    }
    
}

Polygon Polygon::remove_loops()
{
    std::vector<Line2> out;
	for(size_t i=0;i<lines.size();++i)
	{
	    Line2 a=lines[i];
	    restart:
    	for(size_t j=lines.size()-1;j>i+1;--j)
    	{
    	    if (i==0 && j==lines.size()-1) continue; //special case - first line does not 'overlap' with last.
    	    Line2 b=lines[j];
    	    std::vector<Line2> isect=a.intersect(b);
    	    if (isect.size())
    	    { //intersect
    	        Line2& start_a=isect[1];
    	        Line2& end_b=isect[4];
    	        assert(start_a.get_v2()==end_b.get_v1());
    	        out.push_back(start_a);
    	        i=j;
    	        a=end_b;
    	        goto restart;
    	    }    	    
    	}
    	//no intersect at all
    	out.push_back(a);        
	}
	Polygon ret;
	ret.kind=kind;
	ret.lines=out;
	ret.shape=shape;
	return ret;
}
bool Polygon::operator==(const Polygon& o)const
{
	if (lines.size()!=o.lines.size()) return false;
	if (kind!=o.kind) return false;
	for(size_t offset=0;offset<lines.size();++offset)
	{
		bool equal=true;
		for(size_t i=0;i<lines.size();++i)
		{
			int ai=i;
			int bi=(i+offset)%lines.size();
			if (!(lines[ai]==o.lines[bi]))
			{
				equal=false;
			}
		}
		if (equal) return true;
	}
	return false;
}

}
