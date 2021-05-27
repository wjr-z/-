#pragma once
#ifndef MATH_H
#define MATH_H
#include "bfloat.h"

#define _MATH_BEGIN namespace Math{
#define _MATH_END }
#define _POLY_BEGIN namespace POLY{
#define _POLY_END }

_MATH_BEGIN

std::string _10bit_to_2bit(const std::string& s) {//10进制字符串转化为2进制字符串
	return bint(s).to2bit().tostr();
}
std::string _2bit_to_10bit(const std::string& s) {//2进制字符串转化为10进制字符串
	return bint2(s).to10bit().tostr();
}


_MATH_END


#endif MATH_H