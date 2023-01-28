#ifndef CELL_POP_H_
#define CELL_POP_H_

#include <stdint.h>
#include "cell.h"

#define NUM_CELL_POP_PARAMS 5 /* not including arrays */

struct cell_pop
{
	uint32_t num_cells;
	uint32_t max_num_input;
	uint32_t max_num_output;
	float prob_input; /* probability that any given cell gets input from previous layer*/
	float prob_output;
	uint32_t *inputs; /* input connectivity array */
	uint32_t *outputs; /* output connectivity array */

	struct cell *cells;
};

void init_cell_pop(struct cell_pop *model_cell_pop, json &cell_pop_params, json &cell_params);
void init_cell_pop_arrs(struct cell_pop *model_cell_pop, json &cell_params);
void calc_cell_pop_poiss_step(struct cell_pop *input_pop, uint32_t ts);
void calc_cell_pop_act_step(struct cell_pop *in_cell_pop, struct cell_pop *curr_cell_pop, uint32_t ts);
void free_cell_pop_arrs(struct cell_pop *model_cell_pop);

void print_inputs(struct cell_pop *model_cell_pop);

#endif /* CELL_POP_H_ */

