#ifndef SHAPE_MERGE_LINE_H_INCLUDED
#define SHAPE_MERGE_LINE_H_INCLUDED

#include "shapemerge2d/vertex.h"
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <stdexcept>
#include <limits.h>

namespace shapemerge2d
{
	int sign(int val);
	class IntersectionResult;
	class Vector;
	class Line
	{
		///Amount in reservoir at start of line.
		///-1 means not calculated yet.
		int res_start;
		///0 means start moving in x-direction
		///1 means start moving in y-direction
		int startdir;
		///Actual coordinates of line end points.
		Vertex v1,v2;
		friend class LinePointGenerator;
	public:
		///Will construct a zero-length line at origo
		Line();
		///Line between the two vertices given. The line
		///does not include the v2 vertex. It does include
		///the v1.
		static Line between(const Vertex& v1,const Vertex& v2,int res_start=-1,int startdir=0);
		Line reversed() const;
		int x1()const {return v1.x;}
		int x2()const {return v2.x;}
		int y1()const {return v1.y;}
		int y2()const {return v2.y;}
		///Start point of line
		Vertex get_v1() const;
		///End point of line
		Vertex get_v2() const;

		Vector delta() const;

#ifndef SWIG
		bool ideal_intersection2(const Line& b,Vertex& where);
#endif
		///returns Vertex(-1e9,-1e9) if no intersection
		Vertex ideal_intersection(const Line& b)
		{
			Vertex r;
			if (!ideal_intersection2(b,r))
				r=Vertex(-1e9,-1e9);
			return r;
		}
		static bool plausible_integer_intersection2(const Line& a,const Line& b,Vertex& where);
		static Vertex plausible_integer_intersection(const Line& a,const Line& b)
		{
			Vertex r;
			if (!plausible_integer_intersection2(a,b,r))
				r=Vertex(-1e9,-1e9);
			return r;
		}

		///Find intersection point between lines, if exists.
		///@return true if lines exist, otherwise false.
		IntersectionResult intersect(const Line& o) const;
	};
	struct IntersectionResult
	{
		///first half of line a.
		Line a1;bool hasa1;
		///second half of line a.
		Line a2;bool hasa2;
		///first half of line a.
		Line b1;bool hasb1;
		///second half of line a.
		Line b2;bool hasb2;
		///overlapping part between both lines.
		Line o;
		///if null, there is in fact no overlap at all.
		bool overlap;
		IntersectionResult()
		{
			overlap=false;
			hasa1=hasa2=hasb1=hasb2=false;
		}
	};
	/*
	 * ax+by=c
	 * dx+ey=f
	 * solve for x and y
	*/
	bool solve_ineq(
			double a,double b,double c,
			double d,double e,double f,
			double& x,double& y
			);
	class FirstOctant
	{
		int signx;
		int signy;
		bool swapxy;
	public:
		FirstOctant(const Line& calibration);
		Vertex transform_vertex(const Vertex& v);
		Vector transform_vector(const Vector& v);
		Line transform_line(const Line& v);
		Vertex untransform_vertex(const Vertex& v);
		Vector untransform_vector(const Vector& v);
		Line untransform_line(const Line& v);
	};
	struct PyNextRes
	{

		Vertex v;
		bool b;

		Vertex get_vertex();
		bool get_bool();
	};
	class LinePointGenerator
	{
		///Start x position
		int x1;
		///Start y position
		int y1;
		///End x position
		int x2;
		///End y position
		int y2;
		///capacity of x-step reserve
		int cap_x;
		///capacity of y-step reserve
		int cap_y;
		///amount to take from x-step reserve each step
		int take_x;
		///amount to take from y-step reserve each step
		int take_y;
		///Starting reserves, x and y
		int start_res_x,start_res_y;

		///Step size in x and y.
		int step_x,step_y;
		int startdir; //0 == x, 1 == y
		int numsteps;
		int ymajor;

	public:
		int get_num_steps()
		{
			return numsteps;
		}
		IntersectionResult intersect(const LinePointGenerator& b)
		{

			return IntersectionResult();
		}
		/**
		 * Return the first p-value for which the
		 * line will enter the column with x-coordinate 'x'
		 * returns INT_MIN if line does not reach x.
		 */
		int get_first_p_for(int x)
		{
			if (x<x1 || x>x2)
				return INT_MIN;
			if (x1>x2)
				throw std::runtime_error("get_first_p_for only works for lines where x2>=x1");
			if (x1==x2)
			{
				assert(x==x1);
				return 0;
			}
			int needed_x_incs=x-x1;
			printf("NEeded x_incs: %d\n",needed_x_incs);
			int temp_res_x=(needed_x_incs*cap_x);
			printf("res_x burned: %d\n",temp_res_x);
			int x_steps=(temp_res_x+start_res_x)/take_x;
			printf("x-steps: %d\n",x_steps);
			int p=2*x_steps;
			if (startdir==0)
				p-=1;
			return p;
		}
		Line reversed() const;
		Vertex get_by_param_impl(int p,int& res,int& enddir) const
		{
			if (p>numsteps)
				throw std::runtime_error("p is out of range");
			//printf("p:%d\n",p);
			int x_steps=p/2;
			int y_steps=p/2;
			if (p&1)
			{
				if (startdir==1)
					y_steps+=1;
				else
					x_steps+=1;
				//printf("Startdir: %d\n",startdir);
				assert(startdir==0 || startdir==1);
				//printf(startdir==1 ? "starts with y-move\n" : "starts with x-move\n");
			}
			//printf("Steps: %d,%d, startres: %d,%d, cap: %d,%d\n",x_steps,y_steps,start_res_x,start_res_y,cap_x,cap_y);
			int temp_res_x=start_res_x-take_x*x_steps;
			int temp_res_y=start_res_y-take_y*y_steps;

			//printf("temp_res: %d(%d),%d(%d)\n",temp_res_x,cap_x,temp_res_y,cap_y);

			int needed_x_incs=((-temp_res_x)+cap_x)/cap_x;
			int needed_y_incs=((-temp_res_y)+cap_y)/cap_y;
			//printf("Needed incs: %d,%d\n",needed_x_incs,needed_y_incs);
			int x=x1+step_x*needed_x_incs;
			int y=y1+step_y*needed_y_incs;

			int fin_res_x=temp_res_x+needed_x_incs*cap_x;
			assert(fin_res_x>0);
			int fin_res_y=temp_res_y+needed_y_incs*cap_y;
			assert(fin_res_y>0);

			if (ymajor)
				res=fin_res_x;
			else
				res=fin_res_y;

			enddir=(startdir+p)&1;
			if (p==0)
			{
				assert(x==x1);
				assert(y==y1);
			}
			return Vertex(x,y);
		}
		Vertex get_by_param(int p) const
		{
			int dummy=0;
			int dummy2=0;
			return get_by_param_impl(p,dummy,dummy2);
		}

		~LinePointGenerator(){};
		bool operator==(const LinePointGenerator& lg);

		LinePointGenerator copy()
		{
			return *this;
		}
		LinePointGenerator(const Line& line)
		{
			startdir=line.startdir;
			x1=line.get_v1().x;
			y1=line.get_v1().y;
			x2=line.get_v2().x;
			y2=line.get_v2().y;
			Vector delta=line.get_v2()-line.get_v1();
			if (delta.x==0 && delta.y==0)
			{
				step_x=0;
				step_y=0;
				cap_x=10;
				take_x=10;
				cap_y=10;
				take_y=10;
				start_res_x=5;
				start_res_y=5;
				numsteps=0;
				ymajor=0;
			}
			else
			{
				step_x=sign(delta.x);
				step_y=sign(delta.y);
				if ((std::abs)(delta.x)>(std::abs)(delta.y))
				{
					cap_x=10;
					take_x=10;
					cap_y=(std::abs)(delta.x);
					take_y=(std::abs)(delta.y);
					start_res_x=5;
					start_res_y=line.res_start;
					if (start_res_y<=0)
						start_res_y=(cap_y+1)/2;
					if (start_res_y>cap_y)
						start_res_y=cap_y;
					numsteps=(std::abs)(delta.x)*2;
					ymajor=0;
				}
				else
				{
					cap_x=(std::abs)(delta.y);
					take_x=(std::abs)(delta.x);
					cap_y=10;
					take_y=10;
					start_res_x=line.res_start;
					start_res_y=5;
					if (start_res_x<=0)
						start_res_x=(cap_x+1)/2;
					if (start_res_x>cap_x)
						start_res_x=cap_x;
					numsteps=(std::abs)(delta.y)*2;
					ymajor=1;
				}
				assert(start_res_x>0);
				assert(start_res_y>0);
			}

		}


#if 0
		bool next(Vertex& v)
		{
			for(;;)
			{
				if (p>max_p)
					return false;
				++p;
				if (p&1)
				{
					res_x-=take_x;
					if (res_x<=0)
					{
						v=Vertex(x,y);
						x+=step_x;
						res_x+=cap_x;
						break;
					}
				}
				else
				{
					res_y-=take_y;
					if (res_y<=0)
					{
						v=Vertex(x,y);
						y+=step_y;
						res_y+=cap_y;
						break;
					}
				}
			}
			return true;
		}
		/**
		 * This needs to be tested rigorously, since it is easy
		 * to make off-by-one errors here!
		 */
		void forward(int dp)
		{
			if (p+dp>max_p)
				throw std::runtime_error("Can't forward that far");
			int start_even=!!(p&1);
			int new_odd=(dp+(!start_even))/2;
			int new_even=(dp+start_even)/2;
			printf("forward: Number of odd cycles: %d\n",new_odd);
			printf("forward: Number of even cycles: %d\n",new_even);
			p+=dp;
			int predict_res_x=res_x-new_odd*take_x;
			int predict_res_y=res_y-new_even*take_y;
			int num_x_steps=(predict_res_x-cap_x)/cap_x;
			int predict_x=x+step_x*-num_x_steps;
			int num_y_steps=(predict_res_y-cap_y)/cap_y;
			int predict_y=y+step_y*-num_y_steps;
			predict_res_x+=cap_x*num_x_steps;
			predict_res_y+=cap_y*num_y_steps;

#if 0
			int predict_res_x=res_x+new_odd*take_x;
			int predict_res_y=res_y+new_even*take_y;

			int num_x_steps=(predict_res_x)/cap_x;
			int predict_x=x-step_x*num_x_steps;

			int num_y_steps=(predict_res_y)/cap_y;
			int predict_y=y-step_y*num_y_steps;

			predict_res_x-=cap_x*num_x_steps;
			predict_res_y-=cap_y*num_y_steps;

#endif

			for(int i=0;i<new_odd;++i)
			{
				res_x-=take_x;
				if (res_x<=0)
				{
					x+=step_x;
					res_x+=cap_x;
				}
			}
			printf("Predict x: %d (x: %d)\n",predict_x,x);
			assert(x==predict_x);
			for(int i=0;i<new_even;++i)
			{
				res_y-=take_y;
				if (res_y<=0)
				{
					y+=step_y;
					res_y+=cap_y;
				}
			}
			printf("Predict y: %d (y: %d)\n",predict_y,y);
			assert(y==predict_y);
		}


//#error rewrite 'next', 'forward' and 'rewind'
		void rewind(int dp)
		{
			if (p-dp<0)
				throw std::runtime_error("Can't rewind that far");


			int start_even=!(p&1); //we evaluate, then subtract, when reversing
			int new_odd=(dp+(!start_even))/2;
			int new_even=(dp+start_even)/2;
			printf("rewind: Number of odd cycles: %d\n",new_odd);
			printf("rewind: Number of even cycles: %d\n",new_even);
			p-=dp;
			int predict_res_x=res_x+new_odd*take_x;
			int predict_res_y=res_y+new_even*take_y;

			int num_x_steps=(predict_res_x)/cap_x;
			int predict_x=x-step_x*num_x_steps;

			int num_y_steps=(predict_res_y)/cap_y;
			printf("Num x-steps: %d, num y-steps: %d\n",num_x_steps,num_y_steps);
			int predict_y=y-step_y*num_y_steps;

			predict_res_x-=cap_x*num_x_steps;
			predict_res_y-=cap_y*num_y_steps;

			for(int i=0;i<new_odd;++i)
			{
				res_x+=take_x;
				if (res_x>0)
				{
					x-=step_x;
					res_x-=cap_x;
				}
			}
			printf("Predict x: %d (x: %d)\n",predict_x,x);
			printf("Predict res_x: %d (res_x: %d)\n",predict_res_x,res_x);
			assert(x==predict_x);
			assert(res_x==predict_res_x);
			for(int i=0;i<new_even;++i)
			{
				res_y+=take_y;
				if (res_y>0)
				{
					y-=step_y;
					res_y-=cap_y;
				}
			}
			printf("Predict y: %d (y: %d)\n",predict_y,y);
			printf("Predict res_y: %d (res_y: %d)\n",predict_res_y,res_y);
			assert(y==predict_y);
			assert(res_y==predict_res_y);
		}

		static void intersect_driver(
				Line& pa,
				Line& pb)
		{
#if 0
			FirstOctant oct(pa);
			Line a=oct.transform_line(pa);
			Line b=oct.transform_line(pb);
			if (b.x1()==b.x2())
			{ //b is a vertical line
					if (a.x1()<=b.x1() && a.x2()>=b.x1())
					{ //b is vertical line, not a.
						LinePointGenerator ag(a);
						ag.fast_forward_to_x(b.x1());
						Vertex v=ag.next();
						assert(v.get_x()==b.x1());
						int min_y=std::min(b.y1(),b.y2());
						int max_y=std::max(b.y1(),b.y2());
						if (v.get_y()>=min_y and v.get_y()<=max_y)
						{
							Vertex v1;
							Vertex v2=v1;
							if (ag.next(v2))
								if (v2.get_x()!=v.get_x()) //check if there are two pixels overlapping
									v2=v1; //only one pixel overlap


						}
					}
					else
					{ //two vertical lines, not on same x-coordinate

					}
			}
			for(;;)
			{


			}
#endif
		}

#endif



	};

}
#endif
