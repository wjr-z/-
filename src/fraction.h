#ifndef FRACTION_H
#define FRACTION_H

#include "bint.h"
#include "mtool.h"

namespace Math {

	template<typename T,typename = 
		typename std::enable_if_t<in_type<T,int,long long,bint>::value>>
	class fraction {
	public:
		fraction()
			:Numerator(0),Denominator(1){

		}

		fraction(const fraction& other) 
			:Numerator(other.Numerator), Denominator(other.Denominator){

		}

		fraction(fraction&& other) 
			:Numerator(std::move(other.Numerator)),
			Denominator(std::move(other.Denominator)){

		}

		fraction(int x)
			:Numerator(x), Denominator(1) {

		}

		fraction(int x, int y) {
			int g=mgcd(x,y);
			x/=g;
			y/=g;
			if(y<0)x*=-1,y*=-1;
			Numerator = x;
			Denominator = y;
		}

		~fraction() {

		}

		fraction& operator+=(const fraction& other) {
			T G = mgcd(Denominator,other.Denominator);
			T A = Denominator / G;
			T B = other.Denominator / G;
			Numerator = Numerator * B + other.Numerator * A;
			Denominator = A * B * G;
			maintain();
			return *this;
		}

		friend fraction operator+(const fraction&lhs, const fraction& rhs) {
			fraction ans(lhs);
			ans+=rhs;
			return ans;
		}

		fraction& operator-=(const fraction& other) {
			T G = mgcd(Denominator, other.Denominator);
			T A = Denominator / G;
			T B = other.Denominator / G;
			Numerator = Numerator * B - other.Numerator * A;
			Denominator = A * B * G;
			maintain();
			return *this;
		}

		friend fraction operator-(const fraction& lhs, const fraction& rhs) {
			fraction ans(lhs);
			ans-=rhs;
			return ans;
		}

		fraction& operator*=(const fraction& other) {
			Numerator*=other.Numerator;
			Denominator*=other.Denominator;
			maintain();
			return *this;
		}

		friend fraction operator*(const fraction& lhs, const fraction& rhs) {
			fraction ans(lhs);
			ans*=rhs;
			return ans;
		}

		fraction& operator/=(const fraction& other) {
			if (this == &other) {
				Numerator = 1;
				Denominator = 1;
				return *this;
			}
			Numerator *= other.Denominator;
			Denominator *= other.Numerator;
			maintain();
			return *this;
		}

		friend fraction operator/(const fraction& lhs, const fraction& rhs) {
			fraction ans(lhs);
			ans/=rhs;
			return ans;
		}

		bool operator<(const fraction& other) {
			return Numerator * other.Denominator < Denominator * other.Numerator;
		}

		bool operator<=(const fraction& other) {
			return Numerator * other.Denominator <= Denominator* other.Numerator;
		}

		bool operator==(const fraction& other) {
			return Numerator == other.Numerator && Denominator == other.Denominator;
		}

		bool operator!=(const fraction& other) {
			return Numerator != other.Numerator || Denominator != other.Denominator;
		}

		bool operator>(const fraction& other) {
			return !(*this<=other);
		}

		bool operator>=(const fraction& other) {
			return !(*this<other);
		}

		friend istream& operator>>(istream& in, fraction& x) {
			in>>x.Numerator;
			char c=in.rdbuf()->sgetc();
			if (c == '/') {
				in.rdbuf()->snextc();
				in >> x.Denominator;
			}
			x.maintain();
			return in;
		}

		friend ostream& operator<<(ostream& out, const fraction& x) {
			out<<x.Numerator;
			if (x.Denominator != 1) {
				out << "/";
				out << x.Denominator;
			}
			return out;
		}

	private:

		template<typename T,std::enable_if_t<std::is_same<T,bint>::value>* = nullptr>
		void anti(T& x) {
			x.oppsite();
		}

		template<typename T,std::enable_if_t<!std::is_same<T, bint>::value>* = nullptr>
		void anti(T& x) {
			x=-x;
		}

		template<typename T,std::enable_if_t<std::is_same<T, bint>::value>* = nullptr>
		bool not_p(const T& x) {
			return x.is_positive() == false;
		}

		template<typename T, std::enable_if_t<!std::is_same<T, bint>::value>* = nullptr>
		bool not_p(const T& x) {
			return x<0;
		}

		template<typename T>
		T mgcd(T x, T y) {
			while (true) {
				if (!y)return x;
				if (y < x)std::swap(x, y);
				y %= x;
			}
		}

		void maintain() {
			T G = mgcd(Numerator,Denominator);
			if(not_p(G))anti(G);
			Numerator /= G;
			Denominator /= G;
			if(not_p(G))anti(Numerator),anti(Denominator);
		}

		T Numerator, Denominator;
	};
}

#endif