#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "str_util.h"
#include "cell.h"

void init_cell(struct cell *in_cell, pair_t cell_params[])
{
	init_cell_vars(in_cell, cell_params);
	init_cell_arrs(in_cell);
}

void init_cell_vars(struct cell *in_cell, pair_t cell_params[])
{
	in_cell->tau_m       = str_to_float(get_val("tau_m", cell_params, NUM_CELL_PARAMS));
	in_cell->e_thresh    = str_to_float(get_val("e_thresh", cell_params, NUM_CELL_PARAMS));
	in_cell->e_leak      = str_to_float(get_val("e_leak", cell_params, NUM_CELL_PARAMS));
	in_cell->g_leak      = str_to_float(get_val("g_thresh", cell_params, NUM_CELL_PARAMS));
	in_cell->e_syn       = str_to_float(get_val("e_syn", cell_params, NUM_CELL_PARAMS));
	in_cell->g_syn_max   = str_to_float(get_val("g_syn_max", cell_params, NUM_CELL_PARAMS));
	in_cell->p_syn_max   = str_to_float(get_val("p_syn_max", cell_params, NUM_CELL_PARAMS));
	in_cell->tau_s       = str_to_float(get_val("tau_s", cell_params, NUM_CELL_PARAMS));
	in_cell->num_ts      = 1000;
	in_cell->num_inputs  = str_to_int(get_val("num_inputs", cell_params, NUM_CELL_PARAMS));
	in_cell->num_outputs = str_to_int(get_val("num_outputs", cell_params, NUM_CELL_PARAMS));
}

void init_cell_arrs(struct cell *in_cell)
{
	in_cell->voltage = (float *)calloc(in_cell->num_ts, sizeof(float));
	in_cell->spikes = (uint8_t *)calloc(in_cell->num_ts, sizeof(uint8_t));
	in_cell->inputs = (uint32_t *)calloc(in_cell->num_inputs, sizeof(uint32_t));
	in_cell->outputs = (uint32_t *)calloc(in_cell->num_outputs, sizeof(uint32_t));
}

void calc_cell_activity(struct cell *in_cell, uint8_t input_spikes[], float times[],
						float t_0, float step_size)
{
	float k_1, k_2, k_3, k_4;
	times[0]  = t_0;
	in_cell->voltage[0] = in_cell->e_leak;
	for (uint32_t i = 1; i < in_cell->num_ts; i++)
	{
		k_1 = conductance_sum(in_cell,
							  input_spikes[i-1],
							  times[i-1],
							  in_cell->voltage[i-1]); 
		k_2 = conductance_sum(in_cell,
							  input_spikes[i-1],
							  times[i-1] + (step_size / 2),
							  in_cell->voltage[i-1] + step_size * (k_1/2));
		k_3 = conductance_sum(in_cell,
							  input_spikes[i-1],
							  times[i-1] + (step_size / 2),
							  in_cell->voltage[i-1] + step_size * (k_2/2));
		k_4 = conductance_sum(in_cell,
							  input_spikes[i-1],
							  times[i-1] + step_size,
							  in_cell->voltage[i-1] + step_size * k_3);

		in_cell->voltage[i] = in_cell->voltage[i-1]
							+ (k_1 + 2 * k_2 + 2 * k_3 + k_4)
							* (step_size / 6);
		
		in_cell->spikes[i]  = (in_cell->voltage[i] > in_cell->e_thresh) ? '\001' : '\000';
		in_cell->voltage[i] = in_cell->spikes[i] * in_cell->e_thresh
							+ (1 - in_cell->spikes[i]) * in_cell->voltage[i];
		times[i] = times[i-1] + step_size;
	}
}

void reset_cell_arrs(struct cell *in_cell)
{
	memset(in_cell->voltage, 0.0, in_cell->num_ts * sizeof(float));
	memset(in_cell->spikes, '\000', in_cell->num_ts * sizeof(uint8_t));
	memset(in_cell->inputs, 0, in_cell->num_inputs * sizeof(uint32_t));
	memset(in_cell->outputs, 0, in_cell->num_outputs * sizeof(uint32_t));
}

void delete_cell(struct cell *in_cell)
{
	delete_cell_arrs(in_cell);
}

void delete_cell_arrs(struct cell *in_cell)
{
	free(in_cell->voltage);
	free(in_cell->spikes);
	free(in_cell->inputs);
	free(in_cell->outputs);
}

float conductance_sum(struct cell *in_cell, uint8_t input_i, float t_i, float v_i)
{
	float i_leak = -(v_i - in_cell->e_leak) / in_cell->tau_m;
	float i_syn  = -in_cell->g_syn_max * in_cell->p_syn_max * t_i
					* exp(1 - (t_i / in_cell->tau_s)) * (v_i - in_cell->e_syn)
					/ (in_cell->g_leak * in_cell->tau_m * in_cell->tau_s);
	return i_leak + input_i * i_syn;
}

