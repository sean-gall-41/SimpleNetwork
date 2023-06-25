#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include "cell_pop.h"

#define NUM_LAYERS 3

/* three layer network model */
struct network
{
	struct cell_pop input_layer;
	//struct cell_pop hidden_layer;
	struct cell_pop output_layer;
};

void init_network(struct network *model_network, json &cell_pop_params, json &cell_params);
void init_network_connections(struct network *model_network);
void calc_net_act_step(struct network *model_network, uint32_t ts);
void free_network(struct network *model_network);

#endif /* NETWORK_H_ */

