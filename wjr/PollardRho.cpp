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

bool MillerRobin(int n, const int k = 5) {
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

bool isprime(int n) {
	return MillerRobin(n);
}

int rho(int x) {
	if (x == 4)
		return 2;
	while (true) {
		int c = randdata(1, x - 1);
		auto f = [=](const int& a) { return (a *1ll* a + c) % x; };
		int t, r, p(1), d;
		t = r = randdata(1, x - 1);
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
	if(x<2)return ;
	if (isprime(x)) {
		ans.push_back(x);
		return ;
	}
	int pri=rho(x);
	while(x%pri==0)x/=pri;
	pollard_rho(pri,ans);
	pollard_rho(x,ans);
}
vector<int> pollard_rho(int x) {
	vector<int>ans;
	pollard_rho(x,ans);
	return ans;
}

bint modpow(bint a, bint b,const bint&mod) {
	bint ans(1);
	while (!b.iszero()) {
		if(b[0]&1)
			ans*=a,ans%=mod;
		a*=a;
		a%=mod;
		b/=2;
	}
	return ans;
}

bool witness(const bint&n,const bint&S,bint seed,const bint&d,int r) {
	seed=std::move(modpow(seed,d,n));
	if(seed==1)return true;
	for (int i = 0; i < r; ++i) {
		if(seed==S)return true;
		seed*=seed;
		seed%=n;
		if(seed==1)return false;
	}return false;
}

bool MillerRobin(const bint& n, int k=5) {
	if(n[0]%2==0)return false;
	bint d(n-1);
	int r=0;
	while (d % 2 == 0) {
		d/=2;
		++r;
	}
	bint seed;
	bint L=bint(4),R=bint(n-2);
	bint S(n-1);
	if(!witness(n,S,std::move(bint(2)),d,r))return false;//可以筛去绝大部分的合数
	if(!witness(n,S,std::move(bint(3)), d, r))return false;
	if(n>61&& !witness(n,S, std::move(bint(61)), d, r))return false;
	for (int i = 0; i < k; ++i) {
		seed=std::move(randdata(L,R));
		if (!witness(n,S,std::move(seed),d, r))return false;
	}
	return true;
}

bool isprime(const bint& n) {
	if(n<=10000000000)return isprime(n.toint());
	return MillerRobin(n);
}


bint rho(const bint&x) {
	if (x == 4)
		return bint(2);
	while (true) {
		bint c = randdata(bint(1), x-1);
		auto f = [=](const bint&a) { return (a*a+c) % x; };
		bint t, r, p(1),d;
		t=r= randdata(bint(1), x - 1);
		int i=0,j=1;
		while (++i) {//开始玄学生成
			r=f(r);
			t=f(f(t));
			p = (p * Math::abs(r - t)) % x;
			if (t==r||!p)break;
			if (!(i & 127) || i == j) {//我们不仅在等127次之后gcd我们还会倍增的来gcd
				d=gcd(p,x);
				if (d > 1)return d;
				if (i == j)t = r, j <<= 1;//维护倍增正确性，并判环（一箭双雕）
			}
		}

	}
	return x;
}

void pollard_rho(bint&x, vector<bint>& ans) {
	if (x < 2)return;
	if (isprime(x)) {
		ans.emplace_back(std::move(x));
		return;
	}
	bint pri = rho(x);
	while (x % pri == 0)x /= pri;
	pollard_rho(pri, ans);
	pollard_rho(x, ans);
}
vector<bint> pollard_rho(bint x) {
	vector<bint>ans;
	pollard_rho(x, ans);
	return ans;
}

_MATH_END