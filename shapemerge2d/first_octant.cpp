#include "first_octant.h"
#include "line.h"
#include "vertex.h"
#include "vector.h"
#include <stdio.h>

namespace shapemerge2d
{

FirstOctant::FirstOctant(const Line& calib,const FirstOctant& base) :
signx(1),signy(1),swapxy(false),x_floor(false),y_floor(false)
{
	Rational dx=calib.get_x2()-calib.get_x1();
	Rational dy=calib.get_y2()-calib.get_y1();
	Rational adx=boost::abs(dx);
	Rational ady=boost::abs(dy);
	if (ady>adx)
		swapxy=true;
	if (dx<0) signx=-1;
	if (dy<0) signy=-1;
	bool temp_x_floor=false;
	if ((!swapxy && signx==-1) || (swapxy && signy==-1))
		temp_x_floor=true;
	bool temp_y_floor=false;
	if ((!swapxy && signy==-1) || (swapxy && signx==-1))
		temp_y_floor=true;
	if (swapxy)
	{
		if (temp_x_floor)
			x_floor=!base.y_floor;
		else
			x_floor=base.y_floor;
		if (temp_y_floor)
			y_floor=!base.x_floor;
		else
			y_floor=base.x_floor;
	}
	else
	{
		if (temp_x_floor)
			x_floor=!base.x_floor;
		else
			x_floor=base.x_floor;
		if (temp_y_floor)
			y_floor=!base.y_floor;
		else
			y_floor=base.y_floor;
	}
}
FirstOctant::FirstOctant(const Line& calib) :
		signx(1),signy(1),swapxy(false),x_floor(false),y_floor(false)
{
	Rational dx=calib.get_x2()-calib.get_x1();
	Rational dy=calib.get_y2()-calib.get_y1();
	Rational adx=boost::abs(dx);
	Rational ady=boost::abs(dy);
	if (ady>adx)
		swapxy=true;
	if (dx<0) signx=-1;
	if (dy<0) signy=-1;
	x_floor=false;
	if ((!swapxy && signx==-1) || (swapxy && signy==-1))
		x_floor=true;
	y_floor=false;
	if ((!swapxy && signy==-1) || (swapxy && signx==-1))
		y_floor=true;
}
std::string FirstOctant::__repr__() const
{
	std::ostringstream str;
	str<<"signx: "<<signx<<" signy: "<<signy<<" swapxy: "<<int(swapxy)<<" x-floor: "<<x_floor<<" y-floor: "<<y_floor ;
	return str.str();
}

Vertex FirstOctant::transform_vertex(const Vertex& v)
{
	Vector t=transform_vector(Vector(v.x,v.y));
	return Vertex(t.x,t.y);
}
Vector FirstOctant::transform_vector(const Vector& v)
{
	int x=v.x;
	int y=v.y;
	x*=signx;
	y*=signy;
	if (swapxy)
		std::swap(x,y);
	return Vector(x,y);
}

Line FirstOctant::transform_line(const Line& v)
{
	Rational x1=signx*v.get_x1();
	Rational y1=signy*v.get_y1();
	Rational x2=signx*v.get_x2();
	Rational y2=signy*v.get_y2();
	Line l;
	if (swapxy)
		l=Line(y1,x1,y2,x2);
	else
		l=Line(x1,y1,x2,y2);
	l.add_tags(v.get_tags());
	return l;
}

Vertex FirstOctant::untransform_vertex(const Vertex& v)
{
	Vector t=untransform_vector(Vector(v.x,v.y));
	return Vertex(t.x,t.y);
}

Vector FirstOctant::untransform_vector(const Vector& v)
{
	//printf("Untransforming: %s\n",v.__repr__().c_str());
	int x=v.x;
	int y=v.y;
	if (swapxy)
		std::swap(x,y);
	x*=signx;
	y*=signy;
	return Vector(x,y);

}
int FirstOctant::floor_x(const Rational& x) const
{
	if (x_floor)
		return ceil(x);
	return floor(x);
}
int FirstOctant::floor_y(const Rational& y) const
{
	if (y_floor)
		return ceil(y);
	return floor(y);
}
int FirstOctant::ceil_x(const Rational& x) const
{
	if (x_floor)
		return floor(x);
	return ceil(x);
}
int FirstOctant::ceil_y(const Rational& y) const
{
	if (y_floor)
		return floor(y);
	return ceil(y);
}

Line FirstOctant::untransform_line(const Line& v)
{
#if 0
	printf("FirstOctant: signx:%d, signy: %d, swapxy: %d\n",
			(int)signx,(int)signy,(int)swapxy);
#endif
	Rational x1,y1,x2,y2;
	if (swapxy)
	{
		x1=signx*v.get_y1();
		y1=signy*v.get_x1();
		x2=signx*v.get_y2();
		y2=signy*v.get_x2();
	}
	else
	{
		x1=signx*v.get_x1();
		y1=signy*v.get_y1();
		x2=signx*v.get_x2();
		y2=signy*v.get_y2();
	}
	Line l=Line(x1,y1,x2,y2);
	l.add_tags(v.get_tags());
	//std::cout<<"Untransformed "<<v.__repr__()<<" into "<<l.__repr__()<<"\n";
	return l;
}
}
