#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cell.h"
#include "spike_gen.h"

void init_rng(void) {srand(time(NULL));}

float rand_float(float min, float max)
{
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}

int spiked()
{
	return (rand_float(0.0, 1.0) < RATE_IN_S) ? 1 : 0;
}

void generate_spikes(uint8_t spikes[], uint32_t num_ts)
{
	for (int i = 0; i < num_ts; i++)
	{
		spikes[i] = (spiked() == 1) ? '\001' : '\000';
	}
}

void reset_sim_arrs(float times[], uint8_t spikes[], uint32_t num_ts)
{
	memset(times, 0.0, num_ts * sizeof(float));
	memset(spikes, '\000', num_ts * sizeof(uint8_t));
}

void print_spikes(uint8_t spikes[], uint32_t num_ts)
{
	for (int i = 0; i < num_ts; i++)
	{
		if (spikes[i]) printf("spiked at time: %d\n", i);
	}
}

