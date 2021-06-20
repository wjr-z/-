#pragma once
#ifndef POLLARDRHO_H
#define POLLARDRHO_H

#define _MATH_BEGIN namespace Math{
#define _MATH_END }

#include <vector>

_MATH_BEGIN

class bint;
bool isprime(int);//int判别质数，已验证
std::vector<int>pollard_rho(int);
bool isprime(const bint&);//大数判别质数
std::vector<bint>pollard_rho(bint);

_MATH_END

#endif POLLARDRHO_H
