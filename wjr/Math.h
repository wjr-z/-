#pragma once
#ifndef MATH_H
#define MATH_H
#include "bfloat.h"
#include "Matrix.h"

_MATH_BEGIN

std::string bit10_to_bit2(const std::string&);//10进制字符串转化为2进制字符串
std::string bit2_to_bit10(const std::string&);//2进制字符串转化为10进制字符串

bint sqrt(const bint&);
bfloat sqrt(const bfloat&);//bfloat开根

/*---目前精度仅能达到大约32位，若设置精度为128及以上，则尽量别用---*/
bfloat sqrtk(const bfloat&,const int&);//1/k次方
bfloat sqrtk(const bfloat&,const bint&);
bfloat sqrtpq(const bfloat&,const bint&,const bint&);

bfloat pow(const bfloat&,const bfloat&);//p^q次方

int log2(const bint&);

bfloat ln(const bfloat&);
bfloat exp(const bfloat&);

_MATH_END


#endif MATH_H