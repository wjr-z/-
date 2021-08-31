#ifndef GEOMETRY_H
/**
 * 暂未完善
 * 之后会实现向量的一系列操作
 */
#define GEOMETRY_H

struct point {
	double x,y;
	explicit point(double x=0,double y=0);
	~point();
};

struct vec {
	double x{},y{};
	explicit vec(double x=0,double y=0);
	explicit vec(const point&start,const point&end);
	vec(const vec&other)noexcept;
	vec(vec&&other)noexcept;
	vec& operator=(const vec&other);
	vec& operator=(vec&&other) noexcept;
	vec& operator+=(const vec&other);
	vec& operator-=(const vec&other);
	vec operator+(const vec&other) const;
	vec operator-(const vec&other) const;
};

vec operator-(const point&lhs,const point&rhs);

double dot(const vec&lhs,const vec&rhs);
double cross(const vec&lhs,const vec&rhs);


#endif