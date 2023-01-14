#ifndef CELL_H_
#define CELL_H_

#include <stdint.h>
#include "jsmn.h"

#define NUM_CELL_PARAMS 9

struct cell
{
	float tau_m;
	float e_thresh;
	float e_leak;
	float g_leak;
	float e_syn;
	float g_syn_max;
	float p_syn_max;
	float tau_s;
	uint32_t num_ts; /* TODO: make a sim param */
	uint32_t num_inputs;
	uint32_t num_outputs;
	/* more channels*/
	float *voltage;
	uint8_t *spikes;

	/* network attribs */
	uint32_t *inputs;
	uint32_t *outputs;
};

void init_cell(struct cell *in_cell, pair_t cell_params[]);
void init_cell_vars(struct cell *in_cell, pair_t cell_params[]);
void init_cell_arrs(struct cell *in_cell);
void calc_cell_activity(struct cell *in_cell, uint8_t input_spikes[], float times[],
						float t_0, float step_size);
void reset_cell_arrs(struct cell *in_cell);
void delete_cell(struct cell *in_cell); /* calls delete cell arrs */ /* TODO: write */
void delete_cell_arrs(struct cell *in_cell);
float conductance_sum(struct cell *in_cell, uint8_t input_i, float t_i, float v_i);

#endif /* CELL_H_ */

