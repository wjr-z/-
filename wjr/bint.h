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
* bintabs(a) : 返回a的绝对值，非必要则尽量不用，避免拷贝耗时
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

#include "Array.h"

_MATH_BEGIN

/*---biginter类(10bit)---*/
/*---每一位对10^8取模的动态扩展大整数类---*/

#define bintzero bint()

//#define BINTDEBUG


/*---友元函数声明---*/

//------------------------------------------------------------------//

class bfloat;
class bint2;
class bint;

ostream& operator<<(ostream&, const bint&);
istream& operator>>(istream&, bint&);

bool operator<(const bint&, const bint&);
bool operator<(const bint&, const int&);
bool operator<(const int&, const bint&);
bool operator==(const bint&, const bint&);
bool operator==(const bint&, const int&);
bool operator==(const int&, const bint&);
bool operator<=(const bint&, const bint&);
bool operator<=(const bint&, const int&);
bool operator<=(const int&, const bint&);
bool operator>(const bint&, const bint&);
bool operator>(const bint&, const int&);
bool operator>(const int&, const bint&);
bool operator>=(const bint&, const bint&);
bool operator>=(const bint&, const int&);
bool operator>=(const int&, const bint&);
bool operator!=(const bint&, const bint&);
bool operator!=(const bint&, const int&);
bool operator!=(const int&, const bint&);

bint operator>>(bint, const int&);
bint operator<<(bint, const int&);

bint operator+(bint);
bint operator-(bint);
bint Factorial(const bint&);


bint operator+(const bint&, const bint&);
bint operator+(bint&&,const bint&);
bint operator+(const bint&,bint&&);
bint operator+(bint&&,bint&&);
bint operator+(bint, const int&);
bint operator+(const int&, bint);
bint operator-(const bint&, const bint&);
bint operator-(bint&&,const bint&);
bint operator-(const bint&,bint&&);
bint operator-(bint&&,bint&&);
bint operator-(bint, const int&);
bint operator-(const int&, bint);

bint operator*(const bint&,const bint&);
bint operator*(bint&&,const bint&);
bint operator*(const bint&,bint&&);
bint operator*(bint&&,bint&&);
bint operator*(bint, const int&);
bint operator*(const int&, bint);
bint operator/(const bint&, const bint&);
bint operator/(const bint&, const int&);
bint operator/(const int&, const bint&);
bint operator%(const bint&, const bint&);
bint operator%(const bint&, const int&);
bint operator%(const int&, const bint&);

bint qpow(bint, bint);
bint qpow(bint, int);
bint qpow(int, bint);

bint abs(bint);
bint gcd(const bint&,const bint&);
bint randdata(const bint&L,const bint&R);
bint randdata(const int);

ostream& operator<<(ostream& out, const bint2& x);

bint2 qpow(bint2, int);

//------------------------------------------------------------------//

class bint {
	friend class bfloat;
private:
	Array<int>vec;//每一位存一个int
	bool positive;//positive为true表示为正数，否则为负数
	/*---从不同数据类型初始化---*/
	void assign(int);
	void assign(const long long&);
	void assign(const char*);
	void assign(const std::string&);

	/*---快速加法，但不完全快速---*/
	/*---有待改进---*/
	static void quickadd(bint&, const bint&, const bool&);
	/*---对于低精度的优化，即降低了常数---*/
	static void addint(bint&, int, const bool&);
	/*---快速减法，但实际上就是朴素的压位减法，只是加了个特判---*/
	static void quickdel(bint&, const bint&, const bool&);
	/*---低精度特判---*/
	static void delint(bint&, int, const bool&);
	/*---时间复杂度是O(m*(n-m))---*/
	/*---进行了部分优化，使得常数略小---*/
	static bint randomdivide(const bint&, const bint&);//与答案的有效位数以及B的长度有关
	/*---大约是O(nlog^2n)，但是常数非常大，估计是我算法选错了---*/
	static bint matchdivide(const bint&, const bint&);//稳定的除法，复杂度在O(A*nlogn)左右，常数很大
	/*---当B的长度大于A的长度的2/3时只有1的误差---*/
	static bint largedivide(const bint&, const bint&);
	static bint smalldivide(const bint&, const bint&);
	static bint knuthdivide(const bint&,const bint&);
	static bint quickdivide(const bint&, const bint&);
	/*---低精度除法---*/
	static bint divideint(const bint&, int);
	/*---Karatsuba算法---*/
	/*---复杂度为O(n*m^0.517)---*/
	/*---当m<=64时使用暴力乘法---*/
	static void Karatsuba(const bint&, const bint&, bint&);
	/*---快速乘法，自动选择不同算法---*/
	static void quickmul(bint&, const bint&);
	/*---低精度乘法---*/
	static void mulint(const bint&, const int&, bint&);
	static bint Factorial(const bint&, const bint&);

	bint inv(int = -1)const;//求精度为lim的逆元，很慢！
	void clear();

	void resize(const size_t&);
	const size_t size()const;
	void reserve(const size_t&);
	bint(const int& len, int):vec(len),positive(true){
	#ifdef BINTDEBUG
		cout<<"bint构造函数\n";
	#endif
	}
public:
	/*---初始化为other的[L,R]元素---*/
	void assign(const bint& other, const int& L, const int& R);
	const bool iszero()const;
	const bool ispositive()const;
	bint()noexcept :positive(true) {
	#ifdef BINTDEBUG
		cout<<"bint构造函数\n";
	#endif BINTDEBUG
	}
	~bint() {
	#ifdef BINTDEBUG
		cout << "bint析构函数\n";
	#endif
	}
	explicit bint(const int& val)noexcept {
	#ifdef BINTDEBUG
		cout << "bint构造函数\n";
	#endif 
		assign(val);
	}
	explicit bint(const long long& val)noexcept :positive(true) {
	#ifdef BINTDEBUG
		cout << "bint构造函数\n";
	#endif 
		assign(val);
	}
	explicit bint(const char* s)noexcept :positive(true) {
	#ifdef BINTDEBUG
		cout << "bint构造函数\n";
	#endif
		assign(s);
	}
	explicit bint(const std::string& s)noexcept :positive(true) {
	#ifdef BINTDEBUG
		cout << "bint构造函数\n";
	#endif
		assign(s);
	}
	bint(const bint& other)noexcept :vec(other.vec), positive(other.positive) {
	#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
	#endif
	}
	bint(bint&& other)noexcept : vec(std::move(other.vec)),positive(other.positive) {
	#ifdef BINTDEBUG
		cout<<"bint右值拷贝构造函数\n";
	#endif
	}
	bint(const bint& other, const bool& _positive)noexcept :vec(other.vec), positive(_positive) {
	#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
	#endif 
	}
	bint(bint&&other, const bool& _positive)noexcept :vec(std::move(other.vec)), positive(_positive) {
	#ifdef BINTDEBUG
		cout << "bint右值拷贝构造函数\n";
	#endif 
	}

	bint(const bint& other, const int& L, const int& R)noexcept : vec(max(1,R-L)),positive(true){
	#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
	#endif
		assign(other,L,R);
	}
	bint(const Array<int>& _vec, const bool& _positive)noexcept :vec(_vec), positive(_positive) {
	#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
	#endif

	}
	bint& operator=(const int& val)noexcept {
	#ifdef BINTDEBUG
		cout<<"bint复制函数\n";
	#endif
		assign(val);
		return*this;
	}
	bint& operator=(const long long& val)noexcept {
	#ifdef BINTDEBUG
		cout << "bint复制函数\n";
	#endif
		assign(val);
		return*this;
	}
	bint& operator=(const char* s)noexcept {
	#ifdef BINTDEBUG
		cout << "bint复制函数\n";
	#endif
		assign(s);
		return*this;
	}
	bint& operator=(const std::string& s)noexcept {
	#ifdef BINTDEBUG
		cout << "bint复制函数\n";
	#endif
		assign(s);
		return*this;
	}
	bint& operator=(const bint& other)noexcept {
	#ifdef BINTDEBUG
		cout << "bint复制函数\n";
	#endif
		vec = other.vec;
		positive = other.positive;
		return*this;
	}
	bint& operator=(bint&& other)noexcept {
	#ifdef BINTDEBUG
		cout<<"bint右值复制\n";
	#endif
		vec=std::move(other.vec);
		positive=other.positive;
		return*this;
	}

	const size_t length()const;
	void relength(const int&);
	void reverse(int = -1);

	const int& operator[](const size_t&)const;//下标的const 访问，略快于非const动态扩展访问
	int& operator[](const size_t&);//非const 的动态扩展访问
	int& save_at(const size_t&);//vec[index]，但是去掉了动态扩展，且可以修改

	uint at(const size_t&)const;//10进制下的index位，取值0~9,因此用short就够了
	void set(const size_t&, const uint&);//10进制位的index位修改
	void randdata(const bint&other);

	friend ostream& operator<<(ostream&, const bint&);
	friend istream& operator>>(istream&, bint&);


	/*
	* 防止int强制转换，同时后期可以针对int进行特别优化
	*/
	friend bool operator<(const bint&, const bint&);
	friend bool operator<(const bint&, const int&);
	friend bool operator<(const int&, const bint&);
	friend bool operator==(const bint&, const bint&);
	friend bool operator==(const bint&, const int&);
	friend bool operator==(const int&, const bint&);
	friend bool operator<=(const bint&, const bint&);
	friend bool operator<=(const bint&, const int&);
	friend bool operator<=(const int&, const bint&);
	friend bool operator>(const bint&, const bint&);
	friend bool operator>(const bint&, const int&);
	friend bool operator>(const int&, const bint&);
	friend bool operator>=(const bint&, const bint&);
	friend bool operator>=(const bint&, const int&);
	friend bool operator>=(const int&, const bint&);
	friend bool operator!=(const bint&, const bint&);
	friend bool operator!=(const bint&, const int&);
	friend bool operator!=(const int&, const bint&);

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

	bint& operator++();
	bint operator++(int);
	bint& operator--();
	bint operator--(int);

	bint& operator<<=(const int&);
	bint& operator>>=(const int&);
	friend bint operator>>(bint, const int&);
	friend bint operator<<(bint, const int&);

	friend bint operator+(bint);
	friend bint operator-(bint);
	friend bint Factorial(const bint&);


	friend bint operator+(const bint&, const bint&);
	friend bint operator+(bint&&, const bint&);
	friend bint operator+(const bint&, bint&&);
	friend bint operator+(bint&&, bint&&);
	friend bint operator+(bint, const int&);
	friend bint operator+(const int&, bint);
	friend bint operator-(const bint&, const bint&);
	friend bint operator-(bint&&, const bint&);
	friend bint operator-(const bint&, bint&&);
	friend bint operator-(bint&&, bint&&);
	friend bint operator-(bint, const int&);
	friend bint operator-(const int&, bint);

	friend bint operator*(const bint&, const bint&);
	friend bint operator*(bint&&, const bint&);
	friend bint operator*(const bint&, bint&&);
	friend bint operator*(bint&&,bint&&);
	friend bint operator*(bint, const int&);
	friend bint operator*(const int&, bint);
	friend bint operator/(const bint&, const bint&);
	friend bint operator/(const bint&, const int&);
	friend bint operator/(const int&, const bint&);
	friend bint operator%(const bint&, const bint&);
	friend bint operator%(const bint&, const int&);
	friend bint operator%(const int&, const bint&);

	friend bint qpow(bint, bint);
	friend bint qpow(bint, int);
	friend bint qpow(int, bint);

	void quick_mul_10();//O(n)乘10，但省去了部分运算
	bint& quick_mul_10k(const int& = 1);//O(n)乘10^k
	bint& quick_divide_10k(const int& = 1);
	void abs();//变为绝对值，即positive取true
	friend bint abs(bint);
	void swap(bint&other);

	int toint()const;//转为int
	long long toll()const;//转为long long
	std::string tostr()const;//转为字符串

	bint2 to2bit()const;//转为2进制bint
	//用于测试三种乘法
#ifdef TEST
	friend bint test1(const bint& a, const bint& b) {
		if (a.iszero() || b.iszero()) {
			return bintzero;
		}
		bint c;
		Array_func::SlowMul(a.vec, b.vec, c.vec);

		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;
	}
	friend bint test2(const bint& a, const bint& b) {
		if (a.iszero() || b.iszero()) {
			return bintzero;
		}
		bint c;
		Karatsuba(a,b,c);
		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;

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

		if (!(_max >> 5)) {
			Array_func::SlowMul(a.vec, b.vec, c.vec);
		}
		else {
			(_min<= (quicklog2(_max) << 2) + 16) ?
				Array_func::SlowMul(a.vec, b.vec, c.vec) :
				FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
		}

		if (!(_max >> 5)) {
			mode=1;
		}
		else {
			(_min <= (quicklog2(_max)<<1) + 32) ?
				mode = 1 :
				mode = 3;
		}

		c.positive = !(a.positive ^ b.positive);
		if (c.iszero())c.positive = true;
		return c;
	}
#endif
};

template<typename Ty>
void swap(bint& lhs, bint& rhs) {
	lhs.swap(rhs);
}



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

	static bint2 add(const bint2&, const bint2&, const bool&);
	static void quickadd(bint2&, const bint2&, const bool&, const int&);
	static void addint(bint2& a, int b, const bool& _positive) {
		if (b > (a[0] ^ maxuint))//溢出
			a.saveadd(1);
		a[0] += b;
		a.positive = _positive;
	}

	static bint2 del(const bint2& a, const bint2& b, const bool& _positive) {
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
	static void quickdel(bint2& a, const bint2& b, const bool& _positive, const int& head = 0) {
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

	static void delint(bint2& a, int b, const bool& _positive) {
		a.positive = _positive;
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
	static void quickmul(const bint2& a, const bint2& b, bint2& c) {
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
	bint2()noexcept :positive(true) {
		vec[0] = 0;
	}
	~bint2() {

	}
	explicit bint2(const int& val)noexcept :positive(true) {
		assign(val);
	}
	explicit bint2(const long long& val)noexcept :positive(true) {
		assign(val);
	}
	explicit bint2(const char* s)noexcept :positive(true) {
		assign(s);
	}
	explicit bint2(const std::string& s)noexcept :positive(true) {
		assign(s);
	}

	bint2(const bint2& other)noexcept :vec(other.vec), positive(other.positive) {

	}
	bint2(bint2&& other)noexcept :vec(std::move(other.vec)), positive(other.positive) {

	}
	bint2(const Array2& _vec, const bool& _positive)noexcept :vec(_vec), positive(_positive) {

	}
	bint2& operator=(const int& val) noexcept {
		assign(val);
		return*this;
	}
	bint2& operator=(const long long& val) noexcept {
		assign(val);
		return*this;
	}
	bint2& operator=(const bint2& other)noexcept {
		if (this == &other)return*this;
		vec = other.vec;
		positive = other.positive;
		return*this;
	}
	bint2& operator=(bint2&& other) noexcept{
		vec=std::move(other.vec);
		positive=other.positive;
		return*this;
	}
	bint2& operator=(const char* s) noexcept{
		assign(s);
		return*this;
	}
	bint2& operator=(const std::string& s)noexcept {
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
	void set(const int&, const bool&);

	bool operator<(const bint2&)const;
	bool operator==(const bint2&)const;
	bool operator<=(const bint2&)const;
	bool operator>(const bint2&)const;
	bool operator>=(const bint2&)const;
	bool operator!=(const bint2&)const;

	friend ostream& operator<<(ostream& out, const bint2& x);

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

	friend bint2 qpow(bint2, int);

	std::string tostr()const;
	bint to10bit()const;
};


_MATH_END

#endif BINT_H
