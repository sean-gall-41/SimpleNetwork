#ifndef ANALYSIS_H_H
#define ANALYSIS_H_H

#include <stdint.h>

void calc_smooth_inst_fire_rates(uint8_t *spike_train, uint32_t n_time_bins);
void plot_rasters(uint8_t *raster, uint32_t trial_len, uint32_t num_cells);
void plot_mean_inst_fire_rate(uint8_t *raster, uint32_t trial_len, uint32_t num_cells);
void plot_inst_fire_rate_cell(uint8_t *raster, uint32_t trial_len, uint32_t cell_id);

#endif /* ANALYSIS_H_H */
