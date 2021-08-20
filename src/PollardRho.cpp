#include "PollardRho.h"

#include "math_func.h"

namespace Math {

	using std::vector;

	static int mod_pow(int a, int b, int mod) {
		int ans = 1;
		while (b) {
			if (b & 1)ans = a * 1ll * ans % mod;
			a = a * 1ll * a % mod;
			b >>= 1;
		}return ans;
	}

	static bool witness(int n, int seed, int d, int r) {
		seed = mod_pow(seed, d, n);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == n - 1)return true;
			seed = seed * 1ll * seed % n;
			if (seed == 1)return false;
		}return false;
	}

	static bool MillerRobin(int n) {
		if (n == 2 || n == 3 || n == 5)return true;
		if (n == 1 || !(n & 1) )return false;
		const int r = _minx(n-1);
		const int d=(n-1)>>r;
		//int范围内仅需测试2,3,5,7
		if (!witness(n, 2, d, r))return false;
		if (!witness(n, 3, d, r))return false;
		if (!witness(n, 5, d, r))return false;
		if (!witness(n, 7, d, r))return false;
		return true;
	}

	bool is_prime(int n) {
		return MillerRobin(n);
	}

	static int rho(int x) {
		if (x == 4)
			return 2;
		while (true) {
			const int c = randint(1, x - 1);
			auto f = [&c,&x](int a) { return static_cast<int>((a * 1ll * a + c) % x); };
			int r, p(1);
			int t = r = randint(1, x - 1);
			int i = 0, j = 1;
			int step=32,cnt1=0;
			while (++i) {//开始玄学生成
				r = f(r);
				p = (p *1ll* abs(r - t)) % x;
				if (!p)break;
				if (!(i & (step-1)) || i == j) {
					int d = gcd(p, x);
					if (d > 1)return d;
					if (i == j) {
						t = r;
						j <<= 1;
						if(!((++cnt1)&3))
							step<<=1;
					}
				}
			}

		}
	}

	static void pollard_rho(int x, vector<int>& ans) {
		if (x < 2)return;
		if (is_prime(x)) {
			ans.push_back(x);
			return;
		}
		int pri = rho(x);
		while (x % pri == 0)x /= pri;
		pollard_rho(pri, ans);
		pollard_rho(x, ans);
	}
	vector<int> pollard_rho(int x) {
		vector<int>ans;
		pollard_rho(x, ans);
		return ans;
	}
	static void max_pollard_rho(int x, int& Max) {
		if (x < 2 || x <= Max)return;
		if (is_prime(x)) {
			Max = Max >= x ? Max : x;
			return;
		}
		int pri = rho(x);
		while (x % pri == 0)x /= pri;
		max_pollard_rho(pri, Max);
		max_pollard_rho(x, Max);
	}

	int max_prime(int x) {
		int ans = 0;
		max_pollard_rho(x, ans);
		return ans;
	}

	static long long LLMul(long long a, long long b, long long p) {
		long long ans = 0;
		while (b) {
			if (b & 1)ans = (ans + a) % p;
			a = (a + a) % p;
			b >>= 1;
		}
		return ans;
	}

	static long long modpow(long long a, long long b, long long mod) {
		long long ans = 1;
		while (b) {
			if (b & 1)ans = LLMul(ans, a, mod);
			a = LLMul(a, a, mod);
			b >>= 1;
		}return ans;
	}

	static bool witness(long long n, long long seed, long long d, int r) {
		seed = modpow(seed, d, n);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == n - 1)return true;
			seed = LLMul(seed, seed, n);
			if (seed == 1)return false;
		}return false;
	}

	static bool MillerRobin(long long n, const int k = 5) {
		if (n == 2 || n == 3 || n == 5)return true;
		if (n == 1 || !(n & 1))return false;
		long long d = n - 1;
		int r = 0;
		while (!(d & 1)) {
			d >>= 1;
			++r;
		}
		if (!witness(n, 2ll, d, r))return false;
		if (!witness(n, 3ll, d, r))return false;
		if (n > 62 && !witness(n, 61ll, d, r))return false;
		for (int i = 0; i < k; ++i) {
			if (!witness(n, Math::randll(4, n - 2), d, r))return false;
		}
		return true;
	}

	bool is_prime(long long n) {
		if (!(n>>30))return is_prime((int)n);
		return MillerRobin(n);
	}

	static long long rho(long long x) {
		if (x == 4)
			return 2;
		while (true) {
			long long c = randll(1, x - 1);
			auto f = [&c,&x](long long a) { return (LLMul(a, a, x) + c) % x; };
			long long r, p(1);
			long long t = r = randll(1, x - 1);
			int i = 0, j = 1;
			int step = 31, cnt1 = 0;
			while (++i) {//开始玄学生成
				r = f(r);
				p = LLMul(p, Math::abs(r - t), x);
				if ( !p)break;
				if (!(i & (step-1)) || i == j) {//我们不仅在等127次之后gcd我们还会倍增的来gcd
					long long d = Math::gcd(p, x);
					if (d > 1)return d;
					if (i == j) {
						t = r;
						j <<= 1;
						if (!((++cnt1) & 3))
							step <<= 1;
					}
				}
			}

		}
	}

	static void pollard_rho(long long x, vector<long long>& ans) {
		if (x < 2)return;
		if (is_prime(x)) {
			ans.push_back(x);
			return;
		}
		long long pri = rho(x);
		while (x % pri == 0)x /= pri;
		pollard_rho(pri, ans);
		pollard_rho(x, ans);
	}
	vector<long long> pollard_rho(long long x) {
		vector<long long>ans;
		pollard_rho(x, ans);
		return ans;
	}

	static void max_pollard_rho(long long x, long long& Max) {
		if (x < 2 || x <= Max)return;
		if (is_prime(x)) {
			Max = Max >= x ? Max : x;
			return;
		}
		long long pri = rho(x);
		while (x % pri == 0)x /= pri;
		max_pollard_rho(pri, Max);
		max_pollard_rho(x, Max);
	}

	long long max_prime(long long x) {
		long long ans = 0;
		max_pollard_rho(x, ans);
		return ans;
	}
}
