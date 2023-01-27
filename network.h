#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include "cell_pop.h"

#define NUM_LAYERS 3
#define NUM_NETWORK_PARAMS 4 /* not including arrays */

/* three layer network model */
struct network
{
	struct cell_pop input_layer;
	struct cell_pop main_layer;
	struct cell_pop output_layer;
};

void init_network(struct network *model_network, pair_t *cell_pop_params, pair_t *cell_params);
void init_network_connections(struct network *model_network);
void calc_net_act_step(struct network *model_network, uint32_t ts);
void free_network(struct network *model_network);

#endif /* NETWORK_H_ */

