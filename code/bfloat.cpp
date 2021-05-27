#include "bfloat.h"

_MATH_BEGIN

int bfloat::floatlim = 32;

void bfloat::assign(const double& x) {
	if(x<0)positive = false;
	clear();
	base = (long long)floor(x);
	int Length = base.length();
	double y = x - (long long)x;
	for (int i = Length; i < floatlim; ++i, --exp) {
		y *= 10;
		base = base * 10 + (int)floor(y);
		y-=floor(y);
	}
}
void bfloat::assign(const char* s) {
	int Length=strlen(s);
	if(!isrightdouble(s))return;
	clear();
	reserve(min(floatlim,Length)>>3);
	int head=0;
	if(s[head]=='-')++head,positive=false;
	if(s[head]=='+')++head;
	bool is=false;
	for (int i = head; i < Length; ++i) {
		if (s[i] == '.') {
			is=true;
			continue;
		}
	}
}

void bfloat::clear() {
	base=0;
	exp=0;
	positive=true;
}

void bfloat::resize(const int& index) {
	base.resize(index);
}
void bfloat::reserve(const int& index) {
	base.reserve(index);
}

_MATH_END