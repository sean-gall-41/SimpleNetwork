#!/home/sean/Dev/Scripts/Python/.venv/bin/python

"""
    File: raster.py
    Author: Sean Gallogly
    Original Author: Joe Whitley Casto
    Description:
        
           This file generates plots of per-cell rasters and PSTHs.
           The PSTHs are computed as the time-point average over
           rasters across all trials for that cell. The file takes in
           one argument, which is the binary file name (without path)
           that contains the cell-type's raster data.
        
    Usage:

        ./raster.py allGORaster.bin

    Addtl Notes:

        total cell numbers are determined via a dictionary in which the 
        keys are the file's basename. If you use a different naming convention
        for your files, make sure you reflect those changes in the cell_nums 
        dict below.

        The plots are saved to the ./output/imgs folder.

    CHANGELOG:

        (09/10/2022) - altered to take in a bit-packed file where spikes are packed into uint8s
"""

import os
import sys
import numpy as np
import scipy.stats as sts
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

INPUT_FILE_PATH = "./"
OUT_IMG_PATH    = "./"

PRE_CS_MS       = 0
POST_CS_MS      = 0
ISI = 1300 # Inter-stimulus interval *NOT* interspike interval!
TRIAL_LEN       = PRE_CS_MS + ISI + POST_CS_MS
NUM_TRIALS      = 1
RASTER_COL_SIZE = NUM_TRIALS * TRIAL_LEN

# parameters for smoothing via convolution with a Gaussian
FR_SMOOTH_MIN = -80
FR_SMOOTH_MAX = 80
FR_SMOOTH_MU = 0
FR_SMOOTH_SIGMA = 10

# dictionary of file basenames with corresponding cell nums
cell_nums = {
        "input_layer_raster.bin" : 8,
        "hidden_layer_raster.bin" : 64,
        "output_layer_raster.bin" : 16 
}

"""
    Description:

        calculates the instantaneous firing rates at
        the time points t_i for each i in the range [0, N),
        where N is the number of spikes in the input.

        The function first determines the inst firing rates
        for the isis (here interspike intervals :)), and then
        re-aligns these rates with the indices of the spikes
        in the input spike train. The output array of 
        firing rates if padded left and right with zeroes if
        necessary.

        One way to make sense of this algorithm is to construct
        an arbitrarily chosen spike train of a short length, call 
        it n. This is equivalent to choosing a random binary string
        of length n. Then, list out how this data is transformed in
        subsequent lines of the code.

"""
def calc_inst_fire_rates(spike_train):
    run_time = spike_train.size
    spk_times = np.nonzero(spike_train)[0]
    aligned_inst_fire_rates = []

    if spk_times.size == 0:
        return np.zeros(run_time)
    else:
        isi = np.diff(spk_times)
        inst_fire_rates = 1 / isi * 1000
        spike_train_proxy = spike_train[spk_times[0]+1:spk_times[-1]]
        count = 0
        for i in np.arange(spike_train_proxy.size):
            aligned_inst_fire_rates.append(inst_fire_rates[count]) 
            if spike_train_proxy[i] == 1:
                count += 1
        aligned_inst_fire_rates = np.array(aligned_inst_fire_rates)
        prepend = np.zeros(spk_times[0])
        aligned_inst_fire_rates = np.concatenate((prepend, aligned_inst_fire_rates))
        append = np.zeros(run_time - aligned_inst_fire_rates.size)
        aligned_inst_fire_rates = np.concatenate((aligned_inst_fire_rates, append))
        return aligned_inst_fire_rates


if __name__ == '__main__':

    layer_rast_filenames = [ 
                            "input_layer_raster.bin",
                            "hidden_layer_raster.bin",
                            "output_layer_raster.bin"
                           ]

    for file in layer_rast_filenames:
        layer_num_cells = cell_nums[file]
        raw_data = None
        try:
            with open(file, 'rb') as raster_fd:
                raw_data = np.fromfile(raster_fd, np.ubyte) 
                raster_fd.close()
        except FileNotFoundError:
            print(f"[ERROR] '{raster_file_name}' could not be opened.")
            print('[ERROR]: Exiting...')
            sys.exit(3)

        # compute the gaussian which will be convolved with the PSTH to smooth it
        smooth_range = np.arange(FR_SMOOTH_MIN, FR_SMOOTH_MAX + 1, 1)
        smooth_gaussian = sts.norm.pdf(smooth_range, FR_SMOOTH_MU, FR_SMOOTH_SIGMA)

        raw_data = raw_data.reshape((TRIAL_LEN, layer_num_cells))
        rasters  = raw_data.transpose()

        rast_trials_x = np.zeros((layer_num_cells, TRIAL_LEN))
        rast_trials_y = np.ones((layer_num_cells, TRIAL_LEN))

        # prep the raster data for plotting
        for i in np.arange(layer_num_cells):
            rast_trials_y[i,:] *= i
            for j in np.arange(TRIAL_LEN):
                rast_trials_x[i, j] = rasters[i, j] * j

        rast_trials_x = rast_trials_x.reshape((layer_num_cells * TRIAL_LEN)) 
        rast_trials_y = rast_trials_y.reshape((layer_num_cells * TRIAL_LEN)) 
        
        rast_trials_x[rast_trials_x == 0] = np.nan

        # plot the PSTH for this cell
        fig = plt.figure()
        # now do the raster
        ax_1 = plt.subplot(111)
        plt.scatter(rast_trials_x, rast_trials_y, 0.5, 'k')
        plt.xlim((50, TRIAL_LEN - 50)) 
        plt.ylim((-1, layer_num_cells+1))
        ax_1.set_xlabel('cell number', fontsize=12)
        ax_1.set_ylabel('trial number', fontsize=12)
  
        # print which cell we've finished plotting and save to file
        #out_img_file_name = OUT_IMG_PATH + file_base_name + "_" + str(cell_id) + ".png" 
        #print(f"[INFO]: Saving cell '{cell_id}' to file...")
        #plt.savefig(out_img_file_name, format='png', bbox_inches='tight')
        plt.show()
        plt.close(fig)

