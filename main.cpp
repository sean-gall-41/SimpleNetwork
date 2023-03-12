//#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "spike_gen.h"
#include "json.h"
#include "simulation.h"
#include "analysis.h"

#define INPUT_SIM_FILENAME "simulation_params.json"

#define INPUT_LAYER_RASTER_FILENAME "input_layer_raster.bin"
#define HIDDEN_LAYER_RASTER_FILENAME "hidden_layer_raster.bin"
#define OUTPUT_LAYER_RASTER_FILENAME "output_layer_raster.bin"

int main(int argc, char **argv)
{
  std::ifstream sim_param_buf(INPUT_SIM_FILENAME);
	json sim_params = json::parse(sim_param_buf);
	sim_param_buf.close();

  init_rng();

	struct simulation model_sim;
	init_simulation(&model_sim, sim_params);
	run_simulation(&model_sim);

  //plot_rasters(model_sim.input_layer_raster,
  //             model_sim.num_ts,
  //             model_sim.simulated_network.input_layer.num_cells);
  //plot_rasters(model_sim.hidden_layer_raster,
  //             model_sim.num_ts,
  //             model_sim.simulated_network.hidden_layer.num_cells);
  //plot_rasters(model_sim.output_layer_raster,
  //             model_sim.num_ts,
  //             model_sim.simulated_network.output_layer.num_cells);

  plot_inst_fire_rate_cell(model_sim.output_layer_raster,
                           model_sim.num_ts,
                           0);

  //plot_mean_inst_fire_rate(model_sim.input_layer_raster,
  //                         model_sim.num_ts,
  //                         model_sim.simulated_network.input_layer.num_cells);

	//save_raster(model_sim.input_layer_raster,
	//			model_sim.simulated_network.input_layer.num_cells
	//			* model_sim.num_ts,
	//			INPUT_LAYER_RASTER_FILENAME);

	//save_raster(model_sim.hidden_layer_raster,
	//			model_sim.simulated_network.hidden_layer.num_cells
	//			* model_sim.num_ts,
	//			HIDDEN_LAYER_RASTER_FILENAME);

	//save_raster(model_sim.output_layer_raster,
	//			model_sim.simulated_network.output_layer.num_cells
	//			* model_sim.num_ts,
	//			OUTPUT_LAYER_RASTER_FILENAME);
	free_simulation(&model_sim);
	return 0;
}

