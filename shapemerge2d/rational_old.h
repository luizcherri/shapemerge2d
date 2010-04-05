#ifndef SHAPE_MERGE_RATIONAL_H_INCLUDED
#define SHAPE_MERGE_RATIONAL_H_INCLUDED
namespace shapemerge2d
{

class Rational
{
public:
	long a,b;
	Rational(int a,int b);
	int floor() const;
	int ceil() const;
	Rational operator+(const Rational& o) const;
	Rational operator-(const Rational& o) const;
	Rational operator*(const Rational& o) const;
	Rational operator/(const Rational& o) const;
	bool operator==(const Rational& a) const;
};
Rational operator+(const Rational& a,int b);
Rational operator-(const Rational& a,int b);
Rational operator*(const Rational& a,int b);
Rational operator/(const Rational& a,int b);
Rational operator+(int a,const Rational& b);
Rational operator-(int a,const Rational& b);
Rational operator*(int a,const Rational& b);
Rational operator/(int a,const Rational& b);

}

#endif
