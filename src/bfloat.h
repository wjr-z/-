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

	bfloat ln(bfloat);
	bfloat exp(const bfloat&);

	//------------------------------------------------------------------//

	class alignas(8) bfloat {
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
		size_t size() const;
		void reserve(const int&);
		int operator[](const int&)const;
		int& operator[](const int&);
		int& save_at(const int&);
	public:
		bool is_zero() const;
		bool is_positive() const;
		bfloat();
		explicit bfloat(const int& Val);
		explicit bfloat(const double& val);
		explicit bfloat(const char* s);
		explicit bfloat(const std::string& s);
		bfloat(const bfloat& other);
		bfloat(const bint& a, const int& index = 0);
		bfloat& operator=(const int& Val);
		bfloat& operator=(const double& val);
		bfloat& operator=(const char* s) ;
		bfloat& operator=(const std::string& s);
		bfloat& operator=(const bfloat& other);

		size_t length() const;
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
		void add() ;
		void del() ;

	#endif
	};

}

#endif

