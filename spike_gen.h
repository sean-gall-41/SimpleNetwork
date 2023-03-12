#ifndef SPIKE_GEN_H_
#define SPIKE_GEN_H_

#include <stdint.h>

#define RATE_IN_S 0.05
#define RATE_IN_MS 50.0
#define RATE_STD_DEV 2.5

void init_rng(void);
int rand_int(int min, int max);
float rand_float(float min, float max);

int spiked(float thresh = 0);
void reset_sim_arrs(float times[], uint8_t spikes[], uint32_t num_ts);
void print_spikes(uint8_t spikes[], uint32_t num_ts);

#endif /* SPIKE_GEN_H_ */

