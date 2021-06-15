#include "bint.h"
#include <random>
_MATH_BEGIN



/*---bint类部分函数定义---*/

//------------------------------------------------------------------//
/*------private------*/

void bint::assign(const int& v) {
	int val = v;
	clear();
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= bintjw)
		vec[i] = val % bintjw;
}
void bint::assign(const long long& v) {
	long long val = v;
	clear();
	if (val < 0)val = -val, positive ^= 1;
	for (int i = 0; val; ++i, val /= bintjw)
		vec[i] = val % bintjw;
}

void bint::assign(const char* s) {
	int Length = strlen(s);
	if (!isrightint(s))return;
	clear();
	reserve(Length >> 3);
	int i = Length - 1, head = 0, j = 0;
	int fir = 0;
	if (s[fir] == '-')positive ^= 1, ++fir;
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

void bint::quickadd(bint& a, const bint& b, const bool& _positive) {
	int n = a.size(), m = b.size();
	if (n < m) {//若a比b小，则优化失败，转为朴素加法
		bint c(b);
		for (int i = 0; i < n; ++i) {
			c.save_at(i) += a.save_at(i);
			if (c.save_at(i) >= bintjw) {
				c.save_at(i) -= bintjw;
				++c.save_at(i+1);
			}
		}
		for (int tail = n; tail < m && c.save_at(tail) >= bintjw; ++tail) {//答案最多是m+1位
			c.save_at(tail) -= bintjw;
			++c[tail + 1];//当tail=m-1时进位则答案位数加1，且之后不会再进位
		}
		c.positive = _positive;
		if (c.iszero())
			c.positive = true;
		a = std::move(c);
		return;
	}//a比b大，则只用进行b的位数次加法
	for (int i = 0; i < m - 1; ++i) {
		a.save_at(i) += b[i];
		if (a.save_at(i) >= bintjw) {
			a.save_at(i) -= bintjw;
			++a.save_at(i+1);
		}
	}
	a.save_at(m-1)+=b[m-1];
	if (a.save_at(m - 1) >= bintjw) {
		a.save_at(m-1)-=bintjw;
		++a[m];
	}
	for (int tail = m; tail < n && a.save_at(tail) >= bintjw; ++tail) {//如果n=m并且能进位，则上面就已经进位了
		a.save_at(tail) -= bintjw;
		++a[tail + 1];
	}
	a.positive = _positive;
	if (a.iszero())
		a.positive = true;
}
void bint::addint(bint& a, int b, const bool& _positive) {
	if (b >= bintjw - a[0]) {//可能会溢出，因此要用减法这样比较
		long long val = a[0] + b;//防止溢出
		a[0] = val % bintjw;
		b = val / bintjw;
		int head = 1;
		while (a[head] + b >= bintjw) {
			a[head] += b - bintjw;
			b = 1;//考虑到b最大为2e9左右，第一次最多进位10，之后最多进位1
			++head;
		}
		a[head] += b;
	}
	else a[0] += b;//不会溢出，直接加即可
	a.positive = _positive;
}
void bint::quickdel(bint& a, const bint& b, const bool& _positive) {
	if (a.vec < b.vec) {//类似加法
		bint c(b);
		int n = a.size(), m = b.size();
		for (int i = 0; i < n; ++i) {
			if(c.save_at(i) < a.save_at(i)) {
				int j=i+1;
				c.save_at(i)+=bintjw-a.save_at(i);
				while(!c.save_at(j))
					c.save_at(j)=bintjw-1,++j;
				--c.save_at(j);
			}else c.save_at(i)-=a.save_at(i);
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
	for (int i = 0; i < m; ++i) {
		if (a.save_at(i) < b[i]) {
			int j=i+1;
			a.save_at(i)+=bintjw-b[i];
			while (!a.save_at(j)) {
				a.save_at(j)=bintjw-1;
				++j;
			}
			--a.save_at(j);
		}else a.save_at(i)-=b[i];
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
void bint::delint(bint& a, int b, const bool& _positive) {
	a.positive = _positive;
	if (a.size() == 1) {
		if (a.save_at(0) < b) {
			a.save_at(0) = b - a.save_at(0);
			a.positive ^= 1;
		}
		else {
			a.save_at(0) -= b;
			if(!a.save_at(0))
				a.positive=true;
		}
		return;
	}
	if (a < b) {
		int X=a.save_at(1)*bintjw+a.save_at(0);
		int Y=b-X;
		a.save_at(0)=Y%bintjw;
		a.save_at(1)=Y/bintjw;
		if(!a.save_at(1))
			a.resize(1);
		a.positive^=1;
		return ;
	}
	a-=bint(b);
}

bint bint::randomdivide(const bint& A, const bint& B) {
	bint copyA(A), copyB(B);
	copyA.abs(), copyB.abs();
	if (copyA < copyB)return bintzero;//如果更小，返回0
	bint copyB2(copyB + copyB), copyB4(copyB2 + copyB2), copyB8(copyB4 + copyB4);
	bint ans;
	ans.reserve(A.size() - B.size() + 1);//提前预留空间
	int val, head = 0, step = 0;
	val = 1;
	step = A.length() - B.length();
	Array_func::QuickDivide10k(copyA.vec, step);//快速除以10^step次方，将A和B的长度对齐
	val = _10k[step & 7];
	head = step >> 3;
	while (~step) {
		/*---均摊15/9次减法和4次判断---*/
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

bint bint::matchdivide(const bint& A, const bint& B) {
	bint copyA(A), copyB(B);
	copyA.abs(), copyB.abs();
	int lim = copyA.length() + copyB.length();
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

bint bint::largedivide(const bint& A, const bint& B) {
	int n = A.size(), m = B.size(), mid = 2 * m - n - 2;
	bint copyA, copyB;
	copyA.assign(A, mid, n);//舍去2m-n-2位
	copyB.assign(B, mid, m);
	++copyA;//得到大于答案的近似解
	bint ans = copyA / copyB;

	if (Math::abs(A) < Math::abs(B) * ans)//误差在1
		--ans;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}

bint bint::middivide(const bint& A, const bint& B) {
	int n = A.size(), m = B.size(), mid = (5 * m) / 2 - n - 2;
	bint copyA, copyB;
	copyA.assign(A, mid, n);//舍去2m-n-2位
	copyB.assign(B, mid, m);
	++copyA;//得到大于答案的近似解
	bint ans = copyA / copyB;
	copyA = A, copyB = B;
	copyA.abs(), copyB.abs();
	ans += (copyA - ans * copyB) / copyB;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}
bint bint::smalldivide(const bint& A, const bint& B) {
	int n = A.size(), m = B.size(), mid = (n - m) >> 1;
	bint copyA, copyB(Math::abs(B));
	copyA.assign(A, mid, n);
	bint ans = copyA / copyB;
	bint mo = Math::abs(A) - (ans * copyB).quick_mul_10k(mid << 3);
	ans.quick_mul_10k(mid << 3);
	ans += mo / copyB;
	ans.positive = !(A.positive ^ B.positive);
	return ans;
}

bint bint::knuthdivide(const bint& A, const bint& B) {//比我那个优化的暴力略快一点
	bint r(Math::abs(A)), copyB(Math::abs(B));
	bint ans;
	int BW = bintjw;

	if (copyB[copyB.size() - 1] < (BW >> 1)) {
		int a = BW >> 1, b = copyB[copyB.size() - 1];
		int K = (a + b - 1) / b;
		r *= K, copyB *= K;
	}

	int n = copyB.size(), m = r.size() - copyB.size();
	bint copyA(r);
	r.quick_divide_10k(m << 3);
	int i = n + m, j = m;
	do {
		int q;
		if (r.size() > n)
			q = min((long long)BW - 1, (r[n] * 1ll * BW + r[n - 1]) / copyB[n - 1]);
		else if (r.size() > n - 1)
			q = min(BW - 1, r[n - 1] / copyB[n - 1]);
		else q = 0;
		r -= copyB * q;
		while (!r.ispositive())
			--q, r += copyB;
		r.quick_mul_10k(8);
		if (j > 0)r.save_at(0) = copyA.save_at(j-1);
		ans[j] = q;
		--i, --j;
	} while (i >= n);
	int Size=ans.size();
	while (Size>1&&!ans.save_at(Size-1))
		--Size;
	ans.resize(Size);
	return ans;
}


bint bint::quickdivide(const bint& A, const bint& B) {
	if (A.vec < B.vec)return bintzero;
	int n = A.size(), m = B.size();
	if (m == 1)return divideint(A, B[0] * (B.positive ? 1 : -1));//低精度除法
	if (m <= quicklog2(n) + 32) return knuthdivide(A, B);//m比较小直接用O(m*(n-m))算法
	if (m * 3 >= 2 * n)return largedivide(A, B);
	return smalldivide(A, B);
}

bint bint::divideint(const bint& A, int B) {
	bint ans;
	ans.positive = A.positive;
	if (B < 0)ans.positive ^= 1, B *= -1;
	uint copyB=B;
	int Size = A.size();
	ull copyA = 0;
	ans.reserve(Size);
	for (int i = Size - 1; ~i; --i) {
		copyA = copyA * bintjw + A[i];
		if (copyA >= copyB)
			ans[i] = copyA / copyB, copyA %= copyB;
	}
	return ans;
}


void bint::Karatsuba(const bint& a, const bint& b, bint& c) {
	size_t lena = a.size(), lenb = b.size(), _min = min(lena, lenb), _max = max(lena,lenb);
	if (_max<=64) {
		if (a.iszero() || b.iszero()) {
			c = bintzero;
			return;
		}

		Array_func::SlowMul(a.vec, b.vec, c.vec);
		return;
	}

	int midlen = (_max + 1) >> 1;
	bint B(a,0,midlen), D(b,0,midlen);
	Karatsuba(B, D, c);
	bint A(a, midlen, lena), C(b, midlen, lenb);
	bint F;
	Karatsuba(A, C, F);
	bint G;
	Karatsuba(A + B, C + D, G);
	
	G -= c + F;
	c.resize(lena + lenb);
	for (int i = F.size() - 1; ~i; --i) 
		c.save_at(i + (midlen << 1)) += F.save_at(i);
	for (int i = G.size() - 1; ~i; --i)
		c.save_at(i + midlen) += G.save_at(i);
	int Siz=c.size();
	for (int i = midlen; i < Siz - 1 ; ++i) {
		while(c.save_at(i)>=bintjw)
			++c.save_at(i+1),c.save_at(i)-=bintjw;
	}
	while(c.save_at(Siz-1)>=bintjw)
		++c[Siz],c.save_at(Siz-1)-=bintjw;
	Siz=c.size();
	while(Siz>1&&!c.save_at(Siz-1))
		--Siz;
	if(c.size()!=Siz)
		c.resize(Siz);
}

void bint::quickmul(const bint& a, const bint& b, bint& c) {
	if (a.iszero() || b.iszero()) {
		c=bintzero;
		return;
	}
	int n = a.size(), m = b.size(), Min = min(n, m), Max = n ^ m ^ Min;

	/*---根据不同范围选择不同算法---*/

	if (Min != 1) {
		if (!(Max >> 5)) 
			Array_func::SlowMul(a.vec, b.vec, c.vec);
		else {
			(Min <= (quicklog2(Max)<<1)+32) ?
				Array_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}
	}
	else {
		if (n == 1)
			mulint(b, a[0], c);
		else mulint(a, b[0], c);
	}
	c.positive = !(a.positive ^ b.positive);
	if (c.iszero())c.positive = true;
}


void bint::mulint(const bint& a, const int& b, bint& c) {
	if (!b) {
		c.clear();
		return;
	}
	int copyb = b;
	bool f = copyb >= 0;
	if (copyb < 0)copyb *= -1;
	int n = a.size();

	c.resize(n + 1);
	for (int i = n - 1; ~i; --i) {
		ull val = a[i] * 1ll * copyb;
		if (val >= bintjw) {
			c.save_at(i) = val % bintjw;
			c.save_at(i + 1) += val / bintjw;
		}
		else c.save_at(i) = val;
	}
	n = c.size();
	for (int i = 0; i < n - 1 ; ++i)
		if (c.save_at(i) >= bintjw) {
			c.save_at(i+1) += c.save_at(i) / bintjw;
			c.save_at(i) %= bintjw;
		}
	while (n > 1 && !c.save_at(n - 1))
		--n;
	if (n != c.size())
		c.resize(n);
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

void bint::resize(const size_t& len) { vec.resize(len); }
const size_t bint::size()const { return vec.size(); }
void bint::reserve(const size_t& len) { vec.reserve(len); }
const int& bint::operator[](const size_t& index)const { return vec[index]; }
int& bint::operator[](const size_t& index) { return vec[index]; }
int& bint::save_at(const size_t& index) { return vec.save_at(index); }

//-----------------------------------------------//

/*------public------*/

void bint::assign(const bint& other, const int& L, const int& R) {
	int Size = other.size();
	int l = min(L, Size), r = min(R, Size);
	if (l == r) {
		clear();
		return;
	}
	vec.assign(other.vec,l,r);
	int tail = r - l;
	while (tail > 1 && !this->save_at(tail - 1))--tail;
	if (tail != size())resize(tail);
}

const bool bint::iszero() const {
	return vec.iszero();
}
const bool bint::ispositive()const {
	return positive;
}

const size_t bint::length() const { return vec.length(); }
void bint::relength(const int& index) { vec.relength(index); }
void bint::reverse(int Length) {
	if (Length == -1)Length = length();
	for (int i = 0; (i << 1) < Length; ++i) {
		int u = at(i), v = at(Length - 1 - i);
		set(i, v);
		set(Length - 1 - i, u);
	}
}

uint bint::at(const size_t& index)const {
	return vec.at(index);
}
void bint::set(const size_t& index, const uint& val) {
	vec.set(index, val);
}

void bint::randdata(const bint& other) {
	static std::mt19937 mt_rand(time(NULL));
	int Siz=other.size();
	resize(Siz);
	bool tmp=true;
	for (int i = Siz - 1; i >= 0; --i) {
		int HEAD;
		if(tmp)
			HEAD=mt_rand()%(other[i]+1);
		else HEAD=mt_rand()%bintjw;
		if(HEAD!=other[i])tmp=false;
		(*this)[i]=HEAD;
	}
}

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

bool operator<(const bint& lhs, const bint& rhs) {
	return ((lhs.positive == rhs.positive) && (lhs.vec < rhs.vec)) ^ (!lhs.positive);
}
bool operator<(const bint& lhs, const int& rhs) {
	return lhs < bint(rhs);//经典偷懒 
}
bool operator<(const int& lhs, const bint& rhs) {
	return bint(lhs) < rhs;
}

bool operator==(const bint& lhs, const bint& rhs) {
	return ((lhs.positive == rhs.positive) && lhs.vec == rhs.vec);
}
bool operator==(const bint& lhs, const int& rhs) {
	return lhs == bint(rhs);
}
bool operator==(const int& lhs, const bint& rhs) {
	return bint(lhs) == rhs;
}

bool operator<=(const bint& lhs, const bint& rhs) {
	return !(((lhs.positive == rhs.positive) && (lhs.vec > rhs.vec)) ^ (!lhs.positive));
}
bool operator<=(const bint& lhs, const int& rhs) {
	return lhs <= bint(rhs);
}
bool operator<=(const int& lhs, const bint& rhs) {
	return bint(lhs) <= rhs;
}

bool operator>(const bint& lhs, const bint& rhs) { return !(lhs <= rhs); }
bool operator>(const bint& lhs, const int& rhs) { return lhs > bint(rhs); }
bool operator>(const int& lhs, const bint& rhs) { return bint(lhs) > bint(rhs); }

bool operator>=(const bint& lhs, const bint& rhs) { return !(lhs < rhs); }
bool operator>=(const bint& lhs, const int& rhs) { return lhs >= bint(rhs); }
bool operator>=(const int& lhs, const bint& rhs) { return bint(lhs) >= rhs; }

bool operator!=(const bint& lhs, const bint& rhs) { return !(lhs == rhs); }
bool operator!=(const bint& lhs, const int& rhs) { return lhs != bint(rhs); }
bool operator!=(const int& lhs, const bint& rhs) { return bint(lhs) != rhs; }


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
		(*this) /= (1 << index);
		return*this;
	}
	(*this) /= qpow(bint(2), index);
	return*this;
}
bint& bint::operator<<=(const int& index) {
	if (index < 32) {
		(*this) *= (1 << index);
		return*this;
	}
	(*this) *= qpow(bint(2), index);
	return*this;
}

bint operator>>(bint a, const int& index) {
	if (index < 32) {
		a /= (1 << index);
		return a;
	}
	a /= qpow(bint(2), index);
	return a;
}

bint operator<<(bint a, const int& index) {
	if (index < 32) {
		a *= (1 << index);
		return a;
	}
	a *= qpow(bint(2), index);
	return a;
}


bint operator+(bint X) {
	return X;
}

bint operator-(bint X) {
	if (X.iszero())return bintzero;
	return bint(std::move(X),X.positive^1);
}

bint operator+(bint a, const bint& b) {
	a+=b;
	return a;
}

bint operator+(bint a, const int& b) {
	a+=b;
	return a;
}

bint operator+(const int& a, bint b) {
	b+=a;
	return b;
}

bint operator-(bint a, const bint& b) {
	a-=b;
	return a;
}

bint operator-(bint a, const int& b) {
	a-=b;
	return a;
}

bint operator-(const int& a, bint b) {
	bint c(std::move(b));
	c -= a;
	c.positive ^= 1;
	if(c.iszero())
		c.positive=true;
	return c;
}

bint operator*(const bint& a, const bint& b) {
	bint c(a.size()+b.size(),-1);
	bint::quickmul(a, b, c);
	return c;
}
bint operator*(const bint& a, const int& b) {
	bint c(a.size()+1,-1);
	bint::mulint(a, b, c);
	return c;
}

bint operator*(const int& a, const bint& b) {
	bint c(b.size()+1,-1);
	bint::mulint(b, a, c);
	return c;
}

bint operator/(const bint& a, const bint& b) {
	if (b.iszero())return a;
	return bint::quickdivide(a, b);
}

bint operator/(const bint& a, const int& b) {
	if (!b)return a;
	return bint::divideint(a, b);
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

bint qpow(bint a, bint b) {
	bint ans(1);
	while (!b.iszero()) {
		if (b.save_at(0) & 1)ans *= a;
		a *= a;
		b /= 2;
	}return ans;
}
bint qpow(bint a, int b) {
	bint ans(1);
	while (b) {
		if (b & 1)ans *= a;
		a *= a;
		b >>= 1;
	}return ans;
}
bint qpow(int a, bint b) {
	return qpow(bint(a), b);
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

bint& bint::quick_mul_10k(const int& k) {
	Array_func::QuickMul10k(this->vec, k);
	return*this;
}

bint& bint::quick_divide_10k(const int& k) {
	Array_func::QuickDivide10k(this->vec, k);
	return*this;
}

void bint::abs() {
	this->positive = true;
}

bint abs(const bint& x) {
	return bint(x.vec, true);
}
bint gcd(const bint& a, const bint& b) {
	return b.iszero()?a:gcd(b,a%b);
}

void bint::swap(bint& other) {
	Math::swap(vec,other.vec);
	std::swap(positive,other.positive);
}

int bint::toint()const {
	int Length = size(), x = vec[0];
	for (int i = 1; i < Length; ++i)
		x = x * bintjw + vec[i];
	return x;
}
long long bint::toll()const {
	int Length = size();
	long long x = vec[0];
	for (int i = 1; i < Length; ++i)
		x = x * bintjw + vec[i];
	return x;
}

std::string bint::tostr()const {
	std::string str;
	int Size = size();
	int x = (*this)[Size - 1];
	while (x) {
		str.push_back(x % 10 + '0');
		x /= 10;
	}
	std::reverse(str.begin(), str.end());
	for (int i = Size - 2; ~i; --i) {
		x = (*this)[i];
		for (int j = 7; ~j; --j)
			str.push_back((x / _10k[j]) % 10 + '0');
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
	int Size = size();
	if (Size <= 16) {
		bint2 x;
		x = vec[Size - 1];
		for (int i = Size - 2; ~i; --i)
			x = x * bint2(bintjw) + vec[i];
		return x;
	}
	int mid = (Size + 1) >> 1;
	bint l, r;
	r.assign(*this, 0, mid);
	l.assign(*this, mid, Size);

	return l.to2bit() * (qpow(bint2(bintjw), mid)) + r.to2bit();
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

void bint2::assign(const char* _s) {
	char* s = const_cast<char*>(_s);
	int Length = strlen(s);
	reserve(Length >> 5);
	int i = Length - 1, head = 0, j = 0;
	int fir = 0;
	while (fir < Length && s[fir] == '-')
		positive ^= 1, ++fir;
	while (i - fir >= 31) {
		for (j = i - 31; j <= i; ++j)
			vec[head] = (vec[head] << 1) + (s[j] - '0');
		++head;
		i -= 32;
	}
	for (j = fir; j <= i; ++j)
		vec[head] = (vec[head] << 1) + (s[j] - '0');
	if (iszero())
		vec[0] = 0;

}
void bint2::assign(const std::string& s) { assign(s.c_str()); }

bint2 bint2::add(const bint2& a, const bint2& b, const bool& _positive) {
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
void bint2::quickadd(bint2& a, const bint2& b, const bool& _positive, const int& head = 0) {
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

ostream& operator<<(ostream& out, const bint2& x) {
	if (!x.positive)out << "-";
	int Length = x.length();
	for (int i = Length - 1; ~i; --i)
		out << x.at(i);
	return out;
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

bint2 qpow(bint2 a, int b) {
	bint2 s(1);
	while (b) {
		if (b & 1)
			s *= a;
		a *= a;
		b >>= 1;
	}return s;
}

std::string bint2::tostr()const {
	std::string str;
	uint Size = size();
	uint x = (*this)[Size - 1];
	while (x) {
		str.push_back((x & 1) + '0');
		x >>= 1;
	}
	std::reverse(str.begin(), str.end());
	for (int i = Size - 2; ~i; --i) {
		x = (*this)[i];
		for (int j = 31; ~j; --j)
			str.push_back('0' + ((x >> j) & 1));
	}return str;
}

/* 和 to2bit 类似的思想
*/

bint bint2::to10bit()const {
	int Size = size();
	if (Size <= 16) {
		bint x;
		x = bint((long long)vec[Size - 1]);
		for (int i = Size - 2; ~i; --i) {
			x <<= 32;
			x += bint((long long)vec[i]);
		}

		return x;
	}
	bint2 l, r;
	int mid = (Size + 1) >> 1;
	r.assign(*this, 0, mid);
	l.assign(*this, mid, Size);
	return l.to10bit() * (qpow(bint(1 << 16), (mid << 1))) + r.to10bit();
}

_MATH_END

