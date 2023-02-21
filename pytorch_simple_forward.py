#!/usr/bin/python

import os
import sys
import json
import numpy as np

import torch
from torch import nn
from torch import optim

import matplotlib as mpl
import matplotlib.pyplot as plt

SIM_PARMS_FN = "simulation_params.json"

class DeepNet(nn.Module):
    def __init__(self, n_inputs, n_hidden, m_outputs):
        super().__init__()
        self.in_layer = nn.Linear(n_inputs, n_hidden)
        self.out_layer = nn.Linear(n_hidden, m_outputs)

    def forward(self, r):
        h = self.in_layer(r)
        y = self.out_layer(h)
        return y

class DeepNetReLU(nn.Module):
    def __init__(self, n_inputs, n_hidden, m_outputs):
        super().__init__()
        self.in_layer = nn.Linear(n_inputs, n_hidden)
        self.out_layer = nn.Linear(n_hidden, 1)
        self.out_layer = nn.Linear(n_hidden, m_outputs)
    
    def forward(self, r):
        h = torch.relu(self.in_layer(r))
        y = self.out_layer(h)
        return y

def get_net_params_from_file(json_fn):
    try:
        with open(json_fn, "r") as fd:
            json_data = json.load(fd)
            return json_data
    except FileNotFoundError:
        print(f"[ERROR] '{json_fn}' could not be opened.")
        print('[ERROR]: Exiting...')
        sys.exit(1)


if __name__ == '__main__':
    np.random.seed(0)
    torch.manual_seed(0) 

    net_params = get_net_params_from_file(SIM_PARMS_FN)['network']

    network = DeepNetReLU(net_params['input_layer']['num_cells'],
                          net_params['hidden_layer_1']['num_cells'],
                          net_params['output_layer']['num_cells'])
    
    input = np.random.rand(net_params['input_layer']['num_cells'])
    input_tensor = torch.tensor(input, dtype=torch.float32)
    print(input_tensor)
    output_tensor = network.forward(input_tensor)
    print(output_tensor)

