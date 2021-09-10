#ifndef MATH_FUNC_H
#define MATH_FUNC_H

#include <cstdint>

#define QUICK //使用空间换时间

#define TEST
#ifdef TEST
extern int mode;
#endif

namespace Math {

	enum{ bintjw = 100000000 };
	static const uint32_t maxuint = static_cast<uint32_t>(-1);//最大的unsigned int
	static const uint64_t maxull = static_cast<uint64_t>(-1);
	static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,bintjw,1 };

	//声明Ooura FFT 相关函数
	extern "C" {
		extern void cdft(int, int, double*);
		extern void rdft(int, int, double*);
	}

	template<typename Ty>
	Ty abs(Ty x){return x>=0?x:-x;}

	uint32_t qlog2(uint32_t x);
	uint32_t qlog2(int x);
	uint32_t qlog2(uint64_t x);
	uint32_t qlog2(long long x);

	uint32_t qlog10(uint32_t x);
	uint32_t qlog10(int x);
	uint32_t qlog10(uint64_t x);
	uint32_t qlog10(long long x);

	int findNumber(uint32_t n);

	template<typename Ty>
	constexpr Ty lowbit(const Ty&x){return (x&-x);}
	
	constexpr int _minx(uint32_t x);//最小的1所在位置
	
	template<typename Ty>
	constexpr bool isPowTwo(const Ty&x) {
		return (x!=0)&&!(x&(x-1));
	}

	template<typename Ty>
	constexpr Ty sqr(const Ty& val) { return val * val; }

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