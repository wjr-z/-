#include "bint.h"

#include <assert.h>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "mtool.h"

#ifdef BINTDEBUG
using std::cout;
#endif

#ifdef QUICK
#include <unordered_map>
#endif

namespace Math {

	/*---bint类部分函数定义---*/
	//------------------------------------------------------------------//
	/*------private------*/

	void bint::assign(int val) {
		positive = true;
		if (val < 0) {
			val = -val;
			positive ^= 1;
		}
		//val最多只会占用两位
		if (val >= bintjw) {
			vec.resize(2);
			vec.save_at(1) = val / bintjw;
			vec.save_at(0) = val % bintjw;
		}
		else {
			vec.resize(1);
			vec.save_at(0) = val;
		}
	}

	void bint::assign(long long val) {
		positive = true;
		if (val < 0) {
			val = -val;
			positive ^= 1;
		}
		//val最多占用三位
		if (val >= 10000000000000000ll) {
			vec.resize(3);
			vec.save_at(0) = val % bintjw;
			val /= bintjw;
			vec.save_at(1) = val % bintjw;
			vec.save_at(2) = val / bintjw;
		}
		else if (val >= bintjw) {
			vec.resize(2);
			vec.save_at(1) = val / bintjw;
			vec.save_at(0) = val % bintjw;
		}
		else {
			vec.resize(1);
			vec.save_at(0) = val;
		}
	}

	void bint::assign(const char* s) {

		const size_t Length = strlen(s);

		assert(isrightint(s));
		clear(); //清空
		size_t i = Length - 1, head = 0, j, fir = 0;
		if (s[fir] == '-') {
			positive ^= 1;
			++fir;
		}
		while (s[fir] == '0' && fir < Length - 1)
			++fir;
		resize(((Length - fir - 1) >> 3) + 1);
		int* Vec = begin();
		while (i >= 7 + fir) {
			//从后往前每8位一搞
			for (j = i - 7; j <= i; ++j)
				Vec[head] = Vec[head] * 10 + s[j] - '0';
			++head;
			i -= 8;
			if (i + 1 == fir)return;
		}
		for (j = fir; j <= i; ++j)
			Vec[head] = Vec[head] * 10 + s[j] - '0';
		if (is_zero())positive = true;
	}

	void bint::assign(const std::string& _s) {
		assign(_s.c_str());
	}

	//--------------------------------------//

	//将绝对值相加，符号位为a的符号位
	void bint::quickadd(bint& a, const bint& b) {
		if (!a) {
			a = b;
			a.positive = true;
			return;
		}
		if (!b)return;

		const size_t n = a.size(), m = b.size();
		if (n < m) {
			//若a比b小，则优化失败，转为朴素加法
			bint c(b, a.positive);
			int* vc = c.begin();
			const int* va = a.begin();
			int jw=0;
			for (size_t i = 0; i < n; ++i) {
				vc[i] += va[i]+jw;
				jw=0;
				if (vc[i] >= bintjw) {
					vc[i] -= bintjw;
					jw=1;
				}
			}
			vc[n]+=jw;

			for (size_t tail = n; tail < m && vc[tail] >= bintjw; ++tail) {
				//答案最多是m+1位
				vc[tail] -= bintjw;
				++c.at(tail + 1); //当tail=m-1时进位则答案位数加1，且之后不会再进位
			}
			a = std::move(c);
			return;
		} //a比b大，则只用进行b的位数次加法

		int* tst = a.begin();
		const int* vb = b.begin();
		int jw=0;
		for (size_t i = 0; i < m - 1; ++i) {
			tst[i] += vb[i]+jw;
			jw=0;
			if (tst[i] >= bintjw) {
				tst[i] -= bintjw;
				jw=1;
			}
		}
		tst[m - 1] += vb[m - 1]+jw;
		if (tst[m - 1] >= bintjw) {
			tst[m - 1] -= bintjw;
			++a.at(m);
		}
		for (size_t tail = m; tail < n && (tst[tail] == bintjw); ++tail) {
			//如果n=m并且能进位，则上面就已经进位了
			tst[tail] = 0;
			++a.at(tail + 1);
		}
	}

	void bint::quickadd(bint& a, bint&& b) {
		if (!a) {
			a = std::move(b);
			a.positive = true;
			return;
		}
		if (!b)return;
		const size_t n = a.size(), m = b.size();
		if (n < m) {
			//若a比b小，则优化失败，转为朴素加法
			b.positive=a.positive;
			int jw=0;
			for (size_t i = 0; i < n; ++i) {
				b.save_at(i) += a.save_at(i)+jw;
				jw=0;
				if (b.save_at(i) >= bintjw) {
					b.save_at(i) -= bintjw;
					jw=1;
				}
			}
			b.save_at(n)+=jw;
			for (size_t tail = n; tail < m && b.save_at(tail) >= bintjw; ++tail) {
				//答案最多是m+1位
				b.save_at(tail) -= bintjw;
				++b.at(tail + 1); //当tail=m-1时进位则答案位数加1，且之后不会再进位
			}
			a = std::move(b);
			return;
		} //a比b大，则只用进行b的位数次加法
		int jw=0;
		for (size_t i = 0; i < m - 1; ++i) {
			a.save_at(i) += b.at(i)+jw;
			jw=0;
			if (a.save_at(i) >= bintjw) {
				a.save_at(i) -= bintjw;
				jw=1;
			}
		}
		a.save_at(m - 1) += b.at(m - 1)+jw;
		if (a.save_at(m - 1) >= bintjw) {
			a.save_at(m - 1) -= bintjw;
			++a.at(m);
		}
		for (size_t tail = m; tail < n && a.save_at(tail) >= bintjw; ++tail) {
			//如果n=m并且能进位，则上面就已经进位了
			a.save_at(tail) -= bintjw;
			++a.at(tail + 1);
		}
	}

	//加int绝对值
	void bint::addint(bint& a, int b) {
		uint32_t copyb(b);
		if (copyb + a.save_at(0) >= bintjw) {
			//可能会溢出，因此要用减法这样比较
			const uint32_t val = a.save_at(0) + copyb;
			a.save_at(0) = val % bintjw;
			copyb = val / bintjw;
			size_t head = 1;
			const size_t Size = a.size();
			while (head < Size && a.save_at(head) + copyb >= bintjw) {
				a.save_at(head) += copyb - bintjw;
				copyb = 1; //考虑到b最大为2e9左右，第一次最多进位10，之后最多进位1
				++head;
			}
			a.at(head) += copyb;
		}
		else a.save_at(0) += b; //不会溢出，直接加即可
	}

	//若|a|>=|b| 则为a的符号，否则取反
	void bint::quickdel(bint& a, const bint& b) {
		if (!a) {
			a = b;
			a.positive = false;
			return;
		}
		if (!b)
			return;
		//|a|<|b|
		if (a.vec < b.vec) {
			bint c(b, !a.positive);
			const size_t n = a.size();
			int* vc = c.begin();
			const int* va = a.begin();
			for (size_t i = 0, j; i < n; ++i) {
				vc[i] -= va[i];
				j = i;
				while (vc[j] < 0) {
					vc[j] += bintjw;
					--vc[j + 1];
					++j;
				}
			}

			c.maintain();
			a = std::move(c);
			return;
		}
		const size_t m = b.size();
		for (size_t i = 0, j; i < m; ++i) {
			if (a.save_at(i) < b.at(i)) {
				j = i + 1;
				a.save_at(i) += bintjw - b.at(i);
				while (!a.save_at(j)) {
					a.save_at(j) = bintjw - 1;
					++j;
				}
				--a.save_at(j);
			}
			else a.save_at(i) -= b.at(i);
		}

		a.maintain();
		if (!a)a.positive = true;
	}

	void bint::quickdel(bint& a, bint&& b) {
		if (!a) {
			a = std::move(b);
			a.positive = false;
			return;
		}
		if (!b)return;
		if (a.vec < b.vec) {
			//类似加法
			bint c(std::move(b), !a.positive);
			const size_t n = a.size();
			for (size_t i = 0, j; i < n; ++i) {
				if (c.save_at(i) < a.save_at(i)) {
					j = i + 1;
					c.save_at(i) += bintjw - a.save_at(i);
					while (!c.save_at(j)) {
						c.save_at(j) = bintjw - 1, ++j;
					}
					--c.save_at(j);
				}
				else c.save_at(i) -= a.save_at(i);
			}

			c.maintain();
			a = std::move(c);
			return;
		}
		const size_t m = b.size();
		for (size_t i = 0, j; i < m; ++i) {
			if (a.save_at(i) < b.at(i)) {
				j = i + 1;
				a.save_at(i) += bintjw - b.at(i);
				while (!a.save_at(j)) {
					a.save_at(j) = bintjw - 1;
					++j;
				}
				--a.save_at(j);
			}
			else a.save_at(i) -= b.at(i);
		}

		a.maintain();
		if (!a)a.positive = true;
	}

	void bint::delint(bint& a, int b) {
		if (a.positive ? a < b : a > -b) {
			switch (a.size()) {
				int y;
			case 1: {
				y = b - a.save_at(0);
				a.save_at(0) = y;
				break;
			}
			case 2: {
				y = b - (a.save_at(0) + a.save_at(1) * bintjw);
				if (y >= bintjw) {
					a.save_at(1) = y / bintjw;
					a.save_at(0) = y % bintjw;
				}
				else {
					a.resize(1);
					a.save_at(0) = y;
				}
				break;
			}
			default: break;
			}
			a.positive = !a.positive;
			return;
		}
		if (a.save_at(0) < b) {
			const uint32_t p = b / bintjw, q = b % bintjw;
			a.save_at(0) -= q;
			if (a.save_at(0) < 0) {
				a.save_at(0) += bintjw;
				size_t i = 1;
				while (!a.save_at(i)) {
					a.save_at(i) = bintjw - 1;
					++i;
				}
				--a.save_at(i);
			}
			a.save_at(1) -= p;
			if (a.save_at(1) < 0) {
				a.save_at(1) += bintjw;
				size_t i = 2;
				while (!a.save_at(i)) {
					a.save_at(i) = bintjw - 1;
					++i;
				}
				--a.save_at(i);
			}
			a.maintain();
		}
		else a.save_at(0) -= b;
	}

	void bint::largedivide(bint& A, bint& B) {
		const size_t  mid = 2 * B.size() - A.size() - 2;
		bint copyA(A,mid,A.size());
		bint copyB(B,mid,B.size());
		++copyA; //得到大于答案的近似解
		quickdivide(copyA,copyB);
		--copyB;
		if(copyB<0) {
			copyB.quick_mul_10k(mid<<3);
			copyB+=A;
		}
		else {
			if (!copyB) {
				copyB = std::move(A);
			}
			else {
				copyB.quick_mul_10k(mid << 3);
				memcpy(copyB.begin(), A.begin(), sizeof(int) * mid);
			}
		}
		copyB-=bint(B,0,mid) * copyA;
		if (!copyB.positive) {
			--copyA;
			copyB += B;
		}
		A=std::move(copyA);
		B=std::move(copyB);
	}

	void bint::smalldivide(bint& A, bint& B) {
		const size_t mid = (A.size() - B.size()) >> 1;
		
		bint copyA(A,mid,A.size());
		bint mo(B);
		quickdivide(copyA,mo);
		copyA.quick_mul_10k(mid<<3);
		mo.quick_mul_10k(mid<<3);
		memcpy(mo.begin(), A.begin(), sizeof(int) * mid);
		quickdivide(mo, B);
		A=std::move(copyA);
		memcpy(A.begin(),mo.begin(),sizeof(int)*mo.size());
	}
	
	void bint::knuthdivide(bint& A, bint& B) {
		bint copyA(std::move(A)), copyB(std::move(B));
		bint ans;
		
		const int bw = bintjw;
		const int a = bw >> 1, b = copyB.at(copyB.size() - 1);
		const int k = (a + b - 1) / b;

		if (b < a) {
			copyA *= k;
			copyB *= k;
		}

		const size_t n = copyB.size(), m = copyA.size() - copyB.size();
		bint r(copyA,m,n+m);
		size_t i = n + m, j = m;

		const int tst = copyB.save_at(n - 1);

		ans.resize(m + 1);
		const auto ql = ans.begin();

		do {
			int q;
			if (r.size() > n)
				q = std::min(bw - 1, static_cast<int>((r.save_at(n) * 1ll * bw + r.save_at(n - 1)) / tst));
			else if (r.size() > n - 1)
				q = std::min(bw - 1, r.save_at(n - 1) / tst);
			else q = 0;

			r -= copyB * q;

			while (!r.is_positive()) {
				--q;
				r += copyB;
			}
			if (j > 0) {
				r.quick_mul_10k(8);
				r.save_at(0) = copyA.save_at(j - 1);
			}
			ql[j] = q;
			--i;
			--j;
		}
		while (i >= n);

		size_t size = ans.size();
		while (size > 1 && !ans.save_at(size - 1))
			--size;
		ans.resize(size);
		
		A=std::move(ans);
		B=std::move(r);
		if(k!=1)B/=k;
	}

	void bint::quickdivide(bint& A, bint& B) {
		assert(!B.is_zero());
		if (A.vec < B.vec) {
			B=std::move(A);
			A=bint();
			return ;
		}
		const size_t n = A.size(), m = B.size();
		if (m == 1) {
			const bint ans=divideint(A, B.at(0)); //低精度除法
			B=A-ans*B.at(0);
			A=ans;
			return ;
		}
		if (m <= ((static_cast<uint64_t>(qlog2(n)) + 4) << 1)) {
			return knuthdivide(A, B); //m比较小直接用O(m*(n-m))算法
		}
		if (m * 5 >= n * 3) {
			return largedivide(A, B);
		}
		return smalldivide(A, B);
	}

	bint bint::divideint(const bint& A, int B) {
		if(B==1)return A;
		assert(B!=0);
		bint ans;
		size_t Size = A.size();
		const bool tmp = !(A.positive ^ (B >= 0));
		B = Math::abs(B);
		const uint64_t mask=static_cast<uint64_t>(B)-1ull;
		int mp = -1;
		if (isPowTwo(B))
			mp = _minx(B);
		if (~mp) {
			switch (Size) {
			case 1: {
				ans.save_at(0) = A.at(0) >> mp;
				ans.positive = (!ans ? true : tmp);
				break;
			}
			case 2: {
				const uint64_t val = (A.at(0) + 100000000ll * A.at(1)) >> mp;
				if (val < bintjw)
					ans.save_at(0) = val;
				else {
					ans.resize(2);
					ans.save_at(1) = val / bintjw;
					ans.save_at(0) = val % bintjw;
				}
				ans.positive = (!ans ? true : tmp);
				break;
			}
			default: {
				uint64_t copyA = 0;
				ans.resize(Size);
				for (size_t i = Size - 1; ~i; --i) {
					copyA = copyA * bintjw + A.at(i);
					ans.save_at(i) = copyA >> mp;
					copyA &= mask;
				}
				while (Size > 1 && !ans.save_at(Size - 1))
					--Size;
				ans.resize(Size);
				ans.positive = tmp;
				break;
			}
			}
		}
		else {
			switch (Size) {
			case 1: {
				ans.save_at(0) = A.at(0) / B;
				ans.positive = (!ans ? true : tmp);
				break;
			}
			case 2: {
				const uint64_t val = (A.at(0) + 100000000ll * A.at(1)) / B;
				if (val < bintjw)
					ans.save_at(0) = val;
				else {
					ans.resize(2);
					ans.save_at(1) = val / bintjw;
					ans.save_at(0) = val % bintjw;
				}
				ans.positive = (!ans ? true : tmp);
				break;
			}
			default: {
				const uint32_t copyB = B;
				uint64_t copyA = 0;
				ans.resize(Size);
				for (size_t i = Size - 1; ~i; --i) {
					copyA = copyA * bintjw + A.at(i);
					ans.save_at(i) = copyA / copyB;
					copyA %= copyB;
				}
				while (Size > 1 && !ans.save_at(Size - 1))
					--Size;
				ans.resize(Size);
				ans.positive = tmp;
				break;
			}
			}
		}
		return ans;
	}

	void bint::Karatsuba(const bint& a, const bint& b, bint& c) {
		const size_t lena = a.size(), lenb = b.size(), _max = std::max(lena, lenb), _min = std::min(lena, lenb);
		if (_max <= 32 || (_min << 1) <= _max) {
			if (!a || !b)
				return;
			Array_func::SlowMul(a.vec, b.vec, c.vec);
			return;
		}

		const size_t midlen = (_max + 1) >> 1;
		const bint B(a, 0, midlen), D(b, 0, midlen);
		Karatsuba(B, D, c);
		const bint A(a, midlen, lena), C(b, midlen, lenb);
		bint F;
		Karatsuba(A, C, F);
		bint G;
		Karatsuba(A + B, C + D, G);
		G -= c + F;
		c.resize(lena + lenb);
		const int *vf = F.begin(), *vg = G.begin();
		int* vc = c.begin();
		for (size_t i = F.size() - 1; ~i; --i)
			vc[i + (midlen << 1)] += vf[i];
		for (size_t i = G.size() - 1; ~i; --i)
			vc[i + midlen] += vg[i];
		size_t Siz = c.size();
		for (size_t i = midlen; i < Siz - 1; ++i) {
			while (vc[i] >= bintjw) {
				++vc[i + 1];
				vc[i] -= bintjw;
			}
		}
		while (c.save_at(Siz - 1) >= bintjw) {
			++c.at(Siz);
			c.save_at(Siz - 1) -= bintjw;
		}
		Siz = c.size();
		while (Siz > 1 && !c.save_at(Siz - 1))
			--Siz;
		c.resize(Siz);
	}

	void bint::TOOM_COOK_3(const bint& lhs, const bint& rhs, bint& ans) {
		const size_t lena = lhs.size(), lenb = rhs.size(), largest = std::max(lena, lenb);
		if (largest <= 128) {
			if (!lhs || !rhs)
				return;
			Array_func::SlowMul(lhs.vec, rhs.vec, ans.vec);
			ans.positive = !(lhs.positive ^ rhs.positive);

			return;
		}
		const size_t k = (largest + 2) / 3;

		const bint a0(lhs, 0, k), a1(lhs, k, k << 1), a2(lhs, k << 1, lena);
		const bint b0(rhs, 0, k), b1(rhs, k, k << 1), b2(rhs, k << 1, lenb);

		const bint U0(a0 + a2), V0(b0 + b2), U1(a0 + a2 * 4), V1(b0 + b2 * 4);
		const bint U2(a1 * 2), V2(b1 * 2);
		bint a, b, c, d, e;
		TOOM_COOK_3(a0, b0, a);
		TOOM_COOK_3(U0 + a1, V0 + b1, b);
		TOOM_COOK_3(U0 - a1, V0 - b1, c);
		TOOM_COOK_3(U1 + U2, V1 + V2, d);
		TOOM_COOK_3(U1 - U2, V1 - V2, e);

		bint w0(a), w1((8 * b - 8 * c - d + e) / 12), w2((-30 * a + 16 * b + 16 * c - d - e) / 24),
		     w3((-2 * b + 2 * c + d - e) / 12), w4((6 * a - 4 * b - 4 * c + d + e) / 24);

		ans = ((((w4.quick_mul_10k(k << 3) + w3).quick_mul_10k(k << 3) + w2).quick_mul_10k(k << 3) + w1).
			quick_mul_10k(k << 3)) + w0;

		size_t Siz = ans.size();
		while (Siz > 1 && !ans.save_at(Siz - 1))
			--Siz;
		ans.resize(Siz);
		ans.positive = !(lhs.positive ^ rhs.positive);
		if (!ans)ans.positive = true;
	}

	void bint::quickmul(bint& a, const bint& b) {
		if (!a || !b) {
			a.clear();
			return;
		}
		const size_t n = a.size(), m = b.size(), Min = std::min(n, m), Max = std::max(n, m);

		/*---根据不同范围选择不同算法---*/

		if (Min != 1) {
			(Min <= 32 || (1ull << std::min(static_cast<size_t>(60), (Min - 32) >> 1)) <= Max)
				? Array_func::SlowMul(a.vec, b.vec, a.vec)
				: FFT_Array_func::FFTQuickMul(a.vec, b.vec, a.vec);
			a.positive = !(a.positive ^ b.positive);
		}
		else {
			if (n == 1)mulint(b, a.positive ? a.save_at(0) : -a.save_at(0), a);
			else mulint(a, b.positive ? b.at(0) : -b.at(0), a);
		}
	}

	void bint::mulint(const bint& a, const int& b, bint& c) {
		
		if (!a || !b) {
			c.clear();
			return;
		}

		const size_t n = a.size();
		bool tmp = a.positive;
		uint64_t copyb;
		if (b >= 0)
			copyb = b;
		else {
			tmp ^= 1;
			copyb = -b;
		}

		int mp = -1;
		if (isPowTwo(static_cast<uint32_t>(copyb)))
			mp = _minx(static_cast<uint32_t>(copyb));

		uint64_t Val = 0;
		c.reserve(n+2);
		c.resize(n);
		const auto al = a.begin();
		const auto cl = c.begin();

		if (~mp) {
			for (size_t i = 0; i < n - 1; ++i) {
				Val += static_cast<uint64_t>(al[i]) << mp;
				cl[i] = Val % bintjw;
				Val /= bintjw;

			}
			Val += static_cast<uint64_t>(al[n-1]) << mp;

			if (Val >= bintjw) {
				cl[n-1] = Val % bintjw;
				Val /= bintjw;
				if (Val >= bintjw) {
					c.resize(n+2);
					cl[n+1]=Val/bintjw;
					cl[n] = Val % bintjw;
				}
				else {
					c.resize(n+1);
					cl[n] = Val;
				}
			}
			else cl[n-1] = Val;
		}
		else {
			for (size_t i = 0; i < n - 1; ++i) {
				Val += copyb * al[i];
				cl[i] = Val % bintjw;
				Val /= bintjw;

			}
			Val += copyb * al[n-1];

			if (Val >= bintjw) {
				cl[n-1] = Val % bintjw;
				Val /= bintjw;
				if (Val >= bintjw) {
					c.resize(n+2);
					cl[n+1] = Val / bintjw;
					cl[n] = Val % bintjw;
				}
				else {
					c.resize(n+1);
					cl[n] = Val;
				}
			}
			else cl[n-1] = Val;
		}
		c.positive = tmp;
	}

	static bint factorial(int L, int R) {
		if (R - L <= 2) {
			bint ans(L);
			for (int i = L + 1; i <= R; ++i)
				ans *= i;
			return ans;
		}
		const int mid = (L + R) >> 1;
		return factorial(L, mid) * factorial(mid + 1, R);
	}

	void bint::clear() {
		vec.clear();
		positive = true;
	}

	void bint::resize(const size_t& len) { vec.resize(len); }

	size_t bint::size() const { return vec.size(); }

	void bint::reserve(const size_t& len) { vec.reserve(len); }

	void bint::maintain() {
		size_t Size = size();
		while (Size > 1 && !save_at(Size - 1))--Size;
		if (Size != size())resize(Size);
	}

	bint::bint(const size_t& len, int) : vec(len), positive(true) {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
	}

	int bint::at(const size_t& index) const { return vec.at(index); }

	int& bint::at(const size_t& index) { return vec.at(index); }

	int& bint::save_at(const size_t& index) { return vec.save_at(index); }

	//-----------------------------------------------//
	/*------public------*/

	void bint::assign(const bint& other, const size_t& L, const size_t& R) {
		const size_t Size = other.size();
		const size_t l = std::min(L, Size), r = std::min(R, Size);
		if (l == r) {
			clear();
			return;
		}

		vec.assign(other.vec, l, r);
		size_t tail = r - l;
		while (tail > 1 && !this->save_at(tail - 1))--tail;
		if (tail != size())resize(tail);
	}

	bool bint::is_zero() const {
		return vec.is_zero();
	}

	bool bint::is_positive() const {
		return positive;
	}

	bint::bint() noexcept : positive(true) {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
	}

	bint::~bint()noexcept {
#ifdef BINTDEBUG
		cout << "bint析构函数\n";
#endif
	}

	bint::bint(const int& val) noexcept {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
		assign(val);
	}

	bint::bint(const long long& val) noexcept : positive(true) {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
		assign(val);
	}

	bint::bint(const char* s) noexcept : positive(true) {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
		assign(s);
	}

	bint::bint(const std::string& s) noexcept : positive(true) {
#ifdef BINTDEBUG
		cout << "bint构造函数\n";
#endif
		assign(s);
	}

	bint::bint(const bint& other) noexcept : vec(other.vec), positive(other.positive) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
	}

	bint::bint(bint&& other) noexcept : vec(std::move(other.vec)), positive(other.positive) {
#ifdef BINTDEBUG
		cout << "bint右值拷贝构造函数\n";
#endif
	}

	bint::bint(const bint& other, const bool& _positive) noexcept : vec(other.vec), positive(_positive) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
	}

	bint::bint(bint&& other, const bool& _positive) noexcept : vec(std::move(other.vec)), positive(_positive) {
#ifdef BINTDEBUG
		cout << "bint右值拷贝构造函数\n";
#endif
	}

	bint::bint(const bint& other, const size_t& L, const size_t& R) noexcept
		: vec(other.vec,L,R),positive(true) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
		maintain();
	}

	bint::bint(Array vec, bool positive) noexcept : vec(std::move(vec)), positive(positive) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
	}

	bint::bint(const bint2&other) noexcept{
		(*this) = other.to10bit();
	}

	bint& bint::operator=(const int& val) noexcept {
#ifdef BINTDEBUG
		cout << "bint复制函数\n";
#endif
		assign(val);
		return *this;
	}

	bint& bint::operator=(const long long& val) noexcept {
#ifdef BINTDEBUG
		cout << "bint复制函数\n";
#endif
		assign(val);
		return *this;
	}

	bint& bint::operator=(const char* s) noexcept {
#ifdef BINTDEBUG
		cout << "bint复制函数\n";
#endif
		assign(s);
		return *this;
	}

	bint& bint::operator=(const std::string& s) noexcept {
#ifdef BINTDEBUG
		cout << "bint复制函数\n";
#endif
		assign(s);
		return *this;
	}

	bint& bint::operator=(const bint& other) noexcept {
#ifdef BINTDEBUG
		cout << "bint复制函数\n";
#endif
		if(this==&other)return *this;
		vec = other.vec;
		positive = other.positive;
		return *this;
	}

	bint& bint::operator=(bint&& other) noexcept {
#ifdef BINTDEBUG
		cout << "bint右值复制\n";
#endif
		if(this==&other)return *this;
		vec = std::move(other.vec);
		positive = other.positive;
		return *this;
	}

	bint& bint::operator=(const bint2& other) noexcept {
		*this = other.to10bit();
		return *this;
	}

	size_t bint::length() const { return vec.length(); }

	void bint::relength(const size_t& index) { vec.relength(index); }

	void bint::reverse(size_t Length) {
		if (Length == 0)Length = length();
		for (size_t i = 0; (i << 1) < Length; ++i) {
			int u = vec[i], v = vec[Length - 1 - i];
			vec[i] = v;
			vec[Length-1-i] = u;
		}
	}

	int bint::operator[](const size_t& index) const {
		return vec[index];
	}

	reference bint::operator[](const size_t& index) {
		return vec[index];
	}

	const int* bint::begin() const {
		return vec.begin();
	}

	const int* bint::end() const {
		return vec.end();
	}

	int* bint::begin() {
		return vec.begin();
	}

	int* bint::end() {
		return vec.end();
	}

	void bint::rand(const bint& other) {
		size_t Size = other.size();
		resize(Size);
		bool tmp = true;
		for (size_t i = Size - 1; ~i; --i) {
			int HEAD;
			if (tmp)HEAD = randint(0, other.at(i));
			else HEAD = randint(0, bintjw - 1);
			if (HEAD != other.at(i))tmp = false;
			at(i) = HEAD;
		}
		while (Size > 1 && !save_at(Size - 1))
			--Size;
		resize(Size);
	}

	ostream& operator<<(ostream& out, const bint& x) {
		if (!x.positive)out << "-";
		out << x.at(x.size() - 1);
		for (size_t i = x.size() - 2; ~i; --i) {
			out << std::setw(8) << std::setfill('0') << x.at(i);
		}
		return out;
	}


	istream& read(char*&str,istream&in) {

		using _Ctype = istream::_Ctype;
		using _Traits = std::char_traits<char>;

		size_t Length = 2,ReadLength = 0;

		typename istream::iostate _State = istream::goodbit;
		bool _Changed = false;
		const typename istream::sentry _Ok(in);

		if (_Ok) {
			str = new char[Length + 1];

			const _Ctype& _Ctype_fac = _STD use_facet<_Ctype>(in.getloc());
			typename _Traits::int_type _Meta = in.rdbuf()->sgetc();

			while (true) {
				if (_Traits::eq_int_type(_Traits::eof(), _Meta)) {
					_State |= istream::eofbit;
					break;
				}
				char _ch = _Traits::to_char_type(_Meta);
				if(_ch>='0'&&_ch<='9')break;
				_Meta = in.rdbuf()->snextc();
			}

			for (; ; _Meta = in.rdbuf()->snextc()) {
				if (_Traits::eq_int_type(_Traits::eof(), _Meta)) {
					_State |= istream::eofbit;
					break;
				}
				else {
					char _ch = _Traits::to_char_type(_Meta);
					if (_ch<'0'||_ch>'9') {
						break;
					}
					
					if (ReadLength == Length) {
						Length += Length / 2;
						char* newstr = new char[Length + 1];
						std::memcpy(newstr, str, sizeof(char) * ReadLength);
						delete[]str;
						str = newstr;
					}
					*(str + ReadLength) = _ch;
					++ReadLength;
					_Changed = true;
				}
			}

			str[ReadLength]='\0';
		}


		if (!_Changed) {
			_State |= istream::failbit;
		}

		in.setstate(_State);
		return in;
	}

	istream& operator>>(istream& in, bint& x) {
		char *str=nullptr;
		read(str,in);
		x = str;
		delete[]str;
		return in;
	}

	bool operator<(const bint& lhs, const bint& rhs) {
		return ((lhs.is_positive() == rhs.is_positive()) && (lhs.vec < rhs.vec)) ^ (!lhs.is_positive());
	}

	bool operator<(const bint& lhs, int rhs) {
		if (lhs.positive != (rhs >= 0))return !lhs.positive;
		const bool tmp = !lhs.positive;
		rhs = abs(rhs);
		switch (lhs.size()) {
		case 1: return (lhs.at(0) < rhs) ^ tmp;
		case 2: return (lhs.at(1)<=22&&lhs.at(0) + lhs.at(1) *1ll* bintjw < rhs) ^ tmp;
		default: return tmp;
		}
	}

	bool operator<(const int& lhs, const bint& rhs) {
		return rhs < lhs;
	}

	bool operator==(const bint& lhs, const bint& rhs) {
		return ((lhs.positive == rhs.positive) && lhs.vec == rhs.vec);
	}

	bool operator==(const bint& lhs, int rhs) {
		if (lhs.positive != (rhs >= 0))return false;
		rhs = abs(rhs);
		switch (lhs.size()) {
		case 1:
			return lhs.at(0) == rhs;
		case 2:
			return lhs.at(1)<=22&&(lhs.at(0) + lhs.at(1) *1ll* bintjw) == rhs;
		default:
			return false;
		}
	}

	bool operator==(const int& lhs, const bint& rhs) {
		return rhs == lhs;
	}

	bool operator<=(const bint& lhs, const bint& rhs) {
		return !(((lhs.positive == rhs.positive) && (lhs.vec > rhs.vec)) ^ (!lhs.positive));
	}

	bool operator<=(const bint& lhs, int rhs) {
		if (lhs.positive != (rhs >= 0))return !lhs.positive;
		const bool tmp = !lhs.positive;
		rhs = abs(rhs);
		switch (lhs.size()) {
		case 1: return (lhs.at(0) <= rhs) ^ tmp;
		case 2: return (lhs.at(1)<=22 &&(lhs.at(0) + lhs.at(1) *1ll* bintjw) <= rhs) ^ tmp;
		default: return tmp;
		}
	}

	bool operator<=(const int& lhs, const bint& rhs) {
		return rhs <= lhs;
	}

	bool operator>(const bint& lhs, const bint& rhs) { return !(lhs <= rhs); }

	bool operator>(const bint& lhs, const int& rhs) { return !(lhs <= rhs); }

	bool operator>(const int& lhs, const bint& rhs) { return !(lhs <= rhs); }

	bool operator>=(const bint& lhs, const bint& rhs) { return !(lhs < rhs); }

	bool operator>=(const bint& lhs, const int& rhs) { return !(lhs < rhs); }

	bool operator>=(const int& lhs, const bint& rhs) { return !(lhs < rhs); }

	bool operator!=(const bint& lhs, const bint& rhs) { return !(lhs == rhs); }

	bool operator!=(const bint& lhs, const int& rhs) { return !(lhs == rhs); }

	bool operator!=(const int& lhs, const bint& rhs) { return !(lhs == rhs); }

	bint& bint::operator+=(const bint& b) {
		(positive == b.positive)
			? quickadd(*this, b)
			: quickdel(*this, b);
		return *this;
	}

	bint& bint::operator+=(bint&& b) {
		(positive == b.positive)
			? quickadd(*this, std::move(b))
			: quickdel(*this, std::move(b));
		return *this;
	}

	bint& bint::operator+=(const int& b) {
		const bool f = b >= 0;
		(positive == f)
			? addint(*this, Math::abs(b))
			: delint(*this, Math::abs(b));
		return *this;
	}

	bint& bint::operator-=(const bint& b) {
		(positive == b.positive)
			? quickdel(*this, b)
			: quickadd(*this, b);
		
		return *this;
	}

	bint& bint::operator-=(bint&& b) {
		(positive == b.positive)
			? quickdel(*this, std::move(b))
			: quickadd(*this, std::move(b));
		return *this;
	}

	bint& bint::operator-=(const int& b) {
		(positive == (b >= 0))
			? delint(*this, Math::abs(b))
			: addint(*this, Math::abs(b));
		return *this;
	}

	bint& bint::operator*=(const bint& b) {
		quickmul(*this, b);
		return *this;
	}

	bint& bint::operator*=(const int& b) {
		mulint(*this, b, *this);
		return *this;
	}

	bint& bint::operator/=(const bint& b) {
		return (*this) = (*this) / b;
	}

	bint& bint::operator/=(const int& b) {
		*this=divideint(*this, b);
		return *this;
	}

	bint& bint::operator%=(const bint& b) {
		if (vec < b.vec)return *this;
		(*this) -= b * ((*this) / b);
		return *this;
	}

	bint& bint::operator%=(const int& b) {
		if (vec < Math::abs(b))return *this;
		(*this) -= ((*this) / b) * b;
		return *this;
	}

	bint& bint::operator++() {
		(*this) += 1;
		return *this;
	}

	bint bint::operator++(int) {
		bint x = *this;
		++(*this);
		return x;
	}

	bint& bint::operator--() {
		(*this) -= 1;
		return *this;
	}

	bint bint::operator--(int) {
		bint x = *this;
		--(*this);
		return x;
	}

	bint& bint::operator>>=(const int& index) {
		if (index < 31) {
			(*this) /= (1 << index);
			return *this;
		}
		(*this) /= qpow(bint(2), index);
		return *this;
	}

	bint& bint::operator<<=(const int& index) {
		if (index < 31) {
			(*this) *= (1 << index);
			return *this;
		}
		(*this) *= qpow(bint(2), index);
		return *this;
	}

	bint operator>>(bint a, const int& index) {
		if (index < 31) {
			a /= (1 << index);
			return a;
		}
		a /= qpow(bint(2), index);
		return a;
	}

	bint operator<<(bint a, const int& index) {
		if (index < 31) {
			a *= (1 << index);
			return a;
		}
		a *= qpow(bint(2), index);
		return a;
	}

	bint operator+(bint X) {
		return X;
	}

	bint operator-(bint X) {
		if (!X)return bint();
		return bint(std::move(X), X.is_positive() ^ 1);
	}

	bool operator!(const bint& X) {
		return X.is_zero();
	}

	bool operator!(bint&& X) {
		return X.is_zero();
	}

	bint factorial(int n) {
		return factorial(1, n);
	}

#ifdef QUICK

	static bint quickfib(int n, std::unordered_map<int, bint>& fibmap) {
		if (fibmap.count(n))return fibmap[n];
		fibmap[n] =
		(n & 1
			 ? quickfib(n >> 1,fibmap) * quickfib(n >> 1, fibmap) 
			+ quickfib((n >> 1) + 1, fibmap) * quickfib((n >> 1) + 1, fibmap)
			 : quickfib(n >> 1, fibmap) * (quickfib((n >> 1) + 1, fibmap) 
				 + quickfib((n >> 1) - 1, fibmap)));
		return fibmap[n];
	}
#endif
	bint fibonacci(int n) {
#ifdef QUICK
		std::unordered_map<int, bint> fibmap;
		//使用额外空间
		fibmap[0] = 0;
		fibmap[1] = fibmap[2] = 1;
		fibmap[3] = 2;
		return quickfib(n, fibmap);
#else
		//使用特征方程优化
		++n;
		bint ini[2];
		ini[1] = 1;
		bint ans[2];
		ans[0] = 1;
		bint A, B, C;
		while (n) {
			if (n & 1) {
				A = ini[0] * ans[0], B = ini[1] * ans[1], C = (ini[0] + ini[1]) * (ans[0] + ans[1]);
				ans[0] = A + B;
				ans[1] = C - A;
			}
			A = ini[0] * ini[0];
			B = ini[1] * ini[1];
			C = (ini[0] + ini[1]) * (ini[0] + ini[1]);

			ini[0] = A + B;
			ini[1] = C - A;
			n >>= 1;
		}
		return ans[0];
#endif
	}

	bint operator+(const bint& a, const bint& b) {
		if (a.size() >= b.size()) {
			bint ans(a);
			ans += b;
			return ans;
		}
		bint ans(b);
		ans += a;
		return ans;
	}

	bint operator+(bint&& a, const bint& b) {
		a += b;
		return std::move(a);
	}

	bint operator+(const bint& a, bint&& b) {
		b += a;
		return std::move(b);
	}

	bint operator+(bint&& a, bint&& b) {
		a += std::move(b);
		return std::move(a);
	}

	bint operator+(bint a, const int& b) {
		a += b;
		return a;
	}

	bint operator+(const int& a, bint b) {
		b += a;
		return b;
	}

	bint operator-(const bint& a, const bint& b) {
		bint ans(a);
		ans -= b;
		return ans;
	}

	bint operator-(bint&& a, const bint& b) {
		a -= b;
		return std::move(a);
	}

	bint operator-(const bint& a, bint&& b) {
		b -= a;
		b.positive = (!b ? true : !b.positive);
		return std::move(b);
	}

	bint operator-(bint&& a, bint&& b) {
		a -= std::move(b);
		return std::move(a);
	}

	bint operator-(bint a, const int& b) {
		a -= b;
		return a;
	}

	bint operator-(const int& a, bint b) {
		b -= a;
		b.positive ^= 1;
		if (!b)b.positive = true;
		return b;
	}

	bint operator*(const bint& a, const bint& b) {
		bint ans(a);
		ans *= b;
		return ans;
	}

	bint operator*(bint&& a, const bint& b) {
		a *= b;
		return std::move(a);
	}

	bint operator*(const bint& a, bint&& b) {
		b *= a;
		return std::move(b);
	}

	bint operator*(bint&& a, bint&& b) {
		a *= b;
		return std::move(a);
	}

	bint operator*(bint a, int b) {
		bint::mulint(a, b, a);
		return a;
	}

	bint operator*(const int& a, bint b) {
		bint::mulint(b, a, b);
		return b;
	}

	bint operator/(const bint& a, const bint& b) {
		bint ans(a),mo(b);
		ans.abs(),mo.abs();
		bint::quickdivide(ans,mo);
		return bint(std::move(ans),!(a.positive^b.positive));
	}

	bint operator/(const bint& a, const int& b) {
		return bint::divideint(a, b);
	}

	bint operator/(bint&& a, const int& b) {
		a=bint::divideint(a, b);
		return std::move(a);
	}

	bint operator/(int a, const bint& b) {
		const bool tmp = !(b.positive ^ (a >= 0));
		a = abs(a);
		if (b.size() == 1 && b.at(0) <= a)
			return bint(tmp ? (a / b.at(0)) : -(a / b.at(0)));
		if (b.size() == 2 && ( b.at(0) + b.at(1) *1ll* bintjw <= a))
			return bint(tmp ? (a / (b.at(0) + b.at(1) * bintjw)) : -(a / (b.at(0) + b.at(1) * bintjw)));
		return bint();
	}

	bint operator%(const bint& a, const bint& b) {
		bint ans(a),mo(b);
		ans.abs();
		mo.abs();
		bint::quickdivide(ans,mo);
		return bint(std::move(mo),a.positive);
	}

	bint operator%(const bint& a, const int& b) {
		return a - (a / b) * b;
	}

	bint operator%(const int& a, const bint& b) {
		return a - (a / b) * b;
	}

	void divmod(bint&a,bint&b) {
		return bint::quickdivide(a,b);
	}

	bint qpow(bint a, bint b) {
		bint ans(1);
		while (!b.is_zero()) {
			if (b.save_at(0) & 1)ans *= a;
			a *= a;
			b /= 2;
		}
		return ans;
	}

	bint qpow(const bint& a, size_t b) {
		if (b == 1)return a;
		const bint mid = qpow(a, b >> 1);
		bint ans= b & 1 ? (mid * a) * mid : mid * mid;
		return ans;
	}

	bint qpow(int a, bint b) {
		return qpow(bint(a), b);
	}

	void bint::quick_mul_10() {
		const size_t Length = size();
		vec.reserve(Length + 1);
		if (vec.save_at(Length - 1) >= 10000000)
			vec.at(Length) = vec.save_at(Length - 1) / 10000000;
		for (size_t i = Length - 1; ~i; --i)
			vec.at(i) = (vec.save_at(i) % 10000000) * 10 + vec.save_at(i - 1) / 10000000;
		vec.at(0) = (vec.save_at(0) % 10000000) * 10;
	}

	bint& bint::quick_mul_10k(const size_t& k) {
		Array_func::QuickMul10k(this->vec, k);
		return *this;
	}

	bint& bint::quick_divide_10k(const size_t& k) {
		Array_func::QuickDivide10k(this->vec, k);
		if (is_zero())positive = true;
		return *this;
	}

	void bint::abs() {
		this->positive = true;
	}

	bint abs(bint x) {
		if (x.positive)return x;
		return -std::move(x);
	}

	void bint::oppsite() {
		positive ^= 1;
	}

	bint gcd(bint a, bint b) {
		int ct=0;
		while(true) {
			if(!a)return b<<ct;
			if(!b)return a<<ct;
			const bool is_a = !(a.save_at(0)&1);
			const bool is_b = !(b.save_at(0)&1);
			if(is_a&&is_b) {
				++ct;
				a/=2;
				b/=2;
			}else if(is_a) {
				a/=2;
			}else if(is_b) {
				b/=2;
			}else {
				if (a >= b) {
					a-=b;
				}
				else {
					b-=a;
				}
			}
		}
	}

	bint randbint(const bint& L, const bint& R) {
		bint X;
		X.rand(R - L);
		X += L;
		return X;
	}

	bint randbint(size_t n) {
		bint ans(0);
		size_t Size = (n - 1) >> 3;
		for (size_t i = 0; i < Size; ++i)
			ans.at(i) = randint(0, bintjw - 1);
		for (size_t i = Size << 3; i < n - 1; ++i)
			ans[i] = randint(0, 9);
		ans[n - 1] = randint(1, 9);
		return ans;
	}

	bint rand_prime(const bint& L, const bint& R) {
		bint ans(randbint(L, R));
		if (ans.at(0) % 2 == 0) {
			if (ans > L)--ans;
			else if (ans < R)++ans;
			else return bint(-1);
		}
		if (!is_prime(ans)) {
			bint ans2 = ans;
			while (ans2 > L) {
				ans2 -= 2;
				if (is_prime(ans2))return ans2;
			}
			while (ans < R) {
				ans += 2;
				if (is_prime(ans))return ans;
			}
			return bint(-1);
		}
		return ans;
	}

	bint rand_prime(size_t n) {

		static const int iniPrime=400;
		
		static const int prime[iniPrime] = { 2,3,5,711,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213,2221,2237,2239,2243,2251,2267,2269,2273,2281,2287,2293,2297,2309,2311,2333,2339,2341,2347,2351,2357,2371,2377,2381,2383,2389,2393,2399,2411,2417,2423,2437,2441,2447,2459,2467,2473,2477,2503,2521,2531,2539,2543,2549,2551,2557,2579,2591,2593,2609,2617,2621,2633,2647,2657,2659,2663,2671,2677,2683,2687,2689,2693,2699,2707,2711,2713,2719,2729,2731,2741,2749 };

		bint ans;

		int lim=std::min(static_cast<size_t>(iniPrime),n);

		while(true) {
			do {
				ans = randbint(n);
				if (!(ans.at(0) & 1))
					++ans;
			}while(ans.length()!=n);
			do {
				bool isPrime=true;
				for(int i=0;i<lim;++i) {
					if(ans % prime[i]==0) {
						isPrime=false;
						break;
					}
				}
				if(isPrime&&is_prime(ans))return ans;

				ans-=2;
			}while(ans.length()==n);
		}
	}

	void swap(bint& lhs, bint& rhs) noexcept {
		lhs.swap(rhs);
	}

	void bint::swap(bint& other) noexcept {
		std::swap(vec, other.vec);
		std::swap(positive, other.positive);
	}

	int bint::toint() const {
		switch (size()) {
		case 1: return positive ? vec.at(0) : -vec.at(0);
		case 2: return positive ? (vec.at(0) + vec.at(0) * bintjw) : -(vec.at(0) + vec.at(0) * bintjw);
		default: return 0;
		}
	}

	long long bint::toll() const {
		switch (size()) {
		case 1: return positive ? vec.at(0) : -vec.at(0);
		case 2: return positive ? (vec.at(1) * 1ll * bintjw + vec.at(0)) : -(vec.at(1) * 1ll * bintjw + vec.at(0));
		case 3: return positive
			               ? (vec.at(2) * 10000000000000000ll + vec.at(1) * 1ll * bintjw + vec.at(0))
			               : -(vec.at(2) * 10000000000000000ll + vec.at(1) * 1ll * bintjw + vec.at(0));
		default: return 0;
		}
	}

	std::string bint::tostr() const {
		std::string str;
		const size_t Size = size();
		bool flag = true;
		if (!positive) {
			str.push_back('-');
			flag = false;
		}

		int x = at(Size - 1);
		while (x) {
			str.push_back(x % 10 + '0');
			x /= 10;
		}
		std::reverse(str.begin() + (flag ? 0 : 1), str.end());

		for (size_t i = Size - 2; ~i; --i) {
			x = at(i);
			for (int j = 7; ~j; --j)
				str.push_back((x / _10k[j]) % 10 + '0');
		}
		return str;
	}

	/* 采用了分治的方法进行进制转换，否则是O(n^2)复杂度
	* 思想是对于长度较小的暴力转换
	* 对于长度较大的，取长度一半向上取整
	* 分为 l 和 r ，并分治得到 l 和 r 转换为的 bint2 ，即转化成的 2 进制
	* 然后对于 l.to2bit() 乘以 10^mid再加上 r.to2bit() 即为 当前的 to2bit()
	* 本质就是对于每一位 a_i 乘以 10 ^ i ，然后采用分治来做
	* 期望复杂度是 O( n*log^2 n )，常数带一个 0.67 ，但实际上因为我还没有预处理出 10 ^ mid
	* 导致会额外进行一倍以上的运算，且递归常数较大，所以实际常数较大
	*/

	std::unordered_map<size_t,bint2>to_2_bit_cache;
	//缓存

	bint2 get_2_bit_cache(size_t n) {
		if(to_2_bit_cache.count(n))
			return to_2_bit_cache[n];
		if(n==1)return to_2_bit_cache[1]=bintjw;
		to_2_bit_cache[n]=sqr(get_2_bit_cache(n>>1));
		if(n&1)
			to_2_bit_cache[n]*=bintjw;
		return to_2_bit_cache[n];
	}

	bint2 bint::get2bit() const {
		const size_t Size = size();
		if (Size <= 2) {
			long long G=0;
			for (size_t i = Size - 1; ~i; --i)
				G=G*bintjw + vec.at(i);
			return bint2(G);
		}
		const size_t mid = Size >> 1;
		bint l(*this,mid,Size), r(*this,0,mid);
		return l.get2bit() * get_2_bit_cache(mid) + r.get2bit();
	}

	bint2 bint::to2bit() const {
		to_2_bit_cache.clear();
		return bint2(get2bit(), positive);
	}

	//------------------------------------------------------------------//
	/*---bint2类部分函数定义---*/

	void bint2::assign(int val) {
		resize(1);
		positive = val >= 0;
		vec.save_at(0)=std::abs(val);
	}

	void bint2::assign(uint32_t val) {
		resize(1);
		positive=true;
		vec.save_at(0)=val;
	}

	void bint2::assign(long long val) {
		positive = val >= 0;
		val= std::abs(val);
		if (val <= maxuint) {
			resize(1);
			vec.save_at(0) = val;
		}
		else {
			resize(2);
			vec.save_at(1) = val >> 32;
			vec.save_at(0) = static_cast<uint32_t>(val);
		}
	}

	void bint2::assign(const char* _s) {
		bint str(_s);
		(*this)=str.to2bit();
	}

	void bint2::assign(const std::string& s) {
		assign(s.c_str());
	}

	void bint2::saveadd(size_t index) {
		//index位加1
		size_t Size = size();
		for (; index < Size && (vec.save_at(index) == maxuint); ++index)
			vec.save_at(index) = 0;
		if (index == Size)
			vec.resize(Size+1);
		++vec.save_at(index);
	}

	void bint2::savedel(size_t index) {
		//index位减1，不会出现变号
		size_t _Size = size();
		while (index < _Size && !vec.save_at(index)) {
			//不足则借位
			vec.save_at(index) = maxuint;
			++index;
		}
		--vec.save_at(index);
	}

	void bint2::quickadd(bint2& a, const bint2& b) {
		if (!b)return;
		if (!a) {
			a = b;
			return;
		}
		const size_t n = a.size(), m = b.size();
		if (n < m) {
			//若a比b小，则优化失败，转为朴素加法
			a.resize(m);
			const auto al = a.begin();
			const auto bl = b.begin();
			memcpy(al+n,bl+n,sizeof(uint32_t)*(m-n));
			uint32_t jw = 0;
			for(size_t i = 0;i<n;++i) {
				const uint32_t copy = bl[i]+jw;
				al[i] += bl[i]+jw;
				jw = !copy||(al[i] < copy);
			}
			if(jw) {
				size_t _Add=n;
				for(;_Add<m&&!(++al[_Add]);++_Add);
				if(_Add==m) {
					a.resize(m+1);
					++a.save_at(m);
				}
			}
			return;
		} //a比b大，则只用进行b的位数次加法
		const auto al = a.begin();
		const auto bl = b.begin();
		uint32_t jw=0;
		for(size_t i=0;i<m;++i) {
			uint32_t copy=bl[i]+jw;
			al[i]+=bl[i]+jw;
			jw=!copy||(al[i]<copy);
		}
		if(jw) {
			size_t _Add = m;
			for (; _Add < n && !(++al[_Add]); ++_Add);
			if (_Add == n) {
				a.resize(n + 1);
				++a.save_at(n);
			}
		}
	}

	void bint2::addint(bint2& a, uint32_t b) {
		if (b > (a.save_at(0) ^ maxuint)) //溢出
			a.saveadd(1);
		a.save_at(0) += b;
	}

	void bint2::largedivide(bint2&a,bint2&b) {
		const size_t n = a.size() , m = b.size() , mid = 2 * m - n - 2;
		bint2 copya(a,mid,n);
		bint2 copyb(b,mid,m);
		++copya;
		quickdivide(copya,copyb);
		--copyb;
		if(copyb<0) {
			copyb.quick_mul_2k(mid<<5);
			copyb+=a;
		}else {
			if(!copyb) {
				copyb=std::move(a);
			}else {
				copyb.quick_mul_2k(mid << 5);
				memcpy(copyb.begin(), a.begin(), sizeof(uint32_t) * mid);
			}
		}
		copyb -= bint2(b,0,mid)*copya;
		if(!copyb.positive) {
			--copya;
			copyb+=b;
		}
		a=std::move(copya);
		b=std::move(copyb);
	}

	void bint2::smalldivide(bint2&a,bint2&b) {
		const size_t n = a.size() , m = b.size() , mid = (n - m) >> 1;
		bint2 copya(a,mid,n);
		bint2 mo(b);
		quickdivide(copya,mo);
		copya.quick_mul_2k(mid << 5) ;
		mo.quick_mul_2k(mid << 5);
		memcpy(mo.begin(),a.begin(),sizeof(uint32_t)*mid);
		quickdivide(mo,b);
		a =std::move(copya);
		memcpy(a.begin(),mo.begin(),sizeof(uint32_t)*mo.size());
	}

	void bint2::knuthdivide(bint2&A,bint2&B) {
		bint2 copyA(std::move(A)),copyB(std::move(B));
		bint2 ans;

		const uint64_t bw = 1ull << 32;
		const uint32_t a = 1u << 31;
		const uint32_t b = copyB.save_at(copyB.size() - 1);
		const uint32_t k = ((uint64_t)a + b - 1)/b;
		if(k!=1) {
			copyA *= k;
			copyB *= k;
		}
		const size_t n = copyB.size() , m = copyA.size() - copyB.size();
		bint2 r(copyA, m ,n+m);
		size_t i = n + m, j = m;
		const uint32_t tst = copyB.save_at(n-1);
		ans.resize(m+1);
		const auto ql = ans.begin();
		do {
			uint32_t q;
			if (r.size() > n)
				q = std::min((bw - 1), (static_cast<uint64_t>(r.save_at(n))<<32|r.save_at(n - 1)) / tst);
			else if (r.size() > n - 1)
				q = std::min(static_cast<uint32_t>(bw - 1), r.save_at(n - 1) / tst);
			else q = 0;
			r -= copyB * q;
			while (!r.positive) {
				--q;
				r += copyB;
			}
			if (j > 0) {
				r.quick_mul_2k(32);
				r.save_at(0) = copyA.save_at(j - 1);
			}
			ql[j] = q;
			--i;
			--j;
		}while(~j);
		ans.maintain();
		A = std::move(ans);
		B = r / k;
	}

	void bint2::quickdivide(bint2&a, bint2&b) {
		if(a.vec < b.vec) {
			b = std::move(a);
			a = bint2();
			return ;
		}
		const size_t n = a.size() , m = b.size();
		if(m <= ((static_cast<uint64_t>(qlog2(n))+4)<<1))
			return knuthdivide(a,b);
		if(m*5>=3*n)
			return largedivide(a,b);
		return smalldivide(a,b);
	}

	void bint2::quickdel(bint2& a, const bint2& b) {
		if(!a) {
			a=b;
			a.positive=false;
			return ;
		}
		if(!b) {
			return ;
		}
		const size_t n = a.size(), m = b.size();
		if(a.vec<b.vec) {
			bint2 c(b,!a.positive);
			const auto al = a.begin();
			const auto cl = c.begin();
			for(size_t i = n-1;~i;--i) {
				if(cl[i]<al[i]) {
					c.savedel(i+1);
				}
				cl[i]-=al[i];
			}
			c.maintain();
			a=std::move(c);
			return ;
		}
		const auto al = a.begin();
		const auto bl = b.begin();
		for(size_t i = m-1;~i;--i) {
			if(al[i]<bl[i]) {
				a.savedel(i+1);
			}
			al[i]-=bl[i];
		}
		a.maintain();
	}

	void bint2::delint(bint2& a, uint32_t b) {
		if(a.save_at(0)<b)
			a.savedel(1);
		a.save_at(0)-=b;

		a.maintain();
	}

	void bint2::quickmul(bint2& a, const bint2& b) {
		if (!a || !b) {
			a.clear();
			return;
		}
		const size_t n = a.size(), m = b.size(), _min = std::min(n, m), _max = std::max(n, m);

		if ((_min <= 32) || (1ull << std::min(static_cast<size_t>(60), (_min - 32) >> 1)) <= _max)
			Array2_func::SlowMul(a.vec, b.vec, a.vec);
		else FFT_Array2_func::FFTQuickMul(a.vec, b.vec, a.vec);

		a.positive = !(a.positive ^ b.positive);
	}

	void bint2::mulint(const bint2& a, uint32_t b, bint2& c) {
		if(!a||!b) {
			c.clear();
			return ;
		}

		const size_t n = a.size();

		uint64_t copyb=b;
		uint64_t _Val = 0;
		c.reserve(n+1);
		c.resize(n);

		const auto al = a.begin();
		const auto cl = c.begin();

		for(size_t i=0;i<n;++i) {
			_Val += copyb * al[i];
			cl[i]=static_cast<uint32_t>(_Val);
			_Val>>=32;
		}

		if(_Val) {
			c.resize(n+1);
			c.save_at(n)=_Val;
		}
	}

	bint2 bint2::divideint(const bint2& a, uint32_t b) {
		bint2 ans;
		size_t _Size = a.size();

		const auto al = a.begin();
		switch(_Size) {
		case 1: {
			ans.save_at(0)=al[0]/b;
			break;
		}
		case 2: {
			const uint64_t _Val = (static_cast<uint64_t>(al[1])<<32|al[0])/b;
			if(!((_Val>>32))) {
				ans.save_at(0)=_Val;
			}else {
				ans.resize(2);
				ans.save_at(1)=_Val>>32;
				ans.save_at(0)=static_cast<uint32_t>(_Val);
			}
			break;
		}
		default: {
			uint64_t _Val = 0;
			ans.resize(_Size);
			for(size_t i = _Size-1;~i;--i) {
				_Val = _Val<<32 | al[i];
				ans.save_at(i)=_Val / b;
				_Val %= b;
			}
			ans.maintain();
			break;
		}
		}
		return ans;
	}

	void bint2::clear() {
		vec.clear();
		positive = true;
	}

	bool bint2::is_positive() const {
		return positive;
	}

	bool bint2::is_zero() const {
		return vec.iszero();
	}

	bint2::bint2() noexcept : positive(true) {

	}

	bint2::~bint2() {
		
	}

	bint2::bint2(const int& val) noexcept : positive(true) {
		assign(val);
	}

	bint2::bint2(uint32_t val) noexcept :positive(true){
		assign(val);
	}


	bint2::bint2(const long long& val) noexcept : positive(true) {
		assign(val);
	}

	bint2::bint2(const char* s) noexcept : positive(true) {
		assign(s);
	}

	bint2::bint2(const std::string& s) noexcept : positive(true) {
		assign(s);
	}

	bint2::bint2(const bint2& other) noexcept : vec(other.vec), positive(other.positive) {

	}

	bint2::bint2(bint2&& other) noexcept : vec(std::move(other.vec)), positive(other.positive) {

	}

	bint2::bint2(const bint2& other, const bool& _positive) noexcept : vec(other.vec), positive(_positive) {

	}

	bint2::bint2(bint2&& other, bool _positive) noexcept : vec(std::move(other.vec)), positive(_positive) {

	}

	bint2::bint2(Array2 vec, bool positive) noexcept : vec(std::move(vec)), positive(positive) {

	}

	bint2::bint2(const bint2& other, size_t L, size_t R) noexcept:vec(R>L?R-L:1),positive(true) {
		assign(other,L,R);
	}

	bint2::bint2(const bint&other) noexcept {
		(*this)=other.to2bit();
	}

	bint2::bint2(size_t n, int) noexcept
		:vec(((n - 1) >> 5) + 1) {

	}

	bint2& bint2::operator=(const int& val) noexcept {
		assign(val);
		return *this;
	}

	bint2& bint2::operator=(uint32_t val) noexcept {
		assign(val);
		return *this;
	}


	bint2& bint2::operator=(const long long& val) noexcept {
		assign(val);
		return *this;
	}

	bint2& bint2::operator=(const bint2& other) noexcept {
		if (this == &other)return *this;
		vec = other.vec;
		positive = other.positive;
		return *this;
	}

	bint2& bint2::operator=(bint2&& other) noexcept {
		if(this==&other)return *this;
		vec = std::move(other.vec);
		positive = other.positive;
		return *this;
	}

	bint2& bint2::operator=(const char* s) noexcept {
		assign(s);
		return *this;
	}

	bint2& bint2::operator=(const std::string& s) noexcept {
		assign(s);
		return *this;
	}

	size_t bint2::length() const { return vec.length(); }

	void bint2::relength(size_t index) { vec.relength(index); }

	size_t bint2::size() const { return vec.size(); }

	void bint2::resize(size_t len) { vec.resize(len); }

	void bint2::reserve(const uint32_t& len) { vec.reserve(len); }

	bint2& bint2::operator=(const bint& other) noexcept {
		*this = other.to2bit();
		return *this;
	}

	uint32_t bint2::at(size_t index) const { return vec.at(index); }

	uint32_t& bint2::at(size_t index) { return vec.at(index); }

	uint32_t& bint2::save_at(size_t index) { return vec.save_at(index); }

	bool bint2::operator[](const size_t& index) const { return vec[index]; }

	reference2 bint2::operator[](const size_t& index) { return vec[index]; }

	const uint32_t* bint2::begin() const {
		return vec.begin();
	}

	const uint32_t* bint2::end() const {
		return vec.end();
	}

	uint32_t* bint2::begin() {
		return vec.begin();
	}

	uint32_t* bint2::end() {
		return vec.end();
	}

	void bint2::maintain() {
		vec.maintain();
		if(is_zero())
			positive=true;
	}

	void bint2::assign(const bint2& other, size_t L, size_t R) {
		const size_t Size = other.size();
		const size_t l = std::min(L, Size), r = std::min(R, Size);
		if(l==r) {
			clear();
			return ;
		}
		vec.assign(other.vec,l,r);
		maintain();
	}

	void bint2::assign2bit(const char* s) {
		clear();
		int Length = strlen(s);
		int i = Length - 1, head = 0, j = 0;
		int fir = 0;
		if(s[fir]=='-')
			positive=false,++fir;

		while (s[fir] == '0' && fir < Length - 1)
			++fir;
		resize(((Length-fir-1)>>5)+1);
		const auto Vec = vec.begin();
		while (i >= 31 + fir) {
			for (j = i - 31; j <= i; ++j)
				Vec[head] = (Vec[head] << 1) + (s[j] - '0');
			++head;
			i -= 32;
		}
		for (j = fir; j <= i; ++j)
			Vec[head] = (Vec[head] << 1) + (s[j] - '0');
		if (is_zero())positive = true;
	}

	void bint2::assign2bit(const std::string& str) {
		assign2bit(str.c_str());
	}

	void bint2::rand(const bint2& other) {
		size_t Size = other.size();
		resize(Size);
		bool tmp = true;
		for (size_t i = Size - 1; ~i; --i) {
			uint32_t HEAD;
			if(tmp&&other.at(i)!=maxuint) {
				HEAD = randuint() % (other.at(i) + 1);
			}else HEAD = randuint();
			if (HEAD != other.at(i))tmp = false;
			at(i) = HEAD;
		}
		while (Size > 1 && !save_at(Size - 1))
			--Size;
		resize(Size);
	}

	bool operator<(const bint2& lhs, const bint2& rhs) {
		return ((lhs.positive == rhs.positive) && (lhs.vec < rhs.vec)) ^ (!lhs.positive);
	}

	bool operator<(const bint2&lhs,uint32_t rhs) {
		if(!lhs.positive)return true;
		return (lhs.size()==1&&lhs.at(0)<rhs);
	}

	bool operator<(const bint2& lhs, const int& rhs) {
		if(!lhs.positive&&(rhs>=0))return true;
		if(lhs.positive&&(rhs<0))return false;
		return (lhs.size()==1&&lhs.at(0)<rhs)^(!lhs.positive);
	}

	bool operator<(uint32_t lhs,const bint2&rhs) {
		if(!rhs.positive)return false;
		return (rhs.size()!=1||lhs<rhs.at(0));
	}

	bool operator<(const int& lhs, const bint2& rhs) {
		if((lhs<0)&&rhs.positive)return true;
		if((lhs>=0)&&!rhs.positive)return false;
		return (rhs.size()!=1||lhs<rhs.at(0))^(!rhs.positive);
	}

	bool operator==(const bint2& lhs, const bint2& rhs) {
		return ((lhs.positive == rhs.positive) && lhs.vec == rhs.vec);
	}

	bool operator==(const bint2&lhs,uint32_t rhs) {
		if(!lhs.positive)return false;
		return lhs.size()==1&&lhs.at(0)==rhs;
	}
	
	bool operator==(const bint2& lhs, const int& rhs) {
		if (lhs.positive != (rhs >= 0))return false;
		return (lhs.size()==1&&lhs.at(0)==static_cast<uint32_t>(rhs));
	}

	bool operator==(uint32_t lhs,const bint2&rhs) {
		return rhs==lhs;
	}

	bool operator==(const int& lhs, const bint2& rhs) {
		return rhs==lhs;
	}

	bool operator<=(const bint2& lhs, const bint2& rhs) {
		return !(((lhs.positive == rhs.positive) && (lhs.vec > rhs.vec)) ^ (!lhs.positive));
	}

	bool operator<=(const bint2&lhs,uint32_t rhs) {
		if(!lhs.positive)return true;
		return lhs.size()==1&&lhs.at(0)<=rhs;
	}

	bool operator<=(const bint2& lhs, const int& rhs) {
		if(lhs.positive) {
			if(rhs<0)return false;
			return lhs.size()==1&&lhs.at(0)<=static_cast<uint32_t>(rhs);
		}
		if(rhs>=0)return true;
		return lhs.size()!=1||lhs.at(0)>=static_cast<uint32_t>(rhs);
	}

	bool operator<=(uint32_t lhs,const bint2&rhs) {
		if(rhs.positive) {
			return rhs.size()!=1||lhs<=rhs.at(0);
		}
		return false;
	}

	bool operator<=(const int& lhs, const bint2& rhs) {
		if(rhs.positive) {
			if(lhs<0)return true;
			return rhs.size()!=1||static_cast<uint32_t>(lhs)<=rhs.at(0);
		}
		if(lhs>=0)return false;
		return rhs.size()==1&&static_cast<uint32_t>(lhs)>=rhs.at(0);
	}

	bool operator>(const bint2& lhs, const bint2& rhs) {
		return !(lhs <= rhs);
	}

	bool operator>(const bint2&lhs,uint32_t rhs) {
		return !(lhs<=rhs);
	}

	bool operator>(const bint2& lhs, const int& rhs) {
		return !(lhs<=rhs);
	}

	bool operator>(uint32_t lhs,const bint2&rhs) {
		return !(lhs<=rhs);
	}

	bool operator>(const int& lhs, const bint2& rhs) {
		return !(lhs<=rhs);
	}

	bool operator>=(const bint2& lhs, const bint2& rhs) {
		return !(lhs < rhs);
	}

	bool operator>=(const bint2&lhs,uint32_t rhs) {
		return !(lhs<rhs);
	}
	
	bool operator>=(const bint2& lhs, const int& rhs) {
		return !(lhs<rhs);
	}

	bool operator>=(uint32_t lhs,const bint2&rhs) {
		return !(lhs<rhs);
	}

	bool operator>=(const int& lhs, const bint2& rhs) {
		return !(lhs < rhs);
	}

	bool operator!=(const bint2& lhs, const bint2& rhs) {
		return !(lhs == rhs);
	}

	bool operator!=(const bint2&lhs,uint32_t rhs) {
		return !(lhs==rhs);
	}
	
	bool operator!=(const bint2& lhs, const int& rhs) {
		return !(lhs==rhs);
	}

	bool operator!=(uint32_t lhs,const bint2&rhs) {
		return !(lhs==rhs);
	}

	bool operator!=(const int& lhs, const bint2& rhs) { 
		return !(lhs==rhs);
	}

	ostream& operator<<(ostream& out, const bint2& x) {
		out<<x.to10bit();
		return out;
	}

	istream& operator>>(istream& in, bint2& x) {
		bint G;
		in>>G;
		x = G.to2bit();
		return in;
	}

	bint2& bint2::operator+=(const bint2& b) {
		(positive==b.positive)?
			quickadd(*this,b):
			quickdel(*this,b);
		return *this;
	}

	bint2& bint2::operator+=(uint32_t b) {
		positive?addint(*this,b):delint(*this,b);
		return *this;
	}


	bint2& bint2::operator+=(const int& b) {
		(positive == (b>=0))
			? (positive
				   ? addint(*this, b)
				   : addint(*this, -b))
			: (positive
				   ? delint(*this, -b)
				   : delint(*this, b));
		return *this;
	}

	bint2& bint2::operator-=(const bint2& b) {
		(positive==b.positive)?
			quickdel(*this,b):
			quickadd(*this,b);
		return *this;
	}

	bint2& bint2::operator-=(uint32_t b) {
		positive?delint(*this,b):addint(*this,b);
		return *this;
	}

	bint2& bint2::operator-=(const int& b) {
		(positive == (b>=0))
			? (positive
				   ? delint(*this, b)
				   : delint(*this, -b))
			: (positive
				   ? addint(*this, -b)
				   : addint(*this, b));
		return *this;
	}

	bint2& bint2::operator*=(const bint2& b) {
		quickmul(*this, b);
		return *this;
	}

	bint2& bint2::operator*=(uint32_t b) {
		mulint(*this,b,*this);
		return *this;
	}


	bint2& bint2::operator*=(int b) {
		mulint(*this,std::abs(b),*this);
		positive ^= (b < 0);
		if(is_zero())positive=true;
		return *this;
	}

	bint2& bint2::operator/=(const bint2& b) {
		return (*this) = (*this) / b;
	}

	bint2& bint2::operator/=(uint32_t b) {
		*this=divideint(*this,b);
		return *this;
	}


	bint2& bint2::operator/=(int b) {
		*this=divideint(*this,std::abs(b));
		positive^=(b<0);
		if(is_zero())positive=true;
		return *this;
	}

	bint2& bint2::operator%=(const bint2& b) {
		(*this)=(*this)-(*this/b)*b;
		return*this;
	}

	bint2& bint2::operator%=(uint32_t b) {
		(*this)=(*this)-(*this/b)*b;
		return *this;
	}


	bint2& bint2::operator%=(int b) {
		(*this)=(*this)-(*this/b)*b;
		return*this;
	}

	bint2& bint2::operator++() {
		if(positive) {
			saveadd(0);
		}else {
			savedel(0);
		}
		
		return *this;
	}

	bint2 bint2::operator++(int) {
		bint2 x(*this);
		++(*this);
		return x;
	}

	bint2& bint2::operator--() {
		if(is_zero()) {
			positive=false;
		}
		if(positive) {
			savedel(0);
		}else saveadd(0);
		return *this;
	}

	bint2 bint2::operator--(int) {
		bint2 x(*this);
		--(*this);
		return x;
	}

	bint2 operator+(bint2 X) {
		return X;
	}

	bint2 operator-(bint2 X) {
		if (!X)return bint2();
		return bint2(std::move(X), X.positive ^ 1);
	}

	bool operator!(const bint2& X) {
		return X.is_zero();
	}

	bool operator!(bint2&& X) {
		return X.is_zero();
	}

	bint2 operator+(const bint2& a, const bint2& b) {
		bint2 ans(a);
		ans += b;
		return ans;
	}

	bint2 operator+(bint2&& a, const bint2& b) {
		a += b;
		return std::move(a);
	}

	bint2 operator+(const bint2& a, bint2&& b) {
		b += a;
		return std::move(b);
	}

	bint2 operator+(bint2&& a, bint2&& b) {
		a += b;
		return std::move(a);
	}

	bint2 operator+(bint2 a,uint32_t b) {
		a += b;
		return a;
	}

	bint2 operator+(bint2 a, const int& b) {
		a += b;
		return a;
	}

	bint2 operator+(uint32_t a,bint2 b) {
		b+=a;
		return b;
	}

	bint2 operator+(const int& a, bint2 b) {
		b += a;
		return b;
	}

	bint2 operator-(const bint2& a, const bint2& b) {
		bint2 ans(a);
		ans -= b;
		return ans;
	}

	bint2 operator-(bint2&& a, const bint2& b) {
		a -= b;
		return std::move(a);
	}

	bint2 operator-(const bint2& a, bint2&& b) {
		b -= a;
		b.positive ^= 1;
		if (b.is_zero())b.positive = true;
		return std::move(b);
	}

	bint2 operator-(bint2&& a, bint2&& b) {
		a -= b;
		return std::move(a);
	}

	bint2 operator-(bint2 a,uint32_t b) {
		a -= b;
		return a;
	}

	bint2 operator-(bint2 a, const int& b) {
		a -= b;
		return a;
	}

	bint2 operator-(uint32_t a,bint2 b) {
		b-=a;
		b.positive^=1;
		if(!b)b.positive=true;
		return b;
	}

	bint2 operator-(const int& a, bint2 b) {
		b -= a;
		b.positive ^= 1;
		if (b.is_zero())b.positive = true;
		return b;
	}

	bint2 operator*(const bint2& a, const bint2& b) {
		bint2 ans(a);
		ans *= b;
		return ans;
	}

	bint2 operator*(bint2&& a, const bint2& b) {
		a *= b;
		return std::move(a);
	}

	bint2 operator*(const bint2& a, bint2&& b) {
		b *= a;
		return std::move(b);
	}

	bint2 operator*(bint2&& a, bint2&& b) {
		a *= b;
		return std::move(a);
	}

	bint2 operator*(bint2 a,uint32_t b) {
		a*=b;
		return a;
	}

	bint2 operator*(bint2 a, const int& b) {
		a *= std::abs(b);
		a.positive=a.is_zero()?true:(a.positive^(b<0));
		return a;
	}

	bint2 operator*(uint32_t a,bint2 b) {
		b *= a;
		return b;
	}

	bint2 operator*(const int& a, bint2 b) {
		b *= std::abs(a);
		b.positive=!b?true:(b.positive^(a<0)); 
		return b;
	}

	bint2 operator/(const bint2& a, const bint2& b) {
		bint2 ans(a),mo(b);
		ans.abs(),mo.abs();
		bint2::quickdivide(ans,mo);
		ans.positive=!(a.positive^b.positive);
		if(!ans)ans.positive=true;
		return ans;
	}

	bint2 operator/(const bint2&a,uint32_t b) {
		bint2 ans(a);
		ans /= b;
		return ans;
	}

	bint2 operator/(const bint2& a, const int& b) {
		bint2 ans(a);
		ans /= b;
		return ans;
	}

	bint2 operator/(uint32_t a,const bint2&b) {
		if(b.size()!=1)return bint2();
		bint2 ans(a/b.at(0));
		ans.positive = b.positive;
		if(!ans)ans.positive=true;
		return ans;
	}

	bint2 operator/(const int& a, const bint2& b) {
		if(b.size()!=1)
			return bint2();
		bint2 ans(static_cast<uint32_t>(a) / b.at(0));
		ans.positive=b.positive^(b<0);
		if(!ans)ans.positive=true;
		return ans;
	}

	bint2 operator%(const bint2& a, const bint2& b) {
		bint2 ans(a),mo(b);
		ans.abs(),mo.abs();
		bint2::quickdivide(ans,mo);
		return bint2(mo,a.positive);
	}

	bint2 operator%(const bint2&a,uint32_t b) {
		return a - (a / b) * b;
	}

	bint2 operator%(const bint2& a, const int& b) {
		return a - (a / b) * b;
	}

	bint2 operator%(uint32_t a, const bint2&b) {
		return a - (a / b) * b;
	}

	bint2 operator%(const int& a, const bint2& b) {
		return a - (a / b) * b;
	}

	bint2 operator<<(const bint2& x, int kz) {
		bint2 ans(x);
		ans.quick_mul_2k(kz);
		return ans;
	}

	bint2 operator>>(const bint2& x, int kz) {
		bint2 ans(x);
		ans.quick_divide_2k(kz);
		return ans;
	}

	bint2 operator&(const bint2& lhs, const bint2& rhs) {
		const size_t LhsSize = lhs.size(), RhsSize = rhs.size();
		const uint32_t *_Lhs = lhs.begin(), *_Rhs = rhs.begin();
		if (LhsSize < RhsSize) {
			bint2 ans(lhs);
			uint32_t* _Ans = ans.begin();
			for (size_t i = 0; i < LhsSize; ++i)
				_Ans[i] &= _Rhs[i];
			return ans;
		}
		bint2 ans(rhs);
		uint32_t* _Ans = ans.begin();
		for (size_t i = 0; i < RhsSize; ++i)
			_Ans[i] &= _Lhs[i];
		return ans;
	}

	bint2 operator|(const bint2& lhs, const bint2& rhs) {
		const size_t LhsSize = lhs.size(), RhsSize = rhs.size();
		const uint32_t *_Lhs = lhs.begin(), *_Rhs = rhs.begin();
		if (LhsSize < RhsSize) {
			bint2 ans(rhs);
			uint32_t* _Ans = ans.begin();
			for (size_t i = 0; i < LhsSize; ++i)
				_Ans[i] |= _Lhs[i];
			return ans;
		}
		bint2 ans(lhs);
		uint32_t* _Ans = ans.begin();
		for (size_t i = 0; i < RhsSize; ++i)
			_Ans[i] |= _Rhs[i];
		return ans;
	}

	bint2 operator^(const bint2& lhs, const bint2& rhs) {
		const size_t LhsSize = lhs.size(), RhsSize = rhs.size();
		const uint32_t *_Lhs = lhs.begin(), *_Rhs = rhs.begin();
		if (LhsSize < RhsSize) {
			bint2 ans(rhs);
			uint32_t* Ans = ans.begin();
			for (size_t i = 0; i < LhsSize; ++i)
				Ans[i] ^= _Lhs[i];
			size_t Size = ans.size();
			while (Size > 1 && !ans.save_at(Size - 1))
				--Size;
			return ans;
		}
		bint2 ans(lhs);
		uint32_t* _Ans = ans.begin();
		for (size_t i = 0; i < RhsSize; ++i)
			_Ans[i] ^= _Rhs[i];
		size_t Size = ans.size();
		while (Size > 1 && !ans.save_at(Size - 1))
			--Size;
		return ans;
	}

	bint2& bint2::operator<<=(int kz) {
		quick_mul_2k(kz);
		return *this;
	}

	bint2& bint2::operator>>=(int kz) {
		quick_divide_2k(kz);
		return *this;
	}

	bint2& bint2::operator&=(const bint2& other) {
		const size_t MaxSize = std::max(size(), other.size());
		resize(MaxSize);
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MaxSize; ++i)
			q[i] &= r[i];
		return *this;
	}

	bint2& bint2::operator|=(const bint2& other) {
		size_t MaxSize = std::max(size(), other.size());
		resize(MaxSize);
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MaxSize; ++i)
			q[i] |= r[i];
		return *this;
	}

	bint2& bint2::operator^=(const bint2& other) {
		size_t MinSize = std::min(size(), other.size());
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MinSize; ++i)
			q[i] ^= r[i];
		return *this;
	}

	size_t bint2::count() {
		const uint32_t* Vec = begin();
		const size_t Size = size();
		size_t count = 0;
		for (size_t i = 0; i < Size; ++i)
			count += findNumber(Vec[i]);
		return count;
	}

	bint2 qpow(const bint2& a, size_t b) {
		if (b == 1)return a;
		const bint2 mid = qpow(a, b >> 1);
		return b & 1 ? (mid * a) * mid : mid * mid;
	}

	void bint2::quick_mul_2() {
		Array2_func::QuickMul2k(vec);
	}

	bint2& bint2::quick_mul_2k(size_t k) {
		Array2_func::QuickMul2k(vec, k);
		return *this;
	}

	bint2& bint2::quick_divide_2k(size_t k) {
		Array2_func::QuickDivide2k(vec, k);
		if (is_zero())positive = true;
		return *this;
	}

	void bint2::abs() {
		positive = true;
	}

	bint2 abs(bint2 x) {
		if (x.positive)return x;
		return -std::move(x);
	}

	bint2 randbint2(size_t n) {
		bint2 ans(0);
		const size_t Size = (n - 1) >> 5;
		for (size_t i = 0; i < Size; ++i)
			ans.at(i) = randuint();
		for (size_t i = Size << 5; i < n - 1; ++i)
			ans[i] = randuint()&1;
		ans[n - 1] = true;
		return ans;
	}

	bint2 gcd(bint2 a,bint2 b) {
		size_t ct=0;
		while(true) {
			if(!a)return b<<ct;
			if(!b)return a<<ct;
			const int _mina = _minx(a.save_at(0));
			const int _minb = _minx(b.save_at(0));
			const int _min = std::min(_mina,_minb);
			if(_min) {
				ct += _min;
				a>>=_min;
				b>>=_min;
			}else if(_mina) {
				a>>=_mina;
			}else if(_minb) {
				b>>=_minb;
			}else {
				if(a>=b)a-=b;
				else b-=a;
			}
		}
		
	}

	void bint2::oppsite() {
		positive ^= 1;
	}

	uint32_t bint2::to_uint() const {
		return at(0);
	}

	uint64_t bint2::to_ull() const {
		if(size()==1)
			return at(0);
		return (static_cast<uint64_t>(at(1))<<32)|(at(0));
	}


	std::string bint2::tostr() const {
		std::string str;
		const size_t Size = size();
		uint32_t x = (*this).at(Size - 1);
		while (x) {
			str.push_back((x & 1) + '0');
			x >>= 1;
		}
		std::reverse(str.begin(), str.end());
		if (!positive)
			str = "-" + str;
		for (size_t i = Size - 2; ~i; --i) {
			x = at(i);
			for (int j = 31; ~j; --j)
				str.push_back('0' + ((x >> j) & 1));
		}
		return str;
	}

	/* 和 to2bit 类似的思想*/
	std::unordered_map<size_t,bint>to_10_bit_cache;
	//缓存
	bint get_10_bit_cache(size_t n) {
		if(to_10_bit_cache.count(n))return to_10_bit_cache[n];
		if(n==1)return to_10_bit_cache[1]=bint(1ll<<32);
		to_10_bit_cache[n]=sqr(get_10_bit_cache(n>>1));
		if(n&1)
			to_10_bit_cache[n]*=to_10_bit_cache[1];
		return to_10_bit_cache[n];
	}
	bint bint2::get10bit() const {
		const size_t Size = size();
		if (Size == 1) {
			return bint((long long)vec.at(0));
		}
		const size_t mid = (Size + 1) >> 1;
		const bint2 l(*this,mid,Size), r(*this,0,mid);
		return l.get10bit() * get_10_bit_cache(mid) + r.get10bit();
	}

	bint bint2::to10bit() const {
		to_10_bit_cache.clear();
		return bint(get10bit(), positive);
	}

	bool operator==(const bint& lhs, const bint2& rhs) {
		return lhs == rhs.to10bit();
	}

	bool operator==(const bint2& lhs, const bint& rhs) {
		return lhs.to10bit() == rhs;
	}

	bint sqrt(const bint& A) {
		bint ans[2];
		int now = 1;
		ans[0] = -1;
		ans[1]=1;
		ans[1].quick_mul_10k(A.length()>>1);
		while (abs(ans[0] - ans[1]) > 1) {
			now ^= 1;
			ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) / 2;
		}
		if (ans[now] * ans[now] > A)
			--ans[now];
		return ans[now];
	}

	bint2 sqrt(const bint2& A) {
		bint2 ans[2];
		int now = 1;
		ans[0] = -1;
		ans[1] = 1;
		ans[1] <<= (A.length()>>1);
		while (abs(ans[0] - ans[1]) > 1) {
			now ^= 1;
			ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) / 2;
		}
		if (ans[now] * ans[now] > A)
			--ans[now];
		return ans[now];
	}

	bint modpow(bint a,bint b,const bint&mod) {
		const auto k=mod.size();
		bint mu(1);
		mu.quick_mul_10k(k<<4);
		mu/=mod;
		return modpow(std::move(a),std::move(b),mod,mu);
	}

	bint _Quick_Mod(const bint&x,const bint&mod,const bint&mu) {
		if(x<mod)return x;
		const size_t k = mod.size();
		bint r2(bint(x,k-1,x.size()) * mu);
		r2.quick_divide_10k((k + 1) << 3);
		r2 *= mod;
		if (r2.size() > k + 1) {
			r2.resize(k + 1);
			r2.maintain();
		}
		bint r(bint(x,0,k+1) - r2);
		if (r >= mod)
			r -= mod;
		if(r<0) {
			bint G(1);
			G.quick_mul_10k(k+1);
			r+=G;
		}
		return r;
	}
	bint modpow(bint a,bint b, const bint& mod,const bint&mu) {
		const size_t k=mod.size();
		bint ans(1);
		
		while(!b.is_zero()) {
			if (b.at(0) & 1) {
				ans = _Quick_Mod(ans * a,mod,mu);
			}
			a=_Quick_Mod(a*a,mod,mu);
			b/=2;
		}
		return ans;
	}

	bool witness(const bint& n, const bint& S, bint seed, const bint& d, int r,const bint&mu) {
		seed = modpow(seed, d, n,mu);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == S)return true;
			seed=_Quick_Mod(seed*seed,n,mu);
			if (seed == 1)return false;
		}
		return false;
	}
	bool MillerRobin(const bint& n, const int k = 5) {
		if(n.length()<=8) {
			const int nn=n.toint();
			if(nn==2||nn==3||nn==5)return true;
			if(nn==1||nn== 27509653|| nn==74927161)return false;
		}
		if(n.at(0)%2==0||n.at(0)%5==0)return false;

		bint d(n - 1);
		int r = 0;
		while (d.at(0) % 2 == 0) {
			d /= 2;
			++r;
		}
		const bint S(n - 1);
		bint mu(1);
		mu.quick_mul_10k(n.size()<<4);
		mu/=n;
		if (!witness(n, S, bint(2), d, r,mu))return false; //可以筛去绝大部分的合数
		if (!witness(n, S, bint(3), d, r,mu))return false;
		if (n > 61 && !witness(n, S, bint(61), d, r,mu))return false;
		const auto delta(n-6);
		bint seed;
		for (int i = 0; i < k; ++i) {
			seed.rand(delta);
			seed+=4;
			if (!witness(n, S, seed, d, r,mu))return false;
		}
		return true;
	}

	bool is_prime(const bint& n) {
		return MillerRobin(n);
	}

	bint rho(const bint& x) {
		if (x == 4)
			return bint(2);
		const bint Delta(x-2);
		bint c,r,t;
		const size_t k = x.size();
		bint mu(1);
		mu.quick_mul_10k(k<<4);
		mu/=x;
		
		while (true) {
			c.rand(Delta);
			++c;
			bint p(1), d;
			r.rand(Delta);
			++r;
			t=r;
			int i = 0, j = 1;
			int step=32,cnt1=0;
			while (++i) {
				r*=r;
				r+=c;
				r=_Quick_Mod(r,x,mu);
				p*=abs(r-t);
				p=_Quick_Mod(p,x,mu);
				if (!p)break;
				if (!(i & (step-1)) || i == j) {
					d = gcd(p, x);
					if (d > 1) {
						return d;
					}
					if (i == j) {
						t = r;
						j <<= 1; 
						if(!((++cnt1)&3))
							step<<=1;
					}
				}
			}
		}
	}

	void pollard_rho(bint& x, std::vector<bint>& ans) {
		if (x < 2)return;
		if (is_prime(x)) {
			ans.emplace_back(std::move(x));
			return;
		}
		bint pri = rho(x);
		while (x % pri == 0)x /= pri;
		pollard_rho(pri, ans);
		pollard_rho(x, ans);
	}

	std::vector<bint> pollard_rho(bint x) {
		std::vector<bint> ans;
		pollard_rho(x, ans);
		return ans;
	}

	void max_pollard_rho(bint x, bint& Max) {
		if (x < 2 || x <= Max)return;
		if (is_prime(x)) {
			Max = Max >= x ? Max : x;
			return;
		}
		const bint pri = rho(x);
		while (x % pri == 0)x /= pri;
		max_pollard_rho(pri, Max);
		max_pollard_rho(x, Max);
	}

	bint max_prime(const bint& x) {
		bint ans;
		max_pollard_rho(x, ans);
		return ans;
	}

	bint2 modpow(bint2 a, bint2 b, const bint2& mod) {
		const auto k = mod.size();
		bint2 mu(1);
		mu.quick_mul_2k(k << 6);
		mu /= mod;
		return modpow(std::move(a), std::move(b), mod, mu);
	}

	bint2 _Quick_Mod(const bint2& x, const bint2& mod, const bint2& mu) {
		if (x < mod)return x;
		const size_t k = mod.size();
		bint2 r2(bint2(x, k - 1, x.size()) * mu);
		r2.quick_divide_2k((k + 1) << 5);
		r2 *= mod;
		if (r2.size() > k + 1) {
			r2.resize(k + 1);
			r2.maintain();
		}
		bint2 r(bint2(x, 0, k + 1) - r2);
		if (r >= mod)
			r -= mod;
		if(r<0) {
			bint2 G(1);
			G.quick_mul_2k((k+1)<<5);
			r+=G;
		}
		return r;
	}
	bint2 modpow(bint2 a, bint2 b, const bint2& mod, const bint2& mu) {
		bint2 ans(1);
		size_t _Length = b.length();
		for(size_t i = 0;i<_Length;++i) {
			if(b[i])
				ans = _Quick_Mod(ans * a, mod, mu);
			a=_Quick_Mod(a*a,mod,mu);
		}
		
		return ans;
	}

	bool witness(const bint2& n, const bint2& S, bint2 seed, const bint2& d, int r, const bint2& mu) {
		seed = modpow(seed, d, n, mu);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == S)return true;
			seed = _Quick_Mod(seed * seed, n, mu);
			if (seed == 1)return false;
		}
		return false;
	}
	bool MillerRobin(const bint2& n, const int k = 5) {
		if (n.size()==1) {
			const uint32_t nn = n.to_uint();
			if (nn == 2 || nn == 3 || nn == 5)return true;
			if (nn == 1 || nn == 27509653 || nn == 74927161)return false;
		}
		if (n.at(0) % 2 == 0)
			return false;

		bint2 d(n - 1);
		int r = 0;
		while (d.at(0) % 2 == 0) {
			d >>= 1;
			++r;
		}
		const bint2 S(n - 1);
		bint2 mu(1);
		mu.quick_mul_2k(n.size() << 6);
		mu /= n;
	
		if (!witness(n, S, bint2(2), d, r, mu))
			return false; //可以筛去绝大部分的合数
		if (!witness(n, S, bint2(3), d, r, mu))
			return false;
		if (n > 61 && !witness(n, S, bint2(61), d, r, mu))
			return false;
		const auto delta(n - 6);
		bint2 seed;
		for (int i = 0; i < k; ++i) {
			seed.rand(delta);
			seed += 4;
			if (!witness(n, S, seed, d, r, mu))return false;
		}
		return true;
	}

	bool is_prime(const bint2& n) {
		return MillerRobin(n);
	}

	bint2 rho(const bint2& x) {
		if (x == 4)
			return bint2(2);
		const bint2 Delta(x - 2);
		bint2 c, r, t;
		const size_t k = x.size();
		bint2 mu(1);
		mu.quick_mul_2k(k << 6);
		mu /= x;

		while (true) {
			c.rand(Delta);
			++c;
			bint2 p(1), d;
			r.rand(Delta);
			++r;
			t = r;
			int i = 0, j = 1;
			int step = 32, cnt1 = 0;
			while (++i) {
				r *= r;
				r += c;
				r = _Quick_Mod(r, x, mu);
				p *= abs(r - t);
				p = _Quick_Mod(p, x, mu);
				if (!p)break;
				if (!(i & (step - 1)) || i == j) {
					d = gcd(p, x);
					if (d > 1) {
						return d;
					}
					if (i == j) {
						t = r;
						j <<= 1;
						if (!((++cnt1) & 3))
							step <<= 1;
					}
				}
			}
		}
	}

	void pollard_rho(bint2& x, std::vector<bint2>& ans) {
		if (x < 2)return;
		if (is_prime(x)) {
			ans.emplace_back(std::move(x));
			return;
		}
		bint2 pri = rho(x);
		while (x % pri == 0)x /= pri;
		pollard_rho(pri, ans);
		pollard_rho(x, ans);
	}

	std::vector<bint2> pollard_rho(bint2 x) {
		std::vector<bint2> ans;
		pollard_rho(x, ans);
		return ans;
	}

	void max_pollard_rho(bint2 x, bint2& Max) {
		if (x < 2 || x <= Max)return;
		if (is_prime(x)) {
			Max = Max >= x ? Max : x;
			return;
		}
		const bint2 pri = rho(x);
		while (x % pri == 0)x /= pri;
		max_pollard_rho(pri, Max);
		max_pollard_rho(x, Max);
	}

	bint2 max_prime(const bint2& x) {
		bint2 ans;
		max_pollard_rho(x, ans);
		return ans;
	}
}
