#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "str_util.h"
#include "cell.h"

void init_cell(struct cell *in_cell, json &cell_params)
{
	in_cell->tau_m       = cell_params["tau_m"];        
	in_cell->e_thresh    = cell_params["e_thresh"];
	in_cell->e_leak      = cell_params["e_leak"];
	in_cell->g_leak      = cell_params["g_leak"];
	in_cell->e_syn       = cell_params["e_syn"];
	in_cell->g_syn_max   = cell_params["g_syn_max"];
	in_cell->p_syn_max   = cell_params["p_syn_max"];
	in_cell->tau_s       = cell_params["tau_s"];
	in_cell->num_inputs  = cell_params["num_inputs"];
	in_cell->num_outputs = cell_params["num_outputs"];

	in_cell->voltage = in_cell->e_leak;
	in_cell->spike = '\000';
	in_cell->total_input = 0;

}

void calc_cell_spike(struct cell *in_cell, float prev_ts, float step_size)
{
	float k_1, k_2, k_3, k_4;
	k_1 = conductance_sum(in_cell,
						  /*prev_ts*/0,
						  in_cell->voltage); 
	k_2 = conductance_sum(in_cell,
						  /*prev_ts + */(step_size / 2),
						  in_cell->voltage + step_size * (k_1/2));
	k_3 = conductance_sum(in_cell,
						  /*prev_ts + */(step_size / 2),
						  in_cell->voltage + step_size * (k_2/2));
	k_4 = conductance_sum(in_cell,
						  /*prev_ts + */step_size,
						  in_cell->voltage + step_size * k_3);

	in_cell->voltage += (k_1 + 2 * k_2 + 2 * k_3 + k_4)
						* (step_size / 6);
	
	in_cell->spike  = (in_cell->voltage > in_cell->e_thresh) ? '\001' : '\000';
	in_cell->voltage = in_cell->spike * in_cell->e_thresh
						+ (1 - in_cell->spike) * in_cell->voltage;
}

float conductance_sum(struct cell *in_cell, float prev_ts, float prev_v)
{
	float i_leak = -(prev_v - in_cell->e_leak) / in_cell->tau_m;
	float i_syn  = -in_cell->g_syn_max * in_cell->p_syn_max * prev_ts 
					* exp(1 - (prev_ts / in_cell->tau_s)) * (prev_v - in_cell->e_syn)
					/ (in_cell->g_leak * in_cell->tau_m * in_cell->tau_s);
	return i_leak + in_cell->total_input * i_syn;
}

