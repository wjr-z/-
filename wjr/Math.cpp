#include "Math.h"

_MATH_BEGIN

std::string bit10_to_bit2(const std::string& str) {
	return bint(str).to2bit().tostr();
}

std::string bit2_to_bit10(const std::string & str) {
	return bint2(str).to10bit().tostr();
}

bint sqrt(const bint&A) {
	bint ans[2];
	int now = 1;
	ans[0] = -1;
	ans[1] = A;
	ans[1].quick_divide_10k(A.length()>>1);
	bint Ans(A);
	bint one(1);
	double s=currTime();
	while (abs(ans[0]-ans[1])>one) {
		now ^= 1;
		ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) / 2;
	}
	return ans[now];
}

bfloat sqrt(const bfloat&A) {
	bfloat ans[2];
	int now = 1;
	ans[0] = 0.0;
	bint _base=A.getbase();
	_base.relength((_base.length()>>1)+1);
	ans[1] = _base;
	ans[1].setexp(A.getexp()>>1);
	bfloat Ans(A);
	bfloat Div(0.5);
	while (!approximate(ans[0], ans[1])) {
		now ^= 1;
		ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) * Div;
	}
	return ans[now];
}

bfloat sqrtk(const bfloat& A,const int&k) {//1/k次方
	bfloat ans[2];
	int now = 1;
	ans[0] = 0.0;
	ans[1] = A.getbase();
	ans[1].setexp(A.getexp() /k+1);
	int step = 0;
	bfloat Ans(A);
	bfloat Div=1.0/bfloat(k);
	bfloat K_sub_1=bfloat(k-1);
	while (!approximate(ans[0], ans[1])) {
		now ^= 1;
		ans[now] = (K_sub_1*ans[now ^ 1] + A / (qpow(ans[now ^ 1],k-1))) * Div;
	}return ans[now];
}
bfloat sqrtk(const bfloat& A, const bint& k) {//1/k次方
	bfloat ans[2];
	int now = 1;
	ans[0] = 0.0;
	if(k>=bint(A.getexp()))
		ans[1]=1.0;
	else {
		ans[1] = A.getbase();
		ans[1].setexp(A.getexp()/k.toint()+1);
	}
	int step = 0;
	bfloat Ans(A);
	bfloat Div = 1.0 / bfloat(k);
	bfloat K_sub_1 = bfloat(k - 1);
	while (!approximate(ans[0], ans[1])) {
		now ^= 1;
		ans[now] = (K_sub_1 * ans[now ^ 1] + A / (qpow(ans[now ^ 1], k - 1))) * Div;
	}return ans[now];
}

bfloat sqrtpq(const bfloat& A, const bint& p, const bint& q) {
	return qpow(sqrtk(A,q),p);
}

bfloat pow(const bfloat& A, const bfloat& k) {
	bint K(k.getbase());
	if(k.getexp()>0)
		K.quick_mul_10k(k.getexp());
	if(k.getexp()<0)
		K.quick_divide_10k(-k.getexp());
	bfloat ans(1),copyA(A);
	while (!K.iszero()) {
		if(K.at(0)&1)
			ans*=copyA;
		copyA*=copyA;
		K>>=1;
	}
	if (k.getexp() < 0) {
		bint _base=k.getbase();
		_base.relength(-k.getexp());
		ans*=sqrtpq(A,_base,qpow(bint(10),-k.getexp()));
	}
	if(!k.ispositive())ans=bfloat(1.0)/ans;
	return ans;
}

int log2(const bint& A) {
	return A.to2bit().length()-1;
}

bfloat ln(const bfloat& A) {
	if (A < bfloat(0.0)) 
		return bfloat(0.0);
	int q= (log2(A.getbase()) << 2) + 1;
	bfloat X=sqrtk(bfloat(A.getbase()),q),Y(1.0),ans; 
	X-=bfloat(1.0);
	bool f=true;
	for (int i = 1; i <= 32 ; ++i) {
		Y*=X;
		if (f)
			ans += Y / bfloat(i);
		else ans -= Y / bfloat(i);
		f^=1;
	}
	ans*=bfloat(q);
	bfloat ln10(sqrtk(bfloat(10.0),5));
	ln10-=bfloat(1.0);
	Y=1.0;
	f=true;
	bfloat ans2;
	for (int i = 1; i <= 32; ++i) {
		Y*=ln10;
		if (f)
			ans2 += Y / bfloat(i);
		else ans2 -= Y / bfloat(i);
		f ^= 1;
	}
	ans+=ans2*bfloat(5*A.getexp());
	return ans;
}
bfloat exp(const bfloat& x) {
	return pow(bfloat::exe,x);
}

_MATH_END