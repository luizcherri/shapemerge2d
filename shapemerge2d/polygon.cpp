#include "polygon.h"
#include "line2.h"
#include "math.h"
#include "limits.h"
#include "vector.h"

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
bool Polygon::is_inside(Vertex v)
{
    int startindex=-1;
    int minx=INT_MAX;
    int maxx=INT_MIN;
    if (lines.empty())
        return false;
    for(size_t i=0;i<lines.size();++i)
    {
        Line2& a=lines[i];
        if (a.get_v1().get_x()<minx)
            minx=a.get_v1().get_x();
        if (a.get_v1().get_x()>maxx)
            maxx=a.get_v1().get_x();
        if (a.get_v1().get_y()!=v.y && startindex==-1)
        {
            startindex=i;
        }            
    }
    if (startindex==-1)
    { //all the edges fall on the y-coord of v
        if (v.x>=minx && v.x<=maxx)
            return true;
        else
            return false;
    }
    //printf("Startindex: %d\n",startindex);
    Line2 exiter=Line2(v,Vertex(minx-1,v.y));
    int crossings=0;
    for(size_t ti=0;ti<lines.size();++ti)
    {
        
        Line2 a=lines[(startindex+ti)%lines.size()];
        std::vector<Line2> isect=exiter.intersect(a);
        //printf("Intersect of %s and %s: %d\n",exiter.__repr__().c_str(),a.__repr__().c_str(),(int)isect.size());
        if (isect.empty())
            continue;
        Line2 middle=isect[0];
        assert(middle.get_v1().get_y()==v.y);
        assert(middle.get_v2().get_y()==v.y);
        int mid_min_x=std::min(middle.get_v1().get_x(),middle.get_v2().get_x());
        int mid_max_x=std::max(middle.get_v1().get_x(),middle.get_v2().get_x());
        //printf("Found intersect (%s): %d,%d\n",middle.__repr__().c_str(),mid_min_x,mid_max_x);
        if (mid_min_x>v.x) continue;
        if (mid_min_x<=v.x && mid_max_x>=v.x) 
        {   
            //printf("Vertex falls on edge\n");
            return true; //v falls on edge of polygon, count as inside
        }
        assert(a.get_v1().y!=v.y);
        bool enter_above=(a.get_v1().y>v.y);
        //printf("Line starts above: %d\n",int(enter_above));
        if (a.get_v2().y==v.y)
        {
            //printf("End of line falls on exiter, glancing or not?\n");
            while(a.get_v2().y==v.y)
            {
                assert(ti!=lines.size()-1); //the end vertex (=startvertex) must always be off the v y-coord
                ++ti;
                a=lines[(startindex+ti)%lines.size()];
            }
            bool exit_above=(a.get_v2().y>v.y);
            if (enter_above!=exit_above)
                ++crossings; //an actual crossing, not just a glancing hit
        }
        else
        {
            bool exit_above=(a.get_v2().y>v.y);
            //printf("Line crosses exiter, exits above: %d\n",int(exit_above));
            assert(enter_above!=exit_above); //line actually crosses exiter-line
            ++crossings;
        }        
    }
    return bool(crossings&1);
    
}

enum EventKind
{
    EXIT_EVENT,
    ENTER_EVENT
};
struct Event
{
    Vertex start;
    Vertex p;
    EventKind kind;
    Event(Vertex start,Vertex p,EventKind kind):start(start),p(p),kind(kind){}
    std::string kindstr() const
    {
    	switch(kind)
    	{
		case EXIT_EVENT: return "EXIT_EVENT";
		case ENTER_EVENT: return "ENTER_EVENT";
    	}
    	return "Undefined";
    }
    int val() const
    {
        return  (p-start).taxilength();            
    }
    bool operator<(const Event& e) const
    {
    	if (val()<e.val())
    		return true;
    	if (val()>e.val())
    		return false;
        return kind<e.kind;
    }
    std::string repr() const
    {
    	std::ostringstream s;
    	s<<"Event("<<p.__repr__()<<",kind="<<kindstr()<<")";
    	return s.str();
    }
    
};

static void merge(const Vertex& start,std::vector<Line2>& ret,const Line2& line)
{
	if (ret.empty())
	{
		printf("      Adding initial line: %s\n",line.__repr__().c_str());
		ret.push_back(line);
		return;
	}
	int offret1=(ret.back().get_v1()-start).taxilength();
	int offret2=(ret.back().get_v2()-start).taxilength();
	int offline1=(line.get_v1()-start).taxilength();
	int offline2=(line.get_v2()-start).taxilength();
	printf("       offret: %d-%d, offline: %d-%d\n",
			offret1,offret2,offline1,offline2);
	if (offline1<offret1 || offline2<offret1)
		throw std::runtime_error("Bad order of lines to merge");
	if (offline1>offline2)
		throw std::runtime_error("Bad vertex order in line to merge");
	if (offline1<=offret2)
	{
		if (offline2>offret2)
		{
			//merge
			ret.back()=Line2(ret.back().get_v1(),line.get_v2(),line.get_k(),line.get_m());
			printf("      Merging line: %s, new back: %s\n",line.__repr__().c_str(),
					ret.back().__repr__().c_str());
		}
		else
		{
			printf("      Eating line\n");
			//just eat it
		}
	}
	else
	{
		//no merge
		ret.push_back(line);
		printf("      Just adding line: %s, new back: %s\n",line.__repr__().c_str(),
				ret.back().__repr__().c_str());
	}
}

std::vector<Line2> Polygon::intersect_line(Line2 b)
{
	printf("Intersecting poly with line %s\n",b.__repr__().c_str());
    if (lines.size()<3)
        throw std::runtime_error("Polygon must have at least 3 lines!");
    
    int starti=-1;
	for(size_t i=0;i<lines.size();++i)
	{
	     if (!b.is_on_line(lines[i].get_v1()))
	     {
	 		printf("Point #%zd doesn't fall directly on line!\n",i);
	        starti=i;
	        break;
	     }
   
	}
	if (starti==-1)
	{ //Very special case - the entire polygon falls upon the line!
		printf("Polygon falls on single line!\n");
	    return std::vector<Line2>();
	}
	int curi=starti;
	std::vector<Event> events;


	for(size_t i=0;i<lines.size();++i,++curi)
	{
		curi%=lines.size();
	    Line2 a=lines[curi];
	    printf("#%d line of polygon: %s\n",curi,a.__repr__().c_str());
	    std::vector<Line2> res=b.intersect(a);
	    //int last_excursion=-1;
	    //int start_inside=-1; //1 = start inside, 0 = start outside
	    if (res.size())
	    {
	    	Line2 middle=res[0];
	        Line2 start_a=res[2];
	        Line2 end_a=res[4];
	    	/*if (middle.get_v2()!=end_b.get_v1())
	    		middle=middle.reversed();*/

	        printf("  Intersection! end_a: %s, middle: %s\n",
	        		end_a.__repr__().c_str(),
	        		middle.__repr__().c_str());

	        if (middle.taxilen()>0)
	        { 
	        	printf("    \x1b[33mLine enters the polygon edge!\x1b[0m\n");
	            events.push_back(Event(b.get_v1(),middle.get_v1(),ENTER_EVENT));
	            printf("      Pushed enter event: %s\n",events.back().repr().c_str());
	            events.push_back(Event(b.get_v1(),middle.get_v2(),EXIT_EVENT));
	            printf("      Pushed exit event: %s\n",events.back().repr().c_str());
	        }
	        if (end_a.taxilen()>0)
	        { //exit
	        	printf("    \x1b[32mPolygon edge exists line here\x1b[0m\n");
	            int side=b.side_of_extrapolated_line(end_a.get_v2());
	            printf("      Line exits on %s side of polygon edge\n",
	            		((side<0) ? "left" : ((side>0) ? "right" : "unknown!")));
	            if (side>0)
	            {
    	            events.push_back(Event(b.get_v1(),end_a.get_v1(),ENTER_EVENT));
    	            printf("      Pushed event: %s\n",events.back().repr().c_str());

	            }
	            else
	            if (side<0)
	            {
    	            events.push_back(Event(b.get_v1(),end_a.get_v1(),EXIT_EVENT));
					printf("      Pushed event: %s\n",events.back().repr().c_str());

	            }
	        }
	        if (start_a.taxilen()>0)
	        { //exit
	        	printf("    \x1b[32mPolygon edge enters line here\x1b[0m\n");
	            int side=b.side_of_extrapolated_line(start_a.get_v1());
	            printf("      Line exits on %s side of polygon edge\n",
	            		((side<0) ? "left" : ((side>0) ? "right" : "unknown!")));
	            if (side<0)
	            {
    	            events.push_back(Event(b.get_v1(),start_a.get_v2(),ENTER_EVENT));
    	            printf("      Pushed event: %s\n",events.back().repr().c_str());

	            }
	            else
	            if (side>0)
	            {
    	            events.push_back(Event(b.get_v1(),start_a.get_v2(),EXIT_EVENT));
					printf("      Pushed event: %s\n",events.back().repr().c_str());

	            }
	        }
	    }	
	    else
	    {
			printf("  No intersection.\n");
	    }
	}
	printf("sorting events\n");
	std::sort(events.begin(),events.end());
	if (events.size()==0)
	{
	    std::vector<Line2> ret;
	    if (is_inside(b.get_v1()))
	    {
	    	printf("The line is entirely inside the polygon\n");
	        ret.push_back(b);
	    }
	    else
	    {
	    	printf("The line is entirely outside the polygon\n");
	    }
	    return ret;
	}
	printf("We have %zd events in total\n",events.size());
	printf("The events are:\n");
	for(int i=0;i<(int)events.size();++i)
	{
		printf("  #%d: %s\n",i,events[i].repr().c_str());
	}
    std::vector<Line2> ret;
    int level=0;
    Rational k=b.get_k();
    Rational m=b.get_m();
    Vertex last_enter;
    printf("Processing events:\n");
    for(size_t i=0;i<events.size();++i)
    {
		printf("  Event #%d\n",(int)i);
    	if (events[i].kind==EXIT_EVENT)
    	{
    		printf("   Processing EXIT_EVENT %s\n",events[i].p.__repr__().c_str());
    		if (ret.empty() && level==0)
    		{
        		printf("   Processing virtual start event\n");
    			last_enter=b.get_v1();
    			merge(b.get_v1(),ret,Line2(last_enter,events[i].p,k,m));
    			level=1;
    		}
    		merge(b.get_v1(),ret,Line2(last_enter,events[i].p,k,m));
    		--level;
    		assert(level>=0);
    	}
        if (events[i].kind==ENTER_EVENT)
        {
        	printf("   Processing ENTER_EVENT %s\n",events[i].p.__repr__().c_str());
        	if (level==0)
        		last_enter=events[i].p;
        	++level;
            continue;
        }
    }
    if (level>0)
		merge(b.get_v1(),ret,Line2(last_enter,b.get_v2(),k,m));

    return ret;
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
