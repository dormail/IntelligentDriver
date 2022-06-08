""" IDMInterface.py

creates python functions to use the multi lane IDM binary I wrote in C++
"""

import os
from os.path import exists
import pandas as pd




## A function acting as a interface between python code and the multi lane idm implementation
# Special care needs to be addressed for the directory structure, so BUILD_DIR and IDMBinaryName
# @return [filename, df] filename is the name of the CSV file generated, df is the dataframe with the data
def IDMSimulation(road_length=200, cars=5, lanes=3, step_width=1/30, steps=1800, IDMBinaryName = "microscopicIDM", EU=False,
                 force_rerun=True):
    data_dir = '/tmp/IDM'
    filename = data_dir + "/" + str(road_length) + "_" + str(cars) + "_" + str(lanes) + "_" + str(step_width) + "_" + str(steps) 
    if EU:
        filename = filename + "_EU"
    filename = filename + ".csv"

    options = " --road-length " + str(road_length)
    options = options + " --cars " + str(cars)
    options = options + " --lanes " + str(lanes)
    options = options + " --step-width " + str(step_width)
    options = options + " --integration-steps " + str(steps)
    options = options + " --output-csv " + str(filename)
    
    if EU:
        options = options + " --EU "

    if (exists(filename) and not force_rerun):
        print(f'{filename} already exists')
        print(f'If it shall be rerun specify in arguments or run\n{IDMBinaryName + options}')
        df = pd.read_csv(filename, index_col=False)
        return [filename, df]
    
    print(IDMBinaryName + options)
    os.system('mkdir -p ' + data_dir)
    os.system(IDMBinaryName + options)
    df = pd.read_csv(filename, index_col=False)
    return [filename, df]


