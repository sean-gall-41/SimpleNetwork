#ifndef SIMULATION_H_
#define SIMULATION_H_
#include <stdint.h>
#include "json.h"
#include "simple_network.h"

struct simulation {
	uint32_t num_ts;
	struct network simulated_network;
	uint8_t *input_layer_raster;
	uint8_t *hidden_layer_raster;
	uint8_t *output_layer_raster;
};

void init_simulation(struct simulation *sim, json &simulation_params);
void run_simulation(struct simulation *sim);
void save_raster(uint8_t *raster, uint32_t num_elem, std::string out_rast_name);
void free_simulation(struct simulation *sim);

#endif /* SIMULATION_H_ */
