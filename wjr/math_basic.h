#pragma once
#ifndef MATH_BASIC_H
#define MATH_BASIC_H

#define _MATH_BEGIN namespace Math{
#define _MATH_END }
#define _POLY_BEGIN namespace POLY{
#define _POLY_END }

#define TEST //测试用高精度计时器 ，直接网上找的代码
#ifdef TEST

extern int mode;

#include <Windows.h>
static LARGE_INTEGER freq;

static BOOL initFreq() {
	if (!QueryPerformanceFrequency(&freq))
		return FALSE;
	else
		return TRUE;
}

extern double currTime(); //使用高精度计时器

extern double tot, sta, en;

#define START sta=currTime();
#define END en=currTime();
#define ADD tot+=en-sta;

#endif

#include <iostream>
#include <iomanip>

using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;

_MATH_BEGIN

#undef min
#undef max

#define uint unsigned int
#define ull unsigned long long
static const double PI = 3.14159265358979323846, matheps = 1e-6;//圆周率和浮点误差
static const uint maxuint = (1ll << 32) - 1;//最大的unsigned int

static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1 };

_MATH_END

#endif MATH_BASIC_H