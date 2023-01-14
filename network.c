#include <stdlib.h>
#include "network.h"
#include "util.h"
#include "str_util.h"
#include "jsmn.h"
#include "spike_gen.h"

void init_network(struct network *model_network, pair_t network_params[], pair_t cell_params[])
{
	model_network->num_cells               = str_to_int(get_val("num_cells", network_params, NUM_NETWORK_PARAMS));
	model_network->max_num_input_per_cell  = str_to_int(get_val("max_num_input_per_cell", network_params, NUM_NETWORK_PARAMS));
	model_network->max_num_output_per_cell = str_to_int(get_val("max_num_output_per_cell", network_params, NUM_NETWORK_PARAMS));
	model_network->prob_poiss_input        = str_to_float(get_val("prob_poiss_input", network_params, NUM_NETWORK_PARAMS));
	
	init_network_arrs(model_network, cell_params);
}

void init_network_arrs(struct network *model_network, pair_t cell_params[])
{
	model_network->poisson_inputs = (uint8_t *)calloc(model_network->num_cells, sizeof(uint8_t));
	model_network->cells = (struct cell *)calloc(model_network->num_cells, sizeof(struct cell));
	FOREACH_NELEM(model_network->cells, model_network->num_cells, cp)
	{
		init_cell(cp, cell_params);
	}
}

/*
 * Plan for the algorithm
 *     1) establish which cells get spike input from poisson spike generator (CHECK)
 *     2) pre-process: create distance matrix between cells
 *     3) for every cell, make connections with gaussian or exponential dist on the distances
 *     4) for each probabilistic "hit" add pre-cell to post-cell's input array
 *     5) make connections until a) we are out of cells to connect from (saturate inputs)
 *        or until we are out of cells in total
 */
void init_network_connections(struct network *model_network)
{
	for (uint32_t i = 0; i < model_network->num_cells; i++)
	{
		float rng = rand_float(0.0, 1.0);
		model_network->poisson_inputs[i] = (rng < model_network->prob_poiss_input);
	}
}

/*
 * Plan for the algorithm
 *     2) pre-compute the poisson spikes
 *     1) for every time step, for every cell, integrate inputs, update conductances, then the voltage, determine spikes
 */
void calc_network_activity(struct network *model_network)
{
}

void delete_network_arrs(struct network *model_network)
{
	free(model_network->poisson_inputs);
	FOREACH_NELEM(model_network->cells, model_network->num_cells, cp)
	{
		delete_cell(cp);
	}
	free(model_network->cells);
}

