#pragma once
#ifndef BFLOAT_H
#define BFLOAT_H 
#include "bint.h"

_MATH_BEGIN

class bfloat {
private:
	bint base;//底数
	int exp;//指数
	static int floatlim;//最大精度
	bool positive;//正数 or 负数
	void assign(const double&);
	void assign(const char*);
	void assign(const std::string&);
	void clear();
public:
	bfloat():exp(0),positive(true) {

	}
	bfloat(const double& x):exp(0),positive(true){
		assign(x);
	}

	void resize(const int&);
	void reserve(const int&);
};

_MATH_END

#endif BFLOAT_H

