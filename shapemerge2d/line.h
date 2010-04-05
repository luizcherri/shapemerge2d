#ifndef SHAPE_MERGE_LINE_H_INCLUDED
#define SHAPE_MERGE_LINE_H_INCLUDED
#include "boost/rational.hpp"
#include "vertex.h"
#include <limits.h>
#include <vector>
#include <set>
#include "first_octant.h"

namespace shapemerge2d
{
class FirstOctant;
typedef boost::rational<int64_t> Rational;
int64_t floor(Rational r);
int64_t ceil(Rational r);
int floor2(int a,int b);
int ceil2(int a,int b);

class Line
{
	Rational x1;
	Rational y1;
	Rational x2;
	Rational y2;
	std::set<int> tags;
	static Vertex get_impl(const Line& self,int p);
	static bool intersect_impl(const Line& a,const Line& b,Line& inter,std::vector<Line>& out);
public:
	const std::set<int>& get_tags() const { return tags;}
	std::string __repr__() const
	{
		std::ostringstream os;
		os<<"Line("<<x1<<","<<y1<<"->"<<x2<<","<<y2<<")";
		return os.str();
	}
	std::string intrepr() const
	{
		std::ostringstream os;
		os<<"Line("<<floor(x1)<<","<<floor(y1)<<"->"<<floor(x2)<<","<<floor(y2)<<")";
		return os.str();
	}
	std::string floatrepr() const
	{
		std::ostringstream os;
		os<<"Line("<<double(x1.numerator())/double(x1.denominator())<<","<<
				double(y1.numerator())/double(y1.denominator())<<","<<
					"->"<<
				double(x2.numerator())/double(x2.denominator())<<","<<
				double(y2.numerator())/double(y2.denominator())<<","<<
				")";
		return os.str();
	}

	Line(
		const Rational& x1,
		const Rational& y1,
		const Rational& x2,
		const Rational& y2);
	Line();
	Line(const Vertex& v1,const Vertex& v2);
	Line(int x1a,int x1b,
		int y1a,int y1b,
		int x2a,int x2b,
		int y2a,int y2b);

	//Through grid crossings, not center of grid squares.
	static Line exact_line(Vertex a,Vertex b)
	{
		return Line(a.x,a.y,b.x,b.y);
	}
	void add_tag(int tag);
	void add_tags(const std::set<int>& newtags);

	Rational get_x1() const;
	Rational get_y1() const;
	Rational get_x2() const;
	Rational get_y2() const;

	double get_x1_inexact() const;
	double get_y1_inexact() const;
	double get_x2_inexact() const;
	double get_y2_inexact() const;

	Vertex v1() const;
	Vertex v2() const;

	void y_range(int x,int& y1,int& y2);
	int high_y_of_x(int x)
	{
		int y1=0;
		int y2=0;
		y_range(x,y1,y2);
		return y2;
	}
	int low_y_of_x(int x)
	{
		int y1=0;
		int y2=0;
		y_range(x,y1,y2);
		return y1;
	}
	private:
	bool pinch2(const Vertex& o,Line& a,Line& b,const FirstOctant& base);
	public:
	std::vector<Line> pinch(const Vertex& o)
	{
		Line a,b;
		FirstOctant dummy(Line(Vertex(0,0),Vertex(10,5)));
		if (!pinch2(o,a,b,dummy))
			return std::vector<Line>();
		std::vector<Line> out;
		out.push_back(a);
		out.push_back(b);
		return out;
	}
	private:
	int grid_maxnorm(const FirstOctant& oct) const;
	public:
	int grid_maxnorm() const;
	Line reversed() const;
	int get_smallest_p_of_x(int x) const;
	Vertex get(int p) const
	{
		return get_impl(*this,p);
	}
	std::vector<Line> intersection(const Line& o) const
	{
		std::vector<Line> dummy;
		Line isect;
		std::vector<Line> ret;
		if (intersect_impl(*this,o,isect,dummy))
		{
			ret.push_back(isect);
			return ret;
		}
		return ret;
	}

};

}








#endif
