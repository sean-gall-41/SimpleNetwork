#include <string>
#include <fstream>
#include "simulation.h"

static void init_raster_data(struct simulation *sim)
{
	sim->input_layer_raster  = (uint8_t *)calloc(sim->simulated_network.input_layer.num_cells * sim->num_ts, sizeof(uint8_t));
	sim->hidden_layer_raster = (uint8_t *)calloc(sim->simulated_network.hidden_layer.num_cells * sim->num_ts, sizeof(uint8_t));
	sim->output_layer_raster = (uint8_t *)calloc(sim->simulated_network.output_layer.num_cells * sim->num_ts, sizeof(uint8_t));
}

static void collect_raster_data(struct simulation *sim, uint32_t ts)
{
	copy_spikes_to_raster(&(sim->simulated_network.input_layer), sim->input_layer_raster, ts);
	copy_spikes_to_raster(&(sim->simulated_network.hidden_layer), sim->hidden_layer_raster, ts);
	copy_spikes_to_raster(&(sim->simulated_network.output_layer), sim->output_layer_raster, ts);
}

void save_raster(uint8_t *raster, uint32_t num_elem, std::string out_rast_name)
{
	std::fstream out_rast_buf(out_rast_name.c_str(), std::ios::out | std::ios::binary);
	if (!out_rast_buf.is_open())
	{
		fprintf(stderr, "[ERROR]: Couldn't open '%s' for writing. Continuing...\n", out_rast_name.c_str());
	}
	out_rast_buf.write((char *)raster, num_elem * sizeof(uint8_t));
	out_rast_buf.close();
}

void init_simulation(struct simulation *sim, json &simulation_params)
{
	sim->num_ts = 1300;
	init_network(&(sim->simulated_network), simulation_params["network"], simulation_params["cell"]);
	init_network_connections(&(sim->simulated_network));
	init_raster_data(sim);
}

void run_simulation(struct simulation *sim)
{
	for (uint32_t i = 1; i < sim->num_ts; i++)
	{
		calc_net_act_step(&(sim->simulated_network), i);
		collect_raster_data(sim, i);
	}
}

void free_simulation(struct simulation *sim)
{
	free_network(&(sim->simulated_network));
	free(sim->input_layer_raster);
	free(sim->hidden_layer_raster);
	free(sim->output_layer_raster);
}
