#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <armadillo>
#include <matplot/matplot.h>

#include "analysis.h"

const float FR_SMOOTH_MU = 0;
const float FR_SMOOTH_SUGMA = 100;

typedef arma::Col<uint8_t> u8_cvec_t;
typedef arma::Row<uint8_t> u8_rvec_t;
typedef arma::Col<uint32_t> u32_cvec_t;
typedef arma::Row<uint32_t> u32_rvec_t;
typedef arma::Col<float> f32_cvec_t;
typedef arma::Row<float> f32_rvec_t;
typedef arma::Mat<float> f32_mat_t;
typedef arma::Mat<uint8_t> u8_mat_t;
typedef arma::Mat<uint32_t> u32_mat_t;

void calc_smooth_inst_fire_rates(
    const u8_cvec_t &spike_train,
    const uint32_t n_time_bins
)
{
  // all of below is from old method where compute fr (1 / isi type thing) then smooth

  //u8_cvec_t spike_train_arma_col(spike_train, n_time_bins);

  //u8_cvec_t spike_times = arma::nonzeros(spike_train_arma_col);

  //if (spike_times.n_rows == 0)
  //{
  //  float *zero_spike_train = (float *)calloc(n_time_bins, sizeof(float));
  //  if (zero_spike_train) return zero_spike_train;
  //  else
  //  {
  //    fprintf(stderr, "%s: %s: %d: Failed to allocate zero spike train memory\n",
  //            __FILE__, __func__, __LINE__);
  //    exit(1);
  //  }
  //}
  //f32_cvec_t inst_fire_rates = arma::conv_to<f32_cvec_t>::from(arma::diff(spike_times));
  //inst_fire_rates.transform([](float isi) { return (1.0 / isi) * 1000.0; });
}

// there may be a bug here. debug pls and thanks
void plot_rasters(uint8_t *raster, uint32_t trial_len, uint32_t num_cells)
{
  // slower, but copy the data to the local variable
  u8_mat_t raster2D(raster, num_cells, trial_len);

  u32_mat_t rast_y = arma::ones<u32_mat_t>(num_cells, trial_len);
  u32_cvec_t cell_indices = arma::regspace<u32_cvec_t>(0, num_cells-1);
  u32_rvec_t ts_indices = arma::regspace<u32_rvec_t>(0, trial_len-1);

  rast_y.each_col() %= cell_indices;
  
  u32_mat_t rast_x = arma::conv_to<u32_mat_t>::from(raster2D);
  rast_x.each_row() %= ts_indices;

  // flatten by concatenating rows, aka concatenating rasters for each cell
  u32_rvec_t rast_x_flattened = vectorise(rast_x, 1);
  u32_rvec_t rast_y_flattened = vectorise(rast_y, 1);
  std::vector<double> rast_x_flattened_stl(rast_x_flattened.begin(), rast_x_flattened.end());
  std::vector<double> rast_y_flattened_stl(rast_y_flattened.begin(), rast_y_flattened.end());

  // weirdly re-renders after every single matplot lib call...huh.
  using namespace matplot;
  auto f = figure(false);
  auto ax = f->current_axes();
  auto scatman = ax->scatter(rast_x_flattened_stl, rast_y_flattened_stl, 1.0);
  scatman->marker_color("k");
  ax->xlim({50, trial_len - 50});
  ax->ylim({-1, num_cells + 1});
  ax->xlabel("time step (ms)");
  ax->ylabel("cell number");
  ax->x_axis().label_font_size(12);
  ax->y_axis().label_font_size(12);
  show(); // doesn't actually show, just causes program to wait here ???
}

/*
 * Computes the mean firing rate across cells and then plots the result
 * keep in mind that given the variance across cells, this gives a rather noisy result.
 * could smooth again, but I don't think it would help -> maybe computing fr in two step
 * method could be a better approach? ns
 */
void plot_mean_inst_fire_rate(uint8_t *raster, uint32_t trial_len, uint32_t num_cells)
{
  int32_t gauss_filt_width = trial_len / 2;
  int32_t fr_smooth_min = -gauss_filt_width / 2;
  int32_t fr_smooth_max = gauss_filt_width / 2;

  f32_rvec_t smooth_range = arma::regspace<f32_rvec_t>(
      fr_smooth_min,
      1,
      fr_smooth_max
      );
  f32_rvec_t smooth_gauss_filt = arma::normpdf(
      smooth_range,
      FR_SMOOTH_MU,
      FR_SMOOTH_SUGMA
      );

  u8_mat_t raster2D(raster, num_cells, trial_len);
  f32_mat_t raster2DFloat = arma::conv_to<f32_mat_t>::from(raster2D);
  raster2DFloat.each_row([trial_len, &smooth_gauss_filt](f32_rvec_t &in_row){
    f32_rvec_t fr_est = arma::conv(in_row,
                                   smooth_gauss_filt,
                                   "same");
    float spike_train_mean = arma::sum(in_row) / trial_len;
    float fr_est_mean = arma::mean(fr_est);
    float scale_factor = spike_train_mean / fr_est_mean;
    fr_est *= scale_factor;
    return fr_est;
  });
  f32_rvec_t mean_fr = arma::mean(raster2DFloat);
  std::vector<double> plottable_mean_fr(mean_fr.begin(), mean_fr.end());

  using namespace matplot;

  // weirdly re-renders after every single matplot lib call...huh.
  auto f = figure(false);
  auto ax = f->current_axes();
  auto plotman = ax->plot(plottable_mean_fr, "k-");
  ax->xlim({50, trial_len - 50});
  ax->ylim({0.0, mean_fr.max() + 0.2 * arma::range(mean_fr)});
  ax->xlabel("time step (ms)");
  ax->ylabel("Firing Rate (Hz)");
  ax->x_axis().label_font_size(12);
  ax->y_axis().label_font_size(12);
  show(); // doesn't actually show, just causes program to wait here ???
}

void plot_inst_fire_rate_cell(uint8_t *raster, uint32_t trial_len, uint32_t num_cells, uint32_t cell_id)
{
  int32_t gauss_filt_width = trial_len / 2;
  int32_t fr_smooth_min = -gauss_filt_width / 2;
  int32_t fr_smooth_max = gauss_filt_width / 2;

  f32_rvec_t smooth_range = arma::regspace<f32_rvec_t>(
      fr_smooth_min,
      1,
      fr_smooth_max
      );
  f32_rvec_t smooth_gauss_filt = arma::normpdf(
      smooth_range,
      FR_SMOOTH_MU,
      FR_SMOOTH_SUGMA
      );

  u8_mat_t raster2D(raster, num_cells, trial_len);
  u8_rvec_t cell_raster = raster2D.row(cell_id);
  
  f32_rvec_t cell_raster_float = arma::conv_to<f32_rvec_t>::from(cell_raster);

  f32_rvec_t fr_est = arma::conv(cell_raster_float,
                                 smooth_gauss_filt,
                                 "same");
  float spike_train_mean = arma::sum(cell_raster_float) / trial_len;
  float fr_est_mean = arma::mean(fr_est);
  float scale_factor = spike_train_mean / fr_est_mean;
  fr_est *= scale_factor;
  std::vector<double> plottable_fr(fr_est.begin(), fr_est.end());
  // weirdly re-renders after every single matplot lib call...huh.
  //
  using namespace matplot;
  auto f = figure(false);
  auto ax = f->current_axes();
  auto plotman = ax->plot(plottable_fr, "k-");
  ax->xlim({50, trial_len - 50});
  ax->ylim({0.0, fr_est.max() + 0.5 * arma::range(fr_est)});
  ax->xlabel("time step (ms)");
  ax->ylabel("Firing Rate (Hz)");
  ax->x_axis().label_font_size(12);
  ax->y_axis().label_font_size(12);
  show(); // doesn't actually show, just causes program to wait here ???
}

void plot_raster_cell(uint8_t *raster, uint32_t trial_len, uint32_t num_cells, uint32_t cell_id)
{
  // slower, but copy the data to the local variable
  u8_mat_t raster2D(raster, num_cells, trial_len);
  u8_rvec_t cell_raster = raster2D.row(cell_id);

  f32_rvec_t arma_raster_float = arma::conv_to<f32_rvec_t>::from(cell_raster);
  
  f32_rvec_t ts_indices = arma::regspace<f32_rvec_t>(0, trial_len-1);
  arma_raster_float %= ts_indices;

  std::vector<double> plottable_raster(arma_raster_float.begin(), arma_raster_float.end());
  std::vector<double> ys(trial_len);

  using namespace matplot;
  auto f = figure(false);
  auto ax = f->current_axes();
  auto scatman = ax->scatter(plottable_raster, ys, 1.0);
  scatman->marker_color("k");
  ax->xlim({50, trial_len - 50});
  ax->ylim({-0.5, 0.5});
  ax->xlabel("time step (ms)");
  ax->x_axis().label_font_size(12);
  show(); // doesn't actually show, just causes program to wait here ???
}
