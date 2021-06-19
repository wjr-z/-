#include "Array.h"

_MATH_BEGIN

/*---bint_fuc部分实现---*/

void Array_func::QuickMul10k(Array<int>& a, const int& k) {//快速乘10^k
	if (!k || a.iszero())return;
	int py = k >> 3;
	int Length = a.size();
	int mul10 = _10k[k & 7], mod10 = bintjw / mul10;
	a.reserve(Length + py + 1);
	if (a.save_at(Length - 1) >= mod10)
		a[Length + py] = a.save_at(Length - 1) / mod10;
	for (int i = Length - 1; i; --i)
		a[i + py] = (a.save_at(i) % mod10) * mul10 + a.save_at(i - 1) / mod10;
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
	int mod10 = bintjw / mul10;
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
	for (int i = max(0, befLength - k); i < aftLength; ++i)
		a.set(i, 0);
	while (aftsize > 1 && !a[aftsize - 1])
		--aftsize;
	a.resize(aftsize);
}
void Array_func::SlowMul(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	const int n = A.size(), m = B.size();
	static Allocator<uint>test;
	uint*mid=test.allocate(n+m);

	memset(mid,0,sizeof(uint)*(n+m));
	int Length = n + m - 1;

	for (int i = 0; i < n; ++i) {
		ull AA=static_cast<ull>(A[i]);
		for (int j = 0; j < m; ++j) {
			ull val = AA * B[j] + mid[i+j];

			mid[i + j + 1] += val / bintjw;
			mid[i + j] = val % bintjw;
		}
	}
	if (mid[Length])
		c.resize(Length + 1), c.save_at(Length) = mid[Length];
	else c.resize(Length);
	int*it = &c[0];
	memcpy(it,mid,sizeof(int)*Length);
	test.deallocate(mid,n+m);
}
Array<int> Array_func::SlowMul(const Array<int>& A, const Array<int>& B) {
	Array<int> c;
	SlowMul(A, B, c);
	return c;
}


/*---FFT相关,根据范围选择对应函数---*/

void FFT_Array_func::FFTQuickMul1(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n, m;

	n = A.size()<<3, m = B.size()<<3;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i+=8) {
		uint val=A[i>>3];
		for (int j = 0; j < 8; ++j) {
			uint q=val%10;
			val/=10;
			a[i+j<<1]=a[i+j<<1|1]=q*0.5;
		}

	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i += 8) {
		uint val = B[i >> 3];
		for (int j = 0; j < 8; ++j) {
			uint q = val % 10;
			val /= 10;
			a[i + j << 1] +=q*0.5, a[i + j << 1 | 1] -= q * 0.5;
		}
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
	c.resize((len >> 3) + 1);
	c[0] = 0;
	for (int i = 0; i < len - 7; i += 8) {
		long long now =
			static_cast <long long> (a[i << 1] + 0.5) +
			static_cast <long long> (a[i + 1 << 1] + 0.5) * 10 +
			static_cast <long long> (a[i + 2 << 1] + 0.5) * 100 +
			static_cast <long long> (a[i + 3 << 1] + 0.5) * 1000 +
			static_cast <long long> (a[i+4<<1]+0.5)*10000+
			static_cast <long long> (a[i+5<<1]+0.5)*100000+
			static_cast <long long> (a[i+6<<1]+0.5)*1000000+
			static_cast <long long> (a[i+7<<1]+0.5)*10000000+
			c[i >> 3];
		c[i >> 3] = now % bintjw;
		c[(i >> 3) + 1] = now / bintjw;
	}
	c[len >> 3] += static_cast<long long>(a[len - 7 << 1] + 0.5) +
				   static_cast<long long>(a[len - 6 << 1] + 0.5)*10 +
				   static_cast<long long>(a[len - 5 << 1] + 0.5)*100 +
				   static_cast<long long>(a[len-4<<1]+0.5)*1000+
				   static_cast<long long>(a[len-3<<1]+0.5)*10000+
				   static_cast<long long>(a[len-2<<1]+0.5)*100000+
				   static_cast<long long>(a[len-1<<1]+0.5)*1000000;
	uint Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();
	delete[]a;
}

void FFT_Array_func::FFTQuickMul2(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	int n, m;
	n = A.size()<<2, m = B.size()<<2;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i+=4) {
		uint val=A[i>>2];
		uint q,w,e,r;
		q=val%100;
		val/=100;
		w=val%100;
		val/=100;
		e=val%100;
		val/=100;
		r=val;
		a[i<<1]=a[i<<1|1]=q*0.5;
		a[i+1<<1]=a[i+1<<1|1]=w*0.5;
		a[i+2<<1]=a[i+2<<1|1]=e*0.5;
		a[i+3<<1]=a[i+3<<1|1]=r*0.5;
	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i+=4) {
		uint val = B[i >> 2];
		uint q, w, e, r;
		q = val % 100;
		val /= 100;
		w = val % 100;
		val /= 100;
		e = val % 100;
		val /= 100;
		r = val;
		a[i << 1] +=q*0.5, a[i << 1 | 1] -= q * 0.5;
		a[i + 1 << 1] +=w*0.5, a[i + 1 << 1 | 1] -= w * 0.5;
		a[i + 2 << 1] +=e*0.5, a[i + 2 << 1 | 1] -= e * 0.5;
		a[i + 3 << 1] +=r*0.5, a[i + 3 << 1 | 1] -= r * 0.5;
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

	c.resize((len >> 2) + 1);
	c[0] = 0;
	for (int i = 0; i < len - 3; i += 4) {
		long long now =
			static_cast <long long> (a[i << 1] + 0.5) +
			static_cast <long long> (a[i + 1 << 1] + 0.5) * 100 + 
			static_cast <long long> (a[i+2<<1]+0.5)*10000 +
			static_cast <long long> (a[i+3<<1]+0.5)*1000000 +
			c[i >> 2];
		c[i >> 2] = now % bintjw;
		c[(i >> 2) + 1] = now / bintjw;
	}
	c[len >> 2] += static_cast<long long>(a[len - 3 << 1] + 0.5) +
				   static_cast<long long>(a[len-2<<1]+0.5)*100 +
				   static_cast<long long>(a[len-1<<1]+0.5)*10000;
	size_t Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();

	delete[]a;
}


void FFT_Array_func::FFTQuickMul4(const Array<int>& A, const Array<int>& B, Array<int>& c) {

	size_t n, m;
	n = A.size() << 1, m = B.size() << 1;
	size_t len = n + m - 1, s = 2;
	
	uint bit=quicklog2(len-1)+1;
	s = 1 << bit;
	double*a=new double[s<<1];
	for (int i = 0; i < n; i += 2) {
		uint q = A[i >> 1] / 10000, p = A[i >> 1] - q * 10000;
		a[i << 1] = a[i << 1 | 1] = p * 0.5;
		a[i + 1 << 1] = a[i + 1 << 1 | 1] = q * 0.5;
	}
	
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i += 2) {
		uint q = B[i >> 1] / 10000, p = B[i >> 1] - q * 10000;
		a[i << 1] += p * 0.5, a[i << 1 | 1] -= p * 0.5;
		a[i + 1 << 1] += q * 0.5, a[i + 1 << 1 | 1] -= q * 0.5;
	}
	cdft(s << 1, 1, a);
	for (int i = 0; i < s; ++i) {
		double sa = a[i << 1], sb = a[i << 1 | 1];
		a[i << 1] = sa*sa-sb*sb, a[i << 1 | 1] = 2 * sa * sb;
	}

	cdft(s << 1, -1, a);

	double invs = 1.0 / s;
	for (int i = 0; i < s; ++i)
		a[i << 1] *= invs;
	c.resize((len >> 1) + 1);
	c[0] = 0;
	for (int i = 0; i < len - 1; i += 2) {
		ull now =
			static_cast <ull> (a[i << 1] + 0.5) + static_cast <ull> (a[i + 1 << 1] + 0.5) * 10000 + c[i >> 1];
		c[i >> 1] = now % bintjw;
		c[(i >> 1) + 1] = now / bintjw;
	}
	c[len >> 1] += static_cast<ull>(a[len - 1 << 1] + 0.5);

	size_t Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();
	delete[]a;
}

void FFT_Array_func::FFTQuickMul(const Array<int>& A, const Array<int>& B, Array<int>& c) {
	//保证 k*k*n <= 1e11 k 为 每个元素的最大大小
	size_t Size = A.size() + B.size();
	if (Size <= 1e5)FFTQuickMul4(A, B, c);
	else if (Size <= 1e9)FFTQuickMul2(A, B, c);
	else FFTQuickMul1(A, B, c);
}

Array<int> FFT_Array_func::FFTQuickMul(Array<int>& A, Array<int>& B) {
	Array<int>c;
	FFTQuickMul(A, B, c);
	return c;
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
	int n = A.size(), m = B.size();
	static std::allocator<ull>test;
	ull* mid = test.allocate(n+m);
	memset(mid,0,sizeof(ull)*(n+m));
	int Length = n + m - 1;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			ull val=A[i]*1ll*B[j]+mid[i+j];
			mid[i+j+1]+=val>>32;
			mid[i+j]=val&maxuint;
		}

	if (mid[Length])
		c.resize(Length + 1), c.save_at(Length) = mid[Length];
	else c.resize(Length);
	uint* it = &c[0];
	for(int i=0;i<Length;++i)
		it[i]=mid[i];
	test.deallocate(mid,n+m);
}

Array2 Array2_func::SlowMul(const Array2& A, const Array2& B) {
	Array2 c;
	SlowMul(A, B, c);
	return c;
}


/*---FFT相关,根据范围选择对应函数---*/


void FFT_Array2_func::FFTQuickMul2(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	n = A.size()<<3, m = B.size()<<3;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i += 8) {
		uint val=A[i>>3];
		for (int j = 0; j < 8; ++j) {
			uint q=val&15;
			val>>=4;
			a[i+j<<1]=a[i+j<<1|1]=q*0.5;
		}
	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i += 8) {
		uint val=B[i>>3];
		for (int j = 0; j < 8; ++j) {
			uint q = val & 15;
			val >>= 4;
			a[i+j<<1]+=q*0.5;
			a[i+j<<1|1]-=q*0.5;
		}
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
	c.resize((len>>3)+1);
	c[0]=0;
	for (int i = 0; i < len - 7; i+=8) {
		ull now=static_cast<ull>(a[i<<1]+0.5)
			+(static_cast<ull>(a[i+1<<1]+0.5)<<4)
			+(static_cast<ull>(a[i+2<<1]+0.5)<<8)
			+(static_cast<ull>(a[i+3<<1]+0.5)<<12)
			+(static_cast<ull>(a[i+4<<1]+0.5)<<16)
			+(static_cast<ull>(a[i+5<<1]+0.5)<<20)
			+(static_cast<ull>(a[i+6<<1]+0.5)<<24)
			+(static_cast<ull>(a[i+7<<1]+0.5)<<28)
			+c[i>>3];
		c[i>>3]=now&maxuint;
		c[(i>>3)+1]=now>>32;
	}
	c[len >> 3] += static_cast<long long>(a[len - 7 << 1] + 0.5) +
		(static_cast<long long>(a[len - 6 << 1] + 0.5) << 4) +
		(static_cast<long long>(a[len - 5 << 1] + 0.5) << 8) +
		(static_cast<long long>(a[len-4<<1]+0.5)<<12) +
		(static_cast<long long>(a[len-3<<1]+0.5)<<16) + 
		(static_cast<long long>(a[len-2<<1]+0.5)<<20) +
		(static_cast<long long>(a[len-1<<1]+0.5)<<24)
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
	int n, m;
	n = A.size()<<2, m = B.size()<<2;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i += 4) {
		uint val=A[i>>2];
		uint q,w,e,r;
		q=val&255;
		val>>=8;
		w=val&255;
		val>>=8;
		e=val&255;
		val>>=8;
		r=val&255;
		a[i<<1]=a[i<<1|1]=q*0.5;
		a[i+1<<1]=a[i+1<<1|1]=w*0.5;
		a[i+2<<1]=a[i+2<<1|1]=e*0.5;
		a[i+3<<1]=a[i+3<<1|1]=r*0.5;
	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i += 4) {
		uint val = B[i>>2];
		uint q, w, e, r;
		q = val & 255;
		val >>= 8;
		w = val & 255;
		val >>= 8;
		e = val & 255;
		val >>= 8;
		r = val & 255;
		a[i<<1]+=q*0.5,a[i<<1|1]-=q*0.5;
		a[i+1<<1]+=w*0.5,a[i+1<<1|1]-=w*0.5;
		a[i+2<<1]+=e*0.5,a[i+2<<1|1]-=e*0.5;
		a[i+3<<1]+=r*0.5,a[i+3<<1|1]-=r*0.5;
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
	c.resize((len>>2)+1);
	c[0]=0;
	for (int i = 0; i < len-3; i+=4) {
		ull now=static_cast<ull>(a[i<<1]+0.5)
			+(static_cast<ull>(a[i+1<<1]+0.5)<<8)
			+(static_cast<ull>(a[i+2<<1]+0.5)<<16)
			+(static_cast<ull>(a[i+3<<1]+0.5)<<24)
			+c[i>>2];
		c[i>>2]=now&maxuint;
		c[(i>>2)+1]=now>>32;
	}
	c[len >> 2] += static_cast<long long>(a[len - 3 << 1] + 0.5) +
		(static_cast<long long>(a[len - 2 << 1] + 0.5) << 8) +
		(static_cast<long long>(a[len - 1 << 1] + 0.5) << 16);

	size_t Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();
	delete[]a;
}
void FFT_Array2_func::FFTQuickMul4(const Array2& A, const Array2& B, Array2& c) {
	int n, m;
	n = A.size()<<1, m = B.size()<<1;
	int len = n + m - 1, bit = 1, s = 2;
	bit = quicklog2(len - 1) + 1, s = 1 << bit;

	double* a = new double[s << 1];
	for (int i = 0; i < n; i += 2) {
		uint q = A[i>>1]>>16,p=A[i>>1]&65535;
		a[i<<1]=a[i<<1|1]=p*0.5;
		a[i+1<<1]=a[i+1<<1|1]=q*0.5;
	}
	for (int i = (n << 1); i < (s << 1); ++i)
		a[i] = 0;
	for (int i = 0; i < m; i += 2) {
		uint q = B[i>>1]>>16,p=B[i>>1]&65535;
		a[i<<1]+=p*0.5,a[i<<1|1]-=p*0.5;
		a[i+1<<1]+=q*0.5,a[i+1<<1|1]-=q*0.5;
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
	c.resize((len>>1)+1);
	c[0]=0;
	for (int i = 0; i < len - 1; i+=2) {
		ull now=static_cast<ull>(a[i<<1]+0.5)+(static_cast<ull>(a[i+1<<1]+0.5)<<16)+c[i>>1];
		c[i>>1]=now&maxuint;
		c[(i>>1)+1]=now>>32;
	}
	c[len >> 1] += static_cast<ull>(a[len - 1 << 1] + 0.5);
	size_t Size = c.size();
	while (Size > 1 && !c.save_at(Size - 1))
		--Size;
	if (Size != c.size())c.resize(Size);
	if (c.iszero())
		c.clear();
	delete[]a;
}
void FFT_Array2_func::FFTQuickMul(const Array2& A, const Array2& B, Array2& c) {
	int Size = A.size() + B.size();
	if(Size<=1e4)FFTQuickMul4(A,B,c);
	else if (Size <= 1e7)FFTQuickMul3(A, B, c);
	else FFTQuickMul2(A, B, c);
}

Array2 FFT_Array2_func::FFTQuickMul(Array2& A, Array2& B) {
	Array2 c;
	FFTQuickMul(A, B, c);
	return c;
}

_MATH_END