#include "Matrix.h"

#include <algorithm>

namespace Engine {

	Matrix Matrix::Identity(int size)
	{
		Matrix out = Matrix(size, size);

		for (int i = 0; i < size; i++)
		{
			out.data[i * size + i] = 1;
		};

		return out;
	}

	Matrix Matrix::Add(const Matrix& first, const Matrix& second)
	{
		ASSERT(first.num_columns == second.num_columns && first.num_rows == second.num_rows, "Matrix column/row sizes are not equal!");

		Matrix out = Matrix(first.num_rows, first.num_columns);

		int i = 0;
		for (float item : first.data)
		{
			out.data[i] = item;
			i++;
		}
		
		return out;
	}

	Matrix Matrix::Mul(const Matrix& first, const Matrix& second)
	{
		ASSERT(first.num_columns == second.num_rows, "First matrix column size isn't equal to second matrix row size!");

		const int M = first.num_rows;
		const int N = second.num_columns;
		const int L = first.num_columns;

		// Naive implementation
		Matrix out = Matrix(M, N);

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				for (int k = 0; k < L; k++)
				{
					out.data[j * M + i] += first.data[k * M + i] * second.data[j * L + k];
				}
			}
		}

		return out;
	}

	Matrix Matrix::Add(const Matrix& base, const float scalar)
	{
		Matrix out = Matrix(base.num_rows, base.num_columns);

		int i = 0;
		for (float item : base.data)
		{
			out.data[i] = item + scalar;
			i++;
		}

		return out;
	}

	Matrix Matrix::Mul(const Matrix& base, const float scalar)
	{
		Matrix out = Matrix(base.num_rows, base.num_columns);

		int i = 0;
		for (float item : base.data)
		{
			out.data[i] = item * scalar;
			i++;
		}

		return out;
	}

	Matrix Matrix::Transpose(const Matrix& base)
	{
		Matrix out = Matrix(base.num_columns, base.num_rows);

		for (int i = 0; i < base.num_columns; i++)
		{
			for (int j = 0; j < base.num_rows; j++)
			{
				out.data[i * base.num_rows + j] = base.data[j * base.num_columns + i];
			}
		}

		return out;
	}

	float Matrix::Determinant(const Matrix& base)
	{
		ASSERT(base.num_columns == base.num_rows, "The supplied matrix is not a square matrix!");

		Matrix out = Matrix(base);

		float det = 1.0;
		for (int i = 0; i < out.num_rows; i++)
		{
			int pivot = i;
			for (int j = i + 1; j < out.num_rows; j++)
			{
				if (abs(out.data[j * out.num_rows + i]) > abs(out.data[pivot * out.num_rows + i]))
				{
					pivot = j;
				}
			}
			if (pivot != i)
			{
				std::swap_ranges(
					out.data.begin() + i * out.num_rows,
					out.data.begin() + (i + 1) * out.num_rows,
					out.data.begin() + pivot * out.num_rows
				);
				det *= -1;
			}
			if (out[i * out.num_rows + i] == 0)
			{
				return 0;
			}
			det *= out[i * out.num_rows + i];
			for (int j = i + 1; j < out.num_rows; j++)
			{
				float factor = out.data[j * out.num_rows + i] / out.data[i * out.num_rows + i];
				for (int k = i + 1; k < out.num_rows; k++)
				{
					out.data[j * out.num_rows + k] -= factor * out.data[i * out.num_rows + k];
				}
			}
		}
		return det;
	}

	Matrix Matrix::Inverse(const Matrix& base)
	{
		ASSERT(base.num_columns == base.num_rows, "The supplied matrix is not a square matrix!");
		
		Matrix input = Matrix(base);
		Matrix inverse = Matrix::Identity(base.num_rows);

		for (int i = 0; i < input.num_rows; i++)
		{
			float pivot = input.data[i * input.num_rows + i];
			ASSERT(pivot != 0, "Inverse matrix doesn't exist!");

			for (int j = 0; j < input.num_rows; j++)
			{
				input.data[i * input.num_rows + j] /= pivot;
				inverse.data[i * input.num_rows + j] /= pivot;
			}

			for (int j = 0; j < input.num_rows; j++)
			{
				if (i != j)
				{
					float scale = input[j * input.num_rows + i];
					for (int k = 0; k < input.num_rows; k++)
					{
						input.data[j * input.num_rows + k] -= scale * input.data[i * input.num_rows + k];
						inverse.data[j * input.num_rows + k] -= scale * inverse.data[i * input.num_rows + k];
					}
				}
			}
		}

		return inverse;

	}

	bool Matrix::IsEqual(const Matrix& first, const Matrix& second)
	{
		for (int i = 0; i < 16 ; i++)
		{
			if (first.data[i] != second.data[i]) return false;
		}
		return true;
	}

}