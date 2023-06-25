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
  float init_weight_min;
  float init_weight_max;
	uint32_t *inputs; /* input connectivity array */
	uint32_t *outputs; /* output connectivity array */
  float *input_weights; // for now we test weights on binary input
  float *output_weights;

	struct cell *cells;
};

void init_cell_pop(struct cell_pop *model_cell_pop, json &cell_pop_params, json &cell_params);
void init_cell_pop_arrs(struct cell_pop *model_cell_pop, json &cell_params);
void calc_cell_pop_poiss_step(struct cell_pop *input_pop, uint32_t ts);
void calc_cell_pop_act_step(struct cell_pop *in_cell_pop, struct cell_pop *curr_cell_pop, uint32_t ts);
void calc_cell_pop_weight_updates(struct cell_pop *in_cell_pop, struct cell_pop *curr_cell_pop);

void copy_spikes_to_raster(struct cell_pop *model_cell_pop, uint8_t *raster, uint32_t ts);
void free_cell_pop_arrs(struct cell_pop *model_cell_pop);

void print_inputs(struct cell_pop *model_cell_pop);

#endif /* CELL_POP_H_ */

