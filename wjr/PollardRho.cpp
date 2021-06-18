#include "PollardRho.h"
#include "bint.h"


_MATH_BEGIN

int modpow(int a, int b, int mod) {
	int ans=1;
	while (b) {
		if(b&1)ans=a*1ll*ans%mod;
		a=a*1ll*a%mod;
		b>>=1;
	}return ans;
}

bool witness(int n, int seed, int d, int r) {
	seed = modpow(seed, d, n);
	if (seed == 1)return true;
	for (int i = 0; i < r; ++i) {
		if (seed == n-1)return true;
		seed = seed*1ll*seed%n;
		if (seed == 1)return false;
	}return false;
}

bool MillerRobin(const int& n, const int k = 4) {
	if (n == 2 || n == 3 || n == 5)return true;
	if (n == 1 || !(n&1) || n == 27509653 || n == 74927161)return false;
	int d=n-1;
	int r = 0;
	while (d % 2 == 0) {
		d /= 2;
		++r;
	}
	int seed;
	if (!witness(n, 2, d, r))return false;
	if (!witness(n, 3, d, r))return false;
	if (n>62&&!witness(n, 61, d, r))return false;
	//上面3个实测可以筛去1e8以内绝大多数合数，只剩两个伪素数，因此在上面判掉
	if (n > bintjw) {
		for (int i = 0; i < k; ++i) {
			seed = Math::randdata(4, n - 2);
			if (!witness(n, seed, d, r))return false;
		}
	}
	return true;
}

bool isprime(const int& n) {
	return MillerRobin(n);
}

bint modpow(bint a, bint b,const bint&mod) {
	static bint ans;
	ans=1;
	while (!b.iszero()) {
		if(b[0]&1)
			ans*=a,ans%=mod;
		a*=a;
		a%=mod;
		b/=2;
	}
	return ans;
}

bool witness(const bint&n,bint seed,const bint&d,int r) {
	seed=std::move(modpow(seed,d,n));
	if(seed==1)return true;
	bint S(n-1);
	for (int i = 0; i < r; ++i) {
		if(seed==S)return true;
		seed*=seed;
		seed%=n;
		if(seed==1)return false;
	}return false;
}

bool MillerRobin(const bint& n, int k=3) {
	if(n==2||n==3||n==5)return true;
	if(n==1||n[0]%2==0)return false;
	bint d(n-1);
	int r=0;
	while (d % 2 == 0) {
		d/=2;
		++r;
	}
	bint seed;
	bint L=bint(4),R=bint(n-2);
	if(!witness(n,std::move(bint(2)),d,r))return false;//可以筛去绝大部分的合数
	if(!witness(n, std::move(bint(3)), d, r))return false;
	if(n>61&& !witness(n, std::move(bint(61)), d, r))return false;
	for (int i = 0; i < k; ++i) {
		seed=std::move(randdata(L,R));
		if (!witness(n, std::move(seed),d, r))return false;
	}
	return true;
}

bool isprime(const bint& n) {
	if(n<=10000000000)return isprime(n.toint());
	return MillerRobin(n);
}

_MATH_END