#ifndef MATRIX_H
#define MATRIX_H
#include <stdint.h>
/*---ÓÐ´ýÍêÉÆ---*/

namespace Math {

	template<typename Ty>
	class matrix {
	private:
		Ty**arr;
		size_t row,column;
		void Clear();
		void BuySize();
		void Reallocate(size_t _Row,size_t _Column) ;
	public:
		matrix(size_t row=0,size_t column=0);
		matrix(const matrix& other);
		~matrix();
		matrix& operator=(const matrix& other);
		size_t getRow()const;
		size_t getColumn()const;
		Ty* operator[](const size_t& index);
		Ty* operator[](const size_t& index)const;
		void resize(size_t _Row, size_t _Column);
	};

	template<typename Ty>
	void matrix<Ty>::Clear() {
		if (arr == nullptr)return;
		for (size_t i = 0; i < row; ++i) {
			delete[]arr[i];
		}
		delete[]arr;
		arr = nullptr;
	}

	template<typename Ty>
	void matrix<Ty>::BuySize() {
		if (!row || !column)return;
		arr = new Ty * [row];
		for (size_t i = 0; i < row; ++i) {
			arr[i] = new Ty[column];
		}
	}

	template<typename Ty>
	void matrix<Ty>::Reallocate(size_t _Row, size_t _Column) {
		if (row == _Row && column == _Column)return;
		if (arr != nullptr)Clear();
		row = _Row, column = _Column;
		BuySize();
	}

	template<typename Ty>
	matrix<Ty>::matrix(size_t row , size_t column ) :arr(nullptr), row(row), column(column) {
		BuySize();
	}

	template<typename Ty>
	matrix<Ty>::matrix(const matrix<Ty>& other) :arr(nullptr), row(other.row), column(other.column) {
		BuySize();
		for (size_t i = 0; i < row; ++i)
			for (size_t j = 0; j < column; ++j)
				arr[i][j] = other.arr[i][j];
	}

	template<typename Ty>
	matrix<Ty>::~matrix() { Clear(); }

	template<typename Ty>
	matrix<Ty>& matrix<Ty>::operator=(const matrix<Ty>& other) {
		Reallocate(other.row, other.column);
		for (size_t i = 0; i < row; ++i)
			for (size_t j = 0; j < column; ++j)
				arr[i][j] = other.arr[i][j];
		return*this;
	}

	template<typename Ty>
	size_t matrix<Ty>::getRow()const { return row; }

	template<typename Ty>
	size_t matrix<Ty>::getColumn()const { return column; }

	template<typename Ty>
	Ty* matrix<Ty>::operator[](const size_t& index) { return arr[index]; }

	template<typename Ty>
	Ty* matrix<Ty>::operator[](const size_t& index)const { return arr[index]; }

	template<typename Ty>
	void matrix<Ty>::resize(size_t _Row, size_t _Column) {
		Reallocate(_Row, _Column);
	}

	template<typename Ty>
	matrix<Ty> operator* (const matrix<Ty>& lhs, const matrix<Ty>& rhs) {
		size_t row = lhs.getRow(), column = rhs.getColumn(), mid = lhs.getColumn();
		if (mid != rhs.getRow())return matrix<Ty>();
		matrix<Ty>ans(row, column);
		for (size_t k = 0; k < mid; ++k) {
			for (size_t i = 0; i < row; ++i) {
				for (size_t j = 0; j < column; ++j) {
					ans[i][j] += lhs[i][k] * rhs[k][j];
				}
			}
		}
		return ans;
	}

}


#endif
