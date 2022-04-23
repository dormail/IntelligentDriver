/**
 * @file road.h
 * @author your name (matthias.maile@udo.edu)
 * @brief Classes which implement IDM
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ROAD_H
#define ROAD_H

#include <car.h>
#include <vector>
#include <random>

/**
 * @brief A class implementing the IDM on a single lane
 * 
 */
class OneLaneRoad {
protected:
  std::vector<Car> cars;
  float length = 1000.;
  std::default_random_engine generator;

public:
  OneLaneRoad(unsigned int const car_num, float const len);
  ~OneLaneRoad() {}

  // set methods
  void congestion_at_start(); // create a traffic at the start of the road
  void desired_speed_gaussian(float const mean, float const stddev);

  // get methods
  unsigned int car_number();
  float location(unsigned int const car_index);
  float velocity(unsigned int const car_index);
  float distance_front(unsigned int const car_index);

  // diff eq solvers
  void constant_speed(float const dt);
  void RK4(float const dt);
  void euler(float const dt);

  void location_enforce_boundries();
};

/**
 * @brief A class generalizing the IDM to multiple lanes
 * 
 */
class MultiLaneRoad : public OneLaneRoad {
protected:
  unsigned int lane_num = 3;
  float politeness_factor = .5;
  float switching_threshhold = 0.;

public:
  MultiLaneRoad(float length, unsigned int const lane_num, unsigned int const car_num);
  ~MultiLaneRoad() {}

  // set methods
  void congestion_at_start(); // create a traffic at the start of the road

  // get methods
  unsigned int lane(unsigned int const car_index);
  unsigned int car_in_front(unsigned int const car_index); // returns an index
  Car& car_in_front(Car& car); // returns a reference

/* car1 is the car that is supposed to be in the back so
 * ------car1---car2-------------- should return ---
 */
  float distance(const Car& car1, const Car& car2);
  float distance(unsigned int const car_index1, unsigned int const car_index2) {
      return distance(cars[car_index1], cars[car_index2]);
  }

  // diff eq
  void euler(float const dt);

  // car dynamics
  int change_lane(Car& car, int const lane_change);
  bool should_change(Car& car, unsigned int const lane);

  float acceleration_car(const Car& car, const Car& car_front);
};

/* params:
 *  v        velocity of vehicle
 *  v_next   velocity of vehicle in fron
 *  distance net distance to vehicle in front
 *
 *  Car specific:
 *  v_max    desired velocity
 *  s_min    minimum spacing to vehicle in front
 *  T        safe time head away
 *  a_max    max velocity
 *  b        comfortable breaking
 */
float acceleration(float v, float v_next, float distance, float v_max, float s_min, float T, float a_max, float b);

#endif
