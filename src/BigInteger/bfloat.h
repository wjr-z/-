#ifndef BFLOAT_H
#define BFLOAT_H 

#include "bint.h"

namespace Math {

	/*---友元函数声明---*/

	//------------------------------------------------------------------//

	class bfloat;

	std::ostream& operator<<(std::ostream&, const bfloat&);
	std::istream& operator>>(std::istream&, bfloat&);

	bool operator<(const bfloat&, const bfloat&);
	bool operator<(const bfloat&, const double&);
	bool operator<(const double&, const bfloat&);
	bool operator==(const bfloat&, const bfloat&);
	bool operator==(const bfloat&, const double&);
	bool operator==(const double&, const bfloat&);
	bool operator<=(const bfloat&, const bfloat&);
	bool operator<=(const bfloat&, const double&);
	bool operator<=(const double&, const bfloat&);
	bool operator>(const bfloat&, const bfloat&);
	bool operator>(const bfloat&, const double&);
	bool operator>(const double&, const bfloat&);
	bool operator>=(const bfloat&, const bfloat&);
	bool operator>=(const bfloat&, const double&);
	bool operator>=(const double&, const bfloat&);
	bool operator!=(const bfloat&, const bfloat&);
	bool operator!=(const bfloat&, const double&);
	bool operator!=(const double&, const bfloat&);

	bfloat operator+(const bfloat&);
	bfloat operator-(const bfloat&);


	bfloat operator+(const bfloat&, const bfloat&);
	bfloat operator+(const bfloat&, const double&);
	bfloat operator+(const double&, const bfloat&);
	bfloat operator-(const bfloat&, const bfloat&);
	bfloat operator-(const bfloat&, const double&);
	bfloat operator-(const double&, const bfloat&);

	bfloat operator*(const bfloat&, const bfloat&);
	bfloat operator*(const bfloat&, const double&);
	bfloat operator*(const double&, const bfloat&);
	bfloat operator/(const bfloat&, const bfloat&);
	bfloat operator/(const bfloat&, const double&);
	bfloat operator/(const double&, const bfloat&);
	bfloat fabs(const bfloat&);
	bool approximate(const bfloat&, const bfloat&);
	void setfloatlim(const int&);
	bfloat qpow(bfloat, int);
	bfloat qpow(bfloat, bint);

	bfloat sqrt(const bfloat&);//bfloat开根

	bfloat pow(const bfloat&, const bfloat&);//p^q次方

	int log2(const bint&);

	/*暂时使用的最简单的算法*/
	bfloat ln(bfloat);
	bfloat exp(const bfloat&);

	bfloat atan(bfloat);
	bfloat asin(const bfloat&);
	bfloat acos(const bfloat&);

	bfloat sin(const bfloat&);
	bfloat cos(const bfloat&);

	//------------------------------------------------------------------//

	class bfloat {
	public:
		static int floatlim;
	private:
		bint base;//底数
		int exp;//指数
		void assign(const double&);
		void assign(const char*);
		void assign(const std::string&);
		void clear();
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
		bfloat() :exp(0) {

		}
		explicit bfloat(const int& Val) : base(Val), exp(0) {
			maintain();
		}
		explicit bfloat(const double& val) :exp(0) {
			assign(val);
		}
		explicit bfloat(const char* s) :exp(0) {
			assign(s);
		}
		explicit bfloat(const std::string& s) :exp(0) {
			assign(s);
		}
		bfloat(const bfloat& other) :base(other.base), exp(other.exp) {

		}
		bfloat(const bint& a, const int& index = 0) :base(a), exp(index) {

		}
		bfloat& operator=(const int& Val) {
			base = Val;
			exp = 0;
			return*this;
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
			if (this == &other)return*this;
			base = other.base;
			exp = other.exp;
			return*this;
		}

		const int length()const;
		void relength(const int&);

		int at(const int&)const;
		void set(const int&, const int&);
		void setbase(const bint&);
		void setexp(const int&);

		const bint& getbase()const;
		const int& getexp()const;

		friend std::ostream& operator<<(std::ostream&, const bfloat&);
		friend std::istream& operator>>(std::istream&, bfloat&);

		friend bool operator<(const bfloat&, const bfloat&);
		friend bool operator<(const bfloat&, const double&);
		friend bool operator<(const double&, const bfloat&);
		friend bool operator==(const bfloat&, const bfloat&);
		friend bool operator==(const bfloat&, const double&);
		friend bool operator==(const double&, const bfloat&);
		friend bool operator<=(const bfloat&, const bfloat&);
		friend bool operator<=(const bfloat&, const double&);
		friend bool operator<=(const double&, const bfloat&);
		friend bool operator>(const bfloat&, const bfloat&);
		friend bool operator>(const bfloat&, const double&);
		friend bool operator>(const double&, const bfloat&);
		friend bool operator>=(const bfloat&, const bfloat&);
		friend bool operator>=(const bfloat&, const double&);
		friend bool operator>=(const double&, const bfloat&);
		friend bool operator!=(const bfloat&, const bfloat&);
		friend bool operator!=(const bfloat&, const double&);
		friend bool operator!=(const double&, const bfloat&);

		bfloat& operator+=(const bfloat&);
		bfloat& operator+=(const int&);
		bfloat& operator+=(const double&);
		bfloat& operator-=(const bfloat&);
		bfloat& operator-=(const int&);
		bfloat& operator-=(const double&);
		bfloat& operator*=(const bfloat&);
		bfloat& operator*=(const int&);
		bfloat& operator*=(const double&);
		bfloat& operator/=(const bfloat&);
		bfloat& operator/=(const int&);
		bfloat& operator/=(const double&);

		bfloat& operator++();
		bfloat operator++(int);
		bfloat& operator--();
		bfloat operator--(int);

		friend bfloat operator+(const bfloat&);
		friend bfloat operator-(const bfloat&);

		friend bfloat operator+(const bfloat&, const bfloat&);
		friend bfloat operator+(const bfloat&, const double&);
		friend bfloat operator+(const double&, const bfloat&);
		friend bfloat operator-(const bfloat&, const bfloat&);
		friend bfloat operator-(const bfloat&, const double&);
		friend bfloat operator-(const double&, const bfloat&);

		friend bfloat operator*(const bfloat&, const bfloat&);
		friend bfloat operator*(const bfloat&, const double&);
		friend bfloat operator*(const double&, const bfloat&);
		friend bfloat operator/(const bfloat&, const bfloat&);
		friend bfloat operator/(const bfloat&, const double&);
		friend bfloat operator/(const double&, const bfloat&);
		friend bfloat fabs(const bfloat&);
		friend bool approximate(const bfloat&, const bfloat&);
		friend void setfloatlim(const int&);
		friend bfloat qpow(bfloat, int);
		friend bfloat qpow(bfloat, bint);
		std::string tostr();

		friend bint floor(const bfloat& x);

	#ifdef TEST
		void add() { ++base; }
		void del() { --base; }

	#endif TEST
	};

}

#endif BFLOAT_H

