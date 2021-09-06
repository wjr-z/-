#include "math_func.h"

#include <cstring>
#include <ctime>
#include <iostream>
#include <random>

#include "bint.h"

int mode;

static const char* const tabel="\0\0\1\1\2\2\2\2\3\3\3\3\3\3\3\3";

static const int debruijn[32] = {
	 0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
	31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};

namespace Math {

	uint32_t qlog2(uint32_t x) {
		uint32_t ans = 0;
		if (x >> 16) { ans |= 16; x >>= 16; }
		if (x >> 8)  { ans |= 8;  x >>= 8;  }
		if (x >> 4)  { ans |= 4;  x >>= 4;  }
		return ans | tabel[x];
	}

	uint32_t qlog2(int x) {
		return qlog2(static_cast<uint32_t>(x));
	}

	uint32_t qlog2(uint64_t x) {
		uint32_t ans = 0;
		if (x >> 32) { ans |= 32; x >>= 32; }
		if (x >> 16) { ans |= 16; x >>= 16; }
		if (x >> 8)  { ans |= 8;  x >>= 8;  }
		if (x >> 4)  { ans |= 4;  x >>= 4;  }
		return ans | tabel[x];
	}

	uint32_t qlog2(long long x) {
		return qlog2(static_cast<uint64_t>(x));
	}

	uint32_t qlog10(uint32_t x) {
		uint32_t ans = 0;
		if (x >= bintjw) { ans += 8; x /= bintjw; }
		if (x >= 10000)  { ans += 4; x /= 10000;  }
		if (x >= 100)    { ans += 2; x /= 100;    }
		if (x >= 10)     ++ans;
		return ans;
	}

	uint32_t qlog10(int x) {
		return qlog10(static_cast<uint32_t>(x));
	}

	uint32_t qlog10(uint64_t x) {
		uint32_t ans = 0;
		if (x >= 10000000000000000ll) { ans += 16; x /= 10000000000000000ll; }
		if (x >= 100000000) { ans += 8; x /= bintjw; }
		if (x >= 10000)     { ans += 4; x /= 10000;  }
		if (x >= 100) { ans += 2; x /= 100; }
		if (x >= 10)  ++ans;
		return ans;
	}

	uint32_t qlog10(long long x) {
		return qlog10(static_cast<uint64_t>(x));
	}

	int findNumber(unsigned int n) {
	#ifdef QUICK
		static const char* const bitsperbyte
			= "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"
			"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
			"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
			"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
			"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
			"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
			"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
			"\4\5\5\6\5\6\6\7\5\6\6\7\6\7\7\x8";
		return bitsperbyte[n & 255] +
			bitsperbyte[(n >> 8) & 255] +
			bitsperbyte[(n >> 16) & 255] +
			bitsperbyte[n >> 24];
	#else
		n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
		n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
		n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
		n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
		n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
		return n;
	#endif
	}

	constexpr int _minx(uint32_t x) { 
		return debruijn[((x&((~x)+1)) * 0x077CB531U) >> 27]; 
	}

	bool isrightint(const char* s) {
		size_t Length = strlen(s);
		size_t head = 0;
		if (s[0] == '+' || s[0] == '-')
			++head;
		if(head==Length)return false;
		for (; head < Length; ++head) {
			if (s[head] == '+' || s[head] == '-')return false;
			if (s[head] < '0' || s[head]>'9')return false;
		}
		return true;
	}

	bool isrightdouble(const char* s) {
		size_t Length = strlen(s);
		size_t head = 0;
		if (s[0] == '+' || s[0] == '-')
			++head;
		size_t pointpos = size_t(-1);
		for (; head < Length; ++head) {
			if (s[head] == 'e' || s[head] == 'E')
				break;
			if (s[head] == '.') {
				if (~pointpos)return false;
				pointpos = head;
			}
			else if (s[head] < '0' || s[head]>'9')return false;
		}
		if (pointpos == head - 1)return false;
		if (s[head] == 'e' || s[head] == 'E') {
			++head;
			if (head < Length && (s[head] == '+' || s[head] == '-'))
				++head;
			for (; head < Length; ++head)
				if (s[head] < '0' || s[head]>'9')return false;
		}

		return true;
	}

	uint32_t randuint() {
		static std::mt19937 mt_rand(static_cast<unsigned int>(time(nullptr)));
		return mt_rand();
	}

	int randint(int L, int R) {
		if (L > R)std::swap(L, R);
		return L+static_cast<int>(randuint() % (R - L + 1));
	}

	uint64_t randull() {
		return (((uint64_t)randuint())<<32)|randuint();
	}

	long long randll(long long L, long long R) {
		if (L > R)std::swap(L, R);
		return L + (long long)(randull() % (R - L + 1));
	}

	int gcd(int x, int y) {
		if (x == 0) return y;
		if (y == 0) return x;
		int i=_minx(x), j=_minx(y);
		x>>=i,y>>=j;
		if (j < i) i = j;
		while (true) {
			if (x < y)x ^= y^= x ^= y;
			if (!(x -= y)) 
				return y << i;
			while (!(x & 1))x >>= 1;
		}
	}

	long long gcd(long long x, long long y) {
		if (x == 0) return y;
		if (y == 0) return x;
		long long i, j;
		for (i = 0; !(x & 1); ++i)x >>= 1;
		for (j = 0; !(y & 1); ++j)y >>= 1;
		if (j < i) i = j;
		while (true) {
			if (x < y)x ^= y ^= x ^= y;
			if (!(x -= y)) return y << i;
			while (!(x & 1))x >>= 1;
		}
	}
}
