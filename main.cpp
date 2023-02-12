//#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "spike_gen.h"
#include "json.h"
#include "simulation.h"

#define INPUT_SIM_FILENAME "simulation_params.json"

int main(int argc, char **argv)
{
	std::ifstream sim_param_buf(INPUT_SIM_FILENAME);
	json sim_params = json::parse(sim_param_buf);
	sim_param_buf.close();

	struct simulation model_sim;
	init_simulation(&model_sim, sim_params);
	run_simulation(&model_sim);

	save_raster(model_sim.input_layer_raster,
				model_sim.simulated_network.input_layer.num_cells
				* model_sim.num_ts,
				"input_layer_raster.bin");

	save_raster(model_sim.hidden_layer_raster,
				model_sim.simulated_network.hidden_layer.num_cells
				* model_sim.num_ts,
				"hidden_layer_raster.bin");

	save_raster(model_sim.output_layer_raster,
				model_sim.simulated_network.output_layer.num_cells
				* model_sim.num_ts,
				"output_layer_raster.bin");

	free_simulation(&model_sim);

	return 0;
}

