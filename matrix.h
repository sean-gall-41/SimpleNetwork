//#ifndef MATRIX_H_
//#define MATRIX_H_
//
//#include <stdint.h>
//
//struct uint32_t_matrix
//{
//	uint64_t num_rows;
//	uint64_t num_cols;
//	uint64_t num_elems;
//	uint32_t *elems;
//}
//
//struct uint32_t_matrix *uint32_t_matrix_new();
//struct uint32_t_matrix *uint32_t_matrix_from(uint64_t num_rows, uint64_t num_cols);
//void uint32_t_matrix_reshape(struct uint32_t_matrix *matrix, uint64_t num_rows_new, uint64_t num_cols_new);
//const uint32_t uint32_t_matrix_elem_at(const struct uint32_t_matrix *matrix, size_t row_id, size_t col_id);
//void uint32_t_matrix_elem_set_to(size_t row_id, size_t col_id, uint32_t val);
//void uint32_t_matrix_broadcast_add(struct uint32_t_matrix *matrix, uint32_t val);
//void uint32_t_matrix_broadcast_subtract(struct uint32_t_matrix *matrix, uint32_t val);
//void uint32_t_matrix_broadcast_multiply(struct uint32_t_matrix *matrix, uint32_t val);
//void uint32_t_matrix_broadcast_divide(struct uint32_t_matrix *matrix, uint32_t val);
//void uint32_t_matrix_matrix_multiply(struct uint32_t_matrix *matrix_c,
//									 struct uint32_t_matrix *matrix_a,
//									 struct uint32_t_matrix *matrix_b);
//void uint32_t_matrix_transpose(struct uint32_t_matrix *matrix);
//uint32_t uint32_t_matrix_trace(struct uint32_t_matrix *matrix);
//void  uint32_t_matrix_LU_decomp(struct uint32_t_matrix *matrix,
//								struct uint32_t_matrix *lower,
//								struct uint32_t_matrix *upper);
//uint32_t uint32_t_matrix_determinant(struct uint32_t_matrix *matrix);
//
//uint32_t uint32_t_matrix_rank(struct uint32_t_matrix *matrix);
//uint32_t uint32_t_matrix_nullity(struct uint32_t_matrix *matrix);
//uint32_t uint32_t_matrix_dimension(struct uint32_t_matrix *matrix);
//
//bool uint32_t_matrix_is_zero_matrix(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_diagonal(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_upper_triangular(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_lower_triangular(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_unit_upper_triangular(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_unit_lower_triangular(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_invertible(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_symmetric(struct uint32_t_matrix *matrix);
//bool uint32_t_matrix_is_equal_to(struct uint32_t_matrix *matrix_a, struct uint32_t_matrix *matrix_b);
//
////void uint32_t_matrix_solve_linear_system
//
////void uint32_t_matrix_vector_multiply(struct uint32_t_matrix *matrix_a, struct uint32_t_matrix *matrix_b);
//
//#endif /* MATRIX_H_ */
