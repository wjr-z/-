/* 同样分为了 Array 和 Array2 ，分别对应 bint 和 bint2
* 还有相应的一些功能性函数
*/
#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include "Allocator.h"
#include "math_func.h"


_MATH_BEGIN

//#define ARRAYDEBUG

using std::vector;

/*---友元函数声明---*/

//------------------------------------------------------------------//

class Array2;
bool operator<(const Array2&, const Array2&);
bool operator==(const Array2&, const Array2&);
bool operator<=(const Array2&, const Array2&);
bool operator>(const Array2&, const Array2&);
bool operator>=(const Array2&, const Array2&);
bool operator!=(const Array2&, const Array2&);

//------------------------------------------------------------------//

/*---Array<Ty>类---*/
/*---使用vector---*/

#if defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL!=0
#define _ALLOCATOR_DEBUG
#endif


template<typename Ty>
class Array {
private:
#ifdef _ALLOCATOR_DEBUG //DEBUG版本
	vector<Ty>vec;
#endif
#ifndef _ALLOCATOR_DEBUG
	vector<Ty, Allocator<Ty>>vec;
#endif
	size_t Size;
public:
	Array(const size_t& index = 1)noexcept :vec(index) {
	#ifdef ARRAYDEBUG
		cout<<"Array构造函数\n";
	#endif ARRAYDEBUG
		resize(1);
	}
	Array(const Array& other)noexcept :vec(other.vec), Size(other.Size) {
	#ifdef ARRAYDEBUG
		cout<<"Array左值拷贝构造\n";
	#endif ARRAYDEBUG
	}
	Array(Array&& other)noexcept
		:vec(std::move(other.vec)),Size(other.Size) {
	#ifdef ARRAYDEBUG
		cout<<"Array右值拷贝构造\n";
	#endif ARRAYDEBUG
	}
	Array& operator=(const Array& other)noexcept {
	#ifdef ARRAYDEBUG
		cout<<"Array左值复制\n";
	#endif ARRAYDEBUG
		vec = other.vec;
		Size = other.Size;
		return*this;
	}
	Array& operator=(Array&& other)noexcept {
	#ifdef ARRAYDEBUG
		cout<<"Array右值复制\n";
	#endif ARRAYDEBUG
		vec=std::move(other.vec);
		Size=other.Size;
		return*this;
	}
	const bool iszero()const {
		if ((Size == 1 && this->operator[](0) == 0) || !Size)return true;
		return false;
	}
	const size_t size() const { return Size; }
	const size_t length()const {
		return ((Size - 1) << 3) + quicklog10(this->operator[](Size - 1)) + 1;
	}
	void resize(const size_t& index) { vec.resize(index); Size = index; }
	void reserve(const size_t& index) { vec.reserve(index); }
	void clear() { resize(1); save_at(0) = 0; }
	void relength(const size_t& index) {
		if (!index) { clear(); return; }
		this->resize(((index - 1) >> 3) + 1);
		this->save_at((index - 1) >> 3) %= _10k[((index - 1) & 7) + 1];
	}
	void reverse() {
		std::reverse(vec.begin(), vec.end());
	}
	void append(const Array<Ty>& other) {
		vec.insert(vec.end(), other.vec.begin(), other.vec.end());
	}
	void insert(const size_t& pos, const Array<Ty>& other, const size_t& L, const size_t& R) {
		vec.insert(vec.begin() + pos, other.vec.begin() + L, other.vec.begin() + R);
	}
	void assign(const Array<Ty>&other,const int&L,const int&R) {
		vec.assign(other.vec.begin()+L,other.vec.begin()+R);
		Size=R-L;
	}

	Ty& save_at(const size_t& index) { return vec[index]; }
	const Ty& operator[](const size_t& index)const { return vec[index]; }
	Ty& operator[](const size_t& index) { if (size() <= index)this->resize(index + 1); return save_at(index); }

	void set(const size_t& index, const Ty& val) {
		size_t pos = index >> 3;
		int x = _10k[index & 7];
		(*this)[pos] += (val - (this->operator[](pos) / x) % 10) * x;
	}
	uint at(const int& index)const {
		return (this->operator[](index >> 3) / _10k[index & 7]) % 10;
	}
	void swap(Array<Ty>&other);
};

template<typename Ty>
void Array<Ty>::swap(Array<Ty>& other) {
	vec.swap(other.vec);
	std::swap(Size,other.Size);
}

template<typename Ty>
void swap(Array<Ty>& lhs, Array<Ty>& rhs) {
	lhs.swap(rhs);
}

template<typename T>
bool operator<(const Array<T>& _left, const Array<T>& _right) {
	if (_left.size() != _right.size())return _left.size() < _right.size();
	for (int i = _left.size() - 1; ~i; --i)
		if (_left[i] != _right[i])
			return _left[i] < _right[i];
	return false;
}
template<typename T>
bool operator==(const Array<T>& _left, const Array<T>& _right) {
	if (_left.size() != _right.size())return false;
	for (int i = _left.size() - 1; ~i; --i)
		if (_left[i] != _right[i])
			return false;
	return true;
}
template<typename T>
bool operator<=(const Array<T>& _left, const Array<T>& _right) {
	if (_left.size() < _right.size())return true;
	if (_left.size() > _right.size())return false;
	for (int i = _left.size() - 1; ~i; --i)
		if (_left[i] != _right[i])
			return _left[i] < _right[i];
	return true;
}
template<typename T>
bool operator>(const Array<T>& _left, const Array<T>& _right) {
	return !(_left <= _right);
}

template<typename T>
bool operator>=(const Array<T>& _left, const Array<T>& _right) {
	return !(_left < _right);
}

template<typename T>
bool operator!=(const Array<T>& _left, const Array<T>& _right) {
	return !(_left == _right);
}

template<typename T>
bool headjudge(const Array<T>& lhs, const int& n, const Array<T>& rhs, const int& m) {//lhs的前n位和rhs前m位比较
	if (n != m)return n < m;
	for (int i = lhs.size() - 1; i >= lhs.size() - n; --i)
		if (lhs[i] != rhs[i])
			return lhs[i] < rhs[i];
	return false;
}

/*---biginteger的一些函数---*/

class Array_func {
private:
	static const int jw = 100000000, kz = 8, maxi = jw - 1;//常规进位,10^8
public:
	static void QuickMul10k(Array<int>&, const int& = 1);//快速乘以10^k
	static void QuickDivide10k(Array<int>& a, const int& = 1);//快速除以10^k
	static void SlowMul(const Array<int>&, const Array<int>&, Array<int>&);//暴力乘法
	static Array<int> SlowMul(const Array<int>&, const Array<int>&);
};

class FFT_Array_func {//FFT乘法，使用了Ooura FFT，据说很快，如果有更快的会进行更换
public:
	static void FFTQuickMul1(const Array<int>&, const Array<int>&, Array<int>&);
	static void FFTQuickMul2(const Array<int>&, const Array<int>&, Array<int>&);
	static void FFTQuickMul4(const Array<int>&, const Array<int>&, Array<int>&);
	static void FFTQuickMul(const Array<int>&, const Array<int>&, Array<int>&);
	static Array<int> FFTQuickMul(Array<int>&, Array<int>&);

};


/*---Array2---*/
/*---Array2<Ty>类---*/
/*---使用vector---*/

//ungisned int 
//每一个存储2^32进制数，即32个二进制数
class Array2 {
private:
	vector<uint>vec;
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
		vec=other.vec;
		Size=other.Size;
		return*this;
	}
	Array2& operator=(Array2&& other)noexcept {
		vec=std::move(other.vec);
		Size=other.Size;
		return*this;
	}
	const bool iszero()const {//判断是否为0
		if ((Size == 1 && this->operator[](0) == 0) || !Size)return true;
		return false;
	}
	const size_t size() const { return Size; }//获得size
	const size_t length()const {//求出二进制总长度
		return ((Size - 1) <<5) + quicklog2(this->operator[](Size - 1)) + 1;
	}
	void resize(const size_t& index) { vec.resize(index); Size = index; }
	void reserve(const size_t& index) { vec.reserve(index); }
	void clear() { resize(1); save_at(0) = 0; }

	//一般而言save_at和const oeprator[]的速度相近,优化速度可以将不会越界的改为save_at
	//对于const 变量、函数，使用[]即为const oeprator[]

	uint& save_at(const uint& index) { return vec[index]; }//非动态扩展时所用的下标索引
	const uint& operator[](const uint& index)const { return vec[index]; }//常量下标索引
	uint& operator[](const uint& index) { if (size() <= index)this->resize(index + 1); return save_at(index); }//动态扩展下标索引

	void set(const uint& index, const bool& val) {//第index位设置为val
		uint pos = index >> 5, x = index & 31;
		(*this)[pos] ^= (this->operator[](pos) >> x & 1) << x;//将第index位设置为1
		(*this)[pos] |= val << x;//设置为val

	}
	uint at(const uint& index, const uint& kz = 5)const {//2^(2^kz)进制下的第index位,默认都是2^32
		return (this->operator[](index >> kz) >> (index & ((1 << kz) - 1))) & 1;
	}

	void relength(const uint& index) {//重新分配length
		if (!index) { this->resize(1); (*this)[0] = 0; return; }
		this->resize((index - 1 >> 5) + 1);
		if (index & 31)
			this->save_at(index - 1 >> 5) &= ((1 << index & 31) - 1);//对于最后一个可能高位被舍去
	}

	friend bool operator<(const Array2&, const Array2&);
	friend bool operator==(const Array2&, const Array2&);
	friend bool operator<=(const Array2&, const Array2&);
	friend bool operator>(const Array2&, const Array2&);
	friend bool operator>=(const Array2&, const Array2&);
	friend bool operator!=(const Array2&, const Array2&);
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


_MATH_END


#endif ARRAY_H