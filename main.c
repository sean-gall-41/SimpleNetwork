//#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include "spike_gen.h"
#include "jsmn.h"
#include "network.h"

#define INPUT_CELL_FILE_NAME "cell_params.json"
#define INPUT_CELL_POP_FILE_NAME "cell_pop_params.json"

int main(int argc, char **argv)
{
	//pair_t cell_params[NUM_CELL_PARAMS] = {};
	//pair_t cell_pop_params[NUM_LAYERS * NUM_CELL_POP_PARAMS] = {};

	//int cell_r = jsmn_parse_file(INPUT_CELL_FILE_NAME, cell_params);

	object_t *cell_pop_params = (object_t *)malloc(sizeof(object_t));

	int cell_pop_r = jsmn_parse_file(INPUT_CELL_POP_FILE_NAME, cell_pop_params);
	//struct network model_network;
	//init_network(&model_network, cell_pop_params, cell_params);
	//init_network_connections(&model_network);
	//for (uint32_t i = 1; i < 101; i++)
	//{
	//	calc_net_act_step(&model_network, i);
	//	printf("%hhu ", model_network.output_layer.cells[10].spike);
	//	if (i % 24 == 0 ) printf("\n");
	//}
	printf("\n");
	//gui_init(&argc, &argv, &model_cell);
	//gui_run();

	return 0;
}

