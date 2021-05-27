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

//------------------------------------------------------------------//
/*------private------*/

void bint::assign(const int& v) {
	int val = v;
	clear();
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= jw)
		vec[i] = val % jw;
}
void bint::assign(const long long& v) {
	long long val = v;
	clear();
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= jw)
		vec[i] = val % jw;
}

void bint::assign(const char* s) {
	int Length = strlen(s);
	if(!isrightint(s))return;
	clear();
	reserve(Length >> 3);
	int i = Length - 1, head = 0, j = 0;
	int fir = 0;
	while (fir < Length) {
		if(s[fir]=='-')positive^=-1;
		++fir;
		positive ^= 1, ++fir;
	}
	while (i - fir >= 7) {//从后往前每8位一搞
		for (j = i - 7; j <= i; ++j)
			vec[head] = vec[head] * 10 + s[j] - '0';
		++head;
		i -= 8;
	}
	for (j = fir; j <= i; ++j)
		vec[head] = vec[head] * 10 + s[j] - '0';
}
void bint::assign(const std::string& _s) {
	assign(_s.c_str());
}

//--------------------------------------//

void quickadd(bint& a, const bint& b, const bool& _positive, const int& head = 0) {
	int n = a.size(), m = b.size();
	if (n < m) {//若a比b小，则优化失败，转为朴素加法
		bint c(b);
		for (int i = head; i < n; ++i) {
			c.save_at(i) += a.save_at(i);
			if (c.save_at(i) >= jw) {
				c.save_at(i) -= jw;
				++c[i + 1];
			}
		}
		for (int tail = n; tail < m&&c.save_at(tail)>=jw; ++tail) {//答案最多是m+1位
			c.save_at(tail)-=jw;
			++c[tail+1];//当tail=m-1时进位则答案位数加1，且之后不会再进位
		}
		c.positive = _positive;
		if (c.iszero())
			c.positive = true;
		a = c;
		return;
	}//a比b大，则只用进行b的位数次加法
	for (int i = head; i < m; ++i) {
		a.save_at(i) += b[i];
		if (a.save_at(i) >= jw) {
			a.save_at(i) -= jw;
			++a[i + 1];
		}
	}
	for (int tail = m; tail < n && a.save_at(tail) >= jw; ++tail) {//如果n=m并且能进位，则上面就已经进位了
		a.save_at(tail) -= jw;
		++a[tail + 1];
	}
	a.positive = _positive;
	if (a.iszero())
		a.positive = true;
}
void addint(bint& a, int b, const bool& _positive) {
	if (b >= jw - a[0]) {//可能会溢出，因此要用减法这样比较
		long long val = a[0] + b;//防止溢出
		a[0] = val % jw;
		b = val / jw;
		int head = 1;
		while (a[head] + b >= jw) {
			a[head] += b - jw;
			b = 1;//考虑到b最大为2e9左右，第一次最多进位10，之后最多进位1
			++head;
		}
		a[head] += b;
	}
	else a[0] += b;//不会溢出，直接加即可
	a.positive = _positive;
}
void quickdel(bint& a, const bint& b, const bool& _positive, const int& head = 0) {
	if (a.vec < b.vec) {//类似加法
		bint c(b);
		int n = a.size(), m = b.size();
		for (int i = head; i < n; ++i) {
			c.save_at(i) -= a.save_at(i);
			if (c.save_at(i) < 0) {
				c.save_at(i) += jw;
				--c[i + 1];
			}
		}
		for (int tail = n; tail < m && c.save_at(tail) < 0; ++tail) {
			//实际上因为a<b，则最高位必定不会借位
			//之后进行极致速度优化时进行修改
			c.save_at(tail)+=jw;
			--c[tail+1];
		}

		int Length = c.size();
		while (Length > 1 && !c.save_at(Length - 1))
			--Length;
		if (Length != c.size())
			c.resize(Length);
		c.positive = !_positive;
		if (c.iszero())
			c.positive = true;
		a = c;
		return;
	}
	int n = a.size(), m = b.size();
	for (int i = head; i < m; ++i) {
		a.save_at(i) -= b[i];
		if (a.save_at(i) < 0) {
			a.save_at(i) += jw;
			--a[i + 1];
		}
	}
	for (int tail = m; tail < n && a.save_at(tail) < 0; ++tail) {
		a.save_at(tail) += jw;
		--a[tail + 1];
	}

	int Length = a.size();
	while (Length > 1 && !a.save_at(Length - 1))
		--Length;
	if (Length != a.size())
		a.resize(Length);
	a.positive = _positive;
	if (a.iszero())
		a.positive = true;
}
void delint(bint& a, int b, const bool& _positive) {
	a.positive = _positive;
	if (a.size() == 1 && a[0] <= b) {
		a[0] = b - a[0];
		a.positive ^= 1;
		return;
	}
	if (a[0] < b) {
		int val=a[0]-b;
		a[0]=(val%jw)+jw;
		b=val/jw+1;
		int head=1;
		while (a[head] < b) {
			a[head]+=jw-b;
			b=1;
			++head;
		}
		a[head]-=b;
		int tail = a.size();
		while (!a.save_at(tail - 1))
			--tail;
		a.resize(tail);
	}else a[0]-=b;
}

bint randomdivide(const bint& A, const bint& B) {
	bint copyA(A), copyB(B);
	copyA.abs(), copyB.abs();
	if (copyA < copyB)return bintzero;//如果更小，返回0
	bint copyB2, copyB4, copyB8;  
	copyB2 = copyB + copyB;
	copyB4 = copyB2 + copyB2;
	copyB8 = copyB4 + copyB4;//预处理B的1、2、4、8倍
	bint ans;
	ans.reserve(A.size() - B.size() + 1);//提前预留空间
	int val, head = 0, step = 0;
	val = 1;
	step = A.length() - B.length();
	Array_func::QuickDivide10k(copyA.vec, step);//快速除以10^step次方，将A和B的长度对齐

	val = _10k[step & 7];
	head = step >> 3;
	while (~step) {
		/*---均摊16/9次减法和4次判断---*/
		/*---朴素算法需要均摊5次减法和5次判断---*/
		if (copyA >= copyB8) {
			ans[head] += val << 3;
			quickdel(copyA, copyB8, true);
		}
		if (copyA >= copyB4) {
			ans[head] += val << 2;
			quickdel(copyA, copyB4, true);
		}
		if (copyA >= copyB2) {
			ans[head] += val << 1;
			quickdel(copyA, copyB2, true);
		}
		if (copyA >= copyB) {
			ans[head] += val;
			quickdel(copyA, copyB, true);
		}

		--step;
		if (step == -1)break;
		val = _10k[step & 7];
		head = step >> 3;
		copyA.quick_mul_10();//下一位需要先乘10
		copyA[0] += A.at(step);//再加上下一位
	}
	ans.positive = !(A.positive ^ B.positive);//符号
	return ans;
}

bint matchdivide(const bint&A, const bint&B) {
	bint copyA(A), copyB(B);
	copyA.abs(), copyB.abs();
	int lim = copyA.length() + 1;
	bint ni = copyB.inv(lim);//得到逆元，这一步是该算法复杂度的瓶颈！(精度为lim)
	bint test = ni * copyB;
	Array_func::QuickDivide10k(test.vec, lim);
	bint ans = copyA * ni;
	Array_func::QuickDivide10k(ans.vec, lim);//因为将小数乘10^lim变成整数，现在要除回去并取整
	bint mo = copyA - copyB * ans;
	bint k = mo / copyB;//降低常数的做法，如果ni的精度足够则按理误差在1左右
						//但是算法问题，只能通过得到近似的逆元，但误差也能
						//降低很多，然后将误差递归下去直到得到准确解，之后会修改算法
	ans += k;
	mo -= k * copyB;
	if (!mo.positive)
		++ans;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}

bint largedivide(const bint& A, const bint& B) {
	int n = A.size(), m = B.size(), mid = 2 * m - n - 2;
	bint copyA, copyB;
	copyA.assign(A, mid, n);//舍去2m-n-2位
	copyB.assign(B, mid, m);
	++copyA;//得到大于答案的近似解
	bint ans = copyA / copyB;
	copyA = A, copyB = B;
	copyA.abs(), copyB.abs();
	if (copyA < copyB * ans)//误差在1
		--ans;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}

bint middivide(const bint& A, const bint& B) {
	//让B长度每次长度乘以3/2，这样误差可以卡进3/2 m
	bint copyA(A), copyB(B);
	copyA.abs(), copyB.abs();
	int mid = B.size() >> 1;
	copyB.quick_mul_10k(mid << 3);//将B*10^(8mid)
	bint ans = copyA / copyB;//经过log (n/m) 次可以即可通过largedivide缩小n的范围
	copyB = B;
	copyB.abs();
	bint mo=copyA - (ans*copyB).quick_mul_10k(mid<<3);
	ans.quick_mul_10k(mid<<3);
	ans += mo / copyB;//误差长度是B长度的2/3倍以内，因此可以直接卡进largedivide
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}
bint smalldivide(const bint& A, const bint& B) {
	int n = A.size(), m = B.size(), mid = (n - m) >> 1;
	bint copyA, copyB(B); 
	copyA.assign(A, mid, n);
	copyB.abs();
	bint ans = copyA / copyB;
	copyA = A;
	copyA.abs();
	bint mo = copyA - (ans * copyB).quick_mul_10k(mid << 3);
	ans.quick_mul_10k(mid << 3);
	ans += mo / copyB;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}


bint quickdivide(const bint& A, const bint& B) {
	if (A.vec < B.vec)return bintzero;
	int n = A.size(), m = B.size();
	if(m==1)return divideint(A,B[0]*(B.positive?1:-1));//低精度除法
	if (m <= (sqr(quicklog2(n)) >> 3 ) + 6) return randomdivide(A, B);//m比较小直接用O(m*(n-m))算法
	if (m * 3 < 2 * n) return smalldivide(A, B);
	return largedivide(A,B);
}

bint divideint(const bint& A, int B) {
	bint ans;
	ans.positive = A.positive;
	if (B < 0)ans.positive ^= 1, B *= -1;
	int Size = A.size();
	long long copyA = 0;
	ans.reserve(Size);
	for (int i = Size - 1; ~i; --i) {
		copyA = copyA * jw + A[i];
		if(copyA>=B)
			ans[i] = copyA / B , copyA %= B;
	}
	return ans;
}

void quickadd10k(bint& a, const bint& b, const bool& _positive, const int& k) {
	int n = a.size(), m = b.size();
	if (n < m + k) {//若a比b小，则优化失败，转为朴素加法
		bint c;
		c.reserve(m + k);
		for (int i = 0; i < m; ++i)
			c[i + k] = b[i];
		for (int i = 0; i < n; ++i) {
			c.save_at(i) += a.save_at(i);
			if (c.save_at(i) >= jw) {
				c.save_at(i) -= jw;
				++c[i + 1];
			}
		}
		int tail = n;
		while (c.size() > tail && c.save_at(tail) >= jw) {
			c.save_at(tail) -= jw;
			++c[tail + 1];
			++tail;
		}
		c.positive = _positive;
		if (c.iszero())
			c.positive = true;
		a = c;
		return;
	}//a比b大，则只用进行b的为数次加法
	for (int i = 0; i < m; ++i) {
		a.save_at(i + k) += b[i];
		if (a.save_at(i + k) >= jw) {
			a.save_at(i + k) -= jw;
			++a[i + 1];
		}
	}
	int tail = m;
	while (a.size() > tail && a.save_at(tail) >= jw) {
		a.save_at(tail) -= jw;
		++a[tail + 1];
		++tail;
	}
	a.positive = _positive;
	if (a.iszero())
		a.positive = true;
}

void Karatsuba(const bint& a, const bint& b, bint& c) {
	int lena = a.size(), lenb = b.size(), _min = min(lena, lenb), _max = _min ^ lena ^ lenb;
	if (_min <= 32) {
		Array_func::SlowMul(a.vec, b.vec, c.vec);
		return;
	}
	int midlen = (_max + 1) >> 1;
	bint A, B, C, D;
	A.assign(a, midlen, lena);
	B.assign(a, 0, midlen);
	C.assign(b, midlen, lenb);
	D.assign(b, 0, midlen);
	c = B * D;
	bint F, G;
	Karatsuba(A, C, F);
	Karatsuba(A + B, C + D, G);
	G -= c;
	G -= F;
	quickadd10k(c, G, true, midlen);
	quickadd10k(c, F, true, midlen << 1);
}

void quickmul(const bint& a, const bint& b, bint& c) {
	if (a.iszero() || b.iszero()) {
		c = 0;
		c.positive = true;
		return;
	}
	int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

	/*---根据不同范围选择不同算法---*/
	if (_max <= 32) {
		(_min  <= (quicklog2(_max) * 3) + 8) ?
			Array_func::SlowMul(a.vec, b.vec, c.vec) :
			FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
	}
	else {
		(_min <= (quicklog2(_max) << 1) + 4) ?
			Array_func::SlowMul(a.vec, b.vec, c.vec) :
			FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
	}


	c.positive = !(a.positive ^ b.positive);
	if (c.iszero())c.positive = true;
}

void mulint(const bint& a, const int& b, bint& c) {
	bool f = b >= 0;
	int n = a.size();
	c.reserve(n);
	for (int i = n - 1; ~i; --i) {
		long long val = a[i] * 1ll * b;
		if (val >= jw) {
			c[i] = val % jw;
			c[i + 1] += val / jw;
		}
		else c[i] = val;
	}
	n = c.size();
	for (int i = 0; i < n; ++i)
		if (c[i] >= jw) {
			c[i + 1] += c[i] / jw;
			c[i] %= jw;
		}
	c.positive = !(a.positive ^ f);
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


void bint::clear() {
	vec.clear();
	positive = true;
}

//-----------------------------------------------//

/*------public------*/

void bint::assign(const bint& other, const int& L, const int& R) {
	int Size = other.size();
	int l = min(L, Size), r = min(R, Size);
	resize(max(1, r - l));
	this->save_at(0) = 0;
	for (int i = l; i < r; ++i)
		this->save_at(i - l) = other[i];
	int tail = r - l;
	while (tail > 1 && !this->save_at(tail - 1))--tail;
	if (tail != size())resize(tail);
}

const bool bint::iszero() const {
	return vec.iszero();
}

void bint::resize(const int& len){ vec.resize(len); }
const int bint::size()const { return vec.size(); }
const int bint::length() const { return vec.length(); }
void bint::reserve(const int& len) { vec.reserve(len); }
void bint::relength(const int& index) { vec.relength(index); }
void bint::reverse(){vec.reverse(); }
bint& bint::append(const bint&other){
	if(positive^other.positive)return *this;
	vec.append(other.vec);
	return*this;
}

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


ostream& operator<<(ostream& out, const bint& x) {
	if (!x.positive)out << "-";
	out << x[x.size() - 1];
	for (int i = x.size() - 2; i >= 0; --i)
		out << std::setw(8) << std::setfill('0') << x[i];
	return out;
}

istream& operator>>(istream& in, bint& x) {
	std::string s;
	in >> s;
	x = s;
	return in;
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
	bool f = b >= 0;
	(positive == f) ?
		(positive ? addint(*this, b, true)
			: addint(*this, -b, false))
		: (positive ? delint(*this, -b, true)
			: delint(*this, b, false));
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
	bool f = b >= 0;
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
	mulint(*this, b, *this);
	return*this;
}

bint& bint::operator/=(const bint& b) {
	if (b.iszero())return *this;
	return (*this) = (*this) / b;
}

bint& bint::operator/=(const int& b) {
	if (!b)return *this;
	return (*this) = (*this) / b;
}

bint& bint::operator%=(const bint& b) {
	if (b.iszero())return*this;
	(*this) -= b * ((*this) / b);
	return*this;
}

bint& bint::operator%=(const int& b) {
	if (!b)return *this;
	(*this) -= ((*this) / b) * b;
	return *this;
}

bint& bint::operator|=(const bint& other) {
	if (this == &other)return*this;
	return (*this) = (this->to2bit() | other.to2bit()).to10bit();
}

bint& bint::operator&=(const bint& other) {
	if (this == &other)return*this;
	return (*this) = (this->to2bit() & other.to2bit()).to10bit();
}

bint& bint::operator^=(const bint& other) {
	if (this == &other) { resize(1); vec[0] = 0; return*this; }
	return (*this) = (this->to2bit() ^ other.to2bit()).to10bit();
}

bint& bint::operator++() {
	(*this) += 1;
	return*this;
}

bint bint::operator++(int) {
	bint x = *this;
	(*this) += 1;
	return x;
}

bint& bint::operator--() {
	(*this) -= 1;
	return*this;
}

bint bint::operator--(int) {
	bint x = *this;
	(*this) -= 1;
	return x;
}

bint& bint::operator>>=(const int& index) {
	if (index < 32) {
		(*this) *= (1 << index);
		return*this;
	}
	bint w = qpow(bint(2), index);
	(*this) /= w;
	return*this;
}
bint& bint::operator<<=(const int& index) {
	if (index < 32) {
		(*this) *= (1 << index);
		return*this;
	}
	bint w = qpow(bint(2), index);
	(*this) *= w;
	return*this;
}

bint operator+(const bint& X) {
	return bint(X.vec, true);
}

bint operator-(const bint& X) {
	return bint(X.vec, false);
}

bint operator+(const bint& a, const bint& b) {
	bint c(a);
	return c += b;
}

bint operator+(const bint& a, const int& b) {
	bint c(a);
	return c += b;
}

bint operator+(const int& a, const bint& b) {
	bint c(b);
	return c += a;
}

bint operator-(const bint& a, const bint& b) {
	bint c(a);
	return c -= b;
}

bint operator-(const bint& a, const int& b) {
	bint c(a);
	return c -= b;
}

bint operator-(const int& a, const bint& b) {
	bint c(b);
	c -= a;
	c.positive ^= 1;
	return c;
}

bint operator*(const bint& a, const bint& b) {
	bint c;
	quickmul(a, b, c);
	return c;
}
bint operator*(const bint& a, const int& b) {
	bint c;
	mulint(a, b, c);
	return c;
}

bint operator*(const int& a, const bint& b) {
	bint c;
	mulint(b, a, c);
	return c;
}

bint operator/(const bint& a, const bint& b) {
	if (b.iszero())return a;
	return quickdivide(a, b);
}

bint operator/(const bint& a, const int& b) {
	if (!b)return a;
	return divideint(a, b);
}

bint operator/(const int& a, const bint& b) {
	if (b.iszero())return bint(a);
	if (b.size() == 1 && b[0] <= a)
		return bint(a / b[0]);
	return bintzero;
}

bint operator%(const bint& a, const bint& b) {
	if (b.iszero())return a;
	return a - b * (a / b);
}

bint operator%(const bint& a, const int& b) {
	if (!b)return a;
	return a - (a / b) * b;
}

bint operator%(const int& a, const bint& b) {
	if (b.iszero())return bint(a);
	return a - (a / b) * b;
}

bint operator>>(bint a, const int& index) {
	if (index < 32) {
		a /= (1 << index);
		return a;
	}
	bint w = qpow(bint(2), index);
	a /= w;
	return a;
}

bint operator<<(bint a, const int& index) {
	if (index < 32) {
		a *= (1 << index);
		return a;
	}
	bint w = qpow(bint(2), index);
	a *= w;
	return a;
}

bint operator|(const bint& lhs, const bint& rhs) {
	bint s(lhs);
	s |= rhs;
	return s;
}

bint operator&(const bint& lhs, const bint& rhs) {
	bint s(lhs);
	s &= rhs;
	return s;
}

bint operator^(const bint& lhs, const bint& rhs) {
	bint s(lhs);
	s ^= rhs;
	return s;
}

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
bint bintabs(const bint& x) {
	return bint(x.vec, true);
}
bint qpow(bint a, int b) {
	bint s(1);
	while (b) {
		if (b & 1)
			s *= a;
		a *= a;
		b >>= 1;
	}return s;
}
bint qpow(bint a, bint b) {
	bint s(1);
	while (!b.iszero()) {
		if (b[0] & 1)
			s *= a;
		a *= a;
		b >>= 1;
	}return s;
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
	//其实qpow这一步是可以通过预处理得到的
}


//------------------------------------------------------------------//


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

bint2 add(const bint2& a, const bint2& b, const bool& _positive) {
	int n = a.size(), m = b.size();
	bint2 c;
	c.reserve(n + 1);
	c.resize(n);
	for (int i = 0; i < m; ++i) {
		if (a[i] + b[i] > (c[i] ^ maxuint))//加起来比maxunsigned int 更大
			++c[i + 1];//c[i+1]肯定为0,因为是顺着跑
		c[i] += a[i] + b[i];//自动取模
	}
	for (int i = m; i < n; ++i) {
		if (a[i] > (c[i] ^ maxuint))
			++c[i + 1];

		c[i] += a[i];
	}
	c.positive = _positive;
	if (c.iszero())
		c.positive = true;
	return c;
}
void quickadd(bint2& a, const bint2& b, const bool& _positive, const int& head = 0) {
	int n = a.size(), m = b.size();
	if (n < m) {
		a = add(b, a, _positive);
		return;
	}
	for (int i = head; i < m; ++i) {
		if (b[i] > (a.save_at(i) ^ maxuint))
			a.saveadd(i + 1);//a[i+1]可能会溢出
		a.save_at(i) += b[i];
	}
	a.positive = _positive;
	if (a.iszero())
		a.positive = true;
}


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