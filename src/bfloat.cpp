#include "bfloat.h"
#include <string>
namespace Math {

	//-----------------------------------------------//

	/*---全局---*/

	int bfloat::floatlim = 32;

	void setfloatlim(const int& index) {//仅最初使用，以确保线程安全以及所有的精度相同
		bfloat::floatlim = index;
	}

	//-----------------------------------------------//

	/*---private---*/


	void bfloat::assign(const double& x) {//对于指数形式的现在还没搞
		assign(std::to_string(x));
	}
	void bfloat::assign(const char* s) {
		const int Length = strlen(s);
		if (!isrightdouble(s))return;
		clear();
		reserve(Length >> 3);
		int head = 0;
		if (s[head] == '-')++head;
		if (s[head] == '+')++head;
		int base_head = 0;
		for (bool is = false; head < Length; ++head) {
			if (s[head] == '.') {
				is = true;
				continue;
			}
			if (s[head] == 'e' || s[head] == 'E')break;
			if (is)--exp;
			base[base_head++]=s[head]-'0';
		}
		base.reverse(base_head);

		if (s[head] == 'e' || s[head] == 'E') {
			++head;
			bool scientific_positive = true;
			int X = 0;
			if (head < Length) {
				if (s[head] == '+')
					scientific_positive = true, ++head;
				else scientific_positive = false, ++head;
			}
			for (; head < Length; ++head)
				X = X * 10 + s[head] - '0';
			if (scientific_positive)
				exp += X;
			else exp -= X;
		}

		if (s[0] == '-')base *= -1;
		maintain();
		if (base.is_zero())exp = 0;
	}
	void bfloat::assign(const std::string& s) {
		assign(s.c_str());
	}

	void bfloat::clear() {
		base = 0;
		exp = 0;
	}

	void bfloat::maintain() {
		int Length = length(), Size = size();
		int tail = 0;
		while (!base.at(tail) && tail < Size - 1)
			++tail;
		tail <<= 3;
		while (!base[tail-1] && tail < Length - 1)
			++tail;
		if (Length < bfloat::floatlim) {
			base.quick_divide_10k(tail);
			exp += tail;
			if (base.is_zero())exp = 0;
			return;
		}
		int div = max(Length - bfloat::floatlim, tail);
		base.quick_divide_10k(div);
		exp += div;
		if (base.is_zero())exp = 0;
	}

	void bfloat::resize(const int& index) {
		base.resize(index);
	}

	size_t bfloat::size() const {
		return base.size();
	}
	void bfloat::reserve(const int& index) {
		base.reserve(index);
	}
	int bfloat::operator[](const int& index)const { return base.at(index); }
	int& bfloat::operator[](const int& index) { return base.at(index); }
	int& bfloat::save_at(const int& index) { return base.save_at(index); }


	//-----------------------------------------------//

	/*---public---*/

	bool bfloat::is_zero() const {
		return base.is_zero();
	}

	bool bfloat::is_positive() const {
		return base.is_positive();
	}


	size_t bfloat::length() const {
		return base.length();
	}
	void bfloat::relength(const int& index) {
		base.relength(index);
	}

	int bfloat::at(const int& index)const {
		return base[index];
	}
	void bfloat::set(const int& index, const int& val) {
		base[index]=val;
	}
	void bfloat::setbase(const bint& other) {
		this->base = other;
	}
	void bfloat::setexp(const int& index) {
		this->exp = index;
	}
	const bint& bfloat::getbase()const {
		return this->base;
	}
	const int& bfloat::getexp()const {
		return this->exp;
	}

	ostream& operator<<(ostream& out, const bfloat& x) {
		if (x.base < 0)out << '-';
		const int tail = max(0, -x.exp);
		const size_t Length = x.length();
		int head = Length - 1;

		if (head < tail)
			out << '0';

		for (; head >= tail; --head)
			out << x.base[head];
		if (x.exp > 0) {
			for (head = x.exp; head; --head)
				out << '0';
		}
		if (x.exp < 0) {
			out << '.';
			for (int i = head + 1; i < tail; ++i)
				out << '0';
			for (; head >= 0; --head)
				out << x.base[head];
		}
		return out;
	}

	istream& operator>>(istream& in, bfloat& x) {
		std::string s;
		in >> s;
		x = s;
		return in;
	}

	bool operator<(const bfloat& lhs, const bfloat& rhs) {
		if (lhs.is_positive() != rhs.is_positive())return !lhs.is_positive();//lhs是负数则返回true

		if (rhs.is_zero())
			return !lhs.is_positive();//如果lhs是负数，rhs是0，返回true
									 //如果lhs是正数，rhs为0，返回false
		if (lhs.is_zero())//rhs不为0，但lhs为0
			return rhs.is_positive(); //如果lhs是0，rhs如果为负数，返回false，否则返回true

		const int _min = min(lhs.exp, rhs.exp);//都化为最小的指数进行比较
		const int py1 = lhs.exp - _min, py2 = rhs.exp - _min;//和最小指数的差值
		const int len1 = lhs.length() + py1, len2 = rhs.length() + py2;//化为最小指数后的长度
		if (len1 != len2) //长度不等
			return lhs.is_positive() ^ (len1 > len2);//都是正数，且len1<len2 则返回true
		const int _max = max(py1, py2);
		for (int i = len1 - 1; i >= _max; --i)
			if (lhs.at(i - py1) != rhs.at(i - py2))
				return lhs.is_positive() ^ (lhs.at(i - py1) > rhs.at(i - py2));
		if (!py1 && !py2)return false;//指数相同，全部比较完了还是相同则返回false
		if (!py1)return false;//a还剩下py个不全为0，b还剩下py2个0
		return true;
	}

	bool operator<(const bfloat& lhs, const double& rhs) {
		return lhs < bfloat(rhs);//偷懒
	}
	bool operator<(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) < rhs;
	}

	bool operator==(const bfloat& lhs, const bfloat& rhs) {
		if (lhs.is_positive() != rhs.is_positive())return false;//符号不同
		if (rhs.is_zero())
			return lhs.is_zero();
		if (lhs.is_zero())//r不为0，l为0
			return false;
		const int _min = min(lhs.exp, rhs.exp);
		const int py1 = lhs.exp - _min, py2 = rhs.exp - _min;
		const int len1 = lhs.length() + py1, len2 = rhs.length() + py2;
		if (len1 != len2)
			return false;//必定不同
		const int _max = max(py1, py2);
		for (int i = len1 - 1; i >= _max; --i)
			if (lhs.at(i - py1) != rhs.at(i - py2))
				return false;
		if (!_max)return true;//全部比较完，都相同
		return false;
	}
	bool operator==(const bfloat& lhs, const double& rhs) {
		return lhs == bfloat(rhs);
	}
	bool operator==(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) == rhs;
	}

	bool operator<=(const bfloat& lhs, const bfloat& rhs) {
		if (lhs.is_positive() != rhs.is_positive())//符号不同，不会相同，退化为比较小于
			return !lhs.is_positive();//lhs是负数则返回true

		if (rhs.is_zero())
			return (!lhs.is_positive()) || lhs.is_zero();//lhs为负数或者lhs为0都返回true

		if (lhs.is_zero())//rhs不为0，但lhs为0
			return rhs.is_positive(); //此时必定不相同，rhs为正数则返回true

		const int _min = min(lhs.exp, rhs.exp);//都化为最小的指数进行比较
		const int py1 = lhs.exp - _min, py2 = rhs.exp - _min;//和最小指数的差值
		const int len1 = lhs.length() + py1, len2 = rhs.length() + py2;//化为最小指数后的长度
		if (len1 != len2) //长度不等，必定不相同，退化为比较小于
			return lhs.is_positive() ^ (len1 > len2);//都是正数，且len1<len2 则返回true，都是负数，且len1>len2则返回true
		const int _max = max(py1, py2);
		for (int i = len1 - 1; i >= _max; --i)
			if (lhs.at(i - py1) != rhs.at(i - py2))
				return lhs.is_positive() ^ (lhs.at(i - py1) > rhs.at(i - py2));
		if (!py1 && !py2)return true;//因为可以等于，所以返回true
		if (!py1)return false;//a还剩下py个不全为0，b还剩下py2个0
		return true;//因为每个数末尾都不会有0，所以a后面的肯定比0大
	}
	bool operator<=(const bfloat& lhs, const double& rhs) {
		return lhs <= bfloat(rhs);
	}
	bool operator<=(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) <= rhs;
	}

	bool operator>(const bfloat& lhs, const bfloat& rhs) {
		return !(lhs <= rhs);
	}
	bool operator>(const bfloat& lhs, const double& rhs) {
		return lhs > bfloat(rhs);
	}
	bool operator>(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) > rhs;
	}

	bool operator>=(const bfloat& lhs, const bfloat& rhs) {
		return !(lhs < rhs);
	}
	bool operator>=(const bfloat& lhs, const double& rhs) {
		return lhs >= bfloat(rhs);
	}
	bool operator>=(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) >= rhs;
	}

	bool operator!=(const bfloat& lhs, const bfloat& rhs) {
		return !(lhs == rhs);
	}
	bool operator!=(const bfloat& lhs, const double& rhs) {
		return lhs != bfloat(rhs);
	}
	bool operator!=(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) != rhs;
	}

	bfloat& bfloat::operator+=(const bfloat& other) {
		if (exp < other.exp) {//指数比other的更小
			bint mid = other.getbase();
			if (base.length() + bfloat::floatlim < mid.length() + other.exp - exp)
				*this = other;//base长度小于mid*10^(other.exp-exp)长度-floatlim，则base可以忽略不记
			else {
				base += mid.quick_mul_10k(other.exp - exp);
				maintain();
			}
		}
		else {
			if (base.length() + exp - other.exp <= other.length() + bfloat::floatlim) {
				//other不可忽略不记时
				this->base.quick_mul_10k(exp - other.exp);
				base += other.base;
				exp = other.exp;
				maintain();
			}
		}
		return*this;
	}
	bfloat& bfloat::operator+=(const int& other) {
		(*this) += bfloat(other);
		return*this;
	}
	bfloat& bfloat::operator+=(const double& other) {
		(*this) += bfloat(other);
		return*this;
	}

	bfloat& bfloat::operator-=(const bfloat& other) {
		if (exp < other.exp) {
			bint mid = other.getbase();
			if (base.length() + bfloat::floatlim < mid.length() + other.exp - exp)
				*this = other;//base长度小于mid*10^(other.exp-exp)长度-floatlim，则base可以忽略不记
			else {
				base -= mid.quick_mul_10k(other.exp - exp);
				maintain();
			}
		}
		else {
			if (base.length() + exp - other.exp <= other.length() + bfloat::floatlim) {
				//other不可忽略不记时
				this->base.quick_mul_10k(exp - other.exp);
				base -= other.base;
				exp = other.exp;
				maintain();
			}
		}
		return*this;
	}
	bfloat& bfloat::operator-=(const int& other) {
		(*this) += bfloat(other);
		return*this;
	}
	bfloat& bfloat::operator-=(const double& other) {
		(*this) += bfloat(other);
		return*this;
	}


	bfloat& bfloat::operator*=(const bfloat& other) {
		base *= other.base;
		exp += other.exp;
		maintain();
		return*this;
	}
	bfloat& bfloat::operator*=(const int& other) {
		(*this) *= bfloat(other);
		return*this;
	}
	bfloat& bfloat::operator*=(const double& other) {
		(*this) *= bfloat(other);
		return*this;
	}


	bfloat& bfloat::operator/=(const bfloat& other) {
		if (this->length() - other.length() < floatlim) {
			const int py = floatlim - (length() - other.length());
			base.quick_mul_10k(py);
			exp -= py;
		}
		base /= other.base;
		exp -= other.exp;
		maintain();
		return*this;
	}
	bfloat& bfloat::operator/=(const int& other) {
		(*this) += bfloat(other);
		return*this;
	}
	bfloat& bfloat::operator/=(const double& other) {
		(*this) += bfloat(other);
		return*this;
	}


	bfloat& bfloat::operator++() {
		++base;
		return*this;
	}

	bfloat bfloat::operator++(int) {
		bfloat mid(*this);
		++mid;
		return mid;
	}

	bfloat& bfloat::operator--() {
		--base;
		return*this;
	}
	//
	bfloat bfloat::operator--(int) {
		bfloat mid(*this);
		--mid;
		return mid;
	}

	bfloat operator+(const bfloat& x) {
		return bfloat(x.getbase(), x.getexp());
	}

	bfloat operator-(const bfloat& x) {
		return bfloat(-x.getbase(), x.exp);
	}

	bfloat operator+(const bfloat& lhs, const bfloat& rhs) {
		bfloat ans(lhs);
		ans += rhs;
		return ans;
	}

	bfloat operator+(const bfloat& lhs, const double& rhs) {
		return lhs + bfloat(rhs);
	}

	bfloat operator+(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) + rhs;
	}

	bfloat operator-(const bfloat& lhs, const bfloat& rhs) {
		bfloat ans(lhs);
		ans -= rhs;
		return ans;
	}

	bfloat operator-(const bfloat& lhs, const double& rhs) {
		return lhs - bfloat(rhs);
	}

	bfloat operator-(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) - rhs;
	}

	bfloat operator*(const bfloat& lhs, const bfloat& rhs) {
		bfloat ans(lhs);
		ans *= rhs;
		return ans;
	}

	bfloat operator*(const bfloat& lhs, const double& rhs) {
		return lhs * bfloat(rhs);
	}

	bfloat operator*(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) * rhs;
	}

	bfloat operator/(const bfloat& lhs, const bfloat& rhs) {
		bfloat ans(lhs);
		ans /= rhs;
		return ans;
	}

	bfloat operator/(const bfloat& lhs, const double& rhs) {
		return lhs / bfloat(rhs);
	}

	bfloat operator/(const double& lhs, const bfloat& rhs) {
		return bfloat(lhs) / rhs;
	}

	bfloat fabs(const bfloat& x) {
		return bfloat(abs(x.getbase()), x.exp);
	}

	bool approximate(const bfloat& lhs, const bfloat& rhs) {
		if (Math::abs(static_cast<int>(lhs.length()) + lhs.getexp() 
			- (static_cast<int>(rhs.length()) + rhs.getexp())) > 1)
			return false;
		int tmp = 0;
		if (max(lhs.length(), rhs.length()) >= bfloat::floatlim)tmp = 5;
		const bfloat delta = lhs - rhs;
		if (delta.getexp() > 0)return false;
		return delta.getbase() <= tmp;
	}

	bfloat qpow(bfloat lhs, int rhs) {
		bfloat ans(1);
		while (rhs) {
			if (rhs & 1)
				ans *= lhs;
			lhs *= lhs;
			rhs >>= 1;
		}return ans;
	}
	bfloat qpow(bfloat lhs, bint rhs) {
		bfloat ans(1);
		while (!rhs.is_zero()) {
			if (rhs[0] & 1)
				ans *= lhs;
			lhs *= lhs;
			rhs >>= 1;
		}return ans;
	}

	std::string bfloat::tostr() {//返回一个
		std::string str;
		if (!is_positive())str.push_back('-');
		int Length = length();
		int EXP = exp + Length - 1;

		str.push_back('0' + base[Length-1]);
		if (Length > 1) {
			str.push_back('.');
			for (size_t i = Length - 2; ~i; --i)
				str.push_back('0' + base[i]);
		}
		if (EXP) {
			str.push_back('e');
			if (EXP < 0)
				EXP *= -1, str.push_back('-');
			else str.push_back('+');
			str.append(std::to_string(EXP));
		}

		return str;
	}

	bint floor(const bfloat& x) {
		bint ans = x.getbase();
		if (x.getexp() >= 0)
			ans.quick_mul_10k(x.getexp());
		if (x.getexp() < 0)
			ans.quick_divide_10k(-x.getexp());
		return ans;
	}


	bfloat sqrt(const bfloat& A) {
		bfloat ans[2];
		int now = 1;
		ans[0] = 0.0;
		bint _base;
		_base = 1;
		_base.quick_mul_10k((A.getbase().length() >> 1));

		ans[1] = _base;
		ans[1].setexp(A.getexp() >> 1);

		const bfloat Div(0.5);
		while (!approximate(ans[0], ans[1])) {
			now ^= 1;
			ans[now] = (ans[now ^ 1] + A / ans[now ^ 1]) * Div;
		}
		return ans[now];
	}

	bfloat pow(const bfloat& p, const bfloat& q) {
		return exp(q * ln(p));
	}

	int log2(const bint& A) {
		return A.to2bit().length() - 1;
	}

	bfloat ln(bfloat x) {
		if (x == 1)return bfloat(0);
		if (x < 1)return -ln(1 / x);
		if (x < 1.5) {
			bfloat ans(0), A(1);
			const bfloat x_sub_1(x - 1);
			int tmp = bfloat::floatlim << 1;
			tmp += 32;
			for (int i = 1; i <= tmp; ++i) {
				A *= x_sub_1;
				if (i & 1)ans += A * (1.0 / i);
				else ans -= A * (1.0 / i);
			}
			return ans;
		}
		bfloat w(1);
		while (x >= 1.5)
			w *= 2, x = sqrt(x);
		return w * ln(x);
	}

	bfloat exp(const bfloat& x) {
		if (x >= 0 && x <= 1) {
			int tmp = bfloat::floatlim << 1;
			tmp += 32;
			bfloat ans(1), A(1), d(1);
			for (int i = 1; i <= tmp; ++i) {
				A *= x;
				d *= i;
				ans += A / d;
			}return ans;
		}
		if (x < 0)return bfloat(1.0) / exp(-x);
		const bint _Floor = floor(x);
		const bfloat sqrtEXP = exp(bfloat(0.5)), EXP = sqrtEXP * sqrtEXP;
		if (x - _Floor < 0.5)return qpow(EXP, _Floor) * exp(x - _Floor);
		return qpow(EXP, _Floor) * sqrtEXP * exp(x - _Floor - 0.5);
	}

}

