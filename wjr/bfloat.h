#pragma once
#ifndef BFLOAT_H
#define BFLOAT_H 

#include "bint.h"

_MATH_BEGIN

void setfloatlim(const int&);

class bfloat {
public:
	static int floatlim;
	static bfloat minpoint,exe;
private:
	bint base;//底数
	int exp;//指数
	void assign(const double&);
	void assign(const char*);
	void assign(const std::string&);
	void clear();
	void simplemaintain();
	void maintain();//用于保持精度的

	void resize(const int&);
	const int size()const;
	void reserve(const int&);
	const int& operator[](const int&)const;
	int& operator[](const int&);
	int& save_at(const int&);
public:
	const bool iszero()const;
	const bool ispositive()const;
	explicit bfloat():exp(0) {

	}
	explicit bfloat(const double& val):exp(0){
		assign(val);
	}
	bfloat(const char* s) :exp(0) {
		assign(s);
	}
	bfloat(const std::string& s) :exp(0) {
		assign(s);
	}
	bfloat(const bfloat& other):base(other.base),exp(other.exp) {

	}
	bfloat(const bint& a, const int& index=0) :base(a), exp(index) {

	}
	bfloat& operator=(const double& val) {
		assign(val);
		return*this;
	}
	bfloat& operator=(const char* s) {
		assign(s);
		return*this;
	}
	bfloat& operator=(const std::string& s) {
		assign(s);
		return*this;
	}
	bfloat& operator=(const bfloat& other) {
		if(this==&other)return*this;
		base=other.base;
		exp=other.exp;
	}

	const int length()const;
	void relength(const int&);

	int at(const int&)const;
	void set(const int&,const int&);
	void setexp(const int&);

	const bint&getbase()const;
	const int&getexp()const;

	friend ostream&operator<<(ostream&,const bfloat&);
	friend istream&operator>>(istream&,bfloat&);

	bool operator<(const bfloat&)const;
	bool operator==(const bfloat&)const;
	bool operator<=(const bfloat&)const;
	bool operator>(const bfloat&)const;
	bool operator>=(const bfloat&)const;
	bool operator!=(const bfloat&)const;

	bfloat& operator+=(const bfloat&);
	bfloat& operator-=(const bfloat&);
	bfloat& operator*=(const bfloat&);
	bfloat& operator/=(const bfloat&);

	bfloat& operator++();
	bfloat operator++(int);
	bfloat& operator--();
	bfloat operator--(int);


	friend bfloat operator+(const bfloat&);
	friend bfloat operator-(const bfloat&);


	friend bfloat operator+(const bfloat&, const bfloat&);
	friend bfloat operator-(const bfloat&, const bfloat&);

	friend bfloat operator*(const bfloat&, const bfloat&);
	friend bfloat operator/(const bfloat&, const bfloat&);
	friend bfloat operator/(const bfloat&,const double&);
	friend bfloat operator/(const double&,const bfloat&);
	friend bool approximate(const bfloat&,const bfloat&);
	friend void setfloatlim(const int&);
	friend bfloat qpow(bfloat,int);
	friend bfloat qpow(bfloat,bint);
	std::string tostr();
};



_MATH_END

#endif BFLOAT_H

