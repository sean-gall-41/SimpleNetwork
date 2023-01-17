#ifndef CELL_H_
#define CELL_H_

#include <stdint.h>
#include "jsmn.h"

#define NUM_CELL_PARAMS 10

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
	uint32_t num_inputs;
	uint32_t num_outputs;
	/* more channels*/
	float voltage;
	uint8_t spike;

	/* network attribs */
	uint32_t total_input;
};

void init_cell(struct cell *in_cell, pair_t cell_params[]);
void calc_cell_spike(struct cell *in_cell, float prev_ts, float step_size);
float conductance_sum(struct cell *in_cell, float prev_ts, float prev_v);

#endif /* CELL_H_ */

