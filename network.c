#include <stdlib.h>
#include <limits.h>
#include "set.h"
#include "network.h"
#include "util.h"
#include "str_util.h"
#include "jsmn.h"
#include "spike_gen.h"

void init_network(struct network *model_network, pair_t *cell_pop_params, pair_t *cell_params)
{
	init_cell_pop(&(model_network->input_layer), cell_pop_params, cell_params);
	init_cell_pop(&(model_network->main_layer), cell_pop_params, cell_params);
	init_cell_pop(&(model_network->output_layer), cell_pop_params, cell_params);
}

static void init_cell_pop_input_connections(struct cell_pop *pre_layer, struct cell_pop *curr_layer)
{
	SimpleSet pre_ids;
	set_init(&pre_ids);
	for (uint32_t i = 0; i < curr_layer->num_cells; i++)
	{
		// by re-adding to set of possible inputs, we leave
		// (for now) the number of outputs un-constrained
		for (uint32_t j = 0; j < pre_layer->num_cells; j++)
		{
			set_add(&pre_ids, j);
		}

		for (uint32_t j = 0; j < curr_layer->max_num_input; j++)
		{
			float rng = rand_float(0.0, 1.0);
			if (rng < curr_layer->prob_input && curr_layer->inputs[i * curr_layer->max_num_input + j] == UINT_MAX)
			{
				while (1)
				{
					uint32_t candidate_id = rand_int(0, pre_layer->num_cells);
					if (set_contains(&pre_ids, candidate_id) == SET_TRUE)
					{
						curr_layer->inputs[i * curr_layer->max_num_input + j] = candidate_id;
						set_remove(&pre_ids, candidate_id);
						break;
					}
				}
			}
		}
	}
	set_destroy(&pre_ids);
}


void init_network_connections(struct network *model_network)
{
	struct cell_pop *il = &(model_network->input_layer);
	struct cell_pop *ml = &(model_network->main_layer);
	struct cell_pop *ol = &(model_network->output_layer);

	init_cell_pop_input_connections(il, ml); // init main layer inputs
	init_cell_pop_input_connections(ml, ol); // init output layer inputs
}

/*
 * Plan for the algorithm
 *     2) pre-compute the poisson spikes
 *     1) for every time step, for every cell, integrate inputs, update conductances, then the voltage, determine spikes
 */
void calc_net_act_step(struct network *model_network, uint32_t ts)
{
	calc_cell_pop_poiss_step(&(model_network->input_layer), ts);
	calc_cell_pop_act_step(&(model_network->input_layer), &(model_network->main_layer), ts);
	calc_cell_pop_act_step(&(model_network->main_layer), &(model_network->output_layer), ts);
}

void free_network(struct network *model_network)
{
	free_cell_pop_arrs(&(model_network->input_layer));
	free_cell_pop_arrs(&(model_network->main_layer));
	free_cell_pop_arrs(&(model_network->output_layer));
}

