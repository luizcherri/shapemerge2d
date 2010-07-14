#ifndef SHAPE_MERGE_VECTOR_INCLUDED
#define SHAPE_MERGE_VECTOR_INCLUDED
#include <sstream>

namespace shapemerge2d
{

	struct Vector
	{
		int x,y;
		Vector(int x,int y);
		Vector();
		int get_x()const{return x;}
		int get_y()const{return y;}
		~Vector(){}
		Vector operator-(const Vector& o) const;
		Vector operator+(const Vector& o) const;
		bool operator==(const Vector& o) const;
		int64_t scalarprod(const Vector& o) const
		{
		    return x*o.x+y*o.y;
		}
		std::string __repr__() const
		{
			std::ostringstream os;
			os<<"Vector("<<x<<","<<y<<")";
			return os.str();
		}
		int taxilength() const;
		int maxnorm() const;
		double approxlength() const;
	};
}
#endif
