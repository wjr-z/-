/* bigintger 类
* 分为 bint ( 10-bit ) 和 bint2 (2 - bit)
* bint (10-bit):
*
* 因为均采用了压位，以下 "第 index 位" 代表 10-bit/2-bit 第 index 位的数 ， "第 index 个元素" 代表压位后的数
* 元素代表内部记录的每个数，而非每个位置下的数
*
* 运算符：
* 包含常用的运算符 +、-、*、/、%，其中 * 复杂度是 O ( n log n ) , / 和 % 复杂度是 O ( n log ^ 2 n)
* +=、-=、*=、/=、++、--、<、>、<=、>=、==、!=等
* 赋值为 int、long long、const char* （末尾有\0）、string
*
* at(index) : 返回index位
* 下标访问[index] :（支持const 和非const) 返回 10^8进制下index个元素，会自动扩展，请注意！
* save_at(index) : 第index个元素，去掉了边界检查和动态扩展
*
* a.abs() : a变为绝对值
* abs(a) : 返回a的绝对值，非必要则尽量不用，避免拷贝耗时
*
* a.assign(b,L,R) : 初始化为 b 的 [L,R] 的元素
*
* a.iszero() : 用于判断是否为 0 ，比 a!=0 会更快
*
* a.size() : 返回 a 的元素个数
* a.length() : 返回位数（非元素个数）
*
* a.resize(Size) : 元素个数变为 Size
* a.reserve(Size) : 预留 Size 的容量
* a.relenth(Length) : 将 a 的 位数变为 length
*
* a.quick_mul_10() : 将 a 快速乘以 10 O(n)
* a.quick_mul_10k(k) : 将 a 快速乘以 10 ^ k (O(n))
*
* a.set(index,val) : 将 a 的第 index 位 设置为 val
*
* a.toint() : 返回 a 的 int 值
* a.toll() : 返回 a 的 long long 值
* a.tostr() : 返回 一个 string
* a.to2bit() : 返回 bint2 类型 ，即10进制转为2进制
*
*
* bint2 :
*
* 与bint 基本类似，只是将10^8进位改为了2^32
*
* 现在功能还不完善，以后慢慢补充
*
* a.to10bit() : 返回 bint 类型，即2进制转为10进制
*
*/
#pragma once
#ifndef BINT_H
#define BINT_H

#include <string>
#include <iomanip>
#include <string.h>
#include <iostream>

#include "Array.h"

_MATH_BEGIN

#define TEST //测试用高精度计时器 ，直接网上找的代码
#ifdef TEST

extern int mode;

#include <Windows.h>
static LARGE_INTEGER freq;

static BOOL initFreq() {
	if (!QueryPerformanceFrequency(&freq))
		return FALSE;
	else
		return TRUE;
}

extern double currTime(); //使用高精度计时器

extern double tot, sta, en;

#define START sta=currTime();
#define END en=currTime();
#define ADD tot+=en-sta;

#endif

using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;

class bint2;

#define jw 100000000 //进位

/*---biginter类(10bit)---*/
/*---每一位对10^8取模的动态扩展大整数类---*/

class bint {
private:
	Array<int>vec;//每一位存一个int
	bool positive;//positive为true表示为正数，否则为负数
	/*---从不同数据类型初始化---*/
	void assign(const int&);
	void assign(const long long&);
	void assign(const char*);
	void assign(const std::string&);

	/*---快速加法，但不完全快速---*/
	/*---有待改进---*/
	friend void quickadd(bint& a, const bint& b, const bool& _positive, const int& head = 0) {
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
		for (int i = head; i < m; ++i) {
			a.save_at(i) += b[i];
			if (a.save_at(i) >= jw) {
				a.save_at(i) -= jw;
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
	/*---对于低精度的优化，即降低了常数---*/
	friend void addint(bint& a, int b, const bool& _positive) {
		if (b >= jw - a[0]) {//可能会溢出，因此要这样比较
			long long val = a[0] + b;
			a[0] = val % jw;
			b = val / jw;
			int head = 1;
			while (a[head] + b >= jw) {
				a[head] += b - jw;
				b = 1;
			}
			a[head] += b;
		}
		else a[0] += b;
		a.positive = _positive;
	}
	/*---快速减法，但实际上就是朴素的压位减法，只是加了个特判---*/
	friend void quickdel(bint& a, const bint& b, const bool& _positive, const int& head = 0) {
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
			int tail = m;
			while (c.size() > tail && c.save_at(tail) < 0) {
				c.save_at(tail) += jw;
				--c[tail + 1];
				++tail;
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
		int tail = m;
		while (a.size() > tail && a.save_at(tail) < 0) {
			a.save_at(tail) += jw;
			--a[tail + 1];
			++tail;
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
	/*---低精度特判---*/
	friend void delint(bint& a, const int& b, const bool& _positive) {
		a.positive = _positive;
		if (a.size() == 1 && a[0] <= b) {
			a[0] = b - a[0];
			a.positive ^= 1;
			return;
		}

		int head = 0;
		a[head] -= b;
		while (a[head] < 0) {
			a[head] += jw;
			--a[head + 1];
		}
		int tail = a.size();
		while (!a.save_at(tail - 1))
			--tail;
		a.resize(tail);
		a.positive = _positive;
	}
	/*---时间复杂度是O(m*(n-m))---*/
	/*---进行了部分优化，使得常数略小---*/
	friend bint randomdivide(const bint& A, const bint& B) {//与答案的有效位数以及B的长度有关
		bint copyA(A), copyB(B);
		copyA.abs(), copyB.abs();
		if (copyA < copyB)return 0;//如果更小，返回0
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
	/*---大约是O(nlog^2n)，但是常数非常大，估计是我算法选错了---*/
	friend bint matchdivide(const bint& A, const bint& B) {//稳定的除法，复杂度在O(A*nlogn)左右，常数很大

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
	/*---当B的长度大于A的长度的2/3时只有1的误差---*/
	friend bint largedivide(const bint& A, const bint& B) {
		int n = A.size(), m = B.size(), mid = 2 * m - n - 2;
		bint copyA, copyB;
		copyA.assign(A, mid, n);
		copyB.assign(B, mid, m);
		++copyA;
		bint ans = copyA / copyB;
		copyA = A, copyB = B;
		copyA.abs(), copyB.abs();
		if (copyA < copyB * ans)
			--ans;
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}
	friend bint middivide(const bint& A, const bint& B) {
		//让B长度每次长度乘以3/2，这样误差可以卡进3/2 m
		bint copyA(A), copyB(B);
		copyA.abs(), copyB.abs();
		int mid = B.size() >> 1;
		copyB.quick_mul_10k(mid << 3);
		bint ans = copyA / copyB;
		ans.quick_mul_10k(mid << 3);
		copyB = B;
		copyB.abs();
		ans += (copyA - ans * copyB) / copyB;
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	friend bint quickdivide(const bint& A, const bint& B) {
		if (A.vec < B.vec)return 0;
		int n = A.size(), m = B.size();
		//if(m==1)return divideint(A,B[0]*(B.positive?1:-1));//低精度除法
		if (m <= sqr((quicklog2(n) + 1) >> 1) + 6)return randomdivide(A, B);//m比较小直接用O(m*(n-m))算法
		if (m * 3 >= 2 * n) return largedivide(A, B);//m范围减半，n范围减少至m的一倍
		return middivide(A, B);
		return matchdivide(A, B);//范围较大
	}
	/*---低精度除法---*/
	friend bint divideint(const bint& A, int B) {
		bint ans;
		ans.positive = A.positive;
		if (B < 0)ans.positive ^= 1, B *= -1;
		int Size = A.size();
		unsigned long long copyA = 0;
		for (int i = Size - 1; ~i; --i) {
			copyA = copyA * jw + A[i];
			ans[i] += copyA / B;
			copyA %= B;
		}
		return ans;
	}
	/*---Karatsuba算法求乘法---*/
	/*---实际使用上几乎没有任意一个范围比FFT和暴力快，大概是常数太大---*/
	friend void quickadd10k(bint& a, const bint& b, const bool& _positive, const int& k) {
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
	/*---实测只有极少数的范围Karatsuba比其他两种更快---*/
	/*---效率差别不明显，因此并未使用，且貌似内存占用很离谱---*/
	friend void Karatsuba(const bint& a, const bint& b, bint& c) {
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
	/*---快速乘法，自动选择不同算法---*/
	friend void quickmul(const bint& a, const bint& b, bint& c) {
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return;
		}
		int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

		/*---根据不同范围选择不同算法---*/
		if (_max <= 32) {
			(_min * 1ll * _min <= (quicklog2(_max) * 3) + 8) ?
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
	/*---低精度乘法---*/
	friend void mulint(const bint& a, const int& b, bint& c) {
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
	bint inv(int = -1)const;//求精度为lim的逆元，很慢！
public:
	const bool iszero()const;
	bint() :positive(true) {
		vec[0] = 0;
	}
	bint(const int& val) :positive(true) {
		assign(val);
	}
	bint(const long long& val) :positive(true) {
		assign(val);
	}
	bint(const char* s) :positive(true) {
		assign(s);
	}
	bint(const std::string& s) :positive(true) {
		assign(s);
	}
	bint(const bint& other) :vec(other.vec), positive(other.positive) {

	}
	bint(const Array<int>& _vec, const bool& _positive) :vec(_vec), positive(_positive) {

	}
	bint& operator=(const int& val) {
		assign(val);
		return*this;
	}
	bint& operator=(const long long& val) {
		assign(val);
		return*this;
	}
	bint& operator=(const char* s) {
		assign(s);
		return*this;
	}
	bint& operator=(const std::string& s) {
		assign(s);
		return*this;
	}
	bint& operator=(const bint& other) {
		if (this == &other)return*this;
		vec = other.vec;
		positive = other.positive;
		return*this;
	}

	const int& operator[](const int&)const;//下标的const 访问，略快于非const动态扩展访问
	int& operator[](const int&);//非const 的动态扩展访问
	int at(const int&)const;//10进制下的index位，取值0~9
	int& save_at(const int&);//vec[index]，但是去掉了动态扩展，且可以修改
	void set(const int&, const int&);//10金直辖的index位修改

	bool operator<(const bint&)const;
	bool operator==(const bint&)const;
	bool operator<=(const bint&)const;
	bool operator>(const bint&)const;
	bool operator>=(const bint&)const;
	bool operator!=(const bint&)const;

	friend ostream& operator<<(ostream& out, const bint& x) {
		if (!x.positive)out << "-";
		out << x[x.size() - 1];
		for (int i = x.size() - 2; i >= 0; --i)
			out << std::setw(8) << std::setfill('0') << x[i];
		return out;
	}
	friend istream& operator>>(istream& in, bint& x) {
		std::string s;
		in >> s;
		x = s;
		return in;
	}

	bint& operator+=(const bint&);
	bint& operator+=(const int&);
	bint& operator-=(const bint&);
	bint& operator-=(const int&);
	bint& operator*=(const bint&);
	bint& operator*=(const int&);
	bint& operator/=(const bint&);
	bint& operator/=(const int&);
	bint& operator%=(const bint&);
	bint& operator%=(const int&);
	bint& operator|=(const bint&);
	bint& operator&=(const bint&);
	bint& operator^=(const bint&);

	bint& operator++();
	bint& operator++(int);
	bint& operator--();
	bint& operator--(int);

	bint& operator<<=(const int&);
	bint& operator>>=(const int&);

	friend bint operator+(const bint& a, const bint& b) {
		bint c(a);
		return c += b;
	}
	friend bint operator+(const bint& a, const int& b) {
		bint c(a);
		return c += b;
	}
	friend bint operator+(const int& a, const bint& b) {
		bint c(b);
		return c += a;
	}
	friend bint operator-(const bint& a, const bint& b) {
		bint c(a);
		return c -= b;
	}
	friend bint operator-(const bint& a, const int& b) {
		bint c(a);
		return c -= b;
	}
	friend bint operator-(const int& a, const bint& b) {
		bint c(b);
		c -= a;
		c.positive ^= 1;
		return c;
	}

	friend bint operator*(const bint& a, const bint& b) {
		bint c;
		quickmul(a, b, c);
		return c;
	}
	friend bint operator*(const bint& a, const int& b) {
		bint c;
		mulint(a, b, c);
		return c;
	}
	friend bint operator*(const int& a, const bint& b) {
		bint c;
		mulint(b, a, c);
		return c;
	}
	friend bint operator/(const bint& a, const bint& b) {
		if (b.iszero())return a;
		return quickdivide(a, b);
	}
	friend bint operator/(const bint& a, const int& b) {
		if (!b)return a;
		return divideint(a, b);
	}
	friend bint operator/(const int& a, const bint& b) {
		if (b.iszero())return a;
		if (b.size() == 1 && b[0] <= a)
			return a / b[0];
		return 0;
	}
	friend bint operator%(const bint& a, const bint& b) {
		if (b.iszero())return a;
		return a - b * (a / b);
	}
	friend bint operator%(const bint& a, const int& b) {
		if (!b)return a;
		return a - (a / b) * b;
	}
	friend bint operator%(const int& a, const bint& b) {
		if (b.iszero())return a;
		return a - (a / b) * b;
	}

	friend bint operator>>(bint a, const int& index) {
		if (index < 32) {
			a /= (1 << index);
			return a;
		}
		bint w = qpow(bint(2), index);
		a /= w;
		return a;
	}
	friend bint operator<<(bint a, const int& index) {
		if (index < 32) {
			a *= (1 << index);
			return a;
		}
		bint w = qpow(bint(2), index);
		a *= w;
		return a;
	}
	friend bint operator|(const bint& lhs, const bint& rhs) {
		bint s(lhs);
		s|=rhs;
		return s;
	}
	friend bint operator&(const bint& lhs, const bint& rhs) {
		bint s(lhs);
		s&=rhs;
		return s;
	}
	friend bint operator^(const bint& lhs, const bint& rhs) {
		bint s(lhs);
		s^=rhs;
		return s;
	}

	/*---初始化为other的[L,R]元素---*/
	void assign(const bint& other, const int& L, const int& R) {
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

	void resize(const int&);
	const int size()const;
	const int length()const;
	void reserve(const int&);
	void relength(const int&);

	void quick_mul_10();//O(n)乘10，但省去了部分运算
	bint& quick_mul_10k(const int& = 1);//O(n)乘10^k
	void abs();//变为绝对值，即positive取true

	friend bint abs(const bint& x) {
		return bint(x.vec, true);
	}

	friend bint qpow(bint a, int b) {//快速幂求b次方
		bint s(1);
		while (b) {
			if (b & 1)
				s *= a;
			a *= a;
			b >>= 1;
		}return s;
	}
	friend bint qpow(bint a, bint b) {//暂未用算法，该算法是朴素算法，较慢
		bint s(1);
		while (!b.iszero()) {
			if (b[0] & 1)
				s *= a;
			a *= a;
			b >>= 1;
		}
		return s;
	}
	int toint()const;//转为int
	long long toll()const;//转为long long
	std::string tostr()const;//转为字符串

	bint2 to2bit()const;//转为2进制bint

#ifdef TEST
	friend bint test1(const bint& a, const bint& b) {
		bint c;
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return c;
		}
		int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);
		Array_func::SlowMul(a.vec, b.vec, c.vec);

		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;
	}
	friend void test2(const bint& a, const bint& b) {
		bint c;
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return;
		}
		Karatsuba(a, b, c);
		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;

	}
	friend bint test3(const bint& a, const bint& b) {
		bint c;
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return c;
		}
		int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

		FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;
	}
	friend bint test4(const bint& a, const bint& b) {
		bint c;
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return c;
		}
		int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

		if (_max <= 32) {
			(_min <= (quicklog2(_max) * 3) + 8) ?
				Array_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}
		else {
			(_min <= (quicklog2(_max) << 1) + 8) ?
				Array_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}

		if (_max <= 32) {
			(_min <= (quicklog2(_max) * 3) + 8) ?
				mode = 1 :
				mode = 3;
		}
		else {
			(_min <= (quicklog2(_max) << 1) + 8) ?
				mode = 1 :
				mode = 3;
		}
		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;
	}
#endif
};


/* bigintger类(2bit)
* 2^32为一位，完美（划掉）采用了溢出
* 10bit设计很多10相关，这个可以被编译器用位运算优化
* 但2bit可以用更多的位运算优化，甚至可以采用一些汇编优化啥的
* 效率方面，暂时比10bit快约25%~30%，但考虑到10进制和2进制转化并不是很快（大约O(n log  n),但常数大)
* 因此建议初始数据是10进制则用bint，是2进制则用bint2
* 并且由于最初是写的bint(10bit)，bint2是后来加上来的，因此不太完善
* 因为10-bit平时使用会更便捷，因此我可能会优先完善10-bit
*/

class bint2 {
private:
	Array2 vec;//以2^32为进制
	bool positive;//positive为true表示为正数，否则为负数

	void assign(const int&);
	void assign(const long long&);
	void assign(const char*);
	void assign(const std::string&);

	//saveadd和savedel均为均摊O(1)
	void saveadd(int index) {//unsigned int 的安全加1
		while (vec[index] == maxuint) {//超出当前size的话vec[index]为0，肯定会终止
			vec[index] = 0;
			++index;
		}
		++vec[index];
	}
	void savedel(int index) {//unsigned int 的安全减1，保证最终答案不会为负数
		int Size = size();
		while (index < Size && !vec[index]) {//不足则借位
			vec[index] = maxuint;
			++index;
		}
		--vec[index];
		if (index == Size - 1 && index && !vec[index])
			resize(index);
	}

	friend bint2 add(const bint2& a, const bint2& b, const bool& _positive) {
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
	friend void quickadd(bint2& a, const bint2& b, const bool& _positive, const int& head = 0) {
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
	friend void addint(bint2& a, int b, const bool& _positive) {
		if (b > (a[0] ^ maxuint))//溢出
			a.saveadd(1);
		a[0] += b;
		a.positive = _positive;
	}

	friend bint2 del(const bint2& a, const bint2& b, const bool& _positive) {
		int n = a.size(), m = b.size();
		bint2 c;
		c.reserve(n + 1);
		c.resize(n);
		for (int i = n - 1; i >= m; --i)
			c[i] = a[i];
		for (int i = m - 1; ~i; --i) {
			if (a[i] < b[i] && (c[i] < b[i] - a[i]))
				c.savedel(i + 1);
			c[i] += a[i] - b[i];
		}

		int Length = c.size();
		while (Length > 1 && !c.save_at(Length - 1))
			--Length;
		if (Length != c.size())
			c.resize(Length);

		c.positive = _positive;
		if (c.iszero())
			c.positive = true;
		return c;
	}
	friend void quickdel(bint2& a, const bint2& b, const bool& _positive, const int& head = 0) {
		if (a.vec < b.vec) {
			a = del(b, a, !_positive);
			return;
		}
		int n = a.size(), m = b.size();
		for (int i = m - 1; i >= head; --i) {
			if (a[i] < b[i])
				a.savedel(i + 1);
			a.save_at(i) -= b[i];
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

	friend void delint(bint2& a, int b, const bool& _positive) {
		a.positive = _positive;
		if (b < 0)b *= -1;

		if (a[0] >= b) {
			a[0] -= b;
			return;
		}
		a.savedel(1);
		a[0] += b;

		int tail = a.size();
		while (!a.save_at(tail - 1))
			--tail;
		a.resize(tail);
		a.positive = _positive;
	}
	friend void quickmul(const bint2& a, const bint2& b, bint2& c) {
		if (a.iszero() || b.iszero()) {
			c = 0;
			c.positive = true;
			return;
		}
		int n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

		if (_max <= 32) {
			(_min <= (quicklog2(_max) * 3) + 8) ?
				Array2_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array2_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}
		else {
			(_min <= (quicklog2(_max) << 1) + 8) ?
				Array2_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array2_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}

		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
	}
public:
	const bool iszero()const;
	bint2() :positive(true) {
		vec[0] = 0;
	}
	bint2(const int& val) :positive(true) {
		assign(val);
	}
	bint2(const long long& val) :positive(true) {
		assign(val);
	}
	bint2(const char* s) :positive(true) {
		assign(s);
	}
	bint2(const std::string& s) :positive(true) {
		assign(s);
	}

	bint2(const bint2& other) :vec(other.vec), positive(other.positive) {

	}
	bint2(const Array2& _vec, const bool& _positive) :vec(_vec), positive(_positive) {

	}
	bint2& operator=(const int& val) {
		assign(val);
		return*this;
	}
	bint2& operator=(const long long& val) {
		assign(val);
		return*this;
	}
	bint2& operator=(const bint2& other) {
		if (this == &other)return*this;
		vec = other.vec;
		positive = other.positive;
		return*this;
	}
	bint2& operator=(const char* s) {
		assign(s);
		return*this;
	}
	bint2& operator=(const std::string& s) {
		assign(s);
		return*this;
	}

	void assign(const bint2& other, const int& L, const int& R) {
		int Size = other.size();
		int l = min(L, Size), r = min(R, Size);
		resize(max(1, r - l));
		this->save_at(0) = 0;
		for (int i = l; i < r; ++i)
			this->save_at(i - l) = other[i];
	}

	const uint& operator[](const int&)const;
	uint& operator[](const int&);
	bool at(const int&)const;
	uint& save_at(const int&);
	//void set(const int&, const bool&);

	bool operator<(const bint2&)const;
	bool operator==(const bint2&)const;
	bool operator<=(const bint2&)const;
	bool operator>(const bint2&)const;
	bool operator>=(const bint2&)const;
	bool operator!=(const bint2&)const;

	bint2&operator|=(const bint2&);
	bint2&operator&=(const bint2&);
	bint2&operator^=(const bint2&);
	friend bint2 operator|(const bint2& lhs, const bint2& rhs) {
		bint2 s(lhs);
		s|=rhs;
		return s;
	}
	friend bint2 operator&(const bint2& lhs, const bint2& rhs) {
		bint2 s(lhs);
		s&=rhs;
		return s;
	}
	friend bint2 operator^(const bint2& lhs, const bint2& rhs) {
		bint2 s(lhs);
		s^=rhs;
		return s;
	}

	friend ostream& operator<<(ostream& out, const bint2& x) {
		if (!x.positive)out << "-";
		int Length = x.length();
		for (int i = Length - 1; ~i; --i)
			out << x.at(i);
		return out;
	}

	bint2& operator+=(const bint2&);
	bint2& operator+=(const int&);
	bint2& operator-=(const bint2&);
	bint2& operator-=(const int&);
	bint2& operator*=(const bint2&);

	friend bint2 operator+(const bint2& a, const bint2& b) {
		bint2 c(a);
		return c += b;
	}
	friend bint2 operator+(const bint2& a, const int& b) {
		bint2 c(a);
		return c += b;
	}
	friend bint2 operator+(const int& a, const bint2& b) {
		bint2 c(b);
		return c += a;
	}
	friend bint2 operator-(const bint2& a, const bint2& b) {
		bint2 c(a);
		return c -= b;
	}
	friend bint2 operator-(const bint2& a, const int& b) {
		bint2 c(a);
		return c -= b;
	}
	friend bint2 operator-(const int& a, const bint2& b) {
		bint2 c(b);
		c -= a;
		c.positive ^= 1;
		return c;
	}

	friend bint2 operator*(const bint2& a, const bint2& b) {
		bint2 c;
		quickmul(a, b, c);
		return c;
	}

	void resize(const uint&);
	const uint size()const;
	const uint length()const;
	void reserve(const uint&);
	void relength(const uint&);
	friend bint2 qpow(bint2 a, int b) {
		bint2 s(1);
		while (b) {
			if (b & 1)
				s *= a;
			a *= a;
			b >>= 1;
		}return s;
	}

	bint to10bit()const;
	std::string tostr()const;
};

#undef jw

_MATH_END

#endif BINT_H
