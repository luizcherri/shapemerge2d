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

/**
 * The Line-class represents a line with a start point and an end-point.
 * The line also has extra information regarding the rasterization of
 * the line. This is used so that it is possible to split a line into
 * two parts, the union of the pixels of the two parts being exactly the
 * set of pixels in the original line.
 */
class Line
{
	///Start point of line.
	Vertex v1;
	///End point of line
	Vertex v2;
	///Line slope, line pixels are described by y=k*x+m
	///For a vertical line, this is 0.
	Rational k;
	///Line offset, line pixels are described by y=k*x+m
	Rational m;
	///Lines can be given integer "tags" which are stored with the lines.
	///THese can be used by algorithms.
	std::set<int> tags;
public:
	Vertex get_v1()const{return v1;}
	Vertex get_v2()const{return v2;}
	const std::set<int>& get_tags() const { return tags;}
	///Return a string representation of the line.
	///The representation only gives start and end points, not details.
	std::string __repr__() const
	{
		std::ostringstream os;
		os<<"Line("<<v1.__repr__()<<","<<v2.__repr__()<<")";
		return os.str();
	}
	///Reverse line (exchange start-point and end-point)
	///without changing the set of pixels occupied by the line.
	Line reversed() const;
	///Return an approximate measure of the length of the line.
	///Not useful for algorithms which expect to be 'stable',
	///but useful for other purposes. The returned value is an
	///approximation. Its precision is not actually that of
	///a double.
    double approx_dist(Vertex p);
    ///Return an approximation of which pixel on the line is
    ///the closest to the given pixel 'p'.
    Vertex approx_closest(Vertex p);
    ///Return true if the line is vertical. This is identical to
    ///v1.x==v2.x.
	bool is_vertical() const;
	Rational get_k() const;
	Rational get_m() const;
	Line();
	Line(const Vertex& v1,const Vertex& v2);
	Line(const Vertex& v1,const Vertex& v2,
		  const Rational& k,const Rational& m);
	/**
	 * Compare the startpoint/endpoint pairs of the two
	 * given vertices lexicographically. This means that
	 * the lines do not necessarily need to occupy the same
	 * pixels, in order to be considered equal.
	 * Be careful!
	 */
	bool operator==(const Line& o) const;
	/**
	 * Compare the startpoint/endpoint pairs of the two
	 * given vertices lexicographically. This means that
	 * the lines do not necessarily need to occupy the same
	 * pixels, in order to be considered equal.
	 * Be careful!
	 */
	bool operator!=(const Line& o) const;
	/**
	 * Compare the startpoint/endpoint pairs of the two
	 * given vertices lexicographically. This means that
	 * the lines do not necessarily need to occupy the same
	 * pixels, in order to be considered equal.
	 * Be careful!
	 */
	bool operator<(const Line& o) const;
	/**
	 * Add an integer 'tag' to the line. The tag will be store
	 * with the line, and can later be found in the set returned by
	 * get_tags().
	 */
	void add_tag(int tag);
	void add_tags(const std::set<int>& newtags);
	/**
	 * Return a hash key based upon the start and end points.
	 */
	int __hash__() const
	{
		return v1.__hash__()+17*v2.__hash__();
	}

	/**
	 * Returns abs(v1.x-v2.x)+abs(v1.y-v2.y)
	 */
	int taxilen() const
	{
		int ret=std::abs(v1.x-v2.x)+std::abs(v1.y-v2.y);
		//printf("taxilen calc for %s: %d\n",
		//		__repr__().c_str(),ret);
		return ret;
	}
	/**
	 * Return all vertices of the line, inefficiently.
	 * This is mainly usedful for debugging.
	 */
	std::vector<Vertex> slow_all_vertices() const;

    /**
     * Consider an infinite line which is a continuation of
     * this line. Now consider the vertex v. If it is
     * to the left of this line, return -1, if it is to
     * the right, return +1, if it is _on_ the line, return
     * 0.
     */
    int side_of_extrapolated_line(Vertex v) const;
    
    ///Routine only used for debugging.
	DbgFloatVertex dbg_point_on_line(Vertex at)const;
    ///Routine only used for debugging.
	DbgFloatVertex dbg_fv1()const
		{
		return dbg_point_on_line(v1);
		}
    ///Routine only used for debugging.
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

	/**
	 * Split the current line at vertex x, giving two lines,
	 * a and b, the union of whose pixels is identical to the
	 * set of pixels in this line.
	 * Precondition: 'x' must be a point on this line.
	 */
	void split(const Vertex& x,Line& a,Line& b) const;
	/**
	 * Split the current line at vertex x, giving two lines,
	 * a and b, the union of whose pixels is identical to the
	 * set of pixels in this line.
	 * Precondition: 'x' must be a point on this line.
	 * Note, this routine always returns the two lines, a and b.
	 */
	std::vector<Line> split(const Vertex& x) const
		{
			std::vector<Line> ret;
			Line a,b;
			split(x,a,b);
			ret.push_back(a);
			ret.push_back(b);
			return ret;
		}
	/**
	 * Split line on two points, x and y, into three parts: a, b and c.
	 * The union of pixels from the three parts is always equal
	 * to the set of pixels in this line.
	 * Precondition: x and y must both be in this line.
	 */
	void split3(const Vertex& x,const Vertex& y,Line& a,Line& b,Line& c) const;
	/**
	 * Split line on two points, x and y, into three parts: a, b and c.
	 * The union of pixels from the three parts is always equal
	 * to the set of pixels in this line.
	 * Precondition: x and y must both be in this line.
	 * This routine always returns the three parts, a, b and c.
	 */
	std::vector<Line> split3(const Vertex& x,const Vertex& y) const
		{
			std::vector<Line> ret;
			Line a,b,c;
			split3(x,y,a,b,c);
			ret.push_back(a);
			ret.push_back(b);
			ret.push_back(c);
			return ret;
		}
	/**
	 * Return true if the given vertex is one of the pixels/vertices
	 * of this line. I.e, return true if vertex v is 'on' this line.
	 */
	bool is_on_line(Vertex v)const;
	/**
	 * Return the range of y-values occupied by this line, at
	 * the given x-value. For non-vertical lines with k<0, this
	 * is always just a single pixel. For vertical lines, and lines
	 * with k>1, this can sometimes be several pixels.
	 *
	 * If the line does not extend to the given x-coordinate,
	 * return false.
	 */
	bool get_yrange(int x,int& y1,int& y2) const;
	/**
	 * Just as get_yrange, but return the two values y1 and y2
	 * in a vector, instead. If the line does not extend to the
	 * x-coordinate, return an emtpy vector.
	 */
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

	std::vector<Vertex> intersection_points(const Line& o) const;
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
	std::vector<Line> intersect(const Line& o) const;

	/**
	 * This routine works like intersect,
	 * however for two lines which just
	 * touch, without crossing, it returns
	 * an empty vector.
	 */
	std::vector<Line> intersect2(const Line& o) const;

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

#ifndef SWIG
inline std::ostream& operator<<(std::ostream& os,const shapemerge2d::Line& x)
{
	os<<x.__repr__();
	return os;
}
#endif









#endif



