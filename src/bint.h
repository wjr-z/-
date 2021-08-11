#ifndef BINT_H
#define BINT_H

#include <iomanip>

#include "Array.h"

namespace Math {

	using std::istream;
	using std::ostream;
	using std::endl;

	/*---biginter类(10bit)---*/
	/*---每一位对10^8取模的动态扩展大整数类---*/


#define BINT_ZERO bint()
#define BINT2_ZERO bint2()

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
	bint factorial(int);
	bint fibonacci(int);

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
	bint qpow(const bint&, size_t);
	bint qpow(int, bint);

	bint abs(bint);
	bint gcd(const bint&, const bint&);
	bint randbint(const bint& L, const bint& R);
	bint randbint(size_t);
	bint randprime(const bint& L, const bint& R); //获取一个[L,R]的随机素数
	bint randprime(size_t); //获取一个长度为n的随机素数
	void swap(bint&, bint&) noexcept;

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

	bint2 operator<<(const bint2&, int);
	bint2 operator>>(const bint2&, int);
	bint2 operator&(const bint2&, const bint2&);
	bint2 operator|(const bint2&, const bint2&);
	bint2 operator^(const bint2&, const bint2&);

	bint2 qpow(const bint2&, size_t);

	bint2 abs(bint2);
	bint2 randbint2(size_t);

	bool operator==(const bint&, const bint2&);
	bool operator==(const bint2&, const bint&);

	std::string bit10_to_bit2(const std::string&); //10进制字符串转化为2进制字符串
	std::string bit2_to_bit10(const std::string&); //2进制字符串转化为10进制字符串

	bint sqrt(const bint&);
	bint2 sqrt(const bint2&);
	bool isprime(const bint&); //大数判别质数
	std::vector<bint> pollard_rho(bint);
	bint max_prime(const bint& x);

	//------------------------------------------------------------------//

	class alignas(8) bint {
		friend class bfloat;
	private:
		Array vec; //每一位存一个int
		bool positive; //positive为true表示为正数，否则为负数
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
		static void TOOM_COOK_3(const bint&, const bint&, bint&);
		/*---快速乘法，自动选择不同算法---*/
		static void quickmul(bint&, const bint&);
		/*---低精度乘法---*/
		static void mulint(const bint&, const int&, bint&);

		bint2 get2bit() const;
		void clear();
		void resize(const size_t&);
		size_t size() const;
		void reserve(const size_t&);
		void pop_back();
		bint(const size_t&, int);
	public:
		/*---初始化为other的[L,R]元素---*/
		void assign(const bint& other, const size_t& L, const size_t& R);
		bool is_zero() const;
		bool ispositive() const;
		bint() noexcept;
		~bint() noexcept;
		explicit bint(const int& val) noexcept;
		explicit bint(const long long& val) noexcept;
		explicit bint(const char* s) noexcept;
		explicit bint(const std::string& s) noexcept;
		bint(const bint& other) noexcept;
		bint(bint&& other) noexcept;
		bint(const bint& other, const bool& _positive) noexcept;
		bint(bint&& other, const bool& _positive) noexcept;

		bint(const bint& other, const size_t& L, const size_t& R) noexcept;
		bint(Array vec, bool positive) noexcept;
		bint& operator=(const int& val) noexcept;
		bint& operator=(const long long& val) noexcept;
		bint& operator=(const char* s) noexcept;
		bint& operator=(const std::string& s) noexcept;
		bint& operator=(const bint& other) noexcept;
		bint& operator=(bint&& other) noexcept;
		bint& operator=(const bint2& other) noexcept;

		size_t length() const;
		void relength(const size_t&);
		void reverse(size_t = 0);

		int at(const size_t&) const;
		int& at(const size_t&);
		int& save_at(const size_t&);

		int operator[](const size_t&) const;
		reference operator[](const size_t&);

		const int* begin() const;
		const int* end() const;
		int* begin();
		int* end();

		void rand(const bint& other);

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

		friend bint operator+(const bint&, const bint&);

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

		friend bint operator-(const bint&, bint&&);
		friend bint operator-(const int&, bint);

		friend bint operator*(bint&&, bint&&);
		friend bint operator*(bint, int);
		friend bint operator*(const int&, bint);
		friend bint operator/(const bint&, const bint&);
		friend bint operator/(const bint&, const int&);
		friend bint operator/(bint&&, const int&);
		friend bint operator/(int, const bint&);

		void quick_mul_10(); //O(n)乘10，但省去了部分运算
		bint& quick_mul_10k(const size_t&); //O(n)乘10^k
		bint& quick_divide_10k(const size_t&);
		void abs(); //变为绝对值，即positive取true
		friend bint abs(bint);
		void oppsite();
		void swap(bint& other) noexcept;

		int toint() const; //转为int
		long long toll() const; //转为long long
		std::string tostr() const; //转为字符串

		bint2 to2bit() const; //转为2进制bint
		//用于测试三种乘法
#ifdef TEST
		friend bint test1(const bint& a, const bint& b) {
			if (a.is_zero() || b.is_zero()) {
				return bint();
			}
			bint c;
			Array_func::SlowMul(a.vec, b.vec, c.vec);

			c.positive = !(a.positive ^ b.positive);
			if (c.is_zero())c.positive = true;
			return c;
		}

		friend bint test2(const bint& a, const bint& b) {
			if (a.is_zero() || b.is_zero()) {
				return bint();
			}
			bint c;
			Karatsuba(a, b, c);
			c.positive = !(a.positive ^ b.positive);
			if (c.is_zero())c.positive = true;
			return c;

		}

		friend bint test3(const bint& a, const bint& b) {
			bint c;
			if (a.is_zero() || b.is_zero()) {
				c = 0;
				c.positive = true;
				return c;
			}

			FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);
			c.positive = !(a.positive ^ b.positive);
			if (c.is_zero())c.positive = true;
			return c;
		}

		friend bint test4(const bint& a, const bint& b) {
			bint c;
			if (a.is_zero() || b.is_zero()) {
				c = 0;
				c.positive = true;
				return c;
			}
			const size_t n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

			if (_min <= 32 || (((1ull << min(static_cast<size_t>(30), max(static_cast<size_t>(0), (_min - 32) >> 1))) <=
				_max)))
				Array_func::SlowMul(a.vec, b.vec, c.vec);
			else FFT_Array_func::FFTQuickMul(a.vec, b.vec, c.vec);


			if (_min <= 32 || (((1ull << min(static_cast<size_t>(30), max(static_cast<size_t>(0), (_min - 32) >> 1))) <=
				_max)))
				mode = 1;
			else mode = 3;

			c.positive = !(a.positive ^ b.positive);
			if (c.is_zero())c.positive = true;
			return c;
		}

		friend bint testTOOM(const bint& a, const bint& b) {
			bint c;
			TOOM_COOK_3(a, b, c);
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
		Array2 vec; //以2^32为进制
		bool positive; //positive为true表示为正数，否则为负数

		void assign(int);
		void assign(long long);
		void assign(const char*);
		void assign(const std::string&);

		//saveadd和savedel均为均摊O(1)
		void saveadd(size_t);
		void savedel(size_t);

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
		bint get10bit() const;
		void clear();
	public:
		void assign(const bint2& other, const int& L, const int& R);
		bool ispositive() const;
		bool iszero() const;
		bint2() noexcept;
		~bint2();
		explicit bint2(const int& val) noexcept;
		explicit bint2(const long long& val) noexcept;
		explicit bint2(const char* s) noexcept;
		explicit bint2(const std::string& s) noexcept;

		bint2(const bint2& other) noexcept;
		bint2(bint2&& other) noexcept;
		bint2(const bint2& other, const bool& _positive) noexcept;
		bint2(bint2&& other, bool _positive) noexcept;
		bint2(Array2 vec, bool positive) noexcept;
		bint2& operator=(const int& val) noexcept;
		bint2& operator=(const long long& val) noexcept;
		bint2& operator=(const bint2& other) noexcept;
		bint2& operator=(bint2&& other) noexcept;
		bint2& operator=(const char* s) noexcept;
		bint2& operator=(const std::string& s) noexcept;
		bint2& operator=(const bint& other) noexcept;

		uint32_t at(size_t) const;
		uint32_t& at(size_t);
		uint32_t& save_at(size_t);

		bool operator[](const size_t& index) const;
		reference2 operator[](const size_t& index);

		const uint32_t* begin() const;
		const uint32_t* end() const;
		uint32_t* begin();
		uint32_t* end();

		void maintain();
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

		friend bint2 operator<<(const bint2&, int);
		friend bint2 operator>>(const bint2&, int);
		friend bint2 operator&(const bint2&, const bint2&);
		friend bint2 operator|(const bint2&, const bint2&);
		friend bint2 operator^(const bint2&, const bint2&);

		bint2& operator&=(const bint2&);
		bint2& operator|=(const bint2&);
		bint2& operator^=(const bint2&);

		size_t count();

		void resize(size_t);
		size_t size() const;
		size_t length() const;
		void reserve(const uint32_t&);
		void relength(size_t);

		void quick_mul_2(); //O(n)乘10，但省去了部分运算
		bint2& quick_mul_2k(size_t = 1); //O(n)乘10^k
		bint2& quick_divide_2k(size_t = 1);
		void abs();
		friend bint2 abs(bint2);
		void oppsite();

		std::string tostr() const;
		bint to10bit() const;
	};
}
#endif
