#ifndef CELL_H_
#define CELL_H_

#include <stdint.h>
#include "json.h"

using json = nlohmann::json;

#define NUM_CELL_PARAMS 10

struct cell
{
	float tau_m;
	float voltage;
  float e_thresh;
	float e_thresh_base;
  float e_thresh_dec;
  float e_thresh_max;
	float e_leak;
	float g_leak;
	float e_syn;
	float g_syn_max;
	float p_syn_max;
	float tau_s;
	float total_input;
	/* network attribs */
	uint32_t num_inputs;
	uint32_t num_outputs;
  uint32_t absolute_refract;
  uint32_t t_since_last;
	/* more channels*/
	uint8_t spike;

};

void init_cell(struct cell *in_cell, json &cell_params);
void calc_cell_spike(struct cell *in_cell, float prev_ts, float step_size);
float conductance_sum(struct cell *in_cell, float prev_ts, float prev_v);

#endif /* CELL_H_ */

