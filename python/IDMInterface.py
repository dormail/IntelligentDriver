""" IDMInterface.py

creates python functions to use the multi lane IDM binary I wrote in C++
"""

import os
import pandas as pd



BUILD_DIR = "build"

## A function acting as a interface between python code and the multi lane idm implementation
# Special care needs to be addressed for the directory structure, so BUILD_DIR and IDMBinaryName
# @return [filename, df] filename is the name of the CSV file generated, df is the dataframe with the data
def IDMSimulation(road_length=200, cars=5, lanes=3, step_width=1/30, steps=1800, IDMBinaryName = "./build/main"):
    filename = BUILD_DIR + "/" + str(road_length) + "_" + str(cars) + "_" + str(lanes) + "_" + str(step_width) + "_" + str(steps) + ".csv"

    options = " --road-length " + str(road_length)
    options = options + " --cars " + str(cars)
    options = options + " --lanes " + str(lanes)
    options = options + " --step-width " + str(step_width)
    options = options + " --integration-steps " + str(steps)
    options = options + " --output-csv " + str(filename)

    #print(IDMBinaryName + options)

    os.system(IDMBinaryName + options)
    df = pd.read_csv(filename, index_col=False)
    return [filename, df]


