#include <math.h>
#include <assert.h>
#include <string.h>
#include "matrix.h"

#define ASSERT(x, msg) \
	if (!(x)) \
	{ \
		fprintf(stderr, "%s: %s:%d:%s\n", (msg), __FILE__, __LINE__, __func__); \
		exit(1); \
	}

struct uint32_t_matrix *uint32_t_matrix_new()
{
	struct uint32_t_matrix *matrix = (struct uin32_t_matrix *)calloc(1, sizeof(struct uint32_t_matrx));
	matrix->num_rows = 0;
	matrix->num_cols = 0;
	matrix->num_elems = 0;
	matrix->elems = NULL;
	return matrix;
}

struct uint32_t_matrix *uint32_t_matrix_from(uint64_t num_rows, uint64_t num_cols)
{
	if (num_rows == 0 && num_cols == 0)
	{
		return uint32_t_matrix_new();
	}
	struct uint32_t_matrix *matrix = (struct uin32_t_matrix *)calloc(1, sizeof(struct uint32_t_matrx));
	matrix->num_rows = num_rows;
	matrix->num_cols = num_cols;
	matrix->num_elems = num_rows * num_cols;
	matrix->elems = (uint32_t *)calloc(matrix->num_elems, sizeof(uint32_t));
	return matrix;
}

void uint32_t_matrix_reshape(struct uint32_t_matrix *matrix, uint64_t num_rows_new, uint64_t num_cols_new)
{
	if (matrix)
	{
		// same number of elements: no need to reshape
		if (matrix->num_rows == num_rows_new && matrix->num_cols == num_cols_new)
		{
			return;
		}
		// new has less rows and cols, copy elems and reset dims
		else if (matrix->num_rows >= num_rows_new && matrix->num_cols > num_cols_new ||
				 matrix->num_rows > num_rows_new && matrix->num_cols >= num_cols_new)
		{
			if (!matrix->elems) return;
			uint32_t *resized_elems = (uint32_t *)calloc(num_rows_new * num_cols_new, sizeof(uint32_t));
			memcpy(resized_elems, matrix->elems, matrix->num_elems * sizeof(uint32_t));
			free(matrix->elems);
			matrix->elems = resized_elems;
			matrix->num_rows = num_rows_new;
			matrix->num_cols = num_cols_new;
			matrix->num_elems = num_rows_new * num_cols_new;
			return;
		}
		// new has more rows and cols, copy elems and reset dims and set remaining elems to zero
		else if (matrix->num_rows <= num_rows_new && matrix->num_cols < num_cols_new ||
				 matrix->num_rows < num_rows_new && matrix->num_cols <= num_cols_new)
		{
			if (!matrix->elems) return;
			uint32_t *resized_elems = (uint32_t *)calloc(num_rows_new * num_cols_new, sizeof(uint32_t));
			memcpy(resized_elems, matrix->elems, matrix->num_elems * sizeof(uint32_t));
			memset(resized_elems + matrix->elems, 0, (num_rows_new * num_cols_new - matrix->elems) * sizeof(uint32_t));
			free(matrix->elems);
			matrix->elems = resized_elems;
			matrix->num_rows = num_rows_new;
			matrix->num_cols = num_cols_new;
			matrix->num_elems = num_rows_new * num_cols_new;
			return;
		}
	}
}

const uint32_t uint32_t_matrix_elem_at(const struct uint32_t_matrix *matrix, size_t row_id, size_t col_id)
{
	if (matrix)
	{
		assert(row_id <= matrix->num_rows - 1);
		assert(col_id <= matrix->num_cols - 1);
		if (matrix->elems)
		{
			return matrix->elems[row_id * matrix->num_cols + col_id];
		}
	}
}

void uint32_t_matrix_elem_set_to(size_t row_id, size_t col_id, uint32_t val)
{
	if (matrix)
	{
		assert(row_id <= matrix->num_rows - 1);
		assert(col_id <= matrix->num_cols - 1);
		if (matrix->elems)
		{
			matrix->elems[row_id * matrix->num_cols + col_id] = val;
		}
	}
}

void uint32_t_matrix_broadcast_add(struct uint32_t_matrix *matrix, uint32_t val)
{
	if (matrix && matrix->elems)
	{
		for (uint32_t i = 0; i < matrix->num_elems; i++)
		{
			matrix->elems[i] += val;
		}
	}
}

void uint32_t_matrix_broadcast_subtract(struct uint32_t_matrix *matrix, uint32_t val)
{
	if (matrix && matrix->elems)
	{
		for (uint32_t i = 0; i < matrix->num_elems; i++)
		{
			matrix->elems[i] -= val;
		}
	}
}

void uint32_t_matrix_broadcast_multiply(struct uint32_t_matrix *matrix, uint32_t val)
{
	if (matrix && matrix->elems)
	{
		for (uint32_t i = 0; i < matrix->num_elems; i++)
		{
			matrix->elems[i] *= val;
		}
	}
}

void uint32_t_matrix_broadcast_divide(struct uint32_t_matrix *matrix, uint32_t val)
{
	assert(val > 0);
	if (matrix && matrix->elems)
	{
		for (uint32_t i = 0; i < matrix->num_elems; i++)
		{
			matrix->elems[i] /= val;
		}
	}
}

void uint32_t_matrix_matrix_multiply(struct uint32_t_matrix *matrix_c,
									 struct uint32_t_matrix *matrix_a,
									 struct uint32_t_matrix *matrix_b)
{
	if (matrix_a && matrix_b && matrix_c)
	{
		assert(matrix_a->num_cols == matrix_b->num_rows);
		if (!matrix_c->elems)
		{
			matrix_c->num_rows = matrix_a->num_rows;
			matrix_c->num_cols = matrix_b->num_cols;
			matrix_c->num_elems = matrix_c->num_rows * matrix_c->num_cols;
			matrix_c->elems = (uint32_t *)calloc(matrix_c->num_elems, sizeof(uint32_t));
		}
		else
		{
			assert(matrix_c->num_rows == matrix_a->num_rows);
			assert(matrix_c->num_cols == matrix_b->num_cols);
		}
		// brute force it, according to definition. optimize later
		uint32_t num_c_rows = matrix_a->num_rows;
		uint32_t num_c_cols = matrix_b->num_cols;
		uint32_t same_dims = matrix_a->num_cols;
		for (uint32_t i = 0; i < num_c_rows; i++)
		{
			for (uint32_t j = 0; j < num_c_cols; j++)
			{
				for (uint32_t k = 0; k < same_dims; k++)
				{
					matrix_c->elems[i * num_c_cols + j] += matrix_a->elems[i * same_dims + k] * matrix_b->elems[k * num_c_cols + j];
				}
			}
		}
	}
}

void uint32_t_matrix_transpose(struct uint32_t_matrix *matrix)
{
	if (matrix && matrix->elems)
	{
		uint32_t *transposed_elems = (uint32_t *)calloc(matrix->num_elems, sizeof(uint32_t));
		uint32_t old_num_rows = matrix->num_rows;
		uint32_t old_num_cols = matrix->num_cols;
		for (uint32_t i = 0; i < old_num_rows; i++)
		{
			for (uint32_t j = 0; j < old_num_cols; j++)
			{
				transposed_elems[j * old_num_rows + i] = matrix->elems[i * old_num_cols + j];
			}
		}
		free(matrix->elems);
		matrix->elems = transposed_elems;
		matrix->num_rows = old_num_cols;
		matrix->num_cols = old_num_rows;
	}
}

uint32_t uint32_t_matrix_trace(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		assert(matrix->num_rows == matrix->num_cols);
		uint32_t diag_id = 0;
		uint32_t trace = 0;
		for (uint32_t i = 0; i < matrix->num_elems; i++)
		{
			if (i == diag_id * matrix->num_cols + diag_id)
			{
				trace += matrix->elems[i];
				diag_id++;
			}
		}
		return trace;
	}
	return -1;
}

// TODO: debug, esp main alg
void  uint32_t_matrix_LU_decomp(struct uint32_t_matrix *matrix,
								struct uint32_t_matrix *lower,
								struct uint32_t_matrix *upper)
{
	// TODO: do more checks
	if (matrix)
	{
		assert(matrix->num_rows == matrix->num_cols);
		if (!(lower || upper))
		{
			// bad side effects: now user owns data
			lower = uint32_t_matrix_from(matrix->num_rows, matrix->num_cols);
			upper = uint32_t_matrix_from(matrix->num_rows, matrix->num_cols);
		}
		// collect the first row of upper matrix from input matrix
		for (uint32_t i = 0; i < matrix->num_cols; i++)
		{
			upper->elems[i] = matrix->elems[i];
		}
		// set lower diag elems to one
		uint32_t diag_id = 0;
		for (uint32_t i = 0; i < lower->num_elems; i++)
		{
			if (i == diag_id * lower->num_cols + diag_id)
			{
				lower->elems[i] = 1;
				diag_id++;
			}
		}
		// main computation. See Suli and Mayers, An Introduction to Numerical Analysis
		for (uint32_t i = 1; i < matrix->num_rows; i++)
		{
			for (uint32_t j = 0; j < i-1; j++)
			{
				float pre_factor = 1.0 / (float)upper->elems[j * upper->num_cols + j];
				float sum = 0.0;
				for (uint32_t k = 0; k < (int32_t)j-2; k++)
				{
					sum += lower->elems[i * lower->num_cols + k] * upper->elems[k * upper->num_cols + j];
				}
				lower->elems[i * lower->num_cols + j] = (uint32_t)(pre_factor * (matrix->elems[i * matrix->num_cols + j] - sum));
			}
			for (uint32_t j = i-1; j < matrix->num_rows; j++)
			{
				uint32_t sum = 0.0;
				for (uint32_t k = 0; k < (int32_t)i-2; k++)
				{
					sum += lower->elems[i * lower->num_cols + k] * upper->elems[k * upper->num_cols + j];
				}
				upper->elems[i * upper->num_cols + j] = matrix->elems[i * matrix->num_cols + j] - sum;
			}
		}
	}
}

// see https://math.stackexchange.com/questions/2857224/efficient-algorithm-for-calculating-determinants
// TODO: depending on use-case, may wish to compute via QR decomposition
uint32_t uint32_t_matrix_determinant(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		assert(matrix->num_rows == matrix->num_cols);
		uint64_t square_dim = matrix->num_rows;
		struct uint32_t_matrix *lower = uint32_t_matrix_from(square_dim, square_dim);
		struct uint32_t_matrix *upper = uint32_t_matrix_from(square_dim, square_dim);
		uint32_t_matrix_LU_decomp(matrix, lower, upper);
		uint32_t product = 1;
		uint32_t diag_id = 0;
		for (uint32_t i = 0; i < square_dim * square_dim; i++)
		{
			if (i == diag_id * square_dim + diag_id)
			{
				product *= upper->elems[i];
				diag_id++;
			}
		}
		free(lower);
		free(upper);
		return product;
	}
	return 0; // iffy return on invalid array
}

uint32_t uint32_t_matrix_rank(struct uint32_t_matrix *matrix)
{
	ASSERT(false, "Not Implemented yet")
}

uint32_t uint32_t_matrix_nullity(struct uint32_t_matrix *matrix)
{
	ASSERT(false, "Not Implemented yet")
}

uint32_t uint32_t_matrix_dimension(struct uint32_t_matrix *matrix)
{
	ASSERT(false, "Not Implemented yet")
}

bool uint32_t_matrix_is_zero_matrix(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			for (uint32_t i = 0; i < matrix->num_elems; i++)
			{
				if (matrix->elems[i] != 0) return false;
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_diagonal(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint32_t diag_id = 0;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_elems; i++)
			{
				if (i = diag_id * square_dim + diag_id) diag_id++;
				else
					if (matrix->elems[i] != 0) return false; // short-circuit on failure
			}
			return true;
		}
	}
	return true; // nullcases return true (considering zero matrices to be null-y diagonal)
}

bool uint32_t_matrix_is_upper_triangular(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_rows; i++)
			{
				for (uint32_t j = 0; j < matrix->num_rows; j++)
				{
					if (i > j && matrix->elems[i * square_dim + j] != 0) return false;
				}
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_lower_triangular(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_rows; i++)
			{
				for (uint32_t j = 0; j < matrix->num_rows; j++)
				{
					if (i < j && matrix->elems[i * square_dim + j] != 0) return false;
				}
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_unit_upper_triangular(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_rows; i++)
			{
				for (uint32_t j = 0; j < matrix->num_rows; j++)
				{
					if (i > j && matrix->elems[i * square_dim + j] != 0) return false;
					if (i == j && matrix->elems[i * square_dim + j] != 1) return false;
				}
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_unit_lower_triangular(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_rows; i++)
			{
				for (uint32_t j = 0; j < matrix->num_rows; j++)
				{
					if (i < j && matrix->elems[i * square_dim + j] != 0) return false;
					if (i == j && matrix->elems[i * square_dim + j] != 1) return false;
				}
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_invertible(struct uint32_t_matrix *matrix)
{
	uint32_t det = uint32_t_matrix_determinant(matrix);
	return (det != 0) ? true : false;
}

bool uint32_t_matrix_is_symmetric(struct uint32_t_matrix *matrix)
{
	if (matrix)
	{
		if (matrix->elems)
		{
			if (matrix->num_rows != matrix->num_cols) return false;
			uint64_t square_dim = matrix->num_cols;
			for (uint32_t i = 0; i < matrix->num_rows; i++)
			{
				for (uint32_t j = 0; j < matrix->num_rows; j++)
				{
					if (i != j)
					{
						if (matrix->elems[i * square_dim + j] != matrix->elems[j * square_dim + i]) return false;
					}
				}
			}
			return true;
		}
	}
	return true;
}

bool uint32_t_matrix_is_equal_to(struct uint32_t_matrix *matrix_a, struct uint32_t_matrix *matrix_b)
{
	if (matrix_a && matrix_b)
	{
		if (matrix_a->elems && matrix_b->elems)
		{
			if (matrix_a->num_elems != matrix_b->num_elems) return false;
			return (memcmp(matrix_a->elems, matrix_b->elems, matrix_a->num_elems * sizeof(uint32_t)) == 0) ? true : false; 
		}
	}
	return false;
}

