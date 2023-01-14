#include "gui.h"
#include "spike_gen.h"
#include "jsmn.h"
#include "network.h"

#define INPUT_CELL_FILE_NAME "cell_params.json"
#define INPUT_NETWORK_FILE_NAME "network_params.json"

int main(int argc, char **argv)
{
	//pair_t cell_params[NUM_CELL_PARAMS] = {};
	//pair_t network_params[NUM_NETWORK_PARAMS] = {};
	//int cell_r = jsmn_parse_file(INPUT_CELL_FILE_NAME, cell_params);
	//int network_r = jsmn_parse_file(INPUT_NETWORK_FILE_NAME, network_params);
	//struct network model_network;
	//init_network(&model_network, network_params, cell_params);
	//init_network_connections(&model_network);
	//calc_network_activity(&model_network);
	//delete_network_arrs(&model_network);
	struct cell model_cell = {
		.tau_m     = 20.0,
		.e_thresh  = -54.0,
		.e_leak    = -70.0,
		.g_leak    = 0.04,
		.e_syn     = 0.00,
		.g_syn_max = 0.8,
		.p_syn_max = 1.0,
		.tau_s     = 10.0,
		.num_ts    = NUM_TS,
		.voltage   = NULL,
		.spikes    = NULL,
	};
	init_cell_arrs(&model_cell);

	gui_init(&argc, &argv, &model_cell);
	gui_run();

	delete_cell_arrs(&model_cell);
	return 0;
}

