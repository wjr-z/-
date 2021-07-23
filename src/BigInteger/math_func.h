#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#include <stdint.h>

#define TEST //测试用高精度计时器 ，直接网上找的代码

extern double currTime(); //使用高精度计时器

extern double tot, sta, en;
extern int mode;

#define START sta=currTime();
#define END en=currTime();
#define ADD tot+=en-sta;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace Math {

#define bintjw 100000000

	static const uint32_t maxuint = (uint32_t)-1;//最大的unsigned int
	static const uint64_t maxull = (uint64_t)-1;

	uint64_t double_to_ull(double);//比强制转型快1倍

	uint32_t quicklog2(int x);
	uint32_t quicklog2(uint32_t x);
	uint32_t quicklog2(long long x);
	uint32_t quicklog2(uint64_t x);

	uint32_t quicklog10(int x);
	uint32_t quicklog10(uint32_t x);
	uint32_t quicklog10(long long x);
	uint32_t quicklog10(uint64_t x);

	int lowbit(int x);
	long long lowbit(long long x);

	int _minx(uint32_t x);//最小的1所在位置

	bool is_power_of_2(uint32_t x);

	template<typename Ty>
	inline Ty sqr(const Ty& val) { return val * val; }

	static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,bintjw,1 };

	//声明Ooura FFT 相关函数
	extern "C" {
		extern void cdft(int, int, double*);
		extern void rdft(int, int, double*);
	}

	bool isrightint(const char*);
	bool isrightdouble(const char*);

	int randint(int, int);
	long long randll(long long, long long);
	int gcd(int, int);
	long long gcd(long long, long long);

}

#endif