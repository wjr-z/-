#include "Array.h"

_MATH_BEGIN

/*---bint_fuc部分实现---*/

void Array_func::QuickMul10k(Array<int>& a, const int& k) {//快速乘10^k
	if (!k||a.iszero())return;
	int py = k >> 3;
	int Length = a.size();
	int mul10 = _10k[k & 7], mod10 = jw / mul10;
	a.reserve(Length + py + 1);
	if (a.save_at(Length - 1) >= mod10)
		a[Length + py] = a.save_at(Length - 1) / mod10;
	for (int i = Length - 1; i; --i)
		a[i + py] = (a.save_at(i) % mod10) * mul10 +  a.save_at(i - 1) / mod10 ;
	a[py] = (a.save_at(0) % mod10) * mul10;
	for (int i = py - 1; ~i; --i)
		a[i] = 0;
}

void Array_func::QuickDivide10k(Array<int>& a, const int& k) {//快速除以10^k
	if (!k)return;
	int Length = a.size();
	int Mo = k % kz, py = k / kz, mul10 = 1;
	for (int i = 0; i < Mo; ++i)
		mul10 *= 10;
	int mod10 = jw / mul10;
	int aftsize = Length - py;
	if (aftsize <= 0) {
		a.resize(1);
		a[0] = 0;
		return;
	}
	int befLength = a.length();
	for (int i = 0; i < aftsize; ++i) {
		int j = i + py;
		a[i] = a[j] / mul10 + (j + 1 == Length ? 0 : (a[j + 1] % mul10) * mod10);
	}
	a.resize(aftsize);
	int aftLength = a.length();
	for (int i = max(0,befLength - k); i < aftLength; ++i)
		a.set(i, 0);
	while (aftsize > 1 && !a[aftsize - 1])
		--aftsize;
	a.resize(aftsize);
}
void Array_func::SlowMul(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n = A.size(), m = B.size();
	int* mid = new int[n + m];
	for (int i = n + m - 1; i >= 0; --i)
		mid[i] = 0;
	int Length = n + m - 1;
	int k,r;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			quickintmul(A[i],B[j],k,r);
			r+=mid[i+j];
			while (r >= jw) {
				r-=jw;
				++k;
			}
			if (k) {
				mid[i+j+1]+=k;
				mid[i+j]=r;
			}else mid[i+j]=r;
		}
	c.resize(0);
	if (mid[Length])
		c.resize(Length + 1), c[Length] = mid[Length];
	else c.resize(Length);
	for (int i = 0; i < Length; ++i)
		c[i] = mid[i];
	delete[]mid;
}
Array<int> Array_func::SlowMul(const Array<int>& A, const Array<int>& B) {
	Array<int> c;
	SlowMul(A, B, c);
	return c;
}


/*---FFT相关,根据范围选择对应函数---*/

void FFT_Array_func::FFTQuickMul1(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n, m;

	Array<short>mida = A.turnto1(), midb = B.turnto1();
	n = mida.size(), m = midb.size();
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;

	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;
	mida.reserve(len+1);
	mida.resize(len);
	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >= 10) {
			if (i + 1 < len)a[(i + 1) << 1] += val / 10;
			else mida[i + 1] = val / 10;
			mida[i] = val % 10;
		}
		else mida[i] = val;
	}
	c = mida.turnback1();
	delete[]a;
}

void FFT_Array_func::FFTQuickMul2(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n, m;
	Array<short>mida = A.turnto2(), midb = B.turnto2();
	n = mida.size(), m = midb.size();
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;

	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;
	mida.reserve(len+1);
	mida.resize(len);
	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >= 100) {
			if (i + 1 < len)a[(i + 1) << 1] += val / 100;
			else mida[i + 1] = val / 100;
			mida[i] = val % 100;
		}
		else mida[i] = val;
	}

	c = mida.turnback2();


	delete[]a;
}


void FFT_Array_func::FFTQuickMul4(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n, m;
	n = A.size()<<1, m = B.size()<<1;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i+=2) {
		int p=A[i>>1]%10000,q=A[i>>1]/10000;

		a[i << 1] = p * 0.5, a[i << 1 | 1] = p * 0.5;
		a[i+1<<1] = q*0.5,a[(i+1<<1)|1]=q*0.5;
	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i+=2) {
		int p=B[i>>1]%10000,q=B[i>>1]/10000;
		a[i << 1] += p * 0.5, a[i << 1 | 1] -= p * 0.5;
		a[i + 1 << 1] += q * 0.5, a[(i + 1 << 1) | 1] -= q * 0.5;
	}
	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;
	c[0]=0;
	for (int i = 0; i < len - 1; i+=2) {
		long long now = 
			static_cast <long long> (a[i << 1] + 0.5) + static_cast <long long> (a[i + 1 << 1] + 0.5) *10000+c[i>>1];
		c[i>>1]=now%100000000;
		c[(i>>1)+1]=now/100000000;
	}
	c[len >> 1] += static_cast<long long>(a[len - 1 << 1] + 0.5);

	uint Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();
	delete[]a;
}

void FFT_Array_func::FFTQuickMul(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	//保证 k*k*n <= 1e11 k 为 每个元素的最大大小
	int Length = A.length() + B.length();
	if (Length <= 1e5)FFTQuickMul4(A, B, c);
	else if (Length <= 1e9)FFTQuickMul2(A, B, c);
	else FFTQuickMul1(A, B, c);
}

Array<int> FFT_Array_func::FFTQuickMul(Array<int>& A, Array<int>& B) {
	Array<int>c;
	FFTQuickMul(A, B, c);
	return c;
}




Array2 Array2::turnto1()const {//2^32 - > 2^2
	uint Size = size(), i, head = 0;
	Array2 turnTo;
	turnTo.reserve(Size << 4);
	for (i = 0; i < Size; ++i) {
		uint val = vec[i];
		for (uint j = 0; j < 16; ++j) {
			turnTo[head++] = val & 3;
			val >>= 2;
		}
	}
	Size = head;
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

Array2 Array2::turnto2()const {//2^32 - > 2^4
	uint Size = size(), i, head = 0;
	Array2 turnTo;
	turnTo.reserve(Size << 3);
	for (i = 0; i < Size; ++i) {
		uint val = vec[i];
		for (uint j = 0; j < 8; ++j) {
			turnTo[head++] = val & 15;
			val >>= 4;
		}
	}
	Size = head;
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

Array2 Array2::turnto3()const {// 2^32 - > 2 ^ 8
	uint Size = size(), i, head = 0;
	Array2 turnTo;
	turnTo.reserve(Size << 2);
	for (i = 0; i < Size; ++i) {
		uint val = vec[i];
		for (uint j = 0; j < 4; ++j) {
			turnTo[head++] = val & 255;
			val >>= 8;
		}
	}
	Size = head;
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}

Array2 Array2::turnto4()const {//2^32 - > 2^16
	uint Size = size(), i, head = 0;
	Array2 turnTo;
	turnTo.reserve(Size << 1);
	for (i = 0; i < Size; ++i) {
		uint val = vec[i];
		for (uint j = 0; j < 2; ++j) {
			turnTo[head++] = val & 65535;
			val >>= 16;
		}
	}
	Size = head;
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}


Array2 Array2::turnback(const uint& kz) const {//2^(2^k) - > 2 ^ (2 ^ 5 )
	uint Size = size(), i = 0, head = 0, blo = 1 << 5 - kz;
	Array2 turnTo;
	turnTo.reserve((Size - 1 >> 5 - kz) + 1);
	if (Size > blo) {
		for (; i < Size - blo; i += blo) {
			for (uint j = 0; j < blo; ++j)
				turnTo[head] |= vec[i + j] << (j << kz);
			++head;
		}
	}
	uint j = i;
	for (; i < Size; ++i)
		turnTo[head] |= vec[i] << (i - j << kz);
	++head;
	Size = head;
	while (Size > 1 && !turnTo.save_at(Size - 1))
		--Size;
	if (Size != turnTo.size())turnTo.resize(Size);
	if (turnTo.iszero())
		turnTo[0] = 0;
	return turnTo;
}



bool operator<(const Array2& lhs, const Array2& rhs) {
	if (lhs.size() != rhs.size())return lhs.size() < rhs.size();
	for (int i = lhs.size() - 1; ~i; --i)
		if (lhs[i] != rhs[i])
			return lhs[i] < rhs[i];
	return false;
}

bool operator==(const Array2& lhs, const Array2& rhs) {
	if (lhs.size() != rhs.size())return false;
	for (int i = lhs.size() - 1; ~i; --i)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}
bool operator<=(const Array2& lhs, const Array2& rhs) {
	if (lhs.size() < rhs.size())return true;
	if (lhs.size() > rhs.size())return false;
	for (int i = lhs.size() - 1; ~i; --i)
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

void Array2_func::QuickMul2k(Array2& a, const uint& k) {//快速乘2^k
	if (!k)return;
	uint py = k >> 5, Size = a.size();
	a.reserve(Size + py + 1);

	if (!(k & 31)) {//32倍数
		for (int i = Size - 1; ~i; --i)
			a[i + py] = a[i];
		for (int i = py - 1; ~i; --i)
			a[i] = 0;
		return;
	}
	uint len1 = k & 31, len2 = 32 - len1, mo = (1 << len2) - 1;
	if (a.save_at(Size - 1) >> len2)
		a[Size + py] = a.save_at(Size - 1) >> len2;
	for (int i = Size - 1; i; --i)
		a[i + py] = ((a.save_at(i) & mo) << len1) + (a.save_at(i - 1) >> len2);
	a[py] = (a.save_at(0) & mo) << len1;

	for (int i = py - 1; ~i; --i)
		a[i] = 0;
}

void Array2_func::QuickDivide2k(Array2& a, const int& k) {//快速除以2^k，即快速右移k位
	if (!k)return;
	uint py = k >> 5, Size = a.size();
	uint aftsize = Size - py, befLength = a.length(), aftLength;
	if (aftsize < 0) {
		a.resize(1);
		a.save_at(0) = 0;
		return;
	}
	if (!(k & 31)) {
		for (int i = 0; i < aftsize; ++i)
			a[i] = a[i + py];
		a.resize(aftsize);
		return;
	}
	uint len1 = k & 31, len2 = 32 - len1, mo = (1 << len1) - 1;
	for (int i = 0; i < aftsize; ++i) {
		int j = i + py;
		a[i] = (a[j] >> len1) + (j + 1 != Size ? ((a[j + 1] & mo) << len2) : 0);
	}

	a.resize(aftsize);
	aftLength = a.length();
	for (int i = befLength - k; i < aftLength; ++i)
		a.set(i, 0);
	while (aftsize > 1 && !a[aftsize - 1])
		--aftsize;

	if (aftsize != a.size())a.resize(aftsize);

}


void Array2_func::SlowMul(const Array2& A, const Array2& B, Array2& c) {
	uint n = A.size(), m = B.size();
	ull* mid = new ull[n + m];
	for (int i = n + m - 1; i >= 0; --i)
		mid[i] = 0;
	uint Size = n + m - 1;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			mid[i + j] += A[i] * 1ll * B[j];
			if (mid[i + j] >> 32) {
				mid[i + j + 1] += mid[i + j] >> 32;
				mid[i + j] = mid[i + j] & maxuint;
			}
		}
	c.resize(0);
	if (mid[Size])
		c.resize(Size + 1), c[Size] = mid[Size];
	else c.resize(Size);
	for (int i = 0; i < Size; ++i)
		c[i] = mid[i];
	delete[]mid;
}

Array2 Array2_func::SlowMul(const Array2& A, const Array2& B) {
	Array2 c;
	SlowMul(A, B, c);
	return c;
}


/*---FFT相关,根据范围选择对应函数---*/


void FFT_Array2_func::FFTQuickMul1(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	Array2 mida = A.turnto1(), midb = B.turnto1();//进制转换
	n = mida.size(), m = midb.size();

	/*FFT初始化*/
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;

	/*FFT*/

	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;

	/*处理处每一位的值*/
	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >> 2) {
			if (i + 1 < len)a[(i + 1) << 1] += val >> 2;
			else mida[i + 1] = val >> 2;
			mida[i] = val & 3;
		}
		else mida[i] = val;
	}
	/*这个貌似没啥必要，因为n 位 和 m 为相乘 最多是 n + m 位 */
	int Size = mida.size();
	while (mida[Size - 1] >> 2) {
		mida[Size] = mida[Size - 1] >> 2;
		mida[Size - 1] &= 3;
	}
	c = mida.turnback(1);
	delete[]a;
}

void FFT_Array2_func::FFTQuickMul2(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	Array2 mida = A.turnto2(), midb = B.turnto2();
	n = mida.size(), m = midb.size();
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;


	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;
	mida.resize(len);
	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >> 4) {
			if (i + 1 < len)a[(i + 1) << 1] += val >> 4;
			else mida[i + 1] = val >> 4;
			mida[i] = val & 15;
		}
		else mida[i] = val;
	}
	int Size = mida.size();
	while (mida[Size - 1] >> 4) {
		mida[Size] = mida[Size - 1] >> 4;
		mida[Size - 1] &= 15;
	}
	c = mida.turnback(2);


	delete[]a;
}

void FFT_Array2_func::FFTQuickMul3(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	Array2 mida = A.turnto3(), midb = B.turnto3();
	n = mida.size(), m = midb.size();
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;

	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;

	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >> 8) {
			if (i + 1 < len)a[(i + 1) << 1] += val >> 8;
			else mida[i + 1] = val >> 8;
			mida[i] = val & 255;
		}
		else mida[i] = val;
	}
	int Size = mida.size();
	while (mida[Size - 1] >> 8) {
		mida[Size] = mida[Size - 1] >> 8;
		mida[Size - 1] &= 255;
	}
	c = mida.turnback(3);
	delete[]a;
}
void FFT_Array2_func::FFTQuickMul4(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	Array2 mida = A.turnto4(), midb = B.turnto4();
	n = mida.size(), m = midb.size();
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; ++i)
		a[i << 1] = mida[i] * 0.5, a[i << 1 | 1] = mida[i] * 0.5;
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; ++i)
		a[i << 1] += midb[i] * 0.5, a[i << 1 | 1] -= midb[i] * 0.5;

	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa * sa - sb * sb, a[i << 1 | 1] = 2 * sa * sb;
	}
	cdft(s << 1, -1, a);
	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;

	for (int i = 0; i < len; ++i) {
		long long val = (long long)(a[i << 1] + 0.5);
		if (val >> 16) {
			if (i + 1 < len)a[(i + 1) << 1] += val >> 16;
			else mida[i + 1] = val >> 16;
			mida[i] = val & 65535;
		}
		else mida[i] = val;
	}
	int Size = mida.size();
	while (mida[Size - 1] >> 16) {
		mida[Size] = mida[Size - 1] >> 16;
		mida[Size - 1] &= 65535;
	}
	c = mida.turnback(4);
	delete[]a;
}
void FFT_Array2_func::FFTQuickMul(const Array2& A, const Array2& B, Array2& c) {
	int Size = A.size() + B.size();
	//为确保精度，每一位尽量不大于1e11，之后可能会调节下阈值
	if (Size <= 1e2)FFTQuickMul4(A, B, c);//最大每一位是2^16*2^16*n 
	else if (Size <= 1e6)FFTQuickMul3(A, B, c);//最大每一位是2^8*2^8*n
	else if (Size <= 1e9)FFTQuickMul2(A, B, c);//最大每一位是2^4*2^4*n
	else FFTQuickMul1(A, B, c);//最大每一位是2^2*2^2*n
}

Array2 FFT_Array2_func::FFTQuickMul(Array2& A, Array2& B) {
	Array2 c;
	FFTQuickMul(A, B, c);
	return c;
}

_MATH_END