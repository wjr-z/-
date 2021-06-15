#include "bfloat.h"
#include <string>
_MATH_BEGIN


//-----------------------------------------------//

/*---全局---*/

void setfloatlim(const int& index) {//仅最初使用，以确保线程安全以及所有的精度相同
	bfloat::floatlim = index;
}

//-----------------------------------------------//

/*---private---*/


void bfloat::assign(const double& x) {//对于指数形式的现在还没搞
	assign(std::to_string(x));
}
void bfloat::assign(const char* s) {
	int Length = strlen(s);
	if (!isrightdouble(s))return;
	clear();
	reserve(Length >> 3);
	int head = 0;
	if (s[head] == '-')++head;
	if (s[head] == '+')++head;
	bool is = false;//是否含有小数点
	int base_head = 0;
	for (; head < Length; ++head) {
		if (s[head] == '.') {
			is = true;
			continue;
		}
		if (s[head] == 'e' || s[head] == 'E')break;
		if (is)--exp;
		base.set(base_head++, s[head] - '0');
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
	if (base.iszero())exp = 0;
}
void bfloat::assign(const std::string& s) {
	assign(s.c_str());
}

void bfloat::clear() {
	base = 0;
	exp = 0;
}

void bfloat::simplemaintain() {
	int Length = length(), Size = size();
	int tail = 0;
	while (!base[tail] && tail < Size)
		++tail;
	tail <<= 3;
	while (!base.at(tail) && tail < Length)
		++tail;
	int div = tail;
	base.quick_divide_10k(div);
	exp += div;
}

void bfloat::maintain() {
	int Length = length(), Size = size();
	int tail = 0;
	while (!base[tail] && tail < Size - 1)
		++tail;
	tail <<= 3;
	while (!base.at(tail) && tail < Length - 1)
		++tail;
	if (Length < bfloat::floatlim) {
		base.quick_divide_10k(tail);
		exp += tail;
		if (base.iszero())exp = 0;
		return;
	}
	int div = max(Length - bfloat::floatlim, tail);
	base.quick_divide_10k(div);
	exp += div;
	if (base.iszero())exp = 0;
}

void bfloat::resize(const int& index) {
	base.resize(index);
}
const int bfloat::size() const {
	return base.size();
}
void bfloat::reserve(const int& index) {
	base.reserve(index);
}
const int& bfloat::operator[](const int& index)const { return base[index]; }
int& bfloat::operator[](const int& index) { return base[index]; }
int& bfloat::save_at(const int& index) { return base.save_at(index); }


//-----------------------------------------------//

/*---public---*/

const bool bfloat::iszero()const {
	return base.iszero();
}
const bool bfloat::ispositive()const {
	return base.ispositive();
}


const int bfloat::length() const {
	return base.length();
}
void bfloat::relength(const int& index) {
	base.relength(index);
}

int bfloat::at(const int& index)const {
	return base.at(index);
}
void bfloat::set(const int& index, const int& val) {
	base.set(index, val);
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
	if (x.base < bintzero)out << '-';
	int tail = max(0, -x.exp), Length = x.length();
	int head = Length - 1;

	if (head < tail)
		out << '0';

	for (; head >= tail; --head)
		out << x.base.at(head);
	if (x.exp > 0) {
		for (head = x.exp; head; --head)
			out << '0';
	}
	if (x.exp < 0) {
		out << '.';
		for (int i = head + 1; i < tail; ++i)
			out << '0';
		for (; head >= 0; --head)
			out << x.base.at(head);
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
	if (lhs.ispositive() != rhs.ispositive())return !lhs.ispositive();//lhs是负数则返回true

	if (rhs.iszero())
		return !lhs.ispositive();//如果lhs是负数，rhs是0，返回true
								 //如果lhs是正数，rhs为0，返回false
	if (lhs.iszero())//rhs不为0，但lhs为0
		return rhs.ispositive(); //如果lhs是0，rhs如果为负数，返回false，否则返回true

	int _min = min(lhs.exp, rhs.exp);//都化为最小的指数进行比较
	int py1 = lhs.exp - _min, py2 = rhs.exp - _min;//和最小指数的差值
	int len1 = lhs.length() + py1, len2 = rhs.length() + py2;//化为最小指数后的长度
	if (len1 != len2) //长度不等
		return lhs.ispositive() ^ (len1 > len2);//都是正数，且len1<len2 则返回true
	int _max = max(py1, py2);
	for (int i = len1 - 1; i >= _max; --i)
		if (lhs.at(i - py1) != rhs.at(i - py2))
			return lhs.ispositive() ^ (lhs.at(i - py1) > rhs.at(i - py2));
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
	if (lhs.ispositive() != rhs.ispositive())return false;//符号不同
	if (rhs.iszero())
		return lhs.iszero();
	if (lhs.iszero())//r不为0，l为0
		return false;
	int _min = min(lhs.exp, rhs.exp);
	int py1 = lhs.exp - _min, py2 = rhs.exp - _min;
	int len1 = lhs.length() + py1, len2 = rhs.length() + py2;
	if (len1 != len2)
		return false;//必定不同
	int _max = max(py1, py2);
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
	if (lhs.ispositive() != rhs.ispositive())//符号不同，不会相同，退化为比较小于
		return !lhs.ispositive();//lhs是负数则返回true

	if (rhs.iszero())
		return (!lhs.ispositive()) || lhs.iszero();//lhs为负数或者lhs为0都返回true

	if (lhs.iszero())//rhs不为0，但lhs为0
		return rhs.ispositive(); //此时必定不相同，rhs为正数则返回true

	int _min = min(lhs.exp, rhs.exp);//都化为最小的指数进行比较
	int py1 = lhs.exp - _min, py2 = rhs.exp - _min;//和最小指数的差值
	int len1 = lhs.length() + py1, len2 = rhs.length() + py2;//化为最小指数后的长度
	if (len1 != len2) //长度不等，必定不相同，退化为比较小于
		return lhs.ispositive() ^ (len1 > len2);//都是正数，且len1<len2 则返回true，都是负数，且len1>len2则返回true
	int _max = max(py1, py2);
	for (int i = len1 - 1; i >= _max; --i)
		if (lhs.at(i - py1) != rhs.at(i - py2))
			return lhs.ispositive() ^ (lhs.at(i - py1) > rhs.at(i - py2));
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

bfloat& bfloat::operator*=(const bfloat& other) {
	base *= other.base;
	exp += other.exp;
	maintain();
	return*this;
}

bfloat& bfloat::operator/=(const bfloat& other) {
	if (this->length() - other.length() < bfloat::floatlim) {
		int py = bfloat::floatlim - (length() - other.length());
		base.quick_mul_10k(py);
		exp -= py;
	}
	base /= other.base;
	exp -= other.exp;
	maintain();
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
	if (std::abs(lhs.length() + lhs.getexp() - rhs.length() - rhs.getexp()) > 1)
		return false;
	int maxn = min(lhs.exp, rhs.exp) - bfloat::floatlim;
	return fabs(lhs - rhs) <= bfloat(bint(5), maxn);
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
	while (!rhs.iszero()) {
		if (rhs.at(0) & 1)
			ans *= lhs;
		lhs *= lhs;
		rhs >>= 1;
	}return ans;
}

std::string bfloat::tostr() {//返回一个
	std::string str;
	if (!ispositive())str.push_back('-');
	int Length = length();
	int EXP = exp + Length - 1;

	str.push_back('0' + base.at(Length - 1));
	if (Length > 1) {
		str.push_back('.');
		for (int i = Length - 2; ~i; --i)
			str.push_back('0' + base.at(i));
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


_MATH_END

