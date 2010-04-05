#include "line.h"
#include "first_octant.h"
#include <stdexcept>
#include <stdio.h>
#include <algorithm>

namespace shapemerge2d
{
int floor2(int a, int b)
{
	return floor(Rational(a, b));
}
int ceil2(int a, int b)
{
	return ceil(Rational(a, b));
}
void Line::add_tag(int tag)
{
	tags.insert(tag);
}
void Line::add_tags(const std::set<int>& newtags)
{
	std::set<int>::iterator it = newtags.begin();
	for (; it != newtags.end(); ++it)
		tags.insert(*it);
}

int Line::grid_maxnorm() const
{
	int fx1 = floor(x1);
	int fy1 = floor(y1);
	int fx2 = floor(x2);
	int fy2 = floor(y2);
	int dx = std::abs(fx2 - fx1);
	int dy = std::abs(fy2 - fy1);
	return std::max(dx, dy);

}

int Line::grid_maxnorm(const FirstOctant& oct) const
{
	int fx1 = oct.floor_x(x1);
	int fy1 = oct.floor_y(y1);
	int fx2 = oct.floor_x(x2);
	int fy2 = oct.floor_y(y2);
	int dx = std::abs(fx2 - fx1);
	int dy = std::abs(fy2 - fy1);
	return std::max(dx, dy);
}

long floor(Rational r)
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
long ceil(Rational r)
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
Line::Line(const Rational& px1, const Rational& py1, const Rational& px2,
		const Rational& py2) :
	x1(px1), y1(py1), x2(px2), y2(py2)
{

}
Line::Line() :
	x1(0), y1(0), x2(0), y2(0)
{
}
Line::Line(const Vertex& v1, const Vertex& v2) :
	x1(v1.x * 2 + 1, 2), y1(v1.y * 2 + 1, 2), x2(v2.x * 2 + 1, 2), y2(v2.y * 2
			+ 1, 2)
{
}
Line::Line(int x1a, int x1b, int y1a, int y1b, int x2a, int x2b, int y2a,
		int y2b) :
	x1(x1a, x1b), y1(y1a, y1b), x2(x2a, x2b), y2(y2a, y2b)
{

}

Vertex Line::v1() const
{
	return Vertex(floor(x1), floor(y1));
}
Vertex Line::v2() const
{
	return Vertex(floor(x2), floor(y2));
}
Rational Line::get_x1() const
{
	return x1;
}
Rational Line::get_y1() const
{
	return y1;
}
Rational Line::get_x2() const
{
	return x2;
}
Rational Line::get_y2() const
{
	return y2;
}
Line Line::reversed() const
{
	return Line(x2, y2, x1, y1);
}

int Line::get_smallest_p_of_x(int x) const
{
	if (x < x1 || x > x2)
		throw std::runtime_error("x out of range");
	if (x1 == x2)
	{
		if (x == floor(x1))
			return 0;
		else
			throw std::runtime_error("x out of range (2)");
	}
	if (x2 < x1)
		throw std::runtime_error(
				"get_smallest_p_of_x only supports lines with x2>=x1");
	Rational delta_x = x2 - x1;
	Rational delta_y = y2 - y1;
	Rational rx = x - x1;
	if (delta_x > delta_y)
	{
		return floor(rx);
	}
	else
	{
		Rational ik = delta_x / delta_y;
		Rational steps = rx / ik;
		return ceil(steps);
	}

}

Vertex Line::get_impl(const Line& pself, int p)
{
	FirstOctant oct(pself);
	Line self = oct.transform_line(pself);
	int tlen = self.grid_maxnorm(oct);
	//printf("Untransfrm. line: %s\n",pself.__repr__().c_str());
	//printf("Transformed line: %s\n",self.__repr__().c_str());
	if (p == 0)
		return oct.untransform_vertex(Vertex(oct.floor_x(self.x1), oct.floor_y(
				self.y1)));
	if (p == tlen)
		return oct.untransform_vertex(Vertex(oct.floor_x(self.x2), oct.floor_y(
				self.y2)));
	Rational delta_x = self.x2 - self.x1;
	Rational delta_y = self.y2 - self.y1;
	//std::cout<<"deltas: "<<delta_x<<","<<delta_y<<std::endl;
	assert(delta_x > 0 && delta_y >= 0 && delta_y <= delta_x);
	Rational k = delta_y / delta_x;
	int base_x = floor(self.x1)+1;
	Rational rel_x = (base_x-self.x1)+(p-1)+Rational(1,2);
			//Rational(p - 1) + self.x1 - Rational(base_x) + Rational(1,
			//2);
	//printf("base_x = %d, p=%d, rel_x=%f, k: %f\n",base_x,p,rel_x.numerator()/(float)rel_x.denominator(),
	//		k.numerator()/float(k.denominator()));
	Rational yexact = self.y1 + rel_x * k;
	int y = oct.floor_y(yexact);
	//printf("yexact: %f\n",yexact.numerator()/float(yexact.denominator()));
	int x = oct.floor_x(self.x1 + rel_x);
	return oct.untransform_vertex(Vertex(x, y));
}

std::set<int> setunion(const std::set<int>& a, const std::set<int>& b)
{
	std::set<int> res;
	std::insert_iterator<std::set<int> > backii(res, res.end());
	std::set_union(a.begin(), a.end(), b.begin(), b.end(), backii);
	return res;
}
bool Line::pinch2(const Vertex& po, Line& oa, Line& ob,const FirstOctant& base)
{

	std::cout << "Pinching " << floatrepr() << " at " << po.__repr__()<< "\n";
	FirstOctant oct(*this,base);
	std::cout<<"Oct:"<<oct.__repr__()<<"\n";
	Line self = oct.transform_line(*this);
	Vertex o = oct.transform_vertex(po);
	std::cout << "Oct transformed Pinching " << self.floatrepr() << " at " << o.__repr__()<< "\n";


	Rational delta_x = self.x2 - self.x1;
	Rational delta_y = self.y2 - self.y1;
	printf("x1: %f\n",self.get_x1_inexact());
	printf("floor self x1: %d\n",oct.floor_x(self.get_x1()));
	int p = o.x - oct.floor_x(self.get_x1());

	int maxp = self.grid_maxnorm();
	printf("p: %d, maxp: %d\n", p, maxp);
	if (p < 0 || p > maxp)
	{
		std::cout<<"pinch op failed\n";
		return false;
	}
	if (p == 0)
	{
		oa = Line(v1(), v1());
		oa.add_tags(tags);
		ob = *this;
		return true;
	}
	if (p == maxp)
	{
		oa = *this;
		ob = Line(v2(), v2());
		ob.add_tags(tags);
		return true;
	}
	int base_x = floor(self.x1);
	Rational rel_x = Rational(p - 1) + self.x1 - Rational(base_x) + Rational(1,
			2);
	Rational k = delta_y / delta_x;
	assert(k.denominator() > INT_MIN && k.denominator() < INT_MAX);
	assert(k.numerator() > INT_MIN && k.numerator() < INT_MAX);

	Rational py = self.y1 + rel_x * k;
	Rational px = self.x1 + rel_x;
	//std::cout<<"first half: "<<self.x1<<","<<self.y1<<"--"<<px<<","<<py<<"\n";
	Line first_half(self.x1, self.y1, px, py);
	Line second_half(px, py, self.x2, self.y2);

#if 1
	Rational fh_delta_x = first_half.x2 - first_half.x1;
	Rational fh_delta_y = first_half.y2 - first_half.y1;
	Rational fh_k = fh_delta_y / fh_delta_x;
	assert(fh_k == k);
	Rational sh_delta_x = second_half.x2 - second_half.x1;
	Rational sh_delta_y = second_half.y2 - second_half.y1;
	Rational sh_k = sh_delta_y / sh_delta_x;
	assert(sh_k == k);
#endif

	first_half.add_tags(tags);
	second_half.add_tags(tags);

	oa = oct.untransform_line(first_half);
	ob = oct.untransform_line(second_half);
	//std::cout<<"Pinch result:"<<oa.__repr__()<<"/"<<ob.__repr__()<<"\n";
	return true;
}
int max3(int a, int b, int c)
{
	return std::max(std::max(a, b), c);
}
int min3(int a, int b, int c)
{
	return std::min(std::min(a, b), c);
}

static void yrange_helper(int x, Rational k, Rational m, int& y1, int& y2,const FirstOctant& oct)
{
	if (k==0)
	{
		y1=oct.floor_y(m);
		y2=oct.floor_y(m);
		return;
	}
	assert(k>=1 || k<=-1);
	Rational tempy1 = x * k + m; //including
	Rational tempy2 = (x + 1) * k + m; //excluding
	/*std::cout<<"x="<<x<<" ty1= "<<tempy1<<"\n";
	std::cout<<"x="<<x<<" ty2= "<<tempy2<<"\n";*/



	if (k>=1)
	{
		//including
		y1 = oct.ceil_y(tempy1 - Rational(oct.get_floorsigny(), 2));
		//3.4 -> 2.9 -> 3
		//3.5 -> 3.0 -> 3
		//3.6 -> 3.1 -> 4

		//excluding
		y2 = oct.ceil_y(tempy2 - Rational(oct.get_floorsigny(), 2))-1;
		//3.4 -> 2.9 -> 3 -> 2
		//3.5 -> 3.0 -> 3 -> 2
		//3.6 -> 3.1 -> 4 -> 3
	}
	else
	{
		//excluding
		y1 = oct.floor_y(tempy2 - Rational(oct.get_floorsigny(), 2))+1;

		//including
		y2 = oct.floor_y(tempy1 - Rational(oct.get_floorsigny(), 2));

	}

	assert(y2>=y1);

}
double Line::get_x1_inexact() const
{
	return x1.numerator()/double(x1.denominator());
}
double Line::get_y1_inexact() const
{
	return y1.numerator()/double(y1.denominator());
}
double Line::get_x2_inexact() const
{
	return x2.numerator()/double(x2.denominator());
}
double Line::get_y2_inexact() const
{
	return y2.numerator()/double(y2.denominator());
}

void Line::y_range(int x,int& py1,int& py2)
{
	if (x<floor(x1) || x>floor(x2))
		throw std::runtime_error("x out of range");
	if (floor(get_x2())==floor(get_x1()))
	{
		py1=floor(y1);
		py2=floor(y2);
		return;
	}
	Rational k = (get_y2() - get_y1()) / (get_x2() - get_x1());
	Rational m = get_y1() - k * get_x1();
	yrange_helper(x,k,m,py1,py2,Line(Vertex(0,0),Vertex(1,1)));
	if (py1<floor(y1))
		py1=floor(y1);
	if (py1>floor(y2))
		py1=floor(y2);

	if (py2<floor(y1))
		py2=floor(y1);
	if (py2>floor(y2))
		py2=floor(y2);
}

bool Line::intersect_impl(const Line& pa, const Line& pb, Line& inter,
		std::vector<Line>& out)
{
	FirstOctant oct(pa);
	printf("intersect_impl: Octant state: %s\n",oct.__repr__().c_str());
	Line a = oct.transform_line(pa);
	Line b = oct.transform_line(pb);
	if (b.get_x1() > b.get_x2())
	{
		b = b.reversed();
	}
	assert(a.get_x1()<=a.get_x2());
	assert(b.get_x1()<=b.get_x2());
	if (oct.floor_x(b.get_x1()) == oct.floor_x(b.get_x2()))
	{
		printf("x1==x2\n\n------------------------\n\n");
		int bx = oct.floor_x(b.get_x1());
		if (bx < oct.floor_x(a.get_x1()) || bx > oct.floor_x(a.get_x2()))
			return false;
		int a_p = bx - oct.floor_x(a.get_x1());
		//std::cout << "a:" << a.__repr__() << "\n";
		Vertex a_v = a.get(a_p);
		//std::cout << "a_v:" << a_v.__repr__() << "\n";
		//printf("bx: %d\n",bx);
		assert(a_v.get_x() == bx);
		int ay = a_v.get_y();
		if (ay < std::min(oct.floor_y(b.get_y1()), oct.floor_y(b.get_y2())) ||
				ay > std::max(oct.floor_y(b.get_y1()),oct.floor_y(b.get_y2())))
			return false;
		//std::cout << "Pinching a and b at " << a_v.__repr__() << "\n";
		Line l1, l2, l3, l4;
		printf("Pinching A\n");
		if (!a.pinch2(a_v, l1, l2, oct))
			return false;
		printf("Pinching B\n");
		if (!b.pinch2(a_v, l3, l4, oct))
			return false;
		inter = oct.untransform_line(Line(a_v, a_v));
		inter.add_tags(a.get_tags());
		inter.add_tags(b.get_tags());
		out.push_back(oct.untransform_line(l1));
		out.push_back(oct.untransform_line(l2));
		out.push_back(oct.untransform_line(l3));
		out.push_back(oct.untransform_line(l4));
		return true;
	}
	printf("x1!=x2\n\n------------------------\n");
	Rational kA=0;
	Rational kB=0;
	//for(int i=0;i<2;++i)
	{
		kA = (a.get_y2() - a.get_y1()) / (a.get_x2() - a.get_x1());
		kB = (b.get_y2() - b.get_y1()) / (b.get_x2() - b.get_x1());
		/*assert(kA<=1);
		if (kA<=kB)
			break;
		Line temp=a;
		a=b;
		b=temp;*/

	}

	Rational mA = a.get_y1() - kA * a.get_x1();
	Rational mB = b.get_y1() - kB * b.get_x1();
	/*std::cout << "a:" << a.__repr__() << "\n";
	std::cout << "b:" << b.__repr__() << "\n";*/
	int x1 = (kA==kB) ? std::max(oct.floor_x(a.get_x1()), oct.floor_x(b.get_x1()))
			:max3(	oct.floor_x(a.get_x1()), oct.floor_x(b.get_x1()),

				std::min(
					oct.floor_x((mA - mB - 1) / (kB - kA)),
					oct.floor_x((mA - mB + 1) / (kB - kA)) + 1
					)-1 //FULFIX!
			);

	int x2 =  (kA==kB) ? std::min(oct.floor_x(a.get_x2()), oct.floor_x(b.get_x2()))
			:min3(	oct.floor_x(a.get_x2()), oct.floor_x(b.get_x2()),

				std::max(
					oct.floor_x((mA - mB + 1) / (kB - kA)) + 1,
					oct.floor_x((mA - mB - 1) / (kB - kA))
					)+1 //FULFIX!
			);


	printf("From %d -> %d\n", x1, x2);
	printf("Line a: %s\n",a.floatrepr().c_str());
	printf("Line b: %s\n",b.floatrepr().c_str());
	Line first_a, first_b;
	Line rest_a, rest_b;
	Line last_a, last_b;
	Line middle_a, middle_b;

	for (int iter = 0; iter < 2; ++iter)
	{
		Vertex found_vertex;
		bool found = false;
		printf("Number of it: %d\n",x2-x1);
		for (int i = 0; i <= x2 - x1; ++i)
		{

///#error To do: Adapt line drawer to draw lines and gridded lines to visualize problems.
			int x = 0;
			if (iter == 0)
				x = x1 + i;
			else
				x = x2 - i;
			Rational rx(2 * x + oct.get_floorsignx(), 2);
			int yA = oct.floor_y(kA * rx + mA);
			printf("Looking for intersection at x=%d, yA=%d\n",x,yA);
			if ((x == oct.floor_x(b.get_x1()) && yA == oct.floor_y(b.get_y1())) ||
					(x == oct.floor_x(b.get_x2()) && yA == oct.floor_y(b.get_y2())))
			{
				printf("Found endpoint match: %d,%d\n",x,yA);
				found_vertex = Vertex(x, yA);
				found = true;
				break;
			}

			if (kB > 1 || kB<-1)
			{

				int yb1 = 0, yb2 = 0;
				yrange_helper(x, kB, mB, yb1, yb2, oct);
				printf("yB1: %d, yB2: %d\n",yb1,yb2);
				std::cout<<"yrange_helper output for "<<x<<": "<<yb1<<","<<yb2<<"\n";
				if (yA >= yb1 && yA <= yb2)
				{
					found_vertex = Vertex(x, yA);
					found = true;
					break;
				}
			}
			else
			{
				int yB=oct.floor_y(kB * rx + mB);
				printf("x: %d, yB:%d\n",x,yB);
				printf("x1: %d, x2: %d,yB(x-1):%d\n",
						x1,x2,
						oct.floor_y(kB * (rx-1) + mB));

				if (yA == yB)
				{
					found_vertex = Vertex(x, yA);
					found = true;
					break;
				}
			}
		}
		if (found == false)
			return false; //no intersection.
		if (iter == 0)
		{
			if (!a.pinch2(found_vertex, first_a, rest_a, oct))
				return false;
			if (!b.pinch2(found_vertex, first_b, rest_b, oct))
				return false;
		}
		else
		{
			if (!rest_a.pinch2(found_vertex, middle_a, last_a, oct))
				return false;
			if (!rest_b.pinch2(found_vertex, middle_b, last_b, oct))
				return false;
		}
	}
	out.push_back(oct.untransform_line(first_a));
	out.push_back(oct.untransform_line(first_b));
	middle_a.add_tags(middle_b.tags);
	//out.push_back(oct.untransform_line(middle_a));
	inter = oct.untransform_line(middle_a);
	out.push_back(oct.untransform_line(last_a));
	out.push_back(oct.untransform_line(last_b));

	return true;
}

}
