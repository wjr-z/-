#include "geometry.h"

point::point(double x,double y):x(x),y(y){}
point::~point(){}

vec::vec(double x,double y):x(x),y(y){}
vec::vec(const point&start,const point&end):x(end.x-start.x),y(end.y-start.y){}
vec::vec(const vec&other):x(other.x),y(other.y){}
vec& vec::operator=(const vec& other) {
	x=other.x;
	y=other.y;
	return *this;
}
vec& vec::operator+=(const vec& other) {
	x+=other.x;
	y+=other.y;
	return *this;
}
vec& vec::operator-=(const vec& other) {
	x-=other.x;
	y-=other.x;
	return *this;
}
vec vec::operator+(const vec&other) {
	return vec(x+other.x,y+other.y);
}

vec vec::operator-(const vec&other) {
	return vec(x-other.x,y-other.y);
}

vec operator-(const point& lhs, const point& rhs) {
	return vec(lhs.x-rhs.x,lhs.y-rhs.y);
}

double dot(const vec& lhs, const vec& rhs) {
	return lhs.x*rhs.x+lhs.y*rhs.y;
}

double cross(const vec& lhs, const vec& rhs) {
	return lhs.x*rhs.y-lhs.y*rhs.x;
}