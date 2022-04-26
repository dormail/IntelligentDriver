/**
 * @file main.cpp
 * @author Matthias Maile (matthias.maile@udo.edu)
 * @brief Main function accepting arguments to start a multi lane idm simulation
 * @date 2022-04-26
 *
 */

#include <car.h>
#include <road.h>
#include <iostream>
#include <map>
//#include <pair>
#include <string>

/**
 * @brief Prints a help page
 *
 * It outputs some helpful information like pausible launch options
 *
 * @param programm_name The string used to execute this program
 */
void print_usage(std::string programm_name)
{
  std::cerr << "Usage: " << programm_name << " <option> <value>\n"
            << "Options:\n"
            << "\t-h, --help\t\t\tPrint this page\n"
            << "\t--road-length <value>\t\tSpecify the length of the road\n"
            << "\t--cars <value>\t\t\tSpecify the amount of cars\n"
            << "\t--lanes <value>\t\t\tSpecify the amount of lanes\n"
            << "\t--step-width <value>\t\tSpecify the integration step width\n"
            << "\t--integration-steps <value>\tSpecify the amount of integration steps\n"
            << "\t--output-csv <filename>\t\tSpecify the output file name for the csv\n"
            << std::endl;
}

/**
 * @brief A function parsing the command line arguments into usable options
 *
 * For the idm we have float and int like parameters and store them in different maps
 *
 * @param argc[in] Amount of command line arguments
 * @param argv[in] Command line arguments
 * @param params_float[out] map to store float arguments
 * @param params_uint[out]  map to store unsigned int arguments
 * @param params_str[out] map to store string like parameters
 * @return 0 everything good
 * @return -1 Help page - main should return 0;
 * @return -2 Something bad - main should return -1;
 */
int parse_arg(int argc, char *const argv[],
              std::map<std::string, float> &params_float,
              std::map<std::string, unsigned int> &params_uint,
              std::map<std::string, std::string> &params_str)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help")
    {
      print_usage(argv[0]);
      return -1;
    }

    // system parameters
    if (arg == "--road-length")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string length_str = argv[i + 1];
      std::pair<std::string, float> input("road_length", std::stof(length_str));
      params_float.insert(input);
    }
    // amount of cars
    if (arg == "--cars")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string cars_str = argv[i + 1];
      std::pair<std::string, unsigned int> input("cars", std::stoul(cars_str));
      params_uint.insert(input);
    }
    // amount of lanes
    if (arg == "--lanes")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string lanes_str = argv[i + 1];
      std::pair<std::string, unsigned int> input("lanes", std::stoul(lanes_str));
      params_uint.insert(input);
    }

    // parameters for time integration
    if (arg == "--step-width")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string step_width_str = argv[i + 1];
      std::pair<std::string, float> input("dt", std::stof(step_width_str));
      params_float.insert(input);
    }
    if (arg == "--integration-steps")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string steps_str = argv[i + 1];
      std::pair<std::string, unsigned int> input("steps", std::stoul(steps_str));
      params_uint.insert(input);
    }
    if (arg == "--output-csv")
    {
      if (i >= argc - 1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string dest_fn = argv[i + 1];
      std::pair<std::string, std::string> input("dest_fn", dest_fn);
      params_str.insert(input);
    }
  }
  return 0;
}

int main(int argc, char *const argv[])
{
  std::map<std::string, float> params_float;
  std::map<std::string, unsigned int> params_uint;
  std::map<std::string, std::string> params_str;

  int res = parse_arg(argc, argv, params_float, params_uint, params_str);

  if (res != 0)
  {
    return res;
  }

  unsigned int cars = 5, lanes = 3;
  float road_length = 200.;

  float dt = 1. / 30.;
  unsigned int steps = 30 * 60;
  std::string dest_fn = "build/output.csv";
  try
  {
    cars = params_uint.at("cars");
  }
  catch (std::out_of_range &err)
  {
    std::cerr << "No car number specified, defaulting to " << cars << std::endl;
  }
  try
  {
    lanes = params_uint.at("lanes");
  }
  catch (std::out_of_range &err)
  {
    std::cerr << "No lane number specified, defaulting to " << lanes << std::endl;
  }
  try
  {
    road_length = params_uint.at("road_length");
  }
  catch (std::out_of_range &err)
  {
    std::cerr << "No road length specified, defaulting to " << road_length << std::endl;
  }
  try
  {
    dt = params_float.at("dt");
  }
  catch (std::out_of_range &err)
  {
    std::cerr << "No time step specified, defaulting to " << dt << std::endl;
  }
  try
  {
    dt = params_uint.at("steps");
  }
  catch (std::out_of_range &err)
  {
    std::cerr << "No step count specified, defaulting to " << steps << std::endl;
  }
  try
  {
    dest_fn = params_str.at("dest_fn");
  }
  catch (std::out_of_range &err)
  {
    dest_fn = "build/output.csv";
    std::cerr << "No location for output csv specified, defaulting to " << dest_fn << std::endl;
  }

  MultiLaneRoad road(road_length, lanes, cars);
  road.euler_to_CSV(dt, steps, dest_fn);

  return 0;
}
