#include <math.h>
#include <assert.h>
#include "matrix.h"

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

