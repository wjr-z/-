/* 同样分为了 Array 和 Array2 ，分别对应 bint 和 bint2
* 还有相应的一些功能性函数
*/
#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include "math_func.h"

_MATH_BEGIN


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

template<typename Ty>
class Array {
private:
	vector<Ty>vec;
	size_t Size;
public:
	Array(const size_t& index = 1) :vec(index) {
		resize(1);
	}
	Array(const Array& other) :vec(other.vec), Size(other.Size) {

	}
	Array& operator=(const Array& other) {
		Size = other.Size;
		vec.assign(other.vec.begin(), other.vec.end());
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
	short at(const int& index)const {
		return (this->operator[](index >> 3) / _10k[index & 7]) % 10;
	}

	/*---默认8个10进制数一划分---*/
	/*---考虑到数太大，FFT会爆精度，因此需要先转化为较小的数---*/
	/*---不用一个函数写是因为之前是一个函数写的，现在在降低常数---*/
	Array<short> turnto1()const;//每1位划分
	Array<short> turnto2()const;//2位划分
	Array<short> turnto4()const;//4位划分
	Array<int>turnback1()const;
	Array<int>turnback2()const;
	Array<int>turnback4()const;
};

template<typename Ty>
Array<short> Array<Ty>::turnto1()const {//10^8进制转换为10&kz进制
	size_t Size = size();
	Array<short>turnTo;
	turnTo.reserve(Size << 3);
	size_t i, head = 0;
	for (i = 0; i < Size; ++i) {
		Ty val = vec[i];
		for (int j = 0; j < 8; ++j) {
			turnTo[head] += (val % 10);
			++head;
			val /= 10;
		}
	}
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

template<typename Ty>
Array<short> Array<Ty>::turnto2()const {
	size_t Size = size();
	Array<short>turnTo;
	turnTo.reserve(Size << 2);
	size_t i, head = 0;
	for (i = 0; i < Size; ++i) {
		Ty val = vec[i];
		turnTo[head++] = val % 100;
		val /= 100;
		turnTo[head++] = val % 100;
		val /= 100;
		turnTo[head++] = val % 100;
		turnTo[head++] = val / 100;
	}
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

template<typename Ty>
Array<short> Array<Ty>::turnto4()const {
	size_t Size = size();
	Array<short>turnTo;
	turnTo.resize(Size << 1);
	size_t i, head = 0;
	for (i = 0; i < Size; ++i) {
		Ty val = vec[i];
		turnTo[head++] = val % 10000;
		turnTo[head++] = val / 10000;
	}
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}


template<typename Ty>
Array<int> Array<Ty>::turnback1()const {
	size_t Size = size();
	Array<int>turnTo;
	turnTo.reserve(Size >> 3);
	size_t i, head = 0;
	for (i = 0; i + 7 < Size; i += 8, ++head) {
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2] +
			1000 * vec[i + 3] + 10000 * vec[i + 4] + 100000 * vec[i + 5] +
			1000000 * vec[i + 6] + 10000000 * vec[i + 7];
	}
	switch (Size - i) {
	case 0:break;
	case 1:
		turnTo[head] = vec[i]; break;
	case 2:
		turnTo[head] = vec[i] + 10 * vec[i + 1]; break;
	case 3:
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2]; break;
	case 4:
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2] + 1000 * vec[i + 3]; break;
	case 5:
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2] + 1000 * vec[i + 3] + 10000 * vec[i + 4]; break;
	case 6:
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2] + 1000 * vec[i + 3] + 10000 * vec[i + 4] + 100000 * vec[i + 5];
		break;
	case 7:
		turnTo[head] = vec[i] + 10 * vec[i + 1] + 100 * vec[i + 2] + 1000 * vec[i + 3] + 10000 * vec[i + 4] + 100000 * vec[i + 5] + 1000000 * vec[i + 6];
		break;
	default:break;
	}
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

template<typename Ty>
Array<int> Array<Ty>::turnback2()const {
	size_t Size = size();
	Array<int>turnTo;
	turnTo.reserve(Size >> 2);
	size_t i, head = 0;
	for (i = 0; i + 3 < Size; i += 4, ++head) {
		turnTo[head] = vec[i] + 100 * vec[i + 1] + 10000 * vec[i + 2] +
			1000000 * vec[i + 3];
	}
	switch (Size - i) {
	case 0:break;
	case 1:
		turnTo[head] = vec[i]; break;
	case 2:
		turnTo[head] = vec[i] + 100 * vec[i + 1]; break;
	case 3:
		turnTo[head] = vec[i] + 100 * vec[i + 1] + 10000 * vec[i + 2]; break;
	default:break;
	}
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo.clear();
	return turnTo;
}

template<typename Ty>
Array<int> Array<Ty>::turnback4()const {
	size_t Size = size();
	Array<int>turnTo;
	turnTo.reserve(Size >> 1);
	size_t i, head = 0;
	for (i = 0; i + 1 < Size; i += 2, ++head) {
		turnTo[head] = vec[i] + 10000 * vec[i + 1];
	}
	if (i + 1 == Size)
		turnTo[head] = vec[i];
	Size = turnTo.size();
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
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
	uint Size;
public:
	Array2(const uint& index = 1) :vec(index) { Size = 1; }
	const bool iszero()const {//判断是否为0
		if ((Size == 1 && this->operator[](0) == 0) || !Size)return true;
		return false;
	}
	const uint size() const { return Size; }//获得size
	const uint length(const uint& blo = 32)const {//求出二进制总长度
		uint Size = size();
		return ((Size - 1) * blo) + quicklog2(this->operator[](Size - 1)) + 1;
	}
	void resize(const uint& index) { vec.resize(index); Size = index; }
	void reserve(const uint& index) { vec.reserve(index); }

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
	Array2 turnto1()const;//2^32 - > 2^2
	Array2 turnto2()const;//2^32 - > 2^4
	Array2 turnto3()const;//2^32 - > 2^8
	Array2 turnto4()const;//2^32 - > 2^16
	Array2 turnback(const uint & = 1)const;//2^(2^k) - > 2^32

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
	static void FFTQuickMul1(const Array2&, const Array2&, Array2&);
	static void FFTQuickMul2(const Array2&, const Array2&, Array2&);
	static void FFTQuickMul3(const Array2&, const Array2&, Array2&);
	static void FFTQuickMul4(const Array2&, const Array2&, Array2&);
	static void FFTQuickMul(const Array2&, const Array2&, Array2&);
	static Array2 FFTQuickMul(Array2&, Array2&);
};


_MATH_END


#endif ARRAY_H