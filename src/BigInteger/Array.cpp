#include "Array.h"
namespace Math {

	Array::Array(const size_t& index)noexcept :vec(index) {
	#ifdef ARRAYDEBUG
		cout << "Array构造函数\n";
	#endif
		resize(1);
	}
	Array::Array(const Array& other)noexcept :vec(other.vec), Size(other.Size) {
	#ifdef ARRAYDEBUG
		cout << "Array左值拷贝构造\n";
	#endif
	}
	Array::Array(Array&& other)noexcept
		:vec(std::move(other.vec)), Size(other.Size) {
	#ifdef ARRAYDEBUG
		cout << "Array右值拷贝构造\n";
	#endif
	}
	Array& Array::operator=(const Array& other)noexcept {
	#ifdef ARRAYDEBUG
		cout << "Array左值复制\n";
	#endif
		vec = other.vec;
		Size = other.Size;
		return*this;
	}
	Array& Array::operator=(Array&& other)noexcept {
	#ifdef ARRAYDEBUG
		cout << "Array右值复制\n";
	#endif
		vec = std::move(other.vec);
		Size = other.Size;
		return*this;
	}

	bool Array::iszero()const {
		return ((Size==1&&this->operator[](0)==0)||!Size)?true:false;
	}

	const size_t& Array::size()const {
		return Size;
	}

	size_t Array::capacity()const {
		return vec.capacity();
	}

	size_t Array::length()const {
		return ((Size - 1) << 3) + quicklog10(this->operator[](Size - 1)) + 1;
	}

	void Array::resize(const size_t& index) {
		vec.resize(index); Size = index;
	}

	void Array::reserve(const size_t& index) {
		vec.reserve(index);
	}

	void Array::clear() {
		resize(1); save_at(0) = 0;
	}

	void Array::relength(const size_t& index) {
		if (!index) { clear(); return; }
		this->resize(((index - 1) >> 3) + 1);
		this->save_at((index - 1) >> 3) %= _10k[((index - 1) & 7) + 1];
	}

	void Array::assign(const Array& other, const size_t& L, const size_t& R) {
		vec.assign(other.vec.begin() + L, other.vec.begin() + R);
		Size = R - L;
	}

	int& Array::save_at(const size_t& index) { return vec[index]; }

	int Array::operator[](const size_t& index)const { return vec[index]; }

	int& Array::operator[](const size_t& index) {
		if (size() <= index)
			this->resize(index + 1);
		return save_at(index);
	}

	void Array::set(const size_t& index, const uint32_t& val) {
		size_t pos = index >> 3;
		int x = _10k[index & 7];
		(*this)[pos] += (val - (this->operator[](pos) / x) % 10) * x;
	}

	uint32_t Array::at(const size_t& index)const {
		return (this->operator[](index >> 3) / _10k[index & 7]) % 10;
	}

	void Array::swap(Array& other) {
		vec.swap(other.vec);
		std::swap(Size, other.Size);
	}

	void swap(Array& lhs, Array& rhs) {
		lhs.swap(rhs);
	}

	bool operator<(const Array& _left, const Array& _right) {
		if (_left.size() != _right.size())return _left.size() < _right.size();
		for (size_t i = _left.size() - 1; ~i; --i)
			if (_left[i] != _right[i])
				return _left[i] < _right[i];
		return false;
	}

	bool operator==(const Array& _left, const Array& _right) {
		if (_left.size() != _right.size())return false;
		for (size_t i = _left.size() - 1; ~i; --i)
			if (_left[i] != _right[i])
				return false;
		return true;
	}

	bool operator<=(const Array& _left, const Array& _right) {
		if (_left.size() < _right.size())return true;
		if (_left.size() > _right.size())return false;
		for (size_t i = _left.size() - 1; ~i; --i)
			if (_left[i] != _right[i])
				return _left[i] < _right[i];
		return true;
	}

	bool operator>(const Array& _left, const Array& _right) {
		return !(_left <= _right);
	}

	bool operator>=(const Array& _left, const Array& _right) {
		return !(_left < _right);
	}

	bool operator!=(const Array& _left, const Array& _right) {
		return !(_left == _right);
	}

	/*---bint_fuc部分实现---*/

	void Array_func::QuickMul10k(Array& a, const size_t& k) {//快速乘10^k
		if (!k || a.iszero())return;
		size_t Length = a.size();
		a.reserve(Length+(k>>3)+1);
		if (k & 7) {
			int mul10 = _10k[k & 7], mod10 = bintjw / mul10;
			if (a.save_at(Length - 1) >= mod10) {
				a.resize(Length+1);
				a.save_at(Length) = a.save_at(Length - 1) / mod10;
			}
			for (size_t i = Length - 1; i; --i)
				a.save_at(i) = (a.save_at(i) % mod10) * mul10 + a.save_at(i - 1) / mod10;
			a.save_at(0) = (a.save_at(0) % mod10) * mul10;
		}
		if (k >> 3) {
			a.vec.insert(a.vec.begin(), k >> 3, 0);
			a.Size+=k>>3;
		}
	}

	void Array_func::QuickDivide10k(Array& a, const size_t& k) {//快速除以10^k
		if (!k)return;
		size_t Size = a.size();
		if (k & 7) {
			size_t Length = a.length();
			size_t Mo = k & 7, mul10 = 1;
			for (size_t i = 0; i < Mo; ++i)
				mul10 *= 10;
			size_t mod10 = bintjw / mul10;
			for (size_t i = 0; i < Size; ++i) {
				a[i] = a[i] / mul10 + (i + 1 == Size ? 0 : (a[i + 1] % mul10) * mod10);
			}
			for (size_t i = (k&7)<Length?Length-(k&7):0; i < Length; ++i)
				a.set(i, 0);
			while (Size > 1 && !a.save_at(Size - 1))
				--Size;
			a.resize(Size);
		}
		if (k >> 3) {
			if ((k >> 3) >= Size) {
				a.clear();
				return ;
			}
			a.vec.erase(a.vec.begin(),a.vec.begin()+(k>>3));
			a.Size-=(k>>3);
		}
	}

	void Array_func::SlowMul(const Array& A, const Array& B, Array& c) {
		size_t n = A.size(), m = B.size();
		static Allocator<uint32_t>test;
		uint32_t* mid = test.allocate(n + m);

		memset(mid, 0, sizeof(uint32_t) * (n + m));

		size_t Length = n + m - 1;

		Array* pl, * ql;
		pl = const_cast<Array*>(&A);
		ql = const_cast<Array*>(&B);
		int* test1 = &((*pl)[0]), * test2 = &((*ql)[0]);

		for (size_t i = 0, j; i < n; ++i) {
			uint64_t AA = static_cast<uint64_t>(test1[i]),Val;
			for (j = 0; j < m; ++j) {
				Val = mid[i+j] + AA * test2[j];

				mid[i + j + 1] += Val / bintjw;
				mid[i + j] = Val % bintjw;
			}
		}

		if (mid[Length]) {
			c.resize(Length + 1);
			c.save_at(Length) = mid[Length];
		}
		else c.resize(Length);
		memcpy(&c[0], mid, sizeof(int) * Length);
		test.deallocate(mid, n + m);
	}

	Array Array_func::SlowMul(const Array& A, const Array& B) {
		Array c;
		SlowMul(A, B, c);
		return c;
	}

	/*---FFT相关,根据范围选择对应函数---*/

	void FFT_Array_func::FFTQuickMul1(const Array& A, const Array& B, Array& c) {
		size_t n = A.size() << 3, m = B.size() << 3;
		size_t len = n + m - 1;
		uint32_t bit = quicklog2(len - 1) + 1;
		size_t s = 1ull << bit;

		double* a = new double[s << 1];
		for (size_t i = 0; i < n; i += 8) {
			uint32_t val = A[i >> 3];
			for (size_t j = 0; j < 8; ++j) {
				uint32_t q = val % 10;
				val /= 10;
				a[(i + j) << 1] = a[(i + j) << 1 | 1] = q * 0.5;
			}

		} 
		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 8) {
			uint32_t val = B[i >> 3];
			for (size_t j = 0; j < 8; ++j) {
				uint32_t q = val % 10;
				val /= 10;
				a[(i + j) << 1] += q * 0.5;
				a[(i + j) << 1 | 1] -= q * 0.5;
			}
		}

		cdft(s << 1, 1, a);
		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb;
			a[i << 1 | 1] = 2 * sa * sb;
		}
		cdft(s << 1, -1, a);
		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;
		c.resize((len >> 3) + 1);
		c[0] = 0;
		for (size_t i = 0; i < len - 7; i += 8) {
			uint64_t now =
				double_to_ull(a[i << 1] + 0.5) +
				double_to_ull(a[(i+1)<<1] + 0.5) * 10 +
				double_to_ull(a[(i+2)<<1] + 0.5) * 100 +
				double_to_ull(a[(i+3)<<1] + 0.5) * 1000 +
				double_to_ull(a[(i+4)<<1] + 0.5) * 10000 +
				double_to_ull(a[(i+5)<<1] + 0.5) * 100000 +
				double_to_ull(a[(i+6)<<1] + 0.5) * 1000000 +
				double_to_ull(a[(i+7)<<1] + 0.5) * 10000000 +
				c[i >> 3];
			c[i >> 3] = now % bintjw;
			c[(i >> 3) + 1] = now / bintjw;
		}
		c[len >> 3] += double_to_ull(a[(len-7)<<1] + 0.5) +
			double_to_ull(a[(len-6)<<1] + 0.5) * 10 +
			double_to_ull(a[(len-5)<<1] + 0.5) * 100 +
			double_to_ull(a[(len-4)<<1] + 0.5) * 1000 +
			double_to_ull(a[(len-3)<<1] + 0.5) * 10000 +
			double_to_ull(a[(len-2)<<1] + 0.5) * 100000 +
			double_to_ull(a[(len-1)<<1] + 0.5) * 1000000;
		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);
		if (c.iszero())
			c.clear();
		delete[]a;
	}

	void FFT_Array_func::FFTQuickMul2(const Array& A, const Array& B, Array& c) {
		size_t n = A.size() << 2, m = B.size() << 2;
		size_t len = n + m - 1;
		uint32_t bit = quicklog2(len - 1) + 1;
		size_t s = 1ull << bit;

		double* a = new double[s << 1];
		for (size_t i = 0; i < n; i += 4) {
			uint32_t val = A[i >> 2];
			uint32_t q, w, e, r;
			q = val % 100;
			val /= 100;
			w = val % 100;
			val /= 100;
			e = val % 100;
			val /= 100;
			r = val;
			a[i << 1] = a[i << 1 | 1] = q * 0.5;
			a[(i+1)<<1] = a[(i+1)<<1 | 1] = w * 0.5;
			a[(i+2)<<1] = a[(i+2)<<1 | 1] = e * 0.5;
			a[(i+3)<<1] = a[(i+3)<<1 | 1] = r * 0.5;
		}
		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 4) {
			uint32_t val = B[i >> 2];
			uint32_t q, w, e, r;
			q = val % 100;
			val /= 100;
			w = val % 100;
			val /= 100;
			e = val % 100;
			val /= 100;
			r = val;
			a[i << 1] += q * 0.5;
			a[i << 1 | 1] -= q * 0.5;
			a[(i+1)<<1] += w * 0.5;
			a[(i+1)<<1 | 1] -= w * 0.5;
			a[(i+2)<<1] += e * 0.5;
			a[(i+2)<<1 | 1] -= e * 0.5;
			a[(i+3)<<1] += r * 0.5;
			a[(i+3)<<1 | 1] -= r * 0.5;
		}

		cdft(s << 1, 1, a);
		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
		}
		cdft(s << 1, -1, a);
		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;

		c.resize((len >> 2) + 1);
		c[0] = 0;
		for (size_t i = 0; i < len - 3; i += 4) {
			uint64_t now =
				double_to_ull(a[i << 1] + 0.5) +
				double_to_ull(a[(i+1)<<1] + 0.5) * 100 +
				double_to_ull(a[(i+2)<<1] + 0.5) * 10000 +
				double_to_ull(a[(i+3)<<1] + 0.5) * 1000000 +
				c[i >> 2];
			c[i >> 2] = now % bintjw;
			c[(i >> 2) + 1] = now / bintjw;
		}
		c[len >> 2] += double_to_ull(a[(len-3)<<1] + 0.5) +
			double_to_ull(a[(len-2)<<1] + 0.5) * 100 +
			double_to_ull(a[(len-1)<<1] + 0.5) * 10000;
		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);
		if (c.iszero())
			c.clear();

		delete[]a;
	}

	void FFT_Array_func::FFTQuickMul4(const Array& A, const Array& B, Array& c) {
		size_t n = A.size() << 1, m = B.size() << 1;
		size_t len = n + m - 1;
		uint32_t bit = quicklog2(len - 1) + 1;
		size_t s = 1ull << bit;
		double* a = new double[s << 1];

		Array* pl, * ql;
		pl = const_cast<Array*>(&A);
		ql = const_cast<Array*>(&B);
		int* test1 = &((*pl)[0]), * test2 = &((*ql)[0]);

		for (size_t i = 0; i < n; i += 2) {
			uint32_t q = test1[i >> 1] / 10000, p = test1[i >> 1] - q * 10000;
			a[i << 1] = a[i << 1 | 1] = p * 0.5;
			a[(i+1)<<1] = a[(i+1)<<1 | 1] = q * 0.5;
		}

		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 2) {
			uint32_t q = test2[i >> 1] / 10000, p = test2[i >> 1] - q * 10000;
			a[i << 1] += p * 0.5;
			a[i << 1 | 1] -= p * 0.5;
			a[(i+1)<<1] += q * 0.5;
			a[(i+1)<<1 | 1] -= q * 0.5;
		}
		cdft(s << 1, 1, a);

		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb;
			a[i << 1 | 1] = 2 * sa * sb;
		}

		cdft(s << 1, -1, a);

		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;

		c.resize((len >> 1) + 1);

		int* arr = &c[0];
		arr[0] = 0;
		for (size_t i = 0; i < len - 1; i += 2) {
			uint64_t now =
				double_to_ull(a[i << 1] + 0.5) + double_to_ull(a[(i+1)<<1] + 0.5) * 10000 + c[i >> 1];
			arr[i >> 1] = now % bintjw;
			arr[(i >> 1) + 1] = now / bintjw;
		}
		arr[len >> 1] += double_to_ull(a[(len-1)<<1] + 0.5);

		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);

		delete[]a;
	}

	void FFT_Array_func::FFTQuickMul(const Array& A, const Array& B, Array& c) {
		//保证 k*k*n <= 1e11 k 为 每个元素的最大大小
		size_t Size = A.size() + B.size();
		if (Size <= 100000)FFTQuickMul4(A, B, c);
		else if (Size <= 1000000000)FFTQuickMul2(A, B, c);
		else FFTQuickMul1(A, B, c);
	}

	Array FFT_Array_func::FFTQuickMul(Array& A, Array& B) {
		Array c;
		FFTQuickMul(A, B, c);
		return c;
	}

	Array2::Array2(const size_t& index)noexcept :vec(index) {
		resize(1);
	}
	Array2::Array2(const Array2& other)noexcept :vec(other.vec), Size(other.Size) {

	}
	Array2::Array2(Array2&& other)noexcept
		:vec(std::move(other.vec)), Size(other.Size) {

	}
	Array2& Array2::operator=(const Array2& other)noexcept {
		vec = other.vec;
		Size = other.Size;
		return*this;
	}
	Array2& Array2::operator=(Array2&& other)noexcept {
		vec = std::move(other.vec);
		Size = other.Size;
		return*this;
	}

	bool Array2::iszero()const {
		if ((Size == 1 && this->operator[](0) == 0) || !Size)return true;
		return false;
	}

	size_t Array2::size()const {
		return Size;
	}

	size_t Array2::length()const {
		return ((Size - 1) << 5) + quicklog2(this->operator[](Size - 1)) + 1;
	}

	void Array2::resize(const size_t& index) {
		vec.resize(index); Size = index;
	}

	void Array2::reserve(const size_t& index) {
		vec.reserve(index);
	}

	void Array2::clear() {
		resize(1); save_at(0) = 0;
	}

	uint32_t& Array2::save_at(const size_t& index) {
		return vec[index];
	}

	uint32_t Array2::operator[](const size_t& index)const {
		return vec[index];
	}

	uint32_t& Array2::operator[](const size_t& index) {
		if (size() <= index)this->resize(index + 1); return save_at(index);
	}

	void Array2::set(const size_t& index, const bool& val) {
		uint32_t pos = index >> 5, x = index & 31;
		(*this)[pos] ^= (this->operator[](pos) >> x & 1) << x;//将第index位设置为1
		(*this)[pos] |= val << x;//设置为val
	}

	bool Array2::at(const size_t& index)const {
		return (this->operator[](index >> 5) >> (index & 31) & 1);
	}

	void Array2::relength(const size_t& index) {
		if (!index) { this->resize(1); (*this)[0] = 0; return; }
		this->resize(((index - 1) >> 5) + 1);
		if (index & 31)
			this->save_at((index - 1) >> 5) &= ((1 << index & 31) - 1);//对于最后一个可能高位被舍去
	}

	bool operator<(const Array2& lhs, const Array2& rhs) {
		if (lhs.size() != rhs.size())return lhs.size() < rhs.size();
		for (size_t i = lhs.size() - 1; ~i; --i)
			if (lhs[i] != rhs[i])
				return lhs[i] < rhs[i];
		return false;
	}

	bool operator==(const Array2& lhs, const Array2& rhs) {
		if (lhs.size() != rhs.size())return false;
		for (size_t i = lhs.size() - 1; ~i; --i)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

	bool operator<=(const Array2& lhs, const Array2& rhs) {
		if (lhs.size() < rhs.size())return true;
		if (lhs.size() > rhs.size())return false;
		for (size_t i = lhs.size() - 1; ~i; --i)
			if (lhs[i] != rhs[i])
				return lhs[i] < rhs[i];
		return true;
	}

	bool operator>(const Array2& lhs, const Array2& rhs) {
		return !(lhs <= rhs);
	}

	bool operator>=(const Array2& lhs, const Array2& rhs) {
		return !(lhs < rhs);
	}

	bool operator!=(const Array2& lhs, const Array2& rhs) {
		return !(lhs == rhs);
	}

	void Array2_func::QuickMul2k(Array2& a, const uint32_t& k) {//快速乘2^k
		if (!k)return;
		if (k & 31) {
			uint32_t Size = a.size();
			a.reserve(Size + 1);
			uint32_t len1 = k & 31, len2 = 32 - len1, mo = (1 << len2) - 1;
			if (a.save_at(Size - 1) >> len2)
				a[Size] = a.save_at(Size - 1) >> len2;
			for (size_t i = Size - 1; i; --i)
				a[i] = ((a.save_at(i) & mo) << len1) + (a.save_at(i - 1) >> len2);
			a[0] = (a.save_at(0) & mo) << len1;
		}

		if (k>>5) {//32倍数
			a.vec.insert(a.vec.begin(), k >> 5, 0);
			a.Size+=k>>5;
		}
	}

	void Array2_func::QuickDivide2k(Array2& a, const int& k) {//快速除以2^k，即快速右移k位
		if (!k)return;
		size_t Length=a.size();
		if (k & 31) {
			uint32_t len1 = k & 31, len2 = 32 - len1, mo = (1 << len1) - 1;
			for (size_t i = 0; i < Length; ++i) {
				a[i] = (a[i] >> len1) + (i + 1 != Length ? ((a[i + 1] & mo) << len2) : 0);
			}

			while (Length > 1 && !a.save_at(Length - 1))
				--Length;
			a.resize(Length);
		}
		if (k >> 5) {
			if (k >> 5 >= Length) {
				a.clear();
				return ;
			}
			a.vec.erase(a.vec.begin(), a.vec.begin()+(k >> 5));
			a.Size-=k>>5;
		}
	}

	void Array2_func::SlowMul(const Array2& A, const Array2& B, Array2& c) {
		size_t n = A.size(), m = B.size();
		static std::allocator<uint64_t>test;
		uint64_t* mid = test.allocate(n + m);
		memset(mid, 0, sizeof(uint64_t) * (n + m));
		size_t Length = n + m - 1;

		for (size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < m; ++j) {
				uint64_t val = (uint64_t)A[i] * B[j] + mid[i + j];
				mid[i + j + 1] += val >> 32;
				mid[i + j] = (uint32_t)val;
			}

		if (mid[Length])
			c.resize(Length + 1), c.save_at(Length) = mid[Length];
		else c.resize(Length);
		uint32_t* it = &c[0];
		for (size_t i = 0; i < Length; ++i)
			it[i] = mid[i];
		test.deallocate(mid, n + m);
	}

	Array2 Array2_func::SlowMul(const Array2& A, const Array2& B) {
		Array2 c;
		SlowMul(A, B, c);
		return c;
	}

	/*---FFT相关,根据范围选择对应函数---*/

	void FFT_Array2_func::FFTQuickMul2(const Array2& A, const Array2& B, Array2& c) {
		size_t n=A.size()<<3, m=B.size()<<3;
		size_t len = n + m - 1;
		uint32_t bit=quicklog2(len-1)+1;
		size_t s = 1ull << bit;

		double* a = new double[s << 1];
		for (size_t i = 0; i < n; i += 8) {
			uint32_t val = A[i >> 3];
			for (int j = 0; j < 8; ++j) {
				uint32_t q = val & 15;
				val >>= 4;
				a[(i + j) << 1] = a[(i + j) << 1 | 1] = q * 0.5;
			}
		}
		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 8) {
			uint32_t val = B[i >> 3];
			for (int j = 0; j < 8; ++j) {
				uint32_t q = val & 15;
				val >>= 4;
				a[(i + j) << 1] += q * 0.5;
				a[(i + j) << 1 | 1] -= q * 0.5;
			}
		}


		cdft(s << 1, 1, a);
		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
		}
		cdft(s << 1, -1, a);
		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;
		c.resize((len >> 3) + 1);
		c[0] = 0;
		for (size_t i = 0; i < len - 7; i += 8) {
			uint64_t now = double_to_ull(a[i << 1] + 0.5)
				+ (double_to_ull(a[(i+1)<<1] + 0.5) << 4)
				+ (double_to_ull(a[(i+2)<<1] + 0.5) << 8)
				+ (double_to_ull(a[(i+3)<<1] + 0.5) << 12)
				+ (double_to_ull(a[(i+4)<<1] + 0.5) << 16)
				+ (double_to_ull(a[(i+5)<<1] + 0.5) << 20)
				+ (double_to_ull(a[(i+6)<<1] + 0.5) << 24)
				+ (double_to_ull(a[(i+7)<<1] + 0.5) << 28)
				+ c[i >> 3];
			c[i >> 3] = now & maxuint;
			c[(i >> 3) + 1] = now >> 32;
		}
		c[len >> 3] += static_cast<long long>(a[(len-7)<<1] + 0.5) +
			(static_cast<long long>(a[(len-6)<<1] + 0.5) << 4) +
			(static_cast<long long>(a[(len-5)<<1] + 0.5) << 8) +
			(static_cast<long long>(a[(len-4)<<1] + 0.5) << 12) +
			(static_cast<long long>(a[(len-3)<<1] + 0.5) << 16) +
			(static_cast<long long>(a[(len-2)<<1] + 0.5) << 20) +
			(static_cast<long long>(a[(len-1)<<1] + 0.5) << 24)
			;

		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);
		if (c.iszero())
			c.clear();
		delete[]a;
	}

	void FFT_Array2_func::FFTQuickMul3(const Array2& A, const Array2& B, Array2& c) {
		size_t n=A.size()<<2, m=B.size()<<2;
		size_t len = n + m - 1;
		uint32_t bit = quicklog2(len-1)+1;
		size_t s = 1ull<<bit;

		double* a = new double[s << 1];
		for (size_t i = 0; i < n; i += 4) {
			uint32_t val = A[i >> 2];
			uint32_t q, w, e, r;
			q = val & 255;
			val >>= 8;
			w = val & 255;
			val >>= 8;
			e = val & 255;
			val >>= 8;
			r = val & 255;
			a[i << 1] = a[i << 1 | 1] = q * 0.5;
			a[(i+1)<<1] = a[(i+1)<<1 | 1] = w * 0.5;
			a[(i+2)<<1] = a[(i+2)<<1 | 1] = e * 0.5;
			a[(i+3)<<1] = a[(i+3)<<1 | 1] = r * 0.5;
		}
		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 4) {
			uint32_t val = B[i >> 2];
			uint32_t q, w, e, r;
			q = val & 255;
			val >>= 8;
			w = val & 255;
			val >>= 8;
			e = val & 255;
			val >>= 8;
			r = val & 255;
			a[i << 1] += q * 0.5;
			a[i << 1 | 1] -= q * 0.5;
			a[(i+1)<<1] += w * 0.5;
			a[(i+1)<<1 | 1] -= w * 0.5;
			a[(i+2)<<1] += e * 0.5;
			a[(i+2)<<1 | 1] -= e * 0.5;
			a[(i+3)<<1] += r * 0.5;
			a[(i+3)<<1 | 1] -= r * 0.5;
		}
		cdft(s << 1, 1, a);
		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
		}
		cdft(s << 1, -1, a);
		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;
		c.resize((len >> 2) + 1);
		c[0] = 0;
		for (size_t i = 0; i < len - 3; i += 4) {
			uint64_t now = double_to_ull(a[i << 1] + 0.5)
				+ (double_to_ull(a[(i+1)<<1] + 0.5) << 8)
				+ (double_to_ull(a[(i+2)<<1] + 0.5) << 16)
				+ (double_to_ull(a[(i+3)<<1] + 0.5) << 24)
				+ c[i >> 2];
			c[i >> 2] = now & maxuint;
			c[(i >> 2) + 1] = now >> 32;
		}
		c[len >> 2] += static_cast<long long>(a[(len-3)<<1] + 0.5) +
			(static_cast<long long>(a[(len-2)<<1] + 0.5) << 8) +
			(static_cast<long long>(a[(len-1)<<1] + 0.5) << 16);

		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);
		if (c.iszero())
			c.clear();
		delete[]a;
	}

	void FFT_Array2_func::FFTQuickMul4(const Array2& A, const Array2& B, Array2& c) {
		size_t n = A.size() << 1, m = B.size() << 1;
		size_t len = n + m - 1;
		uint32_t bit=quicklog2(len-1)+1;
		size_t s=1ll<<bit;

		double* a = new double[s << 1];
		for (size_t i = 0; i < n; i += 2) {
			uint32_t q = A[i >> 1] >> 16, p = A[i >> 1] & 65535;
			a[i << 1] = a[i << 1 | 1] = p * 0.5;
			a[(i+1)<<1] = a[(i+1)<<1 | 1] = q * 0.5;
		}
		for (size_t i = (n << 1); i < (s << 1); ++i)
			a[i] = 0;
		for (size_t i = 0; i < m; i += 2) {
			uint32_t q = B[i >> 1] >> 16, p = B[i >> 1] & 65535;
			a[i << 1] += p * 0.5;
			a[i << 1 | 1] -= p * 0.5;
			a[(i+1)<<1] += q * 0.5;
			a[(i+1)<<1 | 1] -= q * 0.5;
		}

		cdft(s << 1, 1, a);
		for (size_t i = 0; i < s; ++i) {
			double sa = a[i << 1], sb = a[i << 1 | 1];
			a[i << 1] = sa * sa - sb * sb;
			a[i << 1 | 1] = 2 * sa * sb;
		}
		cdft(s << 1, -1, a);
		double invs = 1.0 / s;
		for (size_t i = 0; i < s; ++i)
			a[i << 1] *= invs;

		c.resize((len >> 1) + 1);
		uint32_t*arr=&c[0];
		arr[0]=0;

		for (size_t i = 0; i < len - 1; i += 2) {
			uint64_t now = double_to_ull(a[i << 1] + 0.5) + (double_to_ull(a[(i+1)<<1] + 0.5) << 16) + c[i >> 1];
			arr[i>>1] = (uint32_t)now;
			arr[(i >> 1) + 1] = now >> 32;
		}
		arr[len >> 1] += double_to_ull(a[(len-1)<<1] + 0.5);
		size_t Size = c.size();
		while (Size > 1 && !c.save_at(Size - 1))
			--Size;
		if (Size != c.size())c.resize(Size);

		delete[]a;
	}

	void FFT_Array2_func::FFTQuickMul(const Array2& A, const Array2& B, Array2& c) {
		size_t Size = A.size() + B.size();
		if (Size <= 10000)FFTQuickMul4(A, B, c);
		else if (Size <= 10000000)FFTQuickMul3(A, B, c);
		else FFTQuickMul2(A, B, c);
	}

	Array2 FFT_Array2_func::FFTQuickMul(Array2& A, Array2& B) {
		Array2 c;
		FFTQuickMul(A, B, c);
		return c;
	}
}
