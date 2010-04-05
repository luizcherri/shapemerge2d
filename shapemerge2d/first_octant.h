#ifndef SHAPE_MERGE_FIRST_OCTANT_H_INCLUDED
#define SHAPE_MERGE_FIRST_OCTANT_H_INCLUDED
#include <boost/rational.hpp>
namespace shapemerge2d
{
class Line;
class Vertex;
class Vector;
typedef boost::rational<int64_t> Rational;

class FirstOctant
{
	int signx;
	int signy;
	bool swapxy;

	bool x_floor;
	bool y_floor;
public:
	int get_floorsignx()const{return x_floor ? -1 : 1;}
	int get_floorsigny()const{return y_floor ? -1 : 1;}
	std::string __repr__() const;
	FirstOctant(const Line& calibration);
	FirstOctant(const Line& calibration,const FirstOctant& base);
	Vertex transform_vertex(const Vertex& v);
	Vector transform_vector(const Vector& v);
	Line transform_line(const Line& v);
	Vertex untransform_vertex(const Vertex& v);
	Vector untransform_vector(const Vector& v);
	Line untransform_line(const Line& v);
	int floor_x(const Rational& r) const;
	int floor_y(const Rational& r) const;
	int ceil_x(const Rational& r) const;
	int ceil_y(const Rational& r) const;
};
}
#endif
