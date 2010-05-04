#include "line2.h"
#include <stdexcept>
#include <stdio.h>
#include <algorithm>
#include <math.h>

namespace shapemerge2d
{
static inline Rational absolute(Rational r)
{
	if (r<0)
		return -r;
	return r;
}
static inline double todouble(Rational r)
{
	return double(r.numerator())/double(r.denominator());
}
void Line2::add_tag(int tag)
{
	tags.insert(tag);
}
bool Line2::operator==(const Line2& o) const
{
	return v1==o.v1 && v2==o.v2;
}
bool Line2::operator!=(const Line2& o) const
{
	return !(v1==o.v1 && v2==o.v2);
}
bool Line2::operator<(const Line2& o) const
{
	if (v1<o.v1)
		return true;
	if (o.v1<v1)
		return false;
	return v2<o.v2;
}

void Line2::add_tags(const std::set<int>& newtags)
{
	std::set<int>::iterator it = newtags.begin();
	for (; it != newtags.end(); ++it)
		tags.insert(*it);
}


long ratfloor(Rational r)
{
	int64_t a = r.numerator();
	int64_t b = r.denominator();
	if ((a < 0) == (b < 0))
	{
		a = std::abs(a);
		b = std::abs(b);
		return a / b;
	}
	else
	{
		a = std::abs(a);
		b = std::abs(b);
		return -(a + b - 1) / b;
	}
}

#if 0
static long ceil(Rational r)
{
	int64_t a = r.numerator();
	int64_t b = r.denominator();
	if ((a < 0) == (b < 0))
	{
		a = std::abs(a);
		b = std::abs(b);
		return (a + b - 1) / b;
	}
	else
	{
		a = std::abs(a);
		b = std::abs(b);
		return -a / b;
	}
}
#endif

Line2::Line2() :
		k(0),m(0)
{
}
DbgFloatVertex Line2::dbg_point_on_line(Vertex v)const
{
	if (v1.x==v2.x)
	{
		if (v.x!=v1.x) throw std::runtime_error("Vertex isn't on line");
		DbgFloatVertex ret;
		ret.x=v.x+0.5;
		ret.y=v.y+0.5;
		return ret;
	}
	if (k==0)
	{
		if (v.y!=v1.y) throw std::runtime_error("Vertex isn't on line (2)");
		DbgFloatVertex ret;
		ret.x=v.x+0.5;
		ret.y=v.y+0.5;
		return ret;
	}

	int left=v.x;
	int right=v.x+1;
	int down=v.y;
	int up=v.y+1;
/*
 * (1) kx1+m=down => xa=(down-m)/k
 * (2) kx2+m=up => xb=(up-m)/k
 * (3) k*left+m=ya
 * (4) k*right+m=yb
 */
	Rational ya=k*left+m;
	Rational yb=k*right+m;
	double xa=(down-todouble(m))/todouble(k);
	double xb=(up-todouble(m))/todouble(k);
	double xmin=std::min(xa,xb);
	double xmax=std::max(xa,xb);
	DbgFloatVertex temp;
	temp.x=left+0.5;
	if (temp.x<xmin)
		temp.x=xmin;
	if (temp.x>xmax)
		temp.x=xmax;
	temp.y=todouble(k)*temp.x+todouble(m);
	return temp;
}

Line2::Line2(const Vertex& pv1, const Vertex& pv2) :
	v1(pv1),v2(pv2)
{
	Rational x1(2*v1.x+1,2);
	Rational y1(2*v1.y+1,2);
	Rational x2(2*v2.x+1,2);
	Rational y2(2*v2.y+1,2);

	if (x2!=x1)
		k = (y2 - y1) / (x2 - x1);
	else
		k = 0;
	m = y1 - k * x1;

}
Line2::Line2(const Vertex& pv1, const Vertex& pv2,
		const Rational& pk,const Rational& pm) :
	v1(pv1),v2(pv2),k(pk),m(pm)
{
}

Line2 Line2::reversed() const
{
	Line2 l=*this;
	std::swap(l.v1,l.v2);
	return l;
}

bool Line2::is_vertical() const
{
	if (v1.x==v2.x) return true;
	return false;
}
Rational Line2::get_k() const
{
	return k;
}
Rational Line2::get_m() const
{
	return m;
}

bool Line2::is_on_line(Vertex v)const
{
    int y1=0,y2=0;    
    if (get_yrange(v.x,y1,y2))
    {
        assert(y1<=y2);
        if (v.y>=y1 && v.y<=y2)
            return true;
    }
    return false;
}

bool Line2::get_yrange(int x,int& inty1,int& inty2) const
{
	if (v2.x<v1.x)
	{
		Line2 rev=this->reversed();
		return rev.get_yrange(x,inty1,inty2);
	}
	//printf("get_yrange(%d) lim: [%d,%d]\n",x,v1.x,v2.x);
	if (x<v1.x || x>v2.x)
		return false;
	if (v1.x==v2.x)
	{
		if (v1.x==x)
		{
			inty1=std::min(v1.y,v2.y);
			inty2=std::max(v1.y,v2.y);
			return true;
		}
		else
		{
			return false;
		}
	}
	assert(!is_vertical());
	Rational ry1=k * x + m;
	Rational ry2=k * (x+1) + m;
	//printf("ry1: %f, ry2: %f\n",todouble(ry1),todouble(ry2));
	inty1=ratfloor(ry1);
	inty2=ratfloor(ry2);
	/*if (k>0)
	{
		if (ry1==inty1)
			--inty1;
	}
	if (k<0)
	{
		if (ry2==inty2)
			--inty2;
	}*/

	int ylimlo=std::min(v1.y,v2.y);
	int ylimhi=std::max(v1.y,v2.y);
	//printf("Ylim hi/lo: %d/%d\n",ylimlo,ylimhi);

	if (inty1<ylimlo)
		inty1=ylimlo;
	if (inty1>ylimhi)
		inty1=ylimhi;
	if (inty2<ylimlo)
		inty2=ylimlo;
	if (inty2>ylimhi)
		inty2=ylimhi;
	if (inty1>inty2)
		std::swap(inty1,inty2);
	assert(inty1<=inty2);
	return true;
}

std::vector<Vertex> Line2::slow_all_vertices() const
{
	//printf("Get all vertices\n");
	int x=std::min(v1.x,v2.x);
	int xend=std::max(v1.x,v2.x);
	std::vector<Vertex> ret;
	for(;x<=xend;++x)
	{
		int y1=0,y2=0;

		if (!get_yrange(x,y1,y2))
			throw std::runtime_error("Unexpected error in slow_all_vertices");
		assert(y1<=y2);
		//printf("get yrange: %d: %d-%d\n",x,y1,y2);
		for(int y=y1;y<=y2;++y)
			ret.push_back(Vertex(x,y));
	}
	return ret;
}
std::vector<Vertex> Line2::intersection_points(const Line2& o) const
{
	//printf("New intersect------------------------\n");
	//printf("a: %s, b: %s\n",__repr__().c_str(),o.__repr__().c_str());
	const Rational kA = k;
	const Rational mA = m;
	const Rational kB = o.k;
	const Rational mB = o.m;

	const int minx=std::max(std::min((v1.x),(v2.x)),
					  std::min((o.v1.x),(o.v2.x)));
	const int maxx=std::min(std::max((v1.x),(v2.x)),
					  std::max((o.v1.x),(o.v2.x)));
	//printf("minx: %d, maxx: %d\n",minx,maxx);
	if (minx>maxx)
		return std::vector<Vertex>(); //no intersection
	int ix1=0,ix2=0;

	if (kA==kB && is_vertical()==o.is_vertical())
	{
		//printf("kA==kB\n");
		if (absolute(mA-mB)>1)
		{
			//printf("Parallel lines too far apart\n");
			return std::vector<Vertex>(); //no intersection
		}
		ix1=minx;
		ix2=maxx;
	}
	else if (v1.x==v2.x || o.v1.x==o.v2.x)
	{ //one of the lines is vertical
		ix1=minx;
		ix2=maxx;
	}
	else
	{
		assert(is_vertical()==false && o.is_vertical()==false);
		double dmA=todouble(mA);
		double dmB=todouble(mB);
		double dkA=todouble(kA);
		double dkB=todouble(kB);
		double ydiff=1.0+std::max(fabs(dkA),fabs(dkB));
		double fxa=(dmA - dmB - ydiff) / (dkB - dkA);
		double fxb=(dmA - dmB + ydiff) / (dkB - dkA);
		double size=fabs(dmA)+fabs(dmB)+fabs(dkA)+fabs(dkB);
		double fx1=std::min(fxa,fxb)-(1e-7)*size;
		double fx2=std::max(fxa,fxb)+(1e-7)*size;
		//printf("Ideal x intersect limits: %f %f\n",fx1,fx2);
		ix1=floor(fx1);
		ix2=floor(fx2);
		//printf("Minx/Maxx: %d %d\n",minx,maxx);
		//printf("Integer x intersect limits: %d %d\n",ix1,ix2);
		if (ix1<minx) ix1=minx;
		if (ix1>maxx) ix1=maxx;
		if (ix2<minx) ix2=minx;
		if (ix2>maxx) ix2=maxx;
	}
	int slopeA=0,slopeB=0;
	if (v1.y<v2.y) slopeA=1;
	if (v1.y>v2.y) slopeA=-1;
	if (v2.x<v1.x) slopeA*=-1;
	if (o.v1.y<o.v2.y) slopeB=1;
	if (o.v1.y>o.v2.y) slopeB=-1;
	if (o.v2.x<o.v1.x) slopeB*=-1;

	//printf("ix1: %d, ix2: %d, slopeA: %d, slopeB: %d\n",ix1,ix2,slopeA,slopeB);
	assert(ix2>=ix1);
	Vertex v[2];
	for(int iter=0;iter<2;++iter)
	{
		bool found=false;
		for(int i=0;i<=ix2-ix1;++i)
		{
			int x=0;
			if (iter==0)
				x=ix1+i;
			else
				x=ix2-i;
			int ya1=0,yb1=0,ya2=0,yb2=0;
			if (!get_yrange(x,ya1,ya2))
				throw std::runtime_error("Unexpected error line2.cpp #1");
			if (!o.get_yrange(x,yb1,yb2))
				throw std::runtime_error("Unexpected error line2.cpp #2");
			//printf("%d: yA: %d-%d, yB: %d-%d\n",x,ya1,ya2,yb1,yb2);
			assert(yb2>=yb1);
			assert(ya2>=ya1);
			if (yb1<=ya2 && yb2>=ya1)
			{ //overlaps
				int yhi=std::min(yb2,ya2);
				int ylo=std::max(yb1,ya1);
				//printf("yhi: %d, ilo: %d\n",yhi,ylo);
				if (slopeA>0)
				{
					if (iter==0)
						v[iter]=Vertex(x,ylo);
					else
						v[iter]=Vertex(x,yhi);

				}
				else
				if (slopeA<0)
				{
					if (iter==0)
						v[iter]=Vertex(x,yhi);
					else
						v[iter]=Vertex(x,ylo);
				}
				else
				{
					assert(yhi==ylo);
					v[iter]=Vertex(x,yhi);
				}
				found=true;
				break;
			}
		}
		if (!found)
		{
			//printf("No intersection found\n");
			return  std::vector<Vertex>(); //no intersection
		}
	}
	/*printf("!! The found vertices: %s %s\n",
			v[0].__repr__().c_str(),
			v[1].__repr__().c_str());*/
	std::vector<Vertex> ip;
	ip.push_back(v[0]);
	ip.push_back(v[1]);
	return ip;
}
std::vector<Line2> Line2::intersect(const Line2& o) const
{
	std::vector<Vertex> v=intersection_points(o);
	if (v.size()==0)
		return std::vector<Line2>();
	const Line2& la=*this;
	const Line2& lb=o;

	int a_p0=la.v1.taxidist(v[0]);
	int a_p1=la.v1.taxidist(v[1]);
	int b_p0=lb.v1.taxidist(v[0]);
	int b_p1=lb.v1.taxidist(v[1]);

	bool a_rev=a_p1<a_p0;
	bool b_rev=b_p1<b_p0;

	Line2 la_begin,lb_begin;
	Line2 la_middle,lb_middle;
	Line2 la_end,lb_end;
	la.split3(
			a_rev ? v[1] : v[0],
			a_rev ? v[0] : v[1],
			la_begin,la_middle,la_end);
	/*printf("Did split la %s into %s and %s and %s\n",
			la.__repr__().c_str(),
			la_begin.__repr__().c_str(),
			la_middle.__repr__().c_str(),
			la_end.__repr__().c_str()
			);*/
	lb.split3(
			b_rev ? v[1] : v[0],
			b_rev ? v[0] : v[1],
			lb_begin,lb_middle,lb_end);
	/*printf("Did split lb %s into %s and %s and %s\n",
			lb.__repr__().c_str(),
			lb_begin.__repr__().c_str(),
			lb_middle.__repr__().c_str(),
			lb_end.__repr__().c_str()
			);*/

	std::vector<Line2> out;
	out.push_back(la_middle);
	out.push_back(la_begin);
	out.push_back(lb_begin);
	out.push_back(la_end);
	out.push_back(lb_end);
	return out;
}
void Line2::split(const Vertex& x,Line2& a,Line2& b) const
{
	/*printf("Splitting line %s at %s\n",
			__repr__().c_str(),
			x.__repr__().c_str());*/
	int y1=0,y2=0;
	bool ret=get_yrange(x.x,y1,y2);
	//printf("Yrange at x=%d: %d-%d\n",x.x,y1,y2);
	assert(ret);
	assert(y1<=y2);
	assert(x.y>=y1 && x.y<=y2);
	a=*this;
	a.v2=x;
	b=*this;
	b.v1=x;
}
void Line2::split3(const Vertex& x,const Vertex& y,Line2& a,Line2& b,Line2& c) const
{
	/*printf("Splitting line %s at %s and %s\n",
			__repr__().c_str(),
			x.__repr__().c_str(),
			y.__repr__().c_str()
			);*/
	int begin_y1=0,begin_y2=0;
	bool begin_ret=get_yrange(x.x,begin_y1,begin_y2);
	//printf("Yrange at x=%d: %d-%d\n",x.x,begin_y1,begin_y2);
	int end_y1=0,end_y2=0;
	bool end_ret=get_yrange(y.x,end_y1,end_y2);
	//printf("Yrange at x=%d: %d-%d\n",y.x,end_y1,end_y2);
	assert(begin_ret);
	assert(begin_y1<=begin_y2);
	assert(end_ret);
	assert(end_y1<=end_y2);
	assert(x.y>=begin_y1 && x.y<=begin_y2);
	assert(y.y>=end_y1 && y.y<=end_y2);
	a=*this;
	a.v2=x;
	b=*this;
	b.v1=x;
	b.v2=y;
	c=*this;
	c.v1=y;
}

std::vector<Line2> Line2::intersect2(const Line2& o) const
{
	std::vector<Line2> unf=intersect(o);
	std::vector<Line2> ret;
	if (unf.size()==0)
		return ret;
	assert(unf.size()==5);
	if (unf[0].taxilen()==0) //middle is zero-size
	{
		if ((unf[1].taxilen()==0 || unf[3].taxilen()==0) &&
			(unf[2].taxilen()==0 || unf[4].taxilen()==0))
			return ret;
	}

	ret.push_back(unf[0]);
	for(int i=1;i<(int)unf.size();++i)
	{
		const Line2& l=unf[i];
		if (l.taxilen()!=0)
			ret.push_back(l);
	}
	return ret;
}



}
