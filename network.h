#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include "cell.h"

#define NUM_NETWORK_PARAMS 4 /* not including arrays */

/* single layer network model */
struct network
{
	uint32_t num_cells;
	uint32_t max_num_input_per_cell;
	uint32_t max_num_output_per_cell;
	float prob_poiss_input; /* probability that any given cell gets poisson input */
	uint8_t *poisson_inputs; /* whether given cell gets poisson input or not */
	struct cell *cells;
};

void init_network(struct network *model_network, pair_t network_params[], pair_t cell_params[]);
void init_network_arrs(struct network *model_network, pair_t cell_params[]);
void init_network_connections(struct network *model_network);
void calc_network_activity(struct network *model_network);
void delete_network_arrs(struct network *model_network);

#endif /* NETWORK_H_ */

