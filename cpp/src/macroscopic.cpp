/**
 * @file macroscopic.cpp
 * @author Matthias Maile (matthias.maile@udo.edu)
 * @brief
 * @version 0.1
 * @date 2022-04-29
 *
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
            << "Not so much is implemented\n"
            << "In future this should simulate the system and save macroscopic variables"
            << std::endl;
}


int main(int argc, char *const argv[])
{
  MultiLaneRoad road(7000., 3, 100);
  road.MOBIL_all_cars(120 * .278, 120 * .2 * .278);
  road.fill_right_lanes();

  std::cout << "t,v_ave\n";

  float dt = 1/30.;
  float time = 0.;
  float time_end = 180.;
  for (unsigned int i = 0; time < time_end; ++i)
  {
    std::cout << time << ','
              << road.average_speed()
              << '\n';


    road.euler(dt);
    time += dt;
  }

  return 0;
}
