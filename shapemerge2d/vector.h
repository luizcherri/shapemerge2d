#ifndef SHAPE_MERGE_VECTOR_INCLUDED
#define SHAPE_MERGE_VECTOR_INCLUDED
#include <sstream>

namespace shapemerge2d
{

	/**
	 * The vector class represents a vector with integer components.
	 * It represents the differences between two vertices.
	 */
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
		        return int64_t(x)*o.x+int64_t(y)*o.y;
		}
		std::string __repr__() const
		{
			std::ostringstream os;
			os<<"Vector("<<x<<","<<y<<")";
			return os.str();
		}
		int taxilength() const;
		double approxlength() const;
		int maxnorm() const;
	};
}
#ifndef SWIG
inline std::ostream& operator<<(std::ostream& os,const shapemerge2d::Vector& x)
{
	os<<x.__repr__();
	return os;
}
#endif

#endif
