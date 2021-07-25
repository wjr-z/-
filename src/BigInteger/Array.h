/* 分为了 Array 和 Array2 ，分别对应 bint 和 bint2
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

	class Array_func;

	class Array {
	private:
		friend Array_func;

	#ifdef _ALLOCATOR_DEBUG //DEBUG版本
		vector<int>vec;
	#endif
	#ifndef _ALLOCATOR_DEBUG
		vector<int, Allocator<int>>vec;
	#endif
		size_t Size;
	public:
		Array(const size_t& index = 1)noexcept;
		Array(const Array& other)noexcept;
		Array(Array&& other)noexcept;
		Array& operator=(const Array& other)noexcept;
		Array& operator=(Array&& other)noexcept;
		const int* begin()const;
		const int* end()const;
		int*begin();
		int*end();
		bool iszero()const;
		const size_t& size() const;
		size_t capacity()const;
		size_t length()const;
		void resize(const size_t& index);
		void reserve(const size_t& index);
		void clear();
		void relength(const size_t& index);
		void assign(const Array& other, const size_t& L, const size_t& R);

		int& save_at(const size_t& index);
		int operator[](const size_t& index)const;
		int& operator[](const size_t& index);

		void set(const size_t& index, const uint32_t& val);
		uint32_t at(const size_t& index)const;
		void swap(Array& other);
	};


	/*---biginteger的一些函数---*/

	class Array_func {
	private:
		static const int jw = 100000000;//常规进位,10^8
	public:
		static void QuickMul10k(Array&, const size_t & = 1);//快速乘以10^k
		static void QuickDivide10k(Array& a, const size_t & = 1);//快速除以10^k
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

	class reference2;

	class Array2_func;
	//ungisned int 
	//每一个存储2^32进制数，即32个二进制数
	class Array2 {
		friend reference2;
	private:
		friend Array2_func;
	#ifdef _ALLOCATOR_DEBUG //DEBUG版本
		vector<uint32_t>vec;
	#endif
	#ifndef _ALLOCATOR_DEBUG
		vector<uint32_t, Allocator<uint32_t>>vec;
	#endif
		size_t Size;
		void setbool(const size_t& index, bool val);
		bool atbool(const size_t& index)const;
	public:
		Array2(const size_t& index = 1)noexcept;
		Array2(const Array2& other)noexcept;
		Array2(Array2&& other)noexcept;
		Array2& operator=(const Array2& other)noexcept;
		Array2& operator=(Array2&& other)noexcept ;
		const uint32_t*begin()const ;
		const uint32_t*end()const;
		uint32_t*begin();
		uint32_t*end();
		bool iszero()const;
		size_t size() const;
		size_t length()const;
		void resize(const size_t& index);
		void reserve(const size_t& index);
		void clear();

		uint32_t& save_at(const size_t& index);
		uint32_t at(const size_t&index)const;
		uint32_t&at(const size_t&index);

		bool operator[](const size_t&index)const;
		reference2 operator[](const size_t&index);

		void maintain();//用于弹出头部多余的1

		void relength(const size_t& index);
	};

	class reference2 {
		friend Array2;
	private:
		Array2* Point;
		size_t _Pos;
	public:
		~reference2() noexcept;
		reference2& operator=(bool _Val)noexcept;
		reference2& operator=(const reference2& _Bitref) noexcept ;

		bool operator~() const noexcept;
		operator bool() const noexcept;

	private:
		reference2() noexcept;
		reference2(Array2& _bint2, size_t _Pos) ;
	};

	class Array2_func {
	public:
		static void QuickMul2k(Array2&, const uint32_t & = 1);//快速乘2^k,即左移k位
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


#endif