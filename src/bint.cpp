#include "bint.h"

#include <iomanip>
#include <iostream>

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
		if (!isrightint(s))return; //不合法
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
			for (size_t i = 0; i < n; ++i) {
				vc[i] += va[i];
				if (vc[i] >= bintjw) {
					vc[i] -= bintjw;
					++vc[i + 1];
				}
			}


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
		for (size_t i = 0; i < m - 1; ++i) {
			tst[i] += vb[i];
			if (tst[i] >= bintjw) {
				tst[i] -= bintjw;
				++tst[i + 1];
			}
		}
		tst[m - 1] += vb[m - 1];
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
			bint c(std::move(b), a.positive);
			for (size_t i = 0; i < n; ++i) {
				c.save_at(i) += a.save_at(i);
				if (c.save_at(i) >= bintjw) {
					c.save_at(i) -= bintjw;
					++c.save_at(i + 1);
				}
			}
			for (size_t tail = n; tail < m && c.save_at(tail) >= bintjw; ++tail) {
				//答案最多是m+1位
				c.save_at(tail) -= bintjw;
				++c.at(tail + 1); //当tail=m-1时进位则答案位数加1，且之后不会再进位
			}
			a = std::move(c);
			return;
		} //a比b大，则只用进行b的位数次加法
		for (size_t i = 0; i < m - 1; ++i) {
			a.save_at(i) += b.at(i);
			if (a.save_at(i) >= bintjw) {
				a.save_at(i) -= bintjw;
				++a.save_at(i + 1);
			}
		}
		a.save_at(m - 1) += b.at(m - 1);
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

			c.pop_back();
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

		a.pop_back();
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

			c.pop_back();
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

		a.pop_back();
		if (!a)a.positive = true;
	}

	void bint::delint(bint& a, int b) {
		if (a.positive ? a < b : a > -b) {
			int y;
			switch (a.size()) {
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
			default: ;
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
			a.pop_back();
		}
		else a.save_at(0) -= b;
	}

	bint bint::largedivide(const bint& A, const bint& B) {
		const size_t n = A.size(), m = B.size(), mid = 2 * m - n - 2;
		bint copyA, copyB;
		copyA.assign(A, mid, n); //舍去2m-n-2位
		copyB.assign(B, mid, m);
		++copyA; //得到大于答案的近似解
		bint ans = copyA / copyB;

		if (Math::abs(A) < Math::abs(B) * ans) //误差在1
			--ans;

		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	bint bint::smalldivide(const bint& A, const bint& B) {
		const size_t n = A.size(), m = B.size(), mid = (n - m) >> 1;
		bint copyA;
		const bint copyB(Math::abs(B));
		copyA.assign(A, mid, n);
		bint ans = copyA / copyB;
		const bint mo = Math::abs(A) - (ans * copyB).quick_mul_10k(mid << 3);
		ans.quick_mul_10k(mid << 3);
		ans += mo / copyB;
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	bint bint::knuthdivide(const bint& A, const bint& B) {
		//比我那个优化的暴力略快一点
		bint r(A), copyB(B);
		r.abs();
		copyB.abs();
		bint ans;

		const int bw = bintjw;

		if (copyB.at(copyB.size() - 1) < (bw >> 1)) {
			const int a = bw >> 1, b = copyB.at(copyB.size() - 1);
			const int K = (a + b - 1) / b;
			r *= K;
			copyB *= K;
		}

		const size_t n = copyB.size(), m = r.size() - copyB.size();
		bint copyA(r);
		r.quick_divide_10k(m << 3);
		size_t i = n + m, j = m;

		const int tst = copyB.save_at(n - 1);

		ans.resize(m + 1);
		int* ql = ans.begin();

		do {
			int q;
			if (r.size() > n)
				q = min(static_cast<long long>(bw) - 1, (r.save_at(n) * 1ll * bw + r.save_at(n - 1)) / tst);
			else if (r.size() > n - 1)
				q = min(bw - 1, r.save_at(n - 1) / tst);
			else q = 0;

			r -= copyB * q;

			while (!r.is_positive()) {
				--q, r += copyB;
			}
			r.quick_mul_10k(8);
			if (j > 0)
				r.save_at(0) = copyA.save_at(j - 1);
			ql[j] = q;
			--i;
			--j;
		}
		while (i >= n);

		size_t Size = ans.size();
		while (Size > 1 && !ans.save_at(Size - 1))
			--Size;
		ans.resize(Size);
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	bint bint::quickdivide(const bint& A, const bint& B) {
		if (A.vec < B.vec)return bint();
		const size_t n = A.size(), m = B.size();
		if (m == 1)return divideint(A, B.at(0) * (B.positive ? 1 : -1)); //低精度除法
		if (m <= ((static_cast<uint64_t>(quicklog2(n)) + 16) << 1)) return knuthdivide(A, B); //m比较小直接用O(m*(n-m))算法
		if (m * 5 >= 3 * n)return largedivide(A, B);
		return smalldivide(A, B);
	}

	bint bint::divideint(const bint& A, int B) {
		bint ans;
		size_t Size = A.size();
		const bool tmp = !(A.positive ^ (B >= 0));
		B = Math::abs(B);

		int mp = -1;
		if (is_power_of_2(B))
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
					copyA &= (B - 1);
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
		const size_t lena = a.size(), lenb = b.size(), _max = max(lena, lenb), _min = min(lena, lenb);
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
				++vc[i + 1], vc[i] -= bintjw;
			}
		}
		while (c.save_at(Siz - 1) >= bintjw) {
			++c.at(Siz), c.save_at(Siz - 1) -= bintjw;
		}
		Siz = c.size();
		while (Siz > 1 && !c.save_at(Siz - 1))
			--Siz;
		c.resize(Siz);
	}

	void bint::TOOM_COOK_3(const bint& lhs, const bint& rhs, bint& ans) {
		const size_t lena = lhs.size(), lenb = rhs.size(), largest = max(lena, lenb);
		if (largest <= 64) {
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
		const size_t n = a.size(), m = b.size(), Min = min(n, m), Max = max(n, m);

		/*---根据不同范围选择不同算法---*/

		if (Min != 1) {
			(Min <= 32 || (1ull << min(static_cast<size_t>(60), (Min - 32) >> 1)) <= Max)
				? Array_func::SlowMul(a.vec, b.vec, a.vec)
				: FFT_Array_func::FFTQuickMul(a.vec, b.vec, a.vec);
			a.positive = !(a.positive ^ b.positive);
		}
		else {
			if (n == 1)mulint(b, a.positive ? a.at(0) : -a.at(0), a);
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
			tmp ^= 1, copyb = -b;
		}

		int mp = -1;
		if (is_power_of_2(static_cast<uint32_t>(copyb)))
			mp = _minx(static_cast<uint32_t>(copyb));

		uint64_t Val = 0;

		c.resize(n);

		if (~mp) {
			for (size_t i = 0; i < n - 1; ++i) {
				Val += static_cast<uint64_t>(a.at(i)) << mp;
				c.save_at(i) = Val % bintjw;
				Val /= bintjw;

			}
			Val += static_cast<uint64_t>(a.at(n - 1)) << mp;

			if (Val >= bintjw) {
				c.save_at(n - 1) = Val % bintjw;
				Val /= bintjw;
				if (Val >= bintjw) {
					c.at(n + 1) = Val / bintjw;
					c.at(n) = Val % bintjw;
				}
				else c.at(n) = Val;
			}
			else c.save_at(n - 1) = Val;
		}
		else {
			for (size_t i = 0; i < n - 1; ++i) {
				Val += copyb * a.at(i);
				c.save_at(i) = Val % bintjw;
				Val /= bintjw;

			}
			Val += copyb * a.at(n - 1);

			if (Val >= bintjw) {
				c.save_at(n - 1) = Val % bintjw;
				Val /= bintjw;
				if (Val >= bintjw) {
					c.at(n + 1) = Val / bintjw;
					c.at(n) = Val % bintjw;
				}
				else c.at(n) = Val;
			}
			else c.save_at(n - 1) = Val;
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

	void bint::pop_back() {
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
		const size_t l = min(L, Size), r = min(R, Size);
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

	bint::bint(const bint& other, const size_t& L, const size_t& R) noexcept : vec(R > L ? R - L : 1), positive(true) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
		assign(other, L, R);
	}

	bint::bint(Array vec, bool positive) noexcept : vec(std::move(vec)), positive(positive) {
#ifdef BINTDEBUG
		cout << "bint拷贝构造函数\n";
#endif
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
			vec[u] = v;
			vec[v] = u;
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
			if (tmp)
				HEAD = randint(0, other.at(i));
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
		for (size_t i = x.size() - 2; ~i; --i)
			out << std::setw(8) << std::setfill('0') << x.at(i);
		return out;
	}

	istream& operator>>(istream& in, bint& x) {
		std::string s;
		in >> s;
		x = s;
		return in;
	}

	bool operator<(const bint& lhs, const bint& rhs) {
		return ((lhs.is_positive() == rhs.is_positive()) && (lhs.vec < rhs.vec)) ^ (!lhs.is_positive());
	}

	bool operator<(const bint& lhs, int rhs) {
		if (lhs.positive != (rhs >= 0))return !lhs.positive;
		bool tmp = !lhs.positive;
		rhs = abs(rhs);
		switch (lhs.size()) {
		case 1: return (lhs.at(0) < rhs) ^ tmp;
		case 2: return (lhs.at(1) <= 100 && lhs.at(0) + lhs.at(1) * bintjw < rhs) ^ tmp;
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
			return (lhs.at(1) < 100) && (lhs.at(0) + lhs.at(1) * bintjw) == rhs;
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
		bool tmp = !lhs.positive;
		rhs = abs(rhs);
		switch (lhs.size()) {
		case 1: return (lhs.at(0) <= rhs) ^ tmp;
		case 2: return (lhs.at(1) <= 100 && (lhs.at(0) + lhs.at(1) * bintjw) <= rhs) ^ tmp;
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
		bool f = b >= 0;
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
		divideint(std::move(*this), b);
		return *this;
	}

	bint& bint::operator%=(const bint& b) {
		if (*this < b)return *this;
		(*this) -= b * ((*this) / b);
		return *this;
	}

	bint& bint::operator%=(const int& b) {
		if (*this < b)return *this;
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
	static std::unordered_map<int, bint> fibmap;

	static bint quickfib(int n) {
		if (fibmap.count(n))return fibmap[n];
		return fibmap[n] =
		(n & 1
			 ? quickfib(n >> 1) * quickfib(n >> 1) + quickfib((n >> 1) + 1) * quickfib((n >> 1) + 1)
			 : quickfib(n >> 1) * (quickfib((n >> 1) + 1) + quickfib((n >> 1) - 1)));
	}
#endif
	bint fibonacci(int n) {
#ifdef QUICK
		fibmap[0] = 0;
		fibmap[1] = fibmap[2] = 1;
		fibmap[3] = 2;
		bint ans = quickfib(n);
		fibmap.clear();
		return ans;
#else
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
		return bint::quickdivide(a, b);
	}

	bint operator/(const bint& a, const int& b) {
		return bint::divideint(a, b);
	}

	bint operator/(bint&& a, const int& b) {
		bint::divideint(a, b);
		return std::move(a);
	}

	bint operator/(int a, const bint& b) {
		const bool tmp = !(b.positive ^ (a >= 0));
		a = abs(a);
		if (b.size() == 1 && b.at(0) <= a)
			return bint(tmp ? (a / b.at(0)) : -(a / b.at(0)));
		if (b.size() == 2 && (b.at(1) <= 100 && b.at(0) + b.at(1) * bintjw <= a))
			return bint(tmp ? (a / (b.at(0) + b.at(1) * bintjw)) : -(a / (b.at(0) + b.at(1) * bintjw)));
		return bint();
	}

	bint operator%(const bint& a, const bint& b) {
		return a - (a / b) * b;
	}

	bint operator%(const bint& a, const int& b) {
		if (!b)return a;
		return a - (a / b) * b;
	}

	bint operator%(const int& a, const bint& b) {
		return a - (a / b) * b;
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
		bint mid = qpow(a, b >> 1);
		return b & 1 ? (mid * a) * mid : mid * mid;
	}

	bint qpow(int a, bint b) {
		return qpow(bint(a), b);
	}

	void bint::quick_mul_10() {
		size_t Length = size();
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

	bint gcd(const bint& a, const bint& b) {
		return !b ? a : gcd(b, a % b);
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

	bint randprime(const bint& L, const bint& R) {
		bint ans(randbint(L, R));
		if (ans.at(0) % 2 == 0) {
			if (ans > L)--ans;
			else if (ans < R)++ans;
			else return bint(-1);
		}
		if (!isprime(ans)) {
			bint ans2 = ans;
			while (ans2 > L) {
				ans2 -= 2;
				if (isprime(ans2))return ans2;
			}
			while (ans < R) {
				ans += 2;
				if (isprime(ans))return ans;
			}
			return bint(-1);
		}
		return ans;
	}

	bint randprime(size_t n) {
		bint ans(randbint(n));
		if (ans.at(0) % 2 == 0)
			++ans;
		if (!isprime(ans)) {
			bint ans2 = ans;
			while (ans2.length() == n) {
				ans2 -= 2;
				if (isprime(ans2))return ans2;
			}
			while (true) {
				ans += 2;
				if (isprime(ans))return ans;
			}
		}
		return ans;
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

	bint2 bint::get2bit() const {
		const size_t Size = size();
		if (Size <= 32) {
			bint2 x;
			const bint2 az(bintjw);
			x = vec.at(Size - 1);
			for (size_t i = Size - 2; ~i; --i)
				x = x * az + vec.at(i);
			return x;
		}
		const size_t mid = Size >> 1;
		bint l, r;
		r.assign(*this, 0, mid);
		l.assign(*this, mid, Size);

		return l.to2bit() * (qpow(bint2(bintjw), mid)) + r.to2bit();
		//其实qpow这一步是可以通过预处理得到的
	}

	bint2 bint::to2bit() const {
		return bint2(get2bit(), positive);
	}

	//------------------------------------------------------------------//
	/*---bint2类部分函数定义---*/

	void bint2::assign(int val) {
		positive = true;
		if (val < 0) {
			val = -val, positive ^= 1;
		}
		resize(1);
		vec.save_at(0) = val;
	}

	void bint2::assign(long long val) {
		positive = true;
		if (val < 0) {
			val = -val, positive ^= 1;
		}
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
		const auto s = const_cast<char*>(_s);
		const int Length = strlen(s);
		clear();
		reserve(Length >> 5);
		int i = Length - 1, head = 0, j;
		int fir = 0;
		while (fir < Length && s[fir] == '-')
			positive ^= 1, ++fir;
		while (i - fir >= 31) {
			for (j = i - 31; j <= i; ++j)
				vec.at(head) = (vec.at(head) << 1) + (s[j] - '0');
			++head;
			i -= 32;
		}
		for (j = fir; j <= i; ++j)
			vec.at(head) = (vec.at(head) << 1) + (s[j] - '0');
		if (iszero())positive = true;

	}

	void bint2::assign(const std::string& s) { assign(s.c_str()); }

	void bint2::saveadd(size_t index) {
		//index位加1
		size_t Size = size();
		for (; index < Size && (vec.save_at(index) == maxuint); ++index)
			vec.save_at(index) = 0;
		if (index == Size)
			vec.resize(index);
		++vec.save_at(index);
	}

	void bint2::savedel(size_t index) {
		//index位减1，不会出现变号
		size_t Size = size();
		while (index < Size && !vec.at(index)) {
			//不足则借位
			vec.at(index) = maxuint;
			++index;
		}
		--vec.at(index);
		if (index == Size - 1 && index && !vec.at(index))
			resize(index);
	}

	void bint2::quickadd(bint2& a, const bint2& b, const bool _positive) {
		if (!b)return;
		if (!a) {
			a = b;
			return;
		}
		const size_t n = a.size(), m = b.size();
		if (n < m) {
			//若a比b小，则优化失败，转为朴素加法
			bint2 c(b);
			for (size_t i = 0; i < n; ++i) {
				if (a.save_at(i) > (c.save_at(i) ^ maxuint))
					c.saveadd(i + 1);
				c.save_at(i) += a.save_at(i);
			}
			c.positive = _positive;
			a = std::move(c);
			return;
		} //a比b大，则只用进行b的位数次加法
		for (size_t i = 0; i < m; ++i) {
			if (b.at(i) > (a.save_at(i) ^ maxuint))
				a.saveadd(i + 1); //a[i+1]可能会溢出
			a.save_at(i) += b.at(i);
		}
		a.positive = (!a ? true : _positive);
	}

	void bint2::addint(bint2& a, int b, const bool positive) {
		if (b > (a.at(0) ^ maxuint)) //溢出
			a.saveadd(1);
		a.at(0) += b;
		a.positive = positive;
	}

	bint2 bint2::largedivide(const bint2& A, const bint2& B) {
		const size_t n = A.size(), m = B.size(), mid = 2 * m - n - 2;
		bint2 copyA, copyB;
		copyA.assign(A, mid, n); //舍去2m-n-2位
		copyB.assign(B, mid, m);
		++copyA; //得到大于答案的近似解
		bint2 ans = copyA / copyB;

		if (Math::abs(A) < Math::abs(B) * ans) //误差在1
			--ans;
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	bint2 bint2::smalldivide(const bint2& A, const bint2& B) {
		size_t n = A.size(), m = B.size(), mid = (n - m) >> 1;
		bint2 copyA;
		const bint2 copyB(Math::abs(B));
		copyA.assign(A, mid, n);
		bint2 ans = copyA / copyB;
		const bint2 mo = Math::abs(A) - (ans * copyB).quick_mul_2k(mid << 5);
		ans.quick_mul_2k(mid << 5);
		ans += mo / copyB;
		ans.positive = !(A.positive ^ B.positive);
		return ans;
	}

	bint2 bint2::knuthdivide(const bint2& A, const bint2& B) {
		//比我那个优化的暴力略快一点
		bint2 r(A), copyB(B);
		r.abs();
		copyB.abs();
		bint2 ans;
		const uint64_t bw = static_cast<uint64_t>(maxuint) + 1;

		if (copyB.at(copyB.size() - 1) < (bw >> 1)) {
			const uint32_t a = bw >> 1, b = copyB.at(copyB.size() - 1);
			const long long K = (a + b - 1) / b;
			r *= bint2(K);
			copyB *= bint2(K);
		}

		const size_t n = copyB.size(), m = r.size() - copyB.size();
		bint2 copyA(r);
		r.quick_divide_2k(m << 5);
		size_t i = n + m, j = m;
		do {
			long long q;
			if (r.size() > n)
				q = min((bw - 1), (r.save_at(n) * bw + r.save_at(n - 1)) / copyB.save_at(n - 1));
			else if (r.size() > n - 1)
				q = min(static_cast<uint32_t>(bw - 1), r.save_at(n - 1) / copyB.save_at(n - 1));
			else q = 0;
			r -= copyB * bint2(q);
			while (!r.ispositive()) {
				--q;
				r += copyB;
			}
			r.quick_mul_2k(32);
			if (j > 0)r.save_at(0) = copyA.save_at(j - 1);
			ans.at(j) = q;
			--i;
			--j;
		}while (i >= n);
		int Size = ans.size();
		while (Size > 1 && !ans.save_at(Size - 1))
			--Size;
		ans.resize(Size);
		return ans;
	}

	bint2 bint2::quickdivide(const bint2& A, const bint2& B) {
		if (A.vec < B.vec)return bint2();
		const size_t n = A.size(), m = B.size();
		if (m <= ((static_cast<uint64_t>(quicklog2(n)) + 16) << 1)) return knuthdivide(A, B); //m比较小直接用O(m*(n-m))算法
		if (m * 3 >= 2 * n)return largedivide(A, B);
		return smalldivide(A, B);
	}

	bint2 bint2::del(const bint2& a, const bint2& b, bool _positive) {
		const size_t n = a.size(), m = b.size();
		bint2 c;
		c.reserve(n + 1);
		c.resize(n);
		for (size_t i = n - 1; i >= m; --i)
			c.at(i) = a.at(i);
		for (size_t i = m - 1; ~i; --i) {
			if (a.at(i) < b.at(i) && (c.at(i) < b.at(i) - a.at(i)))
				c.savedel(i + 1);
			c.at(i) += a.at(i) - b.at(i);
		}

		size_t Length = c.size();
		while (Length > 1 && !c.save_at(Length - 1))
			--Length;
		if (Length != c.size())
			c.resize(Length);

		c.positive = _positive;
		if (c.iszero())
			c.positive = true;
		return c;
	}

	void bint2::quickdel(bint2& a, const bint2& b, const bool _positive, int head) {
		if (a.vec < b.vec) {
			a = del(b, a, !_positive);
			return;
		}
		const size_t m = b.size();
		for (int i = m - 1; i >= head; --i) {
			if (a.at(i) < b.at(i))
				a.savedel(i + 1);
			a.save_at(i) -= b.at(i);
		}

		size_t Size = a.size();
		while (Size > 1 && !a.save_at(Size - 1))
			--Size;
		if (Size != a.size())
			a.resize(Size);
		a.positive = _positive;
		if (a.iszero())
			a.positive = true;
	}

	void bint2::delint(bint2& a, int b, const bool _positive) {
		a.positive = _positive;
		if (a.at(0) >= b) {
			a.at(0) -= b;
			return;
		}
		a.savedel(1);
		a.at(0) += maxuint - b + 1;

		int tail = a.size();
		while (!a.save_at(tail - 1))
			--tail;
		a.resize(tail);
		a.positive = _positive;
	}

	void bint2::quickmul(bint2& a, const bint2& b) {
		if (!a || !b) {
			a.clear();
			return;
		}
		const size_t n = a.size(), m = b.size(), _min = min(n, m), _max = max(n, m);

		if ((_min <= 32) || (1ull << min(static_cast<size_t>(60), (_min - 32) >> 1)) <= _max)
			Array2_func::SlowMul(a.vec, b.vec, a.vec);
		else FFT_Array2_func::FFTQuickMul(a.vec, b.vec, a.vec);

		a.positive = !(a.positive ^ b.positive);
	}

	void bint2::mulint(const bint2& a, int b, bint2& c) {
		c = a * bint2(b);
	}

	void bint2::clear() {
		vec.clear();
		positive = true;
	}

	bool bint2::ispositive() const {
		return positive;
	}

	bool bint2::iszero() const {
		return vec.iszero();
	}

	bint2::bint2() noexcept : positive(true) {

	}

	bint2::~bint2() {

	}

	bint2::bint2(const int& val) noexcept : positive(true) {
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

	bint2& bint2::operator=(const int& val) noexcept {
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

	void bint2::resize(size_t len) { vec.resize(len); }

	size_t bint2::size() const { return vec.size(); }

	size_t bint2::length() const { return vec.length(); }

	void bint2::reserve(const uint32_t& len) { vec.reserve(len); }

	void bint2::relength(size_t index) { vec.relength(index); }

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

	void bint2::maintain() { vec.maintain(); }

	void bint2::assign(const bint2& other, const int& L, const int& R) {
		const int Size = other.size();
		const int l = min(L, Size), r = min(R, Size);
		resize(max(1, r - l));
		this->save_at(0) = 0;
		for (int i = l; i < r; ++i)
			this->save_at(i - l) = other.at(i);
	}

	void bint2::randdata(const bint2& other) {
		size_t Size = other.size();
		resize(Size);
		bool tmp = true;
		for (size_t i = Size - 1; ~i; --i) {
			uint32_t HEAD;
			if (tmp)HEAD = randull() % (other.at(i) + 1);
			else HEAD = randuint();
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

	bool operator<(const bint2& lhs, const int& rhs) {
		return lhs < bint2(rhs); //经典偷懒 
	}

	bool operator<(const int& lhs, const bint2& rhs) {
		return bint2(lhs) < rhs;
	}

	bool operator==(const bint2& lhs, const bint2& rhs) {
		return ((lhs.positive == rhs.positive) && lhs.vec == rhs.vec);
	}

	bool operator==(const bint2& lhs, const int& rhs) {
		if (lhs.positive != (rhs >= 0) || lhs.size() > 2)return false;
		if (lhs.size() == 1)
			return lhs.at(0) == static_cast<uint32_t>(rhs);
		return false;
	}

	bool operator==(const int& lhs, const bint2& rhs) {
		if (rhs.positive != (rhs >= 0) || rhs.size() > 2)return false;
		if (rhs.size() == 1)
			return rhs.at(0) == static_cast<uint32_t>(lhs);
		return false;
	}

	bool operator<=(const bint2& lhs, const bint2& rhs) {
		return !(((lhs.positive == rhs.positive) && (lhs.vec > rhs.vec)) ^ (!lhs.positive));
	}

	bool operator<=(const bint2& lhs, const int& rhs) {
		return lhs <= bint2(rhs);
	}

	bool operator<=(const int& lhs, const bint2& rhs) {
		return bint2(lhs) <= rhs;
	}

	bool operator>(const bint2& lhs, const bint2& rhs) { return !(lhs <= rhs); }

	bool operator>(const bint2& lhs, const int& rhs) { return lhs > bint2(rhs); }

	bool operator>(const int& lhs, const bint2& rhs) { return bint2(lhs) > bint2(rhs); }

	bool operator>=(const bint2& lhs, const bint2& rhs) { return !(lhs < rhs); }

	bool operator>=(const bint2& lhs, const int& rhs) { return lhs >= bint2(rhs); }

	bool operator>=(const int& lhs, const bint2& rhs) { return bint2(lhs) >= rhs; }

	bool operator!=(const bint2& lhs, const bint2& rhs) { return !(lhs == rhs); }

	bool operator!=(const bint2& lhs, const int& rhs) { return lhs != bint2(rhs); }

	bool operator!=(const int& lhs, const bint2& rhs) { return bint2(lhs) != rhs; }

	ostream& operator<<(ostream& out, const bint2& x) {
		if (!x.positive)out << "-";
		const size_t Length = x.length();
		for (size_t i = Length - 1; ~i; --i)
			out << x[i];
		return out;
	}

	istream& operator>>(istream& in, bint2& x) {
		std::string str;
		std::cin >> str;
		x = str;
		return in;
	}

	bint2& bint2::operator+=(const bint2& b) {
		(positive == b.positive)
			? (positive
				   ? quickadd(*this, b, true)
				   : quickadd(*this, b, false))
			: (positive
				   ? quickdel(*this, b, true)
				   : quickdel(*this, b, false));
		return *this;
	}

	bint2& bint2::operator+=(const int& b) {
		bool f = b >= 0;
		(positive == f)
			? (positive
				   ? addint(*this, b, true)
				   : addint(*this, -b, false))
			: (positive
				   ? delint(*this, -b, true)
				   : delint(*this, b, false));
		return *this;
	}

	bint2& bint2::operator-=(const bint2& b) {
		(positive == b.positive)
			? (positive
				   ? quickdel(*this, b, true)
				   : quickdel(*this, b, false))
			: (positive
				   ? quickadd(*this, b, true)
				   : quickadd(*this, b, false));
		return *this;
	}

	bint2& bint2::operator-=(const int& b) {
		bool f = b >= 0;
		(positive == f)
			? (positive
				   ? delint(*this, b, true)
				   : delint(*this, b, false))
			: (positive
				   ? addint(*this, b, true)
				   : addint(*this, b, false));
		return *this;
	}

	bint2& bint2::operator*=(const bint2& b) {
		quickmul(*this, b);
		return *this;
	}

	bint2& bint2::operator*=(int k) {
		*this *= bint2(k);
		return *this;
	}

	bint2& bint2::operator/=(const bint2& b) {
		return (*this) = (*this) / b;
	}

	bint2& bint2::operator/=(int b) {
		return (*this) = (*this) / b;
	}

	bint2& bint2::operator%=(const bint2& b) {
		return (*this) = (*this) % b;
	}

	bint2& bint2::operator%=(int b) {
		return (*this) = (*this) % b;
	}

	bint2& bint2::operator++() {
		(*this) += 1;
		return *this;
	}

	bint2 bint2::operator++(int) {
		bint2 x(*this);
		++(*this);
		return x;
	}

	bint2& bint2::operator--() {
		(*this) -= 1;
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
		return X.iszero();
	}

	bool operator!(bint2&& X) {
		return X.iszero();
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
		a += std::move(b);
		return std::move(a);
	}

	bint2 operator+(bint2 a, const int& b) {
		a += b;
		return a;
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
		if (b.iszero())b.positive = true;
		return std::move(b);
	}

	bint2 operator-(bint2&& a, bint2&& b) {
		a -= std::move(b);
		return std::move(a);
	}

	bint2 operator-(bint2 a, const int& b) {
		a -= b;
		return a;
	}

	bint2 operator-(const int& a, bint2 b) {
		b -= a;
		b.positive ^= 1;
		if (b.iszero())b.positive = true;
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

	bint2 operator*(bint2 a, const int& b) {
		return a * bint2(b);
	}

	bint2 operator*(const int& a, bint2 b) {
		return bint2(a) * b;
	}

	bint2 operator/(const bint2& a, const bint2& b) {
		return bint2::quickdivide(a, b);
	}

	bint2 operator/(const bint2& a, const int& b) {
		return bint2::quickdivide(a, bint2(b));
	}

	bint2 operator/(const int& a, const bint2& b) {
		return bint2::quickdivide(bint2(a), b);
	}

	bint2 operator%(const bint2& a, const bint2& b) {
		return a - (a / b) * b;
	}

	bint2 operator%(const bint2& a, const int& b) {
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

	bint2& bint2::operator&=(const bint2& other) {
		const size_t MaxSize = max(size(), other.size());
		resize(MaxSize);
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MaxSize; ++i)
			q[i] &= r[i];
		return *this;
	}

	bint2& bint2::operator|=(const bint2& other) {
		size_t MaxSize = max(size(), other.size());
		resize(MaxSize);
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MaxSize; ++i)
			q[i] |= r[i];
		return *this;
	}

	bint2& bint2::operator^=(const bint2& other) {
		size_t MinSize = min(size(), other.size());
		uint32_t* q = begin();
		const uint32_t* r = other.begin();
		for (size_t i = 0; i < MinSize; ++i)
			q[i] ^= r[i];
		return *this;
	}

	size_t bint2::count() {
		const uint32_t* Vec = begin();
		size_t Size = size(), count = 0;
		for (size_t i = 0; i < Size; ++i)
			count += findnumber1(Vec[i]);
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
		if (iszero())positive = true;
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
		size_t Size = (n - 1) >> 5;
		for (size_t i = 0; i < Size; ++i)
			ans.at(i) = randuint();
		for (size_t i = Size << 5; i < n - 1; ++i)
			ans[i] = randint(0, 1);
		ans[n - 1] = true;
		return ans;
	}

	void bint2::oppsite() {
		positive ^= 1;
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

	bint bint2::get10bit() const {
		const size_t Size = size();
		if (Size <= 16) {
			bint x;
			x = bint(static_cast<long long>(vec.at(Size - 1)));
			for (size_t i = Size - 2; ~i; --i) {
				x <<= 32;
				x += bint(static_cast<long long>(vec.at(i)));
			}

			return x;
		}
		bint2 l, r;
		const size_t mid = (Size + 1) >> 1;
		r.assign(*this, 0, mid);
		l.assign(*this, mid, Size);
		return l.to10bit() * (qpow(bint(1 << 16), (mid << 1))) + r.to10bit();
	}

	bint bint2::to10bit() const {
		return bint(get10bit(), positive);
	}

	bool operator==(const bint& lhs, const bint2& rhs) {
		return lhs == rhs.to10bit();
	}

	bool operator==(const bint2& lhs, const bint& rhs) {
		return lhs.to10bit() == rhs;
	}

	std::string bit10_to_bit2(const std::string& str) {
		return bint(str).to2bit().tostr();
	}

	std::string bit2_to_bit10(const std::string& str) {
		return bint2(str).to10bit().tostr();
	}

	bint sqrt(const bint& A) {
		bint ans[2];
		int now = 1;
		ans[0] = -1;
		ans[1] = A;
		ans[1].quick_divide_10k(A.length() >> 1);
		const bint one(1);
		while (abs(ans[0] - ans[1]) > one) {
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
		ans[1] = A;
		ans[1].quick_divide_2k(A.length() >> 1);
		const bint2 one(1);
		while (abs(ans[0] - ans[1]) > one) {
			now ^= 1;
			ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) / 2;
		}
		if (ans[now] * ans[now] > A)
			--ans[now];
		return ans[now];
	}

	bint modpow(bint a, bint b, const bint& mod) {
		bint ans(1);
		while (!b.is_zero()) {
			if (b.at(0) & 1) {
				ans *= a;
				ans %= mod;
			}
			a *= a;
			a %= mod;
			b >>= 1;
		}
		return ans;
	}

	bool witness(const bint& n, const bint& S, bint seed, const bint& d, int r) {
		seed = modpow(seed, d, n);
		if (seed == 1)return true;
		for (int i = 0; i < r; ++i) {
			if (seed == S)return true;
			seed *= seed;
			seed %= n;
			if (seed == 1)return false;
		}
		return false;
	}

	bool MillerRobin(const bint& n, int k = 5) {
		if (n == 2)return true;
		if (n.at(0) % 2 == 0)return false;
		bint d(n - 1);
		int r = 0;
		while (d.at(0) % 2 == 0) {
			d /= 2;
			++r;
		}
		const auto L = bint(4), R = bint(n - 2);
		const bint S(n - 1);
		if (!witness(n, S, bint(2), d, r))return false; //可以筛去绝大部分的合数
		if (!witness(n, S, bint(3), d, r))return false;
		if (n > 61 && !witness(n, S, bint(61), d, r))return false;

		for (int i = 0; i < k; ++i) {
			bint seed = randbint(L, R);
			if (!witness(n, S, std::move(seed), d, r))return false;
		}
		return true;
	}

	bool isprime(const bint& n) {
		return MillerRobin(n);
	}

	bint rho(const bint& x) {
		if (x == 4)
			return bint(2);
		while (true) {
			bint c = randbint(bint(1), x - 1);
			auto f = [=](const bint& a) { return (a * a + c) % x; };
			bint t, r, p(1), d;
			t = r = randbint(bint(1), x - 1);
			int i = 0, j = 1;
			while (++i) {
				//开始玄学生成
				r = f(r);
				t = f(f(t));
				p = (p * abs(r - t)) % x;
				if (t == r || !p)break;
				if (!(i & 127) || i == j) {
					//我们不仅在等127次之后gcd我们还会倍增的来gcd
					d = gcd(p, x);
					if (d > 1)return d;
					if (i == j) {
						t = r;
						j <<= 1; //维护倍增正确性，并判环（一箭双雕）
					}
				}
			}

		}
	}

	void pollard_rho(bint& x, vector<bint>& ans) {
		if (x < 2)return;
		if (isprime(x)) {
			ans.emplace_back(std::move(x));
			return;
		}
		bint pri = rho(x);
		while (x % pri == 0)x /= pri;
		pollard_rho(pri, ans);
		pollard_rho(x, ans);
	}

	vector<bint> pollard_rho(bint x) {
		vector<bint> ans;
		pollard_rho(x, ans);
		return ans;
	}

	void max_pollard_rho(bint x, bint& Max) {
		if (x < 2 || x <= Max)return;
		if (isprime(x)) {
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
}
