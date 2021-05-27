#pragma once
#ifndef MATH_FUNC_H
#define MATH_FUNC_H


#define _MATH_BEGIN namespace Math{
#define _MATH_END }
#define _POLY_BEGIN namespace POLY{
#define _POLY_END }

#include <math.h>
#include <algorithm>

_MATH_BEGIN


#define uint unsigned int
#define ull unsigned long long
static const double PI = 3.14159265358979323846, matheps = 1e-6;//圆周率和浮点误差
static const uint maxuint=(1ll<<32)-1;//最大的unsigned int

static const unsigned int tabel[16] = { 0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };

inline int quicklog2(unsigned int x) {//快速求log2 ，复杂度 O ( log log n )
	int ans = 0;
	if (x >> 16) { ans += 16; x >>= 16; }
	if (x >> 8) { ans += 8; x >>= 8; }
	if (x >> 4) { ans += 4; x >>= 4; }
	return ans + tabel[x];
}

inline unsigned int quicklog10(unsigned int x) {//快速求 log10，复杂度 O( log log n )
	unsigned int ans=0;
	if(x>=100000000){ans+=8;x/=100000000;}
	if(x>=10000){ans+=4;x/=10000;}
	if(x>=100){ans+=2;;x/=100;}
	if(x>=10){++ans;x/=10;}
	return ans;
}

static const int debruijn[32] = {
	 0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
	31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};
inline uint _minx(int& x) { return debruijn[(uint)((x & -x) * 0x077CB531U) >> 27]; }//最小的1所在位置

#undef min
#undef max

template<typename Ty>
inline Ty min(const Ty& a, const Ty& b) { return a < b ? a : b; }

template<typename Ty>
inline Ty max(const Ty& a, const Ty& b) { return a > b ? a : b; }

template<typename Ty>
inline Ty sqr(const Ty& val) { return val * val; }

inline void swap(int&a,int&b){a^=b;b^=a;a^=b;}
inline void swap(long long&a,long long&b){a^=b;b^=a;a^=b;}

inline int lowbit(const int&x){return x&-x;}
inline long long lowbit(const long long &x){return x&-x;}

static const int _10k[10] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1 };

//声明Ooura FFT 相关函数
extern "C" {
	extern void cdft(int, int, double*, int*, double*);
	extern void rdft(int, int, double*, int*, double*);
}

/*---Ooura FFT相关函数---*/

struct _Ooura {//non-important
private:
	int iplength, wlength;
	int* ip;
	double* w;
public:
	_Ooura() { ip = nullptr; w = nullptr; iplength = wlength = 0; }
	~_Ooura() { delete[]ip; delete[]w; }
	void oourainital(const int& len) {//可能分配的大小过小，需要重新分配
		int len1 = 4 + sqrt(len >> 2), len2 = ((len * 5) >> 3) + 1;
		if (iplength < len1 || wlength < len2) {
			if (iplength <= 1000)
				delete[]ip, ip = new int[iplength = (len1 * 6)];
			else if (iplength <= 10000)
				delete[]ip, ip = new int[iplength = (len1 * 3)];
			else delete[]ip, ip = new int[iplength = (len1 * 1.5)];

			if (wlength <= 10000)
				delete[]w, w = new double[wlength = (len2 << 2)];
			else if (wlength <= 100000)delete[]w, w = new double[wlength = (len << 1)];
			else delete[]w, w = new double[wlength = (len2 * 1.5)]; 
   
			ip[0] = 0;
		}
	}
	int*getip(){return ip;}
	double*getw(){return w;}
};

static _Ooura Oouraip;
static inline void oourainital(const int& len) {//动态扩展和初始化
	Oouraip.oourainital(len);
}
static void cdft(int n, int f, double* cd) {
	if(f==1)oourainital(n);
	cdft(n,f,cd,Oouraip.getip(),Oouraip.getw());
}
static void rdft(int n, int f, double* cd) {
	if(f==1)oourainital(n);
	rdft(n,f,cd, Oouraip.getip(), Oouraip.getw());
}

static bool isrightint(const char* s) {
	int Length=strlen(s);
	int head=0;
	if(s[0]=='+'||s[0]=='-')
		++head;
	for (int i = head; i < Length; ++i) {
		if(s[i]=='+'||s[i]=='-')return false;
		if(s[i]<'0'||s[i]>'9')return false;
	}
	return true;
}
static bool isrightdouble(const char* s) {
	int Length = strlen(s);
	int head = 0;
	if (s[0] == '+' || s[0] == '-')
		++head;
	int pointpos=-1;
	for (int i = head; i < Length; ++i) {
		if (s[i] == '+' || s[i] == '-')return false;
		if (s[i] == '.') {
			if(pointpos!=-1)return false;
			pointpos=i;
		}
		if (s[i] < '0' || s[i]>'9')return false;
	}
	return pointpos!=Length-1;
}


_MATH_END

#endif MATH_FUNC_H