#include "PollardRho.h"
#include "../BigInteger/math_func.h"
#include <vector>
using std::vector;

namespace Math {

	int modpow(int a, int b, int mod) {
		int ans = 1;
		while (b) {
			if (b & 1)ans = a * 1ll * ans % mod;
			a = a * 1ll * a % mod;
			b >>= 1;
		}return ans;
	}

	bool witness(int n, int seed, int d, int r) {
		seed = modpow(seed, d, n);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == n - 1)return true;
			seed = seed * 1ll * seed % n;
			if (seed == 1)return false;
		}return false;
	}

	bool MillerRobin(int n, const int k = 5) {
		if (n == 2 || n == 3 || n == 5)return true;
		if (n == 1 || !(n & 1) || n == 27509653 || n == 74927161)return false;
		int d = n - 1;
		int r = 0;
		while (d % 2 == 0) {
			d /= 2;
			++r;
		}
		int seed;
		if (!witness(n, 2, d, r))return false;
		if (!witness(n, 3, d, r))return false;
		if (n > 62 && !witness(n, 61, d, r))return false;
		//上面3个实测可以筛去1e8以内绝大多数合数，只剩两个伪素数，因此在上面判掉
		if (n > bintjw) {
			for (int i = 0; i < k; ++i) {
				seed = Math::randint(4, n - 2);
				if (!witness(n, seed, d, r))return false;
			}
		}
		return true;
	}

	bool isprime(int n) {
		return MillerRobin(n);
	}

	int rho(int x) {
		if (x == 4)
			return 2;
		while (true) {
			int c = randint(1, x - 1);
			auto f = [=](const int& a) { return (a * 1ll * a + c) % x; };
			int t, r, p(1), d;
			t = r = randint(1, x - 1);
			int i = 0, j = 1;
			while (++i) {//开始玄学生成
				r = f(r);
				p = (p * std::abs(r - t)) % x;
				if (t == r || !p)break;
				if (!(i & 127) || i == j) {//我们不仅在等127次之后gcd我们还会倍增的来gcd
					d = Math::gcd(p, x);
					if (d > 1)return d;
					if (i == j)t = r, j <<= 1;//维护倍增正确性，并判环（一箭双雕）
				}
			}

		}
		return x;
	}

	void pollard_rho(int x, vector<int>& ans) {
		if (x < 2)return;
		if (isprime(x)) {
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
	void max_pollard_rho(int x, int& Max) {
		if (x < 2 || x <= Max)return;
		if (isprime(x)) {
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

	long long LLMul(long long a, long long b, long long p) {
		long long Val = a * b - (long long)((long double)a * b / p) * p;
		return (Val % p + p) % p;
	}

	long long modpow(long long a, long long b, long long mod) {
		long long ans = 1;
		while (b) {
			if (b & 1)ans = LLMul(ans, a, mod);
			a = LLMul(a, a, mod);
			b >>= 1;
		}return ans;
	}

	bool witness(long long n, long long seed, long long d, int r) {
		seed = modpow(seed, d, n);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == n - 1)return true;
			seed = LLMul(seed, seed, n);
			if (seed == 1)return false;
		}return false;
	}

	bool MillerRobin(long long n, const int k = 5) {
		if (n == 2 || n == 3 || n == 5)return true;
		if (n == 1 || !(n & 1))return false;
		long long d = n - 1;
		int r = 0;
		while (!(d & 1)) {
			d >>= 1;
			++r;
		}
		long long seed;
		if (!witness(n, 2ll, d, r))return false;
		if (!witness(n, 3ll, d, r))return false;
		if (n > 62 && !witness(n, 61ll, d, r))return false;
		for (int i = 0; i < k; ++i) {
			seed = Math::randll(4, n - 2);
			if (!witness(n, seed, d, r))return false;
		}
		return true;
	}

	bool isprime(long long n) {
		if (n <= MAXINT)return isprime((int)n);
		return MillerRobin(n);
	}

	long long rho(long long x) {
		if (x == 4)
			return 2;
		while (true) {
			long long c = randll(1, x - 1);
			auto f = [=](long long a) { return (LLMul(a, a, x) + c) % x; };
			long long t, r, p(1), d;
			t = r = randll(1, x - 1);
			int i = 0, j = 1;
			while (++i) {//开始玄学生成
				r = f(r);
				p = LLMul(p, std::abs(r - t), x);
				if (t == r || !p)break;
				if (!(i & 127) || i == j) {//我们不仅在等127次之后gcd我们还会倍增的来gcd
					d = Math::gcd(p, x);
					if (d > 1)return d;
					if (i == j)t = r, j <<= 1;//维护倍增正确性，并判环（一箭双雕）
				}
			}

		}
		return x;
	}

	void pollard_rho(long long x, vector<long long>& ans) {
		if (x < 2)return;
		if (isprime(x)) {
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

	void max_pollard_rho(long long x, long long& Max) {
		if (x < 2 || x <= Max)return;
		if (isprime(x)) {
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