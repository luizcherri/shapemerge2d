#include "shapemerge2d/line.h"
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>


namespace shapemerge2d {

bool solve_ineq(
		double a,double b,double c,
		double d,double e,double f,
		double& x,double& y
		)
{
	/*
	ax+by=c
	dx+ey=f

	y=(f-dx)/e
	ax+b(f-dx)/e=c
	ax+bf/e-bdx/e=c
	ax-bdx/e+bf/e=c
	x(a-bd/e)=c-bf/e

	x=(c-by)/a
	d*(c-by)/a+ey=f
	dc/a-dby/a+ey=f
	y(e-db/a)+dc/a=f
	y=(f-dc/a)/(e-db/a)
	y=(af-dc)/(ae-db)

	x=(c*e-b*f)/(a*e-b*d)
	y=(f*a-d*c)/(a*e-d*b)
	*/
	double q=a*e-b*d;
	if (q==0.0)
	{
		x=0;y=0;
		return false;
	}
	x=(c*e-b*f)/q;
	y=(a*f-d*c)/q;
	return true;

}

FirstOctant::FirstOctant(const Line& calib) :
		signx(1),signy(1),swapxy(false)
{
	int dx=calib.x2()-calib.x1();
	int dy=calib.y2()-calib.y1();
	int adx=std::abs(dx);
	int ady=std::abs(dy);
	if (ady>adx)
		swapxy=true;
	if (dx<0) signx=-1;
	if (dy<0) signy=-1;
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
	Vertex v1=transform_vertex(v.get_v1());
	Vertex v2=transform_vertex(v.get_v2());
	return Line::between(v1,v2);
}

Vertex FirstOctant::untransform_vertex(const Vertex& v)
{
	Vector t=untransform_vector(Vector(v.x,v.y));
	return Vertex(t.x,t.y);
}

Vector FirstOctant::untransform_vector(const Vector& v)
{
	int x=v.x;
	int y=v.y;
	if (swapxy)
		std::swap(x,y);
	x*=signx;
	y*=signy;
	return Vector(x,y);

}

Line FirstOctant::untransform_line(const Line& v)
{
	Vertex v1=untransform_vertex(v.get_v1());
	Vertex v2=untransform_vertex(v.get_v2());
	return Line::between(v1,v2);
}
Line LinePointGenerator::reversed() const
{
	int res=0;
	int enddir=0;
	//printf("Reversed, numsteps=%d\n",numsteps);
	get_by_param_impl(numsteps,res,enddir);
	if (ymajor)
	{
		assert(res>=1 && res<=cap_x);
		res=cap_x-res+1;
	}
	else
	{
		assert(res>=1 && res<=cap_y);
		res=cap_y-res+1;
	}
	assert(enddir==0 || enddir==1);
	enddir=!enddir; //if next forward-dir would have been enddir, next reverse-dir must be !enddir
	//printf(enddir==0 ? "last move was x-move\n" : "last move was y-move\n");
	return Line::between(Vertex(x2,y2),Vertex(x1,y1),res,enddir);
}
Line Line::reversed() const
{
	LinePointGenerator lg(*this);
	return lg.reversed();
}

bool Line::plausible_integer_intersection2(const Line& pla,const Line& plb,Vertex& where)
{
///	static const int epsilon=1e-7;
	FirstOctant oct(pla);
	Line la=oct.transform_line(pla);
	Line lb=oct.transform_line(plb);
	assert(la.x1()<=la.x2());
	if (lb.x1()>lb.x2())
		lb=lb.reversed();
	Vector delta1=la.delta();
	Vector delta2=lb.delta();
	Vertex off1=la.get_v1();
	Vertex off2=lb.get_v1();
	/*
	ax+by=c
	dx+ey=f
	*/
	double c=off2.x-off1.x;
	double f=off2.y-off1.y;
	double a=delta1.x;
	double b=-delta2.x;
	double d=delta1.y;
	double e=-delta2.y;
	double p1=0;
	double p2=0;
	if (!solve_ineq(a,b,c,d,e,f,p1,p2))
		return false;
	//printf("Solved equation: %f, %f\n",p1,p2);
	double approx_x=(off1.x+p1*delta1.x);
	double approx_y=(off1.y+p1*delta1.y);
	if (delta2.x==0)
	{
		throw std::runtime_error("Vertical lines not implemented yet!");
		//can be implemented by simply switching a and b,
		//unless both are vertical. But if both are vertical,
		//then the question of overlap/intersection is very easy.
		return true;
	}
	double northmove1=delta1.y/double(delta1.x);
	double northmove2=delta2.y/double(delta2.x);

	printf("North move1: %f\n",northmove1);
	printf("North move2: %f\n",northmove2);
	double dist_to_roof=1.0-fmod(approx_y,1.0);
	double roof_intersect_x1=dist_to_roof/northmove1;
	double roof_intersect_x2=dist_to_roof/northmove2;
	printf("Line 1 will intersect roof at x=%f\n",roof_intersect_x1);
	printf("Line 2 will intersect roof at x=%f\n",roof_intersect_x2);

	where=oct.untransform_vertex(Vertex(approx_x,approx_y));
	return true;
}

bool Line::ideal_intersection2(const Line& other,Vertex& where)
{
	static const int epsilon=1e-7;
	Vector delta1=delta();
	Vector delta2=other.delta();
	Vertex off1=get_v1();
	Vertex off2=other.get_v1();
	/*
	ax+by=c
	dx+ey=f
	*/
	double c=off2.x-off1.x;
	double f=off2.y-off1.y;
	double a=delta1.x;
	double b=-delta2.x;
	double d=delta1.y;
	double e=-delta2.y;
	double p1=0;
	double p2=0;
	if (!solve_ineq(a,b,c,d,e,f,p1,p2))
		return false;
	//printf("Solved equation: %f, %f\n",p1,p2);
	if (p1<-epsilon || p1>1+epsilon || p2<-epsilon || p2>1+epsilon)
		return false;
	int approx_x=(int)(0.5+off1.x+p1*delta1.x);
	int approx_y=(int)(0.5+off1.y+p1*delta1.y);
	where=Vertex(approx_x,approx_y);
	return true;
}

bool LinePointGenerator::operator==(const LinePointGenerator& o)
{
	return
		x1==o.x1 &&
		y1==o.y1 &&
		cap_x==o.cap_x &&
		cap_y==o.cap_y &&
		take_x==o.take_x &&
		take_y==o.take_y &&
		start_res_x==o.start_res_x &&
		start_res_y==o.start_res_y &&
		step_x==o.step_x &&
		step_y==o.step_y &&
		startdir==o.startdir;
}


	int sign(int val)
	{
		if (val<0) return -1;
		return 1;
	}
	Vertex PyNextRes::get_vertex()
	{
		return v;
	}
	bool PyNextRes::get_bool()
	{
		return b;
	}
	Vertex Line::get_v1() const
	{
		return v1;
	}
	Vertex Line::get_v2() const
	{
		return v2;
	}


	Line::Line():res_start(0),startdir(0)
	{
	}

	Line Line::between(const Vertex& v1,const Vertex& v2,int res_start,int startdir)
	{
		Line l;
		l.v1=v1;
		l.v2=v2;
		l.res_start=res_start;
		l.startdir=startdir;
		return l;
	}

	IntersectionResult Line::intersect(const Line& o) const
	{

	/*
	 * Sample case:
	 *
	 * o1____
	 *       \____
	 *            \___
	 *                o2
	 * dx = 14, dy = 3
	 * d = 14/3 = 4 2/3
	 * start with 14/2. Remove 3 every x-step. Step in y when
	 * below or equal to 0, then add 14 to reservoir.
	 */




	/*
	Written by Ingela: Kvack!
	*/




		return IntersectionResult();
	}

	Vector Line::delta() const
	{
		return v2-v1;
	}

}

