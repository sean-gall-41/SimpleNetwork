#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cell.h"
#include "spike_gen.h"

void init_rng(void) {srand(time(NULL));}

int rand_int(int min, int max)
{
	return rand() % (max - min) + min;
}

float rand_float(float min, float max)
{
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}

// here the input thresh is a threshold variable that equals 
// 1 when a spike is fired and then decays to zero
int spiked(float thresh)
{
	return (rand_float(0.0, 1.0) < RATE_IN_S * (1 - thresh)) ? 1 : 0;
}

void reset_spikes(uint8_t spikes[], uint32_t num_ts)
{
	memset(spikes, '\000', num_ts * sizeof(uint8_t));
}

void print_spikes(uint8_t spikes[], uint32_t num_ts)
{
	for (int i = 0; i < num_ts; i++)
	{
		if (spikes[i]) printf("spiked at time: %d\n", i);
	}
}

