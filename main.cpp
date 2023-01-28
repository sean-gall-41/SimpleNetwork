//#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "spike_gen.h"
#include "json.h"
#include "simple_network.h"

#define INPUT_CELL_FILE_NAME "cell_params.json"
#define INPUT_CELL_POP_FILE_NAME "cell_pop_params.json"

int main(int argc, char **argv)
{
	std::ifstream cell_param_buf(INPUT_CELL_FILE_NAME);
	json cell_params = json::parse(cell_param_buf);
	cell_param_buf.close();

	std::ifstream cell_pop_param_buf(INPUT_CELL_POP_FILE_NAME);
	json cell_pop_params = json::parse(cell_pop_param_buf);
	cell_pop_param_buf.close();

	struct network model_network;
	init_network(&model_network, cell_pop_params, cell_params);
	init_network_connections(&model_network);
	for (uint32_t i = 1; i < 101; i++)
	{
		calc_net_act_step(&model_network, i);
		printf("%hhu ", model_network.output_layer.cells[10].spike);
		if (i % 24 == 0 ) printf("\n");
	}
	printf("\n");
	//gui_init(&argc, &argv, &model_cell);
	//gui_run();

	return 0;
}

