#ifndef TESTS_H_H_
#define TESTS_H_H_

#include "matrix.h"

void uint32_t_matrix_reshape_test(void);
void uint32_t_matrix_elem_at(void);
void uint32_t_matrix_set_to_test(void);
void uint32_t_matrix_broadcast_add_test(void);
void uint32_t_matrix_broadcast_sub_test(void);
void uint32_t_matrix_broadcast_mul_test(void);
void uint32_t_matrix_broadcast_div_test(void);
void uint32_t_matrix_mul_test(void);
void uint32_t_matrix_transpose_test(void);
void uint32_t_matrix_trace_test(void);
void uint32_t_matrix_LU_decomp_test(void);
void uint32_t_matrix_det_test(void);
void uint32_t_matrix_rank_test(void);
void uint32_t_matrix_nullity_test(void);
void uint32_t_matrix_dim_test(void);
bool uint32_t_matrix_is_zero_test(void);
bool uint32_t_matrix_is_diag_test(void);
bool uint32_t_matrix_is_upper_tri_test(void);
bool uint32_t_matrix_is_lower_tri_test(void);
bool uint32_t_matrix_is_unit_upper_tri_test(void);
bool uint32_t_matrix_is_unit_lower_tri_test(void);
bool uint32_t_matrix_is_inv_test(void);
bool uint32_t_matrix_is_sym_test(void);
bool uint32_t_matrix_is_eq_to_test(void);

#endif /* TESTS_H_H_ */
