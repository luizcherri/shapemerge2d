#ifndef LINE2_INCLUDED
#define LINE2_INCLUDED
#include "boost/rational.hpp"
#include "boost/foreach.hpp"
#include "vertex.h"
#include <limits.h>
#include <vector>
#include <set>
#include <stdio.h>

namespace shapemerge2d
{

struct DbgFloatVertex
{
public:
#ifndef SWIG
	double x,y;
#endif
	DbgFloatVertex(){x=0;y=0;}
	double get_x()const{return x;}
	double get_y()const{return x;}
};

typedef boost::rational<int64_t> Rational;
long ratfloor(Rational r);

class Line2
{
	Vertex v1;
	Vertex v2;
	Rational k;
	Rational m;
	std::set<int> tags;
public:
	Vertex get_v1()const{return v1;}
	Vertex get_v2()const{return v2;}
	const std::set<int>& get_tags() const { return tags;}
	std::string __repr__() const
	{
		std::ostringstream os;
		os<<"Line2("<<v1.__repr__()<<","<<v2.__repr__()<<")";
		return os.str();
	}
	Line2 reversed() const;
    double approx_dist(Vertex p);
    Vertex approx_closest(Vertex p);

	bool is_vertical() const;
	Rational get_k() const;
	Rational get_m() const;
	Line2();
	Line2(const Vertex& v1,const Vertex& v2);
	Line2(const Vertex& v1,const Vertex& v2,
		  const Rational& k,const Rational& m);
	bool operator==(const Line2& o) const;
	bool operator!=(const Line2& o) const;
	bool operator<(const Line2& o) const;
	void add_tag(int tag);
	void add_tags(const std::set<int>& newtags);
	int __hash__() const
	{
		return v1.__hash__()+17*v2.__hash__();
	}

	int taxilen() const
	{
		int ret=std::abs(v1.x-v2.x)+std::abs(v1.y-v2.y);
		//printf("taxilen calc for %s: %d\n",
		//		__repr__().c_str(),ret);
		return ret;
	}
	std::vector<Vertex> slow_all_vertices() const;

    /**
     * Consider an infinite line which is a continuation of
     * this line. Now consider the verte xv. If it is
     * to the left of this line, return -1, if it is to
     * the right, return +1, if it is _on_ the line, return
     * 0.
     */
    int side_of_extrapolated_line(Vertex v) const;
    
	DbgFloatVertex dbg_point_on_line(Vertex at)const;
	DbgFloatVertex dbg_fv1()const
		{
		return dbg_point_on_line(v1);
		}
	DbgFloatVertex dbg_fv2()const
		{
		return dbg_point_on_line(v2);
		}
	double get_x1_inexact() const
		{
			return dbg_fv1().x;
		}
	double get_x2_inexact() const
		{
			return dbg_fv2().x;
		}
	double get_y1_inexact() const
		{
			return dbg_fv1().y;
		}
	double get_y2_inexact() const
		{
			return dbg_fv2().y;
		}

	void split(const Vertex& x,Line2& a,Line2& b) const;
	std::vector<Line2> split(const Vertex& x) const
		{
			std::vector<Line2> ret;
			Line2 a,b;
			split(x,a,b);
			ret.push_back(a);
			ret.push_back(b);
			return ret;
		}
	void split3(const Vertex& x,const Vertex& y,Line2& a,Line2& b,Line2& c) const;
	std::vector<Line2> split3(const Vertex& x,const Vertex& y) const
		{
			std::vector<Line2> ret;
			Line2 a,b,c;
			split3(x,y,a,b,c);
			ret.push_back(a);
			ret.push_back(b);
			ret.push_back(c);
			return ret;
		}

    bool is_on_line(Vertex v)const;
	bool get_yrange(int x,int& y1,int& y2) const;
	std::vector<int> get_yrange(int x) const
	{
		std::vector<int> ret;
		int y1=0,y2=0;
		if (!get_yrange(x,y1,y2))
			return ret;
		ret.push_back(y1);
		ret.push_back(y2);
		return ret;
	}
	std::vector<Vertex> intersection_points(const Line2& o) const;
	/**
	 * If the two lines intersect, this
	 * routine always returns exactly 5 lines:
	 *  * intersection
	 *  * start of line a
	 *  * start of line b
	 *  * end of line a
	 *  * end of line b
	 * Otherwise, an empty vector is returned.
	 */
	std::vector<Line2> intersect(const Line2& o) const;

	/**
	 * This routine works like intersect,
	 * however for two lines which just
	 * touch, without crossing, it returns
	 * an empty vector.
	 */
	std::vector<Line2> intersect2(const Line2& o) const;

	/*bool get_xrange(int x,int& y1,int& y2);
	std::vector<int> get_xrange(int x)
	{
		std::vector<int> ret;
		int x1=0,x2=0;
		if (!get_xrange(y,x1,x2))
			return false;
		ret.push_back(x1);
		ret.push_back(x2);
		return ret;
	}*/


};










}










#endif



