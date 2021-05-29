#include "bfloat.h"


_MATH_BEGIN

#undef min
#undef max

//-----------------------------------------------//

/*---全局---*/

int bfloat::floatlim=64;
bfloat bfloat::minpoint="0.000000000000000000000000000000000000000000000000000000000000001";
bfloat bfloat::exe="2.7182818284590452353602874713527";

void setfloatlim(const int& index) {
	bfloat::floatlim=index;
	bfloat::minpoint.setexp(-index+1);
}

//-----------------------------------------------//

/*---private---*/

void bfloat::assign(const double& x) {
	double y=x;
	if(y<0)y=-y;
	clear();
	base = (long long)floor(y);
	int Length = base.length();
	y=y-floor(y);
	for (int i = Length; i < 16; ++i, --exp) {
		y *= 10;
		base = base * 10 + (int)floor(y);
		y-=floor(y);
	}
	if(x<0)base*=-1;
	maintain();
	if (base.iszero())exp = 0;
}
void bfloat::assign(const char* s) {
	int Length=strlen(s);
	if(!isrightdouble(s))return;
	clear();
	reserve(Length>>3);
	int head=0;
	if(s[head]=='-')++head;
	if(s[head]=='+')++head;
	bool is=false;
	for (; head < Length; ++head) {
		if (s[head] == '.') {
			is=true;
			continue;
		}
		if(is)--exp;
		base.quick_mul_10();
		base+=s[head]-'0';
	}
	for (; !is && head < Length; ++head) {
		if(s[head]=='.')break;
		++exp;
	}
	if(s[0]=='-')base*=-1;
	maintain();
	if (base.iszero())exp = 0;
}
void bfloat::assign(const std::string& s) {
	assign(s.c_str());
}

void bfloat::clear() {
	base=0;
	exp=0;
}

void bfloat::simplemaintain() {
	int Length = length(), Size = size();
	int tail = 0;
	while (!base[tail] && tail < Size)
		++tail;
	tail <<= 3;
	while (!base.at(tail) && tail < Length)
		++tail;
	int div =  tail;
	base.quick_divide_10k(div);
	exp += div;
}

void bfloat::maintain() {
	int Length=length(),Size=size();
	int tail=0;
	while(!base[tail]&&tail<Size-1)
		++tail;
	tail<<=3;
	while(!base.at(tail)&&tail<Length-1)
		++tail;
	if (Length < bfloat::floatlim) {
		base.quick_divide_10k(tail);
		exp+=tail;
		if (base.iszero())exp = 0;
		return ;
	}
	int div=max(Length-bfloat::floatlim,tail);
	base.quick_divide_10k(div);
	exp+=div;
	if(base.iszero())exp=0;
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


const int bfloat::length() const{
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
void bfloat::setexp(const int& index) {
	this->exp=index;
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

	if(head<tail)
		out<<'0';

	for (; head >= tail; --head)
		out << x.base.at(head);
	if (x.exp > 0) {
		for (head = x.exp; head; --head)
			out << '0';
	}
	if (x.exp < 0) {
		out << '.';
		for(int i=head+1;i<tail;++i)
			out<<'0';
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

bool bfloat::operator<(const bfloat&other) const {
	if(ispositive()!=other.ispositive())return ispositive()<other.ispositive();
	if(iszero()&&!other.iszero())
		return other.ispositive();
	if(!iszero()&&other.iszero())
		return !ispositive();
	if(iszero()&&other.iszero())
		return false;
	int _min=min(exp,other.exp);
	int py1=exp-_min,py2=other.exp-_min;
	int len1=length()+py1,len2=other.length()+py2;
	if (len1 != len2) 
		return ispositive()^(len1>len2);//都是正数，且len1<len2 则为 1^0 = 1
	int _max=max(py1,py2);
	for(int i=len1-1;i>=_max;--i)
		if((*this).at(i-py1) != other.at(i-py2))
			return ispositive()^((*this).at(i - py1) > other.at(i - py2));
	if(!py1&&!py2)return false;
	if(!py1)return false;
	return true;
}

bool bfloat::operator==(const bfloat&other) const {
	if (ispositive() != other.ispositive())return false;
	if(iszero()&&other.iszero())return true;
	int _min = min(exp, other.exp);
	int py1 = exp - _min, py2 = other.exp - _min;
	int len1 = length() + py1, len2 = other.length() + py2;
	if (len1 != len2)
		return false;//必定不同
	int _max = max(py1, py2);
	for (int i = len1 - 1; i >= _max; --i)
		if ((*this).at(i - py1) != other.at(i - py2))
			return false;
	if(!_max)return true;
	return false;
}

bool bfloat::operator<=(const bfloat&other) const {
	return (*this<other||*this==other);
}

bool bfloat::operator>(const bfloat&other) const {
	return !(*this<=other);
}

bool bfloat::operator>=(const bfloat&other) const {
	return !(*this<other);
}

bool bfloat::operator!=(const bfloat&other) const {
	return !(*this==other);
}

bfloat& bfloat::operator+=(const bfloat&other) {
	if (exp < other.exp) {
		bfloat mid = other;
		base += mid.base.quick_mul_10k(other.exp - exp);
	}
	else {
		this->base.quick_mul_10k(exp-other.exp);
		base+=other.base;
		exp=other.exp;
	}
	maintain();
	return*this;
}

bfloat& bfloat::operator-=(const bfloat&other) {
	if (exp < other.exp) {
		bfloat mid = other;
		base -= mid.base.quick_mul_10k(other.exp - exp);
	}
	else {
		this->base.quick_mul_10k(exp - other.exp);
		base -= other.base;
		exp=other.exp;
	}
	maintain();
	return*this;
}

bfloat& bfloat::operator*=(const bfloat&other) {
	base*=other.base;
	exp+=other.exp;
	maintain();
	return*this;
}

bfloat& bfloat::operator/=(const bfloat&other) {
	if (this->length() - other.length() < bfloat::floatlim) {
		int py = bfloat::floatlim - (length() - other.length());
		base.quick_mul_10k(py);
		exp -= py;
	}
	base/=other.base;
	exp-=other.exp;
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

bfloat operator+(const bfloat&x) {
	return bfloat(x.getbase(),x.getexp());
}

bfloat operator-(const bfloat&x) {
	return bfloat(-x.getbase(),x.exp);
}

bfloat operator+(const bfloat&lhs, const bfloat&rhs) {
	bfloat ans(lhs);
	ans+=rhs;
	return ans;
}

bfloat operator-(const bfloat&lhs, const bfloat&rhs) {
	bfloat ans(lhs);
	ans -= rhs;
	return ans;
}

bfloat operator*(const bfloat&lhs, const bfloat&rhs) {
	bfloat ans(lhs);
	ans *= rhs;
	return ans;
}

bfloat operator/(const bfloat&lhs, const bfloat&rhs) {
	bfloat ans(lhs);
	ans /= rhs;
	return ans;
}

bfloat operator/(const bfloat& lhs, const double& rhs) {
	return lhs/bfloat(rhs);
}

bfloat operator/(const double& lhs, const bfloat& rhs) {
	return bfloat(lhs)/rhs;
}

bool approximate(const bfloat& lhs, const bfloat& rhs) {//
	if(lhs<rhs)
		return rhs-lhs<=bfloat::minpoint;
	else return lhs-rhs<=bfloat::minpoint;
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

std::string bfloat::tostr() {//lim为至多显示前lim个
	std::string str;
	if (this->base < bintzero)str.push_back('-');
	int tail = max(0, -this->exp), Length = this->length();
	int head = Length - 1;

	if (head < tail)
		str .push_back('0');

	for (; head >= tail; --head)
		str.push_back(this->base.at(head));
	if (this->exp > 0) {
		for (head = this->exp; head; --head)
			str.push_back('0');
	}
	if (this->exp < 0) {
		str.push_back('.');
		for (int i = head + 1; i < tail; ++i)
			str.push_back('0');
		for (; head >= 0; --head)
			str.push_back(this->base.at(head));
	}
	return str;
}


_MATH_END

