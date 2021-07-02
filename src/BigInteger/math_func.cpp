#include "math_func.h"
#include <random>

#include <iomanip>

/*---测试用高精度计时器---*/
#ifdef TEST
int mode;
double currTime() //使用高精度计时器
{
	LARGE_INTEGER performanceCount;
	double time;
	if (freq.QuadPart == 0) {
		BOOL bRet = initFreq();
		if (!bRet)
			return 0;
	}
	QueryPerformanceCounter(&performanceCount);
	time = performanceCount.HighPart * 4294967296.0 + performanceCount.LowPart;
	time = time / (freq.HighPart * 4294967296.0 + freq.LowPart);
	return time;
}
double tot, sta, en;

#endif

static const uint tabel[16] = { 0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };
static const int debruijn[32] = {
	 0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
	31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};

union DOUBLE_TO_LONGLONG {
	double p;
	ull q;
}tran;

namespace Math {

	ull double_to_ull(double x) {
		tran.p = x;
		return ((tran.q & ((1ll << 52) - 1)) | (1ll << 52)) >> ((1075) - ((tran.q >> 52) & ((1ll << 11) - 1)));
	}

	uint quicklog2(int x) {
		uint ans = 0;
		if (x >> 16) { ans += 16; x >>= 16; }
		if (x >> 8) { ans += 8; x >>= 8; }
		if (x >> 4) { ans += 4; x >>= 4; }
		return ans + tabel[x];
	}

	uint quicklog2(uint x) {
		uint ans = 0;
		if (x >> 16) { ans += 16; x >>= 16; }
		if (x >> 8) { ans += 8; x >>= 8; }
		if (x >> 4) { ans += 4; x >>= 4; }
		return ans + tabel[x];
	}

	uint quicklog2(long long x) {
		uint ans = 0;
		if (x >> 32) { ans += 32; x >>= 32; }
		if (x >> 16) { ans += 16; x >>= 16; }
		if (x >> 8) { ans += 8; x >>= 8; }
		if (x >> 4) { ans += 4; x >>= 4; }
		return ans + tabel[x];
	}

	uint quicklog2(ull x) {
		uint ans = 0;
		if (x >> 32) { ans += 32; x >>= 32; }
		if (x >> 16) { ans += 16; x >>= 16; }
		if (x >> 8) { ans += 8; x >>= 8; }
		if (x >> 4) { ans += 4; x >>= 4; }
		return ans + tabel[x];
	}

	uint quicklog10(int x) {
		uint ans = 0;
		if (x >= bintjw) { ans += 8; x /= bintjw; }
		if (x >= 10000) { ans += 4; x /= 10000; }
		if (x >= 100) { ans += 2; x /= 100; }
		if (x >= 10) ++ans;
		return ans;
	}

	uint quicklog10(uint x) {
		uint ans = 0;
		if (x >= bintjw) { ans += 8; x /= bintjw; }
		if (x >= 10000) { ans += 4; x /= 10000; }
		if (x >= 100) { ans += 2; x /= 100; }
		if (x >= 10) ++ans;
		return ans;
	}

	uint quicklog10(long long x) {
		uint ans = 0;
		if (x >= 10000000000000000ll) { ans += 16; x /= 10000000000000000ll; }
		if (x >= bintjw) { ans += 8; x /= bintjw; }
		if (x >= 10000) { ans += 4; x /= 10000; }
		if (x >= 100) { ans += 2; x /= 100; }
		if (x >= 10)++ans;
		return ans;
	}

	uint quicklog10(ull x) {
		uint ans = 0;
		if (x >= 10000000000000000ll) { ans += 16; x /= 10000000000000000ll; }
		if (x >= bintjw) { ans += 8; x /= bintjw; }
		if (x >= 10000) { ans += 4; x /= 10000; }
		if (x >= 100) { ans += 2; x /= 100; }
		if (x >= 10)++ans;
		return ans;
	}


	uint _minx(uint x) { return debruijn[(uint)((x & -x) * 0x077CB531U) >> 27]; }

	bool is_power_of_2(uint x) { return !(x & x - 1); }

	int lowbit(int x) { return x & -x; }
	long long lowbit(long long x) { return x & -x; }

	bool isrightint(const char* s) {
		int Length = strlen(s);
		int head = 0;
		if (s[0] == '+' || s[0] == '-')
			++head;
		for (; head < Length; ++head) {
			if (s[head] == '+' || s[head] == '-')return false;
			if (s[head] < '0' || s[head]>'9')return false;
		}
		return true;
	}

	bool isrightdouble(const char* s) {
		int Length = strlen(s);
		int head = 0;
		if (s[0] == '+' || s[0] == '-')
			++head;
		int pointpos = -1;
		for (; head < Length; ++head) {
			if (s[head] == 'e' || s[head] == 'E')
				break;
			if (s[head] == '.') {
				if (pointpos != -1)return false;
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

	int randint(int L, int R) {
		static std::mt19937 mt_rand(time(NULL));
		if (L > R)std::swap(L, R);
		return mt_rand() % (R - L + 1) + L;
	}
	long long randll(long long L, long long R) {
		static std::mt19937 mt_rand(time(NULL));
		if (L > R)std::swap(L, R);
		return mt_rand() % (R - L + 1) + L;
	}
	int gcd(int x, int y) {
		if (x == 0) return y;
		if (y == 0) return x;
		int i, j;
		for (i = 0; !(x & 1); ++i)x >>= 1;
		for (j = 0; !(y & 1); ++j)y >>= 1;
		if (j < i) i = j;
		while (1) {
			if (x < y)x ^= y, y ^= x, x ^= y;
			if (!(x -= y)) return y << i;
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
			if (x < y)x ^= y, y ^= x, x ^= y;
			if (!(x -= y)) return y << i;
			while (!(x & 1))x >>= 1;
		}
	}


}

