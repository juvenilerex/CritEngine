#pragma once

#include <vector>
#include "../Core/Base.h"

template class ENGINE_API std::_Vector_val<std::_Simple_types<float>>;
template class ENGINE_API std::_Compressed_pair<std::allocator<float>, std::_Vector_val<std::_Simple_types<float>>, true>;
template class ENGINE_API std::vector<float, std::allocator<float>>;

namespace Engine {

	struct ENGINE_API Matrix
	{
		//Matrix Data is stored in column major order
		std::vector<float> data;
		int num_rows, num_columns; 
		
		Matrix(int num_rows, int num_columns) : num_columns(num_columns), num_rows(num_rows), data(std::vector<float>(num_columns * num_rows, 0.0)) {};
		Matrix(int num_rows, int num_columns, std::vector<float> data) : num_columns(num_columns), num_rows(num_rows), data(data) {};
		Matrix(const Matrix& base) : num_rows(base.num_rows), num_columns(base.num_columns), data(base.data) {};
		static Matrix Identity(int size);

		//Static Math Operations
		static Matrix Add(const Matrix& first, const Matrix& second);
		static Matrix Mul(const Matrix& first, const Matrix& second);

		static Matrix Add(const Matrix& base, const float scalar);
		static Matrix Mul(const Matrix& base, const float scalar);

		static float Determinant(const Matrix& base);
		static Matrix Transpose(const Matrix& base);
		static Matrix Inverse(const Matrix& base);
		
		static bool IsEqual(const Matrix& first, const Matrix& second);

		//Object Math Operations
		Matrix Add(Matrix& other) const { return Matrix::Add(*this, other); }
		Matrix Mul(Matrix& other) const { return Matrix::Mul(*this, other); }

		Matrix Add(const float scalar) const { return Matrix::Add(*this, scalar); }
		Matrix Mul(const float scalar) const { return Matrix::Mul(*this, scalar); }

		Matrix Transpose() const { return Matrix::Transpose(*this); }
		float Determinant() const { return Matrix::Determinant(*this); }
		Matrix Inverse() const { return Matrix::Inverse(*this); }

		bool IsEqual(Matrix& other) const { return Matrix::IsEqual(*this, other); }

		float operator[](int index) { return this->data[index]; }
		
	};

}