#ifndef MATH_FUNC_H
#define MATH_FUNC_H

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

namespace Math{

	using std::istream;
	using std::ostream;
	using std::cin;
	using std::cout;
	using std::endl;

#define bintjw 100000000
#define uint unsigned int
#define ull unsigned long long

	static const uint maxuint = (uint)- 1;//最大的unsigned int
	static const ull maxull=(ull)-1;

	ull double_to_ull(double);

	uint quicklog2(int x);
	uint quicklog2(uint x);
	uint quicklog2(long long x);
	uint quicklog2(ull x);

	uint quicklog10(int x);
	uint quicklog10(uint x);
	uint quicklog10(long long x);
	uint quicklog10(ull x);

	uint _minx(uint x) ;//最小的1所在位置

	bool is_power_of_2(uint x);

	template<typename Ty>
	inline Ty sqr(const Ty& val) { return val * val; }

	int lowbit(int x);
	long long lowbit(long long x);

	static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,bintjw,1 };

	//声明Ooura FFT 相关函数
	extern "C" {
		extern void cdft(int, int, double*);
		extern void rdft(int, int, double*);
	}

	bool isrightint(const char*);
	bool isrightdouble(const char*);

	int randint(int,int);
	long long randll(long long ,long long);
	int gcd(int,int);
	long long gcd(long long,long long);

}

#endif MATH_FUNC_H