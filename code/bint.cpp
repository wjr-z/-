#include "bint.h"

#define jw 100000000

_MATH_BEGIN

int mode;

/*---测试用高精度计时器---*/
#ifdef TEST
double currTime() //使用高精度计时器
{
	LARGE_INTEGER performanceCount;
	double time;
	BOOL result;
	if (freq.QuadPart == 0) {
		BOOL bRet = initFreq();
		if (!bRet)
			return 0;
	}
	result = QueryPerformanceCounter(&performanceCount);
	time = performanceCount.HighPart * 4294967296.0 + performanceCount.LowPart;
	time = time / (freq.HighPart * 4294967296.0 + freq.LowPart);
	return time;
}
double tot, sta, en;

#endif

/*---bint类部分函数定义---*/

void bint::assign(const int& v) {
	int val = v;
	resize(0);
	positive = true;
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= jw)
		vec[i] = val % jw;
	if (iszero())
		vec[0] = 0;
}
void bint::assign(const long long& v) {
	long long val = v;
	resize(0);
	positive = true;
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= jw)
		vec[i] = val % jw;
	if (iszero())
		vec[0] = 0;
}

void bint::assign(const char* _s) {
	char* s = const_cast<char*>(_s);
	resize(0);
	int Length = strlen(s);
	reserve(Length >> 3);
	int i = Length - 1, head = 0, j = 0;
	int fir = 0;
	while (fir < Length && s[fir] == '-')//负号
		positive ^= 1, ++fir;
	while (i - fir >= 7) {//从后往前每8位一搞
		for (j = i - 7; j <= i; ++j)
			vec[head] = vec[head] * 10 + s[j] - '0';
		++head;
		i -= 8;
	}
	for (j = fir; j <= i; ++j)
		vec[head] = vec[head] * 10 + s[j] - '0';
	if (iszero())
		vec[0] = 0;
}
void bint::assign(const std::string& _s) {
	assign(_s.c_str());
}


const bool bint::iszero() const {
	return vec.iszero();
}
void bint::resize(const int& len){ vec.resize(len); }
const int bint::size()const { return vec.size(); }
const int bint::length() const { return vec.length(); }
void bint::reserve(const int& len) { vec.reserve(len); }
void bint::relength(const int& index) { vec.relength(index); }

const int& bint::operator[](const int& index)const { return vec[index]; }
int& bint::operator[](const int& index) { return vec[index]; }
int bint::at(const int& index)const {
	return vec.at(index);
}
int& bint::save_at(const int& index) { return vec.save_at(index); }
void bint::set(const int& index, const int& val) {
	vec.set(index, val);
}
bool bint::operator<(const bint& other)const {
	return ((positive == other.positive) && (vec < other.vec)) ^ (!positive);
}
bool bint::operator==(const bint& other) const {
	return ((positive == other.positive) && vec == other.vec);
}
bool bint::operator<=(const bint& other) const {
	return !(((positive == other.positive) && (vec > other.vec)) ^ (!positive));
}
bool bint::operator>(const bint& other) const { return !((*this) <= other); }
bool bint::operator>=(const bint& other) const { return !((*this) < other); }
bool bint::operator!=(const bint& other)const { return !((*this) == other); }

void bint::quick_mul_10() {
	int Length = size();
	vec.reserve(Length + 1);
	if (vec.save_at(Length - 1) >= 10000000)
		vec[Length] = vec.save_at(Length - 1) / 10000000;
	for (int i = Length - 1; i; --i)
		vec[i] = (vec.save_at(i) % 10000000) * 10 + vec.save_at(i - 1) / 10000000;
	vec[0] = (vec.save_at(0) % 10000000) * 10;
}
bint& bint::quick_mul_10k(const int& k ) {
	Array_func::QuickMul10k(this->vec, k);
	return*this;
}
void bint::abs() {
	this->positive=true;
}
bint bint::inv(int lim)const {
	bint ni, two;
	int fac = length();
	if (lim == -1)lim = fac << 1;
	two.set(lim, 2);
	if (fac > 1) {//牛顿迭代初值
		int val = 1000 / (this->at(fac - 1) * 10 + this->at(fac - 2) + 1);
		ni.set(lim - fac, val / 10);
		ni.set(lim - fac - 1, val % 10);
	}
	else ni.set(lim - fac, 10 / (this->at(fac - 1) + 1));
	int _maxn = quicklog2(lim) + 2;
	/* 初始 F*G = e 
	* 迭代一次后 G = G * ( 2 - F * G )
	* F*G = e * ( 2 - e )
	* e 初始不小于 0.5
	* 收敛的会很快，但如果 A 和 B 长度相差过大，则即便很小的误差也会被放的很大
	*/
	for (int i = 1; i <= _maxn; ++i) {
		ni = ni * (two - (*this) * ni);
		Array_func::QuickDivide10k(ni.vec, lim);
	}
	ni.positive = this->positive;
	return ni;
}




bint& bint::operator+=(const bint& b) {
	(positive == b.positive) ?
		(positive ? quickadd(*this, b, true)
			: quickadd(*this, b, false))
		: (positive ? quickdel(*this, b, true)
			: quickdel(*this, b, false));
	return*this;
}
bint& bint::operator+=(const int& b) {
	bool f=b>=0;
	(positive==f)?
		(positive?addint(*this,b,true)
			:addint(*this,-b,false))
		:(positive?delint(*this,-b,true)
			:delint(*this,b,false));
	return*this;
}
bint& bint::operator-=(const bint& b) {
	(positive == b.positive) ?
		(positive ? quickdel(*this, b, true)
			: quickdel(*this, b, false))
		: (positive ? quickadd(*this, b, true)
			: quickadd(*this, b, false));
	return*this;
}
bint& bint::operator-=(const int& b) {
	bool f=b>=0;
	(positive == f) ?
		(positive ? delint(*this, b, true)
			: delint(*this, -b, false))
		: (positive ? addint(*this, -b, true)
			: addint(*this, b, false));
	return*this;
}
bint& bint::operator*=(const bint& b) {
	quickmul(*this, b, *this);
	return*this;
}
bint& bint::operator*=(const int& b) {
	mulint(*this,b,*this);
	return*this;
}
bint& bint::operator/=(const bint& b) {
	if (b.iszero())return *this;
	return (*this) = (*this) / b;
}
bint& bint::operator/=(const int& b) {
	if(!b)return *this;
	return (*this)=(*this)/b;
}
bint& bint::operator%=(const bint& b) {
	if (b.iszero())return*this;
	(*this) -= b * ((*this) / b);
	return*this;
}
bint& bint::operator%=(const int& b) {
	if(!b)return *this;
	(*this)-=((*this)/b)*b;
	return *this;
}
bint& bint::operator|=(const bint&other) {
	if(this==&other)return*this;
	return (*this)=(this->to2bit()|other.to2bit()).to10bit();
}
bint& bint::operator&=(const bint&other) {
	if(this==&other)return*this;
	return (*this)=(this->to2bit()&other.to2bit()).to10bit();
}
bint& bint::operator^=(const bint&other) {
	if (this == &other){resize(1);vec[0]=0;return*this; }
	return (*this) = (this->to2bit() ^ other.to2bit()).to10bit();
}
bint& bint::operator++() {
	(*this)+=1;
	return*this;
}
bint&bint::operator++(int) {
	bint x = *this;
	(*this) += 1;
	return x;
}
bint&bint::operator--() {
	(*this)-=1;
	return*this;
}

bint&bint::operator--(int) {
	bint x = *this;
	(*this)-=1;
	return x;
}

bint& bint::operator>>=(const int& index) {
	if (index < 32) {
		(*this)*=(1<<index);
		return*this;
	}
	bint w = qpow(bint(2), index);
	(*this) /= w;
	return*this;
}
bint& bint::operator<<=(const int& index) {
	if (index < 32) {
		(*this)*=(1<<index);
		return*this;
	}
	bint w = qpow(bint(2), index);
	(*this) *= w;
	return*this;
}
int bint::toint()const {
	int Length = size(), x = vec[0];
	for (int i = 1; i < Length; ++i)
		x = x * jw + vec[i];
	return x;
}
long long bint::toll()const {
	int Length = size();
	long long x = vec[0];
	for (int i = 1; i < Length; ++i)
		x = x * jw + vec[i];
	return x;
}

std::string bint::tostr()const {
	std::string str;
	int Size=size();
	int x=(*this)[Size-1];
	while (x) {
		str.push_back(x%10+'0');
		x/=10;
	}
	std::reverse(str.begin(),str.end());
	for (int i = Size - 2; ~i; --i) {
		x=(*this)[i];
		for(int j=7;~j;--j)
			str.push_back((x/_10k[j])%10+'0');
	}
	return str;
}


/* 采用了分治的方法进行进制转换，否则是O(n^2)复杂度
* 思想是对于长度较小的暴力转换
* 对于长度较大的，取长度一半向上取整
* 分为 l 和 r ，并分治得到 l 和 r 转换为的 bint2 ，即转化成的 2 进制
* 然后对于 l.to2bit() 乘以 10^mid再加上 r.to2bit() 即为 当前的 to2bit() 
* 本质就是对于每一位 a_i 乘以 10 ^ i ，然后采用分治来做
* 期望复杂度是 O( n*log n )，常数带一个 0.67 ，但实际上因为我还没有预处理出 10 ^ mid
* 导致会额外进行一倍以上的运算，且递归常数较大，所以实际常数较大
*/
bint2 bint::to2bit()const {
	int Size=size();
	if (Size <= 16) {
		bint2 x;
		x=vec[Size-1];
		for(int i=Size-2;~i;--i)
			x=x*100000000+vec[i];
		return x;
	}
	int mid=(Size+1)>>1;
	bint l,r;
	r.assign(*this,0,mid);
	l.assign(*this,mid,Size);

	return l.to2bit()*qpow(bint2(100000000),mid)+r.to2bit();
}






/*---bint2类部分函数定义---*/

void bint2::assign(const int& v) {
	int val = v;
	resize(0);
	positive = true;
	if (val < 0)val = -val, positive ^= 1;
	vec[0] = v;
	if (iszero())
		vec[0] = 0;
}
void bint2::assign(const long long& v) {
	long long val = v;
	resize(0);
	positive = true;
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val >>= 32)
		vec[i] = val & maxuint;
	if (iszero())
		vec[0] = 0;
}

void bint2::assign(const char*_s) {
	char*s=const_cast<char*>(_s);
	int Length=strlen(s);
	reserve(Length>>5);
	int i=Length-1,head=0,j=0;
	int fir=0;
	while(fir<Length&&s[fir]=='-')
		positive^=1,++fir;
	while (i - fir >= 31) {
		for(j=i-31;j<=i;++j)
			vec[head]=(vec[head]<<1)+(s[j]-'0');
		++head;
		i-=32;
	}
	for(j=fir;j<=i;++j)
		vec[head]=(vec[head]<<1)+(s[j]-'0');
	if(iszero())
		vec[0]=0;

}
void bint2::assign(const std::string&s){assign(s.c_str()); }

const bool bint2::iszero() const {
	return vec.iszero();
}
void bint2::resize(const uint& len) { vec.resize(len); }
const uint bint2::size()const { return vec.size(); }
const uint bint2::length() const { return vec.length(); }
void bint2::reserve(const uint& len) { vec.reserve(len); }
void bint2::relength(const uint& index) { vec.relength(index); }

const uint& bint2::operator[](const int& index)const { return vec[index]; }
uint& bint2::operator[](const int& index) { return vec[index]; }
bool bint2::at(const int& index)const {
	return vec.at(index);
}
uint& bint2::save_at(const int& index) { return vec.save_at(index); }
bool bint2::operator<(const bint2& other)const {
	return ((positive == other.positive) && (vec < other.vec)) ^ (!positive);
}
bool bint2::operator==(const bint2& other) const {
	return ((positive == other.positive) && vec == other.vec);
}
bool bint2::operator<=(const bint2& other) const {
	return !(((positive == other.positive) && (vec > other.vec)) ^ (!positive));
}
bool bint2::operator>(const bint2& other) const { return !((*this) <= other); }
bool bint2::operator>=(const bint2& other) const { return !((*this) < other); }
bool bint2::operator!=(const bint2& other)const { return !((*this) == other); }

bint2& bint2::operator|=(const bint2&other) {
	if(this==&other)return*this;
	int n=size(),m=other.size();
	reserve(m);
	for(int i=0;i<m;++i)
		vec[i]|=other[i];
	return*this;
}

bint2& bint2::operator&=(const bint2&other) {
	if (this == &other)return*this;
	int n = size(), m = other.size();
	if (n >= m) {
		for(int i=0;i<m;++i)
			vec[i]&=other[i];
	}
	else {
		for(int i=0;i<n;++i)
			vec[i]&=other[i];
	}
	int tail=size();
	while(tail>1&&!vec[tail-1])
		--tail;
	if(tail!=size())resize(tail);
	return*this;
}

bint2& bint2::operator^=(const bint2&other) {
	if (this == &other) {resize(1);vec[0]=0;return*this;}
	int n = size(), m = other.size();
	for(int i=0;i<m;++i)
		vec[i]^=other[i];
	int tail = size();
	while (tail > 1 && !vec[tail - 1])
		--tail;
	if (tail != size())resize(tail);
	return*this;
}

bint2& bint2::operator+=(const bint2& b) {
	(positive == b.positive) ?
		(positive ? quickadd(*this, b, true)
			: quickadd(*this, b, false))
		: (positive ? quickdel(*this, b, true)
			: quickdel(*this, b, false));
	return*this;
}
bint2& bint2::operator+=(const int& b) {
	bool f = b >= 0;
	(positive == f) ?
		(positive ? addint(*this, b, true)
			: addint(*this, -b, false))
		: (positive ? delint(*this, -b, true)
			: delint(*this, b, false));
	return*this;
}
bint2& bint2::operator-=(const bint2& b) {
	(positive == b.positive) ?
		(positive ? quickdel(*this, b, true)
			: quickdel(*this, b, false))
		: (positive ? quickadd(*this, b, true)
			: quickadd(*this, b, false));
	return*this;
}
bint2& bint2::operator-=(const int& b) {
	bool f = b >= 0;
	(positive == f) ?
		(positive ? delint(*this, b, true)
			: delint(*this, b, false))
		: (positive ? addint(*this, b, true)
			: addint(*this, b, false));
	return*this;
}
bint2& bint2::operator*=(const bint2& b) {
	quickmul(*this, b, *this);
	return*this;
}

/* 和 to2bit 类似的思想
*/

bint bint2::to10bit()const {
	int Size=size();
	if (Size <= 16) {
		bint x;
		x=bint((long long)vec[Size-1]);
		for (int i = Size - 2; ~i; --i) {
			x<<=32;
			x+=bint((long long)vec[i]);
		}
	
		return x;
	}
	bint2 l,r;
	int mid=(Size+1)>>1;
	r.assign(*this,0,mid);
	l.assign(*this,mid,Size);
	return l.to10bit()*qpow(bint(1<<16),mid<<1)+r.to10bit();
}


std::string bint2::tostr()const {
	std::string str;
	uint Size=size();
	uint x=(*this)[Size-1];
	while (x) {
		str.push_back((x&1)+'0');
		x>>=1;
	}
	std::reverse(str.begin(),str.end());
	for (int i = Size - 2; ~i; --i) {
		x=(*this)[i];
		for(int j=31;~j;--j)
			str.push_back('0'+((x>>j)&1));
	}return str;
}

_MATH_END


#undef jw 