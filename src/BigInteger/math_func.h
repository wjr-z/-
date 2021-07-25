#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#include <stdint.h>
#include <string.h>

#define TEST //测试用高精度计时器 ，直接网上找的代码

extern double currTime(); //使用高精度计时器

extern double tot, sta, en;
extern int mode;

#define START sta=currTime();
#define END en=currTime();
#define ADD tot+=en-sta;

#undef min
#undef max

namespace Math {

#define bintjw 100000000

	template<typename Ty>
	inline Ty abs(Ty x){return x>=0?x:-x;}

	template<typename Ty>
	inline Ty min(Ty lhs,Ty rhs){return lhs<rhs?lhs:rhs;}
	template<typename Ty>
	inline Ty max(Ty lhs,Ty rhs){return lhs>rhs?lhs:rhs;}

	//声明Ooura FFT 相关函数
	extern "C" {
		extern void cdft(int, int, double*);
		extern void rdft(int, int, double*);
	}

	static const uint32_t maxuint = (uint32_t)-1;//最大的unsigned int
	static const uint64_t maxull = (uint64_t)-1;


	uint64_t double_to_ull(double);//比强制转型快1倍

	uint32_t quicklog2(uint32_t x);
	uint32_t quicklog2(int x);
	uint32_t quicklog2(uint64_t x);
	uint32_t quicklog2(long long x);

	uint32_t quicklog10(uint32_t x);
	uint32_t quicklog10(int x);
	uint32_t quicklog10(uint64_t x);
	uint32_t quicklog10(long long x);

	int findnumber1(uint32_t);
	int findnumber1(int);
	int findnumber1(uint64_t);
	int findnumber1(long long);

	int lowbit(int x);
	long long lowbit(long long x);
	int _minx(uint32_t x);//最小的1所在位置
	bool is_power_of_2(uint32_t x);

	template<typename Ty>
	inline Ty sqr(const Ty& val) { return val * val; }

	static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,bintjw,1 };

	bool isrightint(const char*);
	bool isrightdouble(const char*);
	uint32_t randuint();
	int randint(int, int);
	uint64_t randull();
	long long randll(long long,long long );
	int gcd(int, int);
	long long gcd(long long, long long);

}

#endif