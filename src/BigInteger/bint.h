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

#ifndef BINT_H
#define BINT_H

#include "Array.h"

#include <iostream>
#include <iomanip>

namespace Math {

	using std::istream;
	using std::ostream;
	using std::cin;
	using std::cout;
	using std::endl;

	/*---biginter类(10bit)---*/
	/*---每一位对10^8取模的动态扩展大整数类---*/

#define bintzero bint()
#define bint2zero bint2()
//#define BINTDEBUG


/*---友元函数声明---*/

//------------------------------------------------------------------//

	class bint;
	class bint2;

	ostream& operator<<(ostream&, const bint&);
	istream& operator>>(istream&, bint&);

	/*使用了左值引用和右值引用，以最优化速度*/

	bool operator<(const bint&, const bint&);
	bool operator<(const bint&, int);
	bool operator<(const int&, const bint&);
	bool operator==(const bint&, const bint&);
	bool operator==(const bint&, int);
	bool operator==(const int&, const bint&);
	bool operator<=(const bint&, const bint&);
	bool operator<=(const bint&, int);
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
	bool operator!(const bint&);
	bool operator!(bint&&);
	bint Factorial(int);

	bint operator+(const bint&, const bint&);
	bint operator+(bint&&, const bint&);
	bint operator+(const bint&, bint&&);
	bint operator+(bint&&, bint&&);
	bint operator+(bint, const int&);
	bint operator+(const int&, bint);
	bint operator-(const bint&, const bint&);
	bint operator-(bint&&, const bint&);
	bint operator-(const bint&, bint&&);
	bint operator-(bint&&, bint&&);
	bint operator-(bint, const int&);
	bint operator-(const int&, bint);

	bint operator*(const bint&, const bint&);
	bint operator*(bint&&, const bint&);
	bint operator*(const bint&, bint&&);
	bint operator*(bint&&, bint&&);
	bint operator*(bint, int);
	bint operator*(const int&, bint);
	bint operator/(const bint&, const bint&);
	bint operator/(const bint&, const int&);
	bint operator/(bint&&, const int&);
	bint operator/(int, const bint&);
	bint operator%(const bint&, const bint&);
	bint operator%(const bint&, const int&);
	bint operator%(const int&, const bint&);

	bint qpow(bint, bint);
	bint qpow(const bint&, int);
	bint qpow(int, bint);

	bint abs(bint);
	bint gcd(const bint&, const bint&);
	bint randdata(const bint& L, const bint& R);
	bint randdata(size_t);
	void swap(bint&, bint&);

	ostream& operator<<(ostream& out, const bint2& x);
	istream& operator>>(istream& in, bint2& x);

	bool operator<(const bint2&, const bint2&);
	bool operator<(const bint2&, const int&);
	bool operator<(const int&, const bint2&);
	bool operator==(const bint2&, const bint2&);
	bool operator==(const bint2&, const int&);
	bool operator==(const int&, const bint2&);
	bool operator<=(const bint2&, const bint2&);
	bool operator<=(const bint2&, const int&);
	bool operator<=(const int&, const bint2&);
	bool operator>(const bint2&, const bint2&);
	bool operator>(const bint2&, const int&);
	bool operator>(const int&, const bint2&);
	bool operator>=(const bint2&, const bint2&);
	bool operator>=(const bint2&, const int&);
	bool operator>=(const int&, const bint2&);
	bool operator!=(const bint2&, const bint2&);
	bool operator!=(const bint2&, const int&);
	bool operator!=(const int&, const bint2&);

	bint2 operator+(bint2);
	bint2 operator-(bint2);
	bool operator!(const bint2&);
	bool operator!(bint2&&);


	bint2 operator+(const bint2&, const bint2&);
	bint2 operator+(bint2&&, const bint2&);
	bint2 operator+(const bint2&, bint2&&);
	bint2 operator+(bint2&&, bint2&&);
	bint2 operator+(bint2, const int&);
	bint2 operator+(const int&, bint2);
	bint2 operator-(const bint2&, const bint2&);
	bint2 operator-(bint2&&, const bint2&);
	bint2 operator-(const bint2&, bint2&&);
	bint2 operator-(bint2&&, bint2&&);
	bint2 operator-(bint2, const int&);
	bint2 operator-(const int&, bint2);

	bint2 operator*(const bint2&, const bint2&);
	bint2 operator*(bint2&&, const bint2&);
	bint2 operator*(const bint2&, bint2&&);
	bint2 operator*(bint2&&, bint2&&);
	bint2 operator*(bint2, const int&);
	bint2 operator*(const int&, bint2);
	bint2 operator/(const bint2&, const bint2&);
	bint2 operator/(const bint2&, const int&);
	bint2 operator/(const int&, const bint2&);
	bint2 operator%(const bint2&, const bint2&);
	bint2 operator%(const bint2&, const int&);
	bint2 operator%(const int&, const bint2&);

	bint2 qpow(const bint2&, int);

	bint2 abs(bint2);

	bool operator==(const bint&, const bint2&);
	bool operator==(const bint2&, const bint&);


	std::string bit10_to_bit2(const std::string&);//10进制字符串转化为2进制字符串
	std::string bit2_to_bit10(const std::string&);//2进制字符串转化为10进制字符串


	bint sqrt(const bint&);
	bint2 sqrt(const bint2&);
	bool isprime(const bint&);//大数判别质数
	std::vector<bint>pollard_rho(bint);
	bint max_prime(const bint& x);

	//------------------------------------------------------------------//



	class bint {
		friend class bfloat;
	private:
		Array vec;//每一位存一个int
		bool positive;//positive为true表示为正数，否则为负数
		//size_t powk;//vec*10^powk,改起来会很麻烦的吧qwq
		/*---从不同数据类型初始化---*/
		void assign(int);
		void assign(long long);
		void assign(const char*);
		void assign(const std::string&);

		/*---快速加法，但不完全快速---*/
		/*---有待改进---*/
		static void quickadd(bint&, const bint&);
		static void quickadd(bint&, bint&&);
		/*---对于低精度的优化，即降低了常数---*/
		static void addint(bint&, int);
		/*---快速减法，但实际上就是朴素的压位减法，只是加了个特判---*/
		static void quickdel(bint&, const bint&);
		static void quickdel(bint&, bint&&);
		/*---低精度特判---*/
		static void delint(bint&, int);
		/*---当B的长度大于A的长度的2/3时只有1的误差---*/
		static bint largedivide(const bint&, const bint&);
		static bint smalldivide(const bint&, const bint&);
		static bint knuthdivide(const bint&, const bint&);
		static bint quickdivide(const bint&, const bint&);
		/*---低精度除法---*/
		static bint divideint(const bint&, int);
		static void divideint(bint&&, int);
		/*---Karatsuba算法---*/
		/*---复杂度为O(n*m^0.517)---*/
		/*---当m<=64时使用暴力乘法---*/
		static void Karatsuba(const bint&, const bint&, bint&);
		/*---快速乘法，自动选择不同算法---*/
		static void quickmul(bint&, const bint&);
		/*---低精度乘法---*/
		static void mulint(const bint&, const int&, bint&);
		static bint Factorial(int, int);

		bint2 get2bit()const;

		void clear();

		void resize(const size_t&);
		const size_t size()const;
		void reserve(const size_t&);
		bint(const int& len, int) :vec(len), positive(true) {
		#ifdef BINTDEBUG
			cout << "bint构造函数\n";
		#endif
		}
	public:
		/*---初始化为other的[L,R]元素---*/
		void assign(const bint& other, const size_t& L, const size_t& R);
		const bool iszero()const;
		const bool ispositive()const;
		bint()noexcept :positive(true) {
		#ifdef BINTDEBUG
			cout << "bint构造函数\n";
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
		bint(bint&& other)noexcept : vec(std::move(other.vec)), positive(other.positive) {
		#ifdef BINTDEBUG
			cout << "bint右值拷贝构造函数\n";
		#endif
		}
		bint(const bint& other, const bool& _positive)noexcept :vec(other.vec), positive(_positive) {
		#ifdef BINTDEBUG
			cout << "bint拷贝构造函数\n";
		#endif 
		}
		bint(bint&& other, const bool& _positive)noexcept :vec(std::move(other.vec)), positive(_positive) {
		#ifdef BINTDEBUG
			cout << "bint右值拷贝构造函数\n";
		#endif 
		}

		bint(const bint& other, const int& L, const int& R)noexcept : vec(max(1, R - L)), positive(true) {
		#ifdef BINTDEBUG
			cout << "bint拷贝构造函数\n";
		#endif
			assign(other, L, R);
		}
		bint(const Array& _vec, const bool& _positive)noexcept :vec(_vec), positive(_positive) {
		#ifdef BINTDEBUG
			cout << "bint拷贝构造函数\n";
		#endif

		}
		bint& operator=(const int& val)noexcept {
		#ifdef BINTDEBUG
			cout << "bint复制函数\n";
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
			cout << "bint右值复制\n";
		#endif
			vec = std::move(other.vec);
			positive = other.positive;
			return*this;
		}
		bint& operator=(const bint2& other)noexcept;

		const size_t length()const;
		void relength(const size_t&);
		void reverse(size_t = -1);

		const int operator[](const size_t&)const;//下标的const 访问，略快于非const动态扩展访问
		int& operator[](const size_t&);//非const 的动态扩展访问
		int& save_at(const size_t&);//vec[index]，但是去掉了动态扩展，且可以修改

		uint at(const size_t&)const;//10进制下的index位，取值0~9,因此用short就够了
		void set(const size_t&, const uint&);//10进制位的index位修改
		void randdata(const bint& other);

		friend ostream& operator<<(ostream&, const bint&);
		friend istream& operator>>(istream&, bint&);

		/*
		* 防止int强制转换，同时后期可以针对int进行特别优化
		*/
		friend bool operator<(const bint&, const bint&);
		friend bool operator<(const bint&, int);
		friend bool operator==(const bint&, const bint&);
		friend bool operator==(const bint&, int);
		friend bool operator<=(const bint&, const bint&);
		friend bool operator<=(const bint&, int);

		bint& operator+=(const bint&);
		bint& operator+=(bint&&);
		bint& operator+=(const int&);
		bint& operator-=(const bint&);
		bint& operator-=(bint&&);
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

		friend bint Factorial(int);

		friend bint operator-(const bint&, bint&&);
		friend bint operator-(const int&, bint);

		friend bint operator*(bint&&, bint&&);
		friend bint operator*(bint, int);
		friend bint operator*(const int&, bint);
		friend bint operator/(const bint&, const bint&);
		friend bint operator/(const bint&, const int&);
		friend bint operator/(bint&&, const int&);
		friend bint operator/(int, const bint&);

		void quick_mul_10();//O(n)乘10，但省去了部分运算
		bint& quick_mul_10k(const int& = 1);//O(n)乘10^k
		bint& quick_divide_10k(const int& = 1);
		void abs();//变为绝对值，即positive取true
		friend bint abs(bint);
		void swap(bint& other);

		int toint()const;//转为int
		long long toll()const;//转为long long
		std::string tostr()const;//转为字符串

		bint inv(int = -1)const;//求精度为lim的逆元，很慢！
		bint2 to2bit()const;//转为2进制bint
		//用于测试三种乘法
	#ifdef TEST
		friend bint test1(const bint& a, const bint& b) {
			if (a.iszero() || b.iszero()) {
				return bint();
			}
			bint c;
			Array_func::SlowMul(a.vec, b.vec, c.vec);

			c.positive = !(a.positive ^ b.positive);
			if (c.iszero())c.positive = true;
			return c;
		}
		friend bint test2(const bint& a, const bint& b) {
			if (a.iszero() || b.iszero()) {
				return bint();
			}
			bint c;
			Karatsuba(a, b, c);
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
			int n = a.size(), m = b.size();

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

			if (((1 << min(30, max(0, _min - 32 >> 1))) <= _max))
				Array_func::SlowMul(a.vec, b.vec, c.vec);
			else FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);


			if (((1 << min(30, max(0, _min - 32 >> 1))) <= _max))
				mode = 1;
			else mode = 3;

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

		void assign(int);
		void assign(long long);
		void assign(const char*);
		void assign(const std::string&);

		//saveadd和savedel均为均摊O(1)
		void saveadd(int);
		void savedel(int);

		static void quickadd(bint2&, const bint2&, bool);
		static void addint(bint2&, int, bool);

		static bint2 largedivide(const bint2&, const bint2&);
		static bint2 smalldivide(const bint2&, const bint2&);
		static bint2 knuthdivide(const bint2&, const bint2&);
		static bint2 quickdivide(const bint2&, const bint2&);

		static bint2 del(const bint2&, const bint2&, bool);
		static void quickdel(bint2&, const bint2&, bool, int = 0);

		static void delint(bint2& a, int b, bool _positive);
		static void quickmul(bint2& a, const bint2&);
		static void mulint(const bint2& a, int b, bint2& c);
		bint get10bit()const;
		void clear();
	public:
		void assign(const bint2& other, const int& L, const int& R);
		const bool ispositive()const;
		const bool iszero()const;
		bint2()noexcept : positive(true) {
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
		bint2(const bint2& other, const bool& _positive)noexcept :vec(other.vec), positive(_positive) {

		}
		bint2(bint2&& other, bool _positive)noexcept : vec(std::move(other.vec)), positive(_positive) {

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
		bint2& operator=(bint2&& other) noexcept {
			vec = std::move(other.vec);
			positive = other.positive;
			return*this;
		}
		bint2& operator=(const char* s) noexcept {
			assign(s);
			return*this;
		}
		bint2& operator=(const std::string& s)noexcept {
			assign(s);
			return*this;
		}
		bint2& operator=(const bint& other)noexcept;


		const uint operator[](const int&)const;
		uint& operator[](const int&);
		uint& save_at(const int&);

		bool at(const int&)const;
		void set(const int&, const bool&);
		void randdata(const bint2&);

		friend ostream& operator<<(ostream& out, const bint2& x);
		friend istream& operator>>(istream& in, bint2& x);

		friend bool operator<(const bint2&, const bint2&);
		friend bool operator<(const bint2&, const int&);
		friend bool operator<(const int&, const bint2&);
		friend bool operator==(const bint2&, const bint2&);
		friend bool operator==(const bint2&, const int&);
		friend bool operator==(const int&, const bint2&);
		friend bool operator<=(const bint2&, const bint2&);
		friend bool operator<=(const bint2&, const int&);
		friend bool operator<=(const int&, const bint2&);
		friend bool operator>(const bint2&, const bint2&);
		friend bool operator>(const bint2&, const int&);
		friend bool operator>(const int&, const bint2&);
		friend bool operator>=(const bint2&, const bint2&);
		friend bool operator>=(const bint2&, const int&);
		friend bool operator>=(const int&, const bint2&);
		friend bool operator!=(const bint2&, const bint2&);
		friend bool operator!=(const bint2&, const int&);
		friend bool operator!=(const int&, const bint2&);

		bint2& operator+=(const bint2&);
		bint2& operator+=(const int&);
		bint2& operator-=(const bint2&);
		bint2& operator-=(const int&);
		bint2& operator*=(const bint2&);
		bint2& operator*=(int);
		bint2& operator/=(const bint2&);
		bint2& operator/=(int);
		bint2& operator%=(const bint2&);
		bint2& operator%=(int);

		bint2& operator++();
		bint2 operator++(int);
		bint2& operator--();
		bint2 operator--(int);

		friend bint2 operator+(bint2);
		friend bint2 operator-(bint2);
		friend bool operator!(const bint2&);
		friend bool operator!(bint2&&);

		friend bint2 operator+(const bint2&, const bint2&);
		friend bint2 operator+(bint2&&, const bint2&);
		friend bint2 operator+(const bint2&, bint2&&);
		friend bint2 operator+(bint2&&, bint2&&);
		friend bint2 operator+(bint2, const int&);
		friend bint2 operator+(const int&, bint2);
		friend bint2 operator-(const bint2&, const bint2&);
		friend bint2 operator-(bint2&&, const bint2&);
		friend bint2 operator-(const bint2&, bint2&&);
		friend bint2 operator-(bint2&&, bint2&&);
		friend bint2 operator-(bint2, const int&);
		friend bint2 operator-(const int&, bint2);

		friend bint2 operator*(const bint2&, const bint2&);
		friend bint2 operator*(bint2&&, const bint2&);
		friend bint2 operator*(const bint2&, bint2&&);
		friend bint2 operator*(bint2&&, bint2&&);
		friend bint2 operator*(bint2, const int&);
		friend bint2 operator*(const int&, bint2);
		friend bint2 operator/(const bint2&, const bint2&);
		friend bint2 operator/(const bint2&, const int&);
		friend bint2 operator/(const int&, const bint2&);
		friend bint2 operator%(const bint2&, const bint2&);
		friend bint2 operator%(const bint2&, const int&);
		friend bint2 operator%(const int&, const bint2&);


		void resize(const uint&);
		const uint size()const;
		const uint length()const;
		void reserve(const uint&);
		void relength(const uint&);

		friend bint2 qpow(const bint2&, int);

		void quick_mul_2();//O(n)乘10，但省去了部分运算
		bint2& quick_mul_2k(int = 1);//O(n)乘10^k
		bint2& quick_divide_2k(int = 1);
		void abs();
		friend bint2 abs(bint2);

		std::string tostr()const;
		bint to10bit()const;
	};


}
#endif BINT_H
