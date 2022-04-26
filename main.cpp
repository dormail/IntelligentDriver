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
  std::cerr << "Usage: " << programm_name << " <options>\n"
            << "Options:\n"
            << "\t-h, --help\t\tPrint this page\n"
            << "\t--road-length <value>\tSpcify the length of the road\n"
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
 * @return 0 everything good
 * @return -1 Help page - main should return 0;
 * @return -2 Something bad - main should return -1;
 */
int parse_arg(int argc, char *const argv[], std::map<std::string, float> &params_float, std::map<std::string, unsigned int> &params_uint)
{
  //if (argc < 5)
  //{
  //  std::cerr << "Not enough arguments provided\n";
  //  print_usage(argv[0]);
  //  return -2;
  //}

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help")
    {
      print_usage(argv[0]);
      return -1;
    }

    // road length
    if (arg == "--road-length")
    {
      if (i >= argc-1)
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
      if (i >= argc-1)
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
      if (i >= argc-1)
      {
        print_usage(argv[0]);
        return -2;
      }
      std::string lanes_str = argv[i + 1];
      std::pair<std::string, unsigned int> input("lanes", std::stoul(lanes_str));
      params_uint.insert(input);
    }
  }
  return 0;
}

int main(int argc, char *const argv[])
{
  std::map<std::string, float> params_float;
  std::map<std::string, unsigned int> params_uint;

  int res = parse_arg(argc, argv, params_float, params_uint);

  if (res != 0)
  {
    return res;
  }

  unsigned int cars = 5, lanes = 3;
  float road_length = 200.;
  try
  {
    cars = params_uint.at("cars");
  }
  catch(std::out_of_range &err)
  {
    std::cerr << "No car number specified, defaulting to " << cars << std::endl;
  }
  try
  {
    lanes = params_uint.at("lanes");
  }
  catch(std::out_of_range &err)
  {
    std::cerr << "No lane number specified, defaulting to " << lanes << std::endl;
  }
  try
  {
    road_length = params_uint.at("road_length");
  }
  catch(std::out_of_range &err)
  {
    std::cerr << "No road length specified, defaulting to " << road_length << std::endl;
  }

  MultiLaneRoad road(road_length, lanes, cars);

  return 0;
}
