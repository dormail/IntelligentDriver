""" IDMInterface.py

creates python functions to use the multi lane IDM binary I wrote in C++
"""

import os
import pandas as pd


IDMBinaryName = "./build/main"
BUILD_DIR = "build"

def IDMSimulation(road_length=200, cars=5, lanes=3, step_width=1/30, steps=1800):
  filename = BUILD_DIR + "/" + str(road_length) + "_" + str(cars) + "_" + str(lanes) + "_" + str(step_width) + "_" + str(steps) + ".csv"

  options = " --road-length " + str(road_length)
  options = options + " --cars " + str(cars)
  options = options + " --lanes " + str(lanes)
  options = options + " --step-width " + str(step_width)
  options = options + " --integration-steps " + str(steps)
  options = options + " --output-csv " + str(filename)

  print(IDMBinaryName + options)

  os.system(IDMBinaryName + options)
  df = pd.read_csv(filename)
  return df

IDMSimulation()

