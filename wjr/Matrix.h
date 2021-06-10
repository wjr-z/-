#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#define _MATH_BEGIN namespace Math{
#define _MATH_END }

/*---ÓÐ´ýÍêÉÆ---*/

template<typename Ty>
class Array;

_MATH_BEGIN
template<typename Ty>
class Matrix {
private:
	Array<Array<Ty>> vec;
	int row, column;
	void maintain(const int& n, const int& m) {
		row = n;
		column = m;
	}
public:
	Matrix(const int& n = 1, const int& m = 1) :row(n), column(m) {
	}
	void resize(const int& n, const int& m) {
		maintain(n, m);
	}
	Matrix<Ty>& operator*=(Matrix<Ty>&);

	Array<Ty>& operator[](const int& index) { return vec[index]; }
};


template<typename Ty>
Matrix<Ty>& Matrix<Ty>::operator*=(Matrix<Ty>& other) {
	int A = this->row, B = this->column, C = other.column;
	if (B != other.row)return *this;
	Matrix<Ty>ans(A, C);
	for (int k = 0; k < B; ++k)
		for (int i = 0; i < A; ++i) {
			if ((*this)[i].size() < k)continue;
			for (int j = 0; j < C; ++j)
				ans[i][j] += (*this)[i][k] * other[k][j];
		}
	return *this = ans;
}

template<typename Ty>
Matrix<Ty> operator*(Matrix<Ty>& lhs, Matrix<Ty>& rhs) {
	Matrix<Ty>ans(lhs);
	return ans *= rhs;
}

_MATH_END
#endif MATRIX_H

