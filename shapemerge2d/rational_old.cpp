#include "rational.h"
namespace shapemerge2d
{

Rational::Rational(int pa,int pb) : a(pa),b(pb)
{
}
int Rational::floor() const
{
	if ((a<0)==(b<0))
		return a/b;
	else
	{
		if (a<0)
			return ((-a)-b+1)/b;
		else
			return ((a)-(-b)+1)/(-b);
	}
}
int Rational::ceil() const
{
	if ((a<0)==(b<0))
		return (a+b-1)/b;
	else
	{
		if (a<0)
			return (-a)/b;
		else
			return (a)/(-b);
	}
}
Rational Rational::operator+(const Rational& o) const
{
	if (b==o.b)
	{
		return Rational(a+o.a,b);
	}
	return Rational(a*o.b+b*o.a,a.b*o.b);
}
Rational Rational::operator-(const Rational& o) const
{
	if (b==o.b)
	{
		return Rational(a-o.a,b);
	}
	return Rational(a*o.b-b*o.a,a.b*o.b);
}
Rational Rational::operator*(const Rational& o) const
{
	return Rational(a*o.a,b*o.b);
}

Rational Rational::operator/(const Rational& o) const
{
	return Rational(a*o.b,b*o.a);
}
Rational operator+(const Rational& a,int x)
{
	return Rational(a.a+x*a.b,a.b);
}
Rational operator-(const Rational& a,int x)
{
	return Rational(a.a-x*a.b,a.b);
}
Rational operator*(const Rational& a,int x)
{
	return Rational(a.a*x,a.b);
}
Rational operator/(const Rational& a,int x)
{
	return Rational(a.a,a.b*x);

}
Rational operator+(int x,const Rational& b)
{
	return Rational(b.a+x*b.b,b.b);
}
Rational operator-(int x,const Rational& b)
{
	return Rational(b.a-x*b.b,b.b);
}
Rational operator*(int x,const Rational& b)
{
	return Rational(b.a*x,b.b);
}
Rational operator/(int x,const Rational& b)
{
	return Rational(a*b.b,b.a);
}
}
