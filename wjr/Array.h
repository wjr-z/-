/* 同样分为了 Array 和 Array2 ，分别对应 bint 和 bint2
* 还有相应的一些功能性函数
*/
#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include "Allocator.h"
#include "math_func.h"

namespace Math {

	//#define ARRAYDEBUG

	using std::vector;

	/*---友元函数声明---*/

	//------------------------------------------------------------------//
	class Array;

	void swap(Array&, Array&);
	bool operator<(const Array&, const Array&);
	bool operator==(const Array&, const Array&);
	bool operator>(const Array&, const Array&);
	bool operator<=(const Array&, const Array&);
	bool operator>=(const Array&, const Array&);
	bool operator!=(const Array&, const Array&);

	class Array2;

	bool operator<(const Array2&, const Array2&);
	bool operator==(const Array2&, const Array2&);
	bool operator<=(const Array2&, const Array2&);
	bool operator>(const Array2&, const Array2&);
	bool operator>=(const Array2&, const Array2&);
	bool operator!=(const Array2&, const Array2&);

	//------------------------------------------------------------------//

	/*---Array类---*/
	/*---使用vector---*/

#if defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL!=0
#define _ALLOCATOR_DEBUG
#endif

	class Array {
	private:
	#ifdef _ALLOCATOR_DEBUG //DEBUG版本
		vector<int>vec;
	#endif
	#ifndef _ALLOCATOR_DEBUG
		vector<int, Allocator<int>>vec;
	#endif
		size_t Size;
	public:
		Array(const size_t& index = 1)noexcept :vec(index) {
		#ifdef ARRAYDEBUG
			cout << "Array构造函数\n";
		#endif ARRAYDEBUG
			resize(1);
		}
		Array(const Array& other)noexcept :vec(other.vec), Size(other.Size) {
		#ifdef ARRAYDEBUG
			cout << "Array左值拷贝构造\n";
		#endif ARRAYDEBUG
		}
		Array(Array&& other)noexcept
			:vec(std::move(other.vec)), Size(other.Size) {
		#ifdef ARRAYDEBUG
			cout << "Array右值拷贝构造\n";
		#endif ARRAYDEBUG
		}
		Array& operator=(const Array& other)noexcept {
		#ifdef ARRAYDEBUG
			cout << "Array左值复制\n";
		#endif ARRAYDEBUG
			vec = other.vec;
			Size = other.Size;
			return*this;
		}
		Array& operator=(Array&& other)noexcept {
		#ifdef ARRAYDEBUG
			cout << "Array右值复制\n";
		#endif ARRAYDEBUG
			vec=std::move(other.vec);
			Size = other.Size;
			return*this;
		}
		const bool iszero()const;
		const size_t&size() const;
		const size_t capacity()const;
		const size_t length()const;
		void resize(const size_t& index);
		void reserve(const size_t& index);
		void clear();
		void relength(const size_t& index);
		void reverse();
		void assign(const Array& other, const size_t& L, const size_t& R);

		int& save_at(const size_t& index);
		const int operator[](const size_t& index)const;
		int& operator[](const size_t& index);

		void set(const size_t& index, const int& val);
		uint at(const size_t& index)const;
		void swap(Array& other);
	};


	/*---biginteger的一些函数---*/

	class Array_func {
	private:
		static const int jw = 100000000, kz = 8, maxi = jw - 1;//常规进位,10^8
	public:
		static void QuickMul10k(Array&, const size_t& = 1);//快速乘以10^k
		static void QuickDivide10k(Array& a, const size_t& = 1);//快速除以10^k
		static void SlowMul(const Array&, const Array&, Array&);//暴力乘法
		static Array SlowMul(const Array&, const Array&);
	};

	class FFT_Array_func {//FFT乘法，使用了Ooura FFT，据说很快，如果有更快的会进行更换
	public:
		static void FFTQuickMul1(const Array&, const Array&, Array&);
		static void FFTQuickMul2(const Array&, const Array&, Array&);
		static void FFTQuickMul4(const Array&, const Array&, Array&);
		static void FFTQuickMul(const Array&, const Array&, Array&);
		static Array FFTQuickMul(Array&, Array&);

	};


	/*---Array2---*/
	/*---Array2<Ty>类---*/
	/*---使用vector---*/

	//ungisned int 
	//每一个存储2^32进制数，即32个二进制数
	class Array2 {
	private:
	#ifdef _ALLOCATOR_DEBUG //DEBUG版本
		vector<uint>vec;
	#endif
	#ifndef _ALLOCATOR_DEBUG
		vector<uint, Allocator<uint>>vec;
	#endif
		size_t Size;
	public:
		Array2(const size_t& index = 1)noexcept :vec(index) {
			resize(1);
		}
		Array2(const Array2& other)noexcept :vec(other.vec), Size(other.Size) {

		}
		Array2(Array2&& other)noexcept
			:vec(std::move(other.vec)), Size(other.Size) {

		}
		Array2& operator=(const Array2& other)noexcept {
			vec = other.vec;
			Size = other.Size;
			return*this;
		}
		Array2& operator=(Array2&& other)noexcept {
			vec = std::move(other.vec);
			Size = other.Size;
			return*this;
		}
		const bool iszero()const;
		const size_t size() const;
		const size_t length()const;
		void resize(const size_t& index);
		void reserve(const size_t& index);
		void clear();

		//一般而言save_at和const oeprator[]的速度相近,优化速度可以将不会越界的改为save_at
		//对于const 变量、函数，使用[]即为const oeprator[]

		uint& save_at(const size_t& index);
		const uint operator[](const size_t& index)const;
		uint& operator[](const size_t& index);

		void set(const size_t& index, const bool& val);
		uint at(const size_t& index)const;

		void relength(const size_t& index);
	};



	class Array2_func {
	public:
		static void QuickMul2k(Array2&, const uint & = 1);//快速乘2^k,即左移k位
		static void QuickDivide2k(Array2& a, const int& = 1);//快速除2^k，即右移k位
		static void SlowMul(const Array2&, const Array2&, Array2&);
		static Array2 SlowMul(const Array2&, const Array2&);
	};

	/*---biginteger的一些函数---*/

	class FFT_Array2_func {
	public:
		static void FFTQuickMul2(const Array2&, const Array2&, Array2&);
		static void FFTQuickMul3(const Array2&, const Array2&, Array2&);
		static void FFTQuickMul4(const Array2&, const Array2&, Array2&);
		static void FFTQuickMul(const Array2&, const Array2&, Array2&);
		static Array2 FFTQuickMul(Array2&, Array2&);
	};


}


#endif ARRAY_H