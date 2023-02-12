#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "cell_pop.h"
#include "util.h"
#include "str_util.h"
#include "spike_gen.h"

void init_cell_pop(struct cell_pop *model_cell_pop, json &cell_pop_params, json &cell_params)
{
	model_cell_pop->num_cells      = cell_pop_params["num_cells"];
	model_cell_pop->max_num_input  = cell_pop_params["max_num_input"];
	model_cell_pop->max_num_output = cell_pop_params["max_num_output"];
	model_cell_pop->prob_input     = cell_pop_params["prob_input"];
	model_cell_pop->prob_output    = cell_pop_params["prob_output"];
	
	init_cell_pop_arrs(model_cell_pop, cell_params);

}

void init_cell_pop_arrs(struct cell_pop *model_cell_pop, json &cell_params)
{
	model_cell_pop->inputs = (uint32_t *)calloc(model_cell_pop->max_num_input * model_cell_pop->num_cells, sizeof(uint32_t));
	model_cell_pop->outputs = (uint32_t *)calloc(model_cell_pop->max_num_output * model_cell_pop->num_cells, sizeof(uint32_t));
	memset(model_cell_pop->inputs, UINT_MAX, model_cell_pop->max_num_input * model_cell_pop->num_cells * sizeof(uint32_t));
	memset(model_cell_pop->outputs, UINT_MAX, model_cell_pop->max_num_output * model_cell_pop->num_cells * sizeof(uint32_t));

	model_cell_pop->cells = (struct cell *)calloc(model_cell_pop->num_cells, sizeof(struct cell));
	FOREACH_NELEM(model_cell_pop->cells, model_cell_pop->num_cells, cp)
	{
		init_cell(cp, cell_params);
	}
}

void calc_cell_pop_poiss_step(struct cell_pop *input_pop, uint32_t ts)
{
	FOREACH_NELEM(input_pop->cells, input_pop->num_cells, cp)
	{
		cp->spike = spiked();
	}
}

/*
 * Plan for the algorithm
 *     2) pre-compute the poisson spikes
 *     1) for every time step, for every cell, integrate inputs, update conductances, then the voltage, determine spikes
 */
void calc_cell_pop_act_step(struct cell_pop *in_cell_pop, struct cell_pop *curr_cell_pop, uint32_t ts)
{
	for (uint32_t i = 0; i < curr_cell_pop->num_cells; i++)
	{
		uint32_t total_input = 0;
		for (uint32_t j = 0; j < curr_cell_pop->max_num_input; j++)
		{
			uint32_t in_id = curr_cell_pop->inputs[i * curr_cell_pop->max_num_input + j];
			if (in_id != UINT_MAX)
			{
				total_input += in_cell_pop->cells[in_id].spike;
			}
		}
		curr_cell_pop->cells[i].total_input = total_input;
		calc_cell_spike(&(curr_cell_pop->cells[i]), (float)(ts-1), 1.0); // for now, we'll save the step size is 1
	}
}

void copy_spikes_to_raster(struct cell_pop *model_cell_pop, uint8_t *raster, uint32_t ts)
{
	for (uint32_t i = 0; i < model_cell_pop->num_cells; i++)
	{
		raster[ts * model_cell_pop->num_cells + i] = model_cell_pop->cells[i].spike;
	}
}


void free_cell_pop_arrs(struct cell_pop *model_cell_pop)
{
	free(model_cell_pop->inputs);
	free(model_cell_pop->outputs);
	free(model_cell_pop->cells);
}

void print_inputs(struct cell_pop *model_cell_pop)
{
	for (uint32_t i = 0; i < model_cell_pop->num_cells; i++)
	{
		for (uint32_t j = 0; j < model_cell_pop->max_num_input; j++)
		{
			uint32_t val = model_cell_pop->inputs[i * model_cell_pop->max_num_input + j];
			printf("%u ", val);
		}
		printf("\n");
	}
}

