#pragma once
#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#define _MATH_BEGIN namespace Math{
#define _MATH_END }

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

#define bintjw 100000000

#define uint unsigned int
#define ull unsigned long long
static const double PI = 3.14159265358979323846, matheps = 1e-6;//圆周率和浮点误差
static const uint maxuint = (1ll << 32) - 1;//最大的unsigned int

constexpr static const uint tabel[16] = { 0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };
inline uint quicklog2(size_t x) {//快速求log2 ，复杂度 O ( log log n )
	uint ans = 0;
	if (x >> 16) { ans += 16; x >>= 16; }
	if (x >> 8) { ans += 8; x >>= 8; }
	if (x >> 4) { ans += 4; x >>= 4; }
	return ans + tabel[x];
}

inline uint quicklog10(size_t x) {//快速求 log10，复杂度 O( log log n )
	uint ans = 0;
	if (x >= bintjw) { ans += 8; x /= bintjw; }
	if (x >= 10000) { ans += 4; x /= 10000; }
	if (x >= 100) { ans += 2;; x /= 100; }
	if (x >= 10) { ++ans; x /= 10; }
	return ans;
}

static const int debruijn[32] = {
	 0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
	31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};
inline uint _minx(int& x) { return debruijn[(uint)((x & -x) * 0x077CB531U) >> 27]; }//最小的1所在位置

template<typename Ty>
inline Ty sqr(const Ty& val) { return val * val; }

inline void swap(int& a, int& b) { a ^= b; b ^= a; a ^= b; }
inline void swap(long long& a, long long& b) { a ^= b; b ^= a; a ^= b; }

inline int lowbit(const int& x) { return x & -x; }
inline long long lowbit(const long long& x) { return x & -x; }

static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,bintjw,1 };


//声明Ooura FFT 相关函数
extern "C" {
	extern void cdft(int, int, double*);
	extern void rdft(int, int, double*);
}

bool isrightint(const char*);
bool isrightdouble(const char*);



_MATH_END

#endif MATH_FUNC_H