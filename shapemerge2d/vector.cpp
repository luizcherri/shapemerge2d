#include "shapemerge2d/vector.h"
#include <algorithm>
#include <math.h>

namespace shapemerge2d
{

	Vector::Vector(int px,int py) : x(px),y(py)
	{
	}
	Vector::Vector() : x(0),y(0)
	{
	}
	int Vector::taxilength() const
	{
		return std::abs(x)+std::abs(y);
	}
	int Vector::maxnorm() const
	{
		return std::max(std::abs(x),std::abs(y));
	}
	bool Vector::operator==(const Vector& o) const
	{
		return x==o.x && y==o.y;
	}
	Vector Vector::operator-(const Vector& o) const
	{
		return Vector(x-o.x,y-o.y);
	}
	Vector Vector::operator+(const Vector& o) const
	{
		return Vector(x+o.x,y+o.y);
	}
	double Vector::approxlength() const
	{
	    return sqrt(x*(double)x+y*(double)y);
	}

}
