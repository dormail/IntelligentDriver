/* road.cpp */

#include <road.h>
#include <car.h>
#include <vector>
#include <assert.h>
#include <cmath> // pow


#include <iostream>

MultiLaneRoad::MultiLaneRoad(float length, unsigned int const lane_num, unsigned int const car_num) :
  OneLaneRoad(car_num, length), lane_num(lane_num) {
    congestion_at_start();
  }

void MultiLaneRoad::congestion_at_start() {
  unsigned int current_lane = 0;
  float* location = new float[lane_num]();

  for (unsigned int i = 0; i < car_number(); ++i) {
    cars[i].velocity = 0;
    cars[i].location = location[current_lane];
    cars[i].lane = current_lane;

    location[current_lane] += cars[i].length;
    location[current_lane] += cars[i].min_distance;

    ++current_lane;
    current_lane = (current_lane == lane_num) ? 0 : current_lane;
  }
  delete[] location;
}

unsigned int MultiLaneRoad::lane(unsigned int const car_index) {
  return cars[car_index].lane;
}

unsigned int MultiLaneRoad::car_in_front(unsigned int const car_index) {
  unsigned int closest = 0;
  float best_distance = length;
  float tmp_dist;
  for (unsigned int i = 0; i < car_number(); ++i) {
    if (cars[i].lane != cars[car_index].lane)
      continue;
    tmp_dist = distance(cars[car_index], cars[i], length);
    if (tmp_dist < best_distance) {
      best_distance = tmp_dist;
      closest = i;
    }
  }
  return closest;
}

void MultiLaneRoad::euler(float const dt) {
  unsigned int const car_num = car_number();
  unsigned int car_front;
  float accel;
  for(unsigned int i = 0; i<car_num; ++i) {
    cars[i].location += cars[i].velocity * dt;

    car_front = car_in_front(i);

    accel = acceleration(velocity(i), velocity(car_front), distance(cars[i], cars[car_front], length),
        cars[i].desired_velocity, 
        cars[i].min_distance, 
        cars[i].safe_time_headaway, 
        cars[i].max_acceleration,
        cars[i].comfortable_deceleration);
    cars[i].velocity += accel * dt;
    location_enforce_boundries();
  }
}

OneLaneRoad::OneLaneRoad(unsigned int const car_num, float const len) : cars(car_num), length(len)  {
  assert(cars.size() == car_num);

  for(unsigned int i = 0; i<car_num; ++i) {
    cars[i].velocity = 10.;
    cars[i].location = i * float(len / car_num);
  }
}

void OneLaneRoad::congestion_at_start() {
  float location = 0;

  for (auto &element : cars) {
    element.location = location;
    element.velocity = 0;

    location += element.length;
    location += element.min_distance;
  }
}

unsigned int OneLaneRoad::car_number() {
  return cars.size();
}

void OneLaneRoad::constant_speed(float const dt) {
  for (auto &element : cars) {
    element.location += + element.velocity * dt;
  }
  location_enforce_boundries();
}

void OneLaneRoad::euler(float const dt) {
  unsigned int const car_num = car_number();
  float accel;
  for(unsigned int i = 0; i<car_num; ++i) {
    cars[i].location += cars[i].velocity * dt;
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
    accel = acceleration(velocity(i), velocity(i + 1), distance_front(i),
        cars[i].desired_velocity, 
        cars[i].min_distance, 
        cars[i].safe_time_headaway, 
        cars[i].max_acceleration,
        cars[i].comfortable_deceleration);
    cars[i].velocity += accel * dt;
    location_enforce_boundries();
  }
}

float OneLaneRoad::velocity(unsigned int const car_index)
{
  assert(car_index < car_number() + 1);
  if (car_index == car_number()) return cars[0].velocity;
  return cars[car_index].velocity;
}

float OneLaneRoad::location(unsigned int const car_index)
{
  assert(car_index < car_number());
  return cars[car_index].location;
}

float OneLaneRoad::distance_front(unsigned int const car_index)
{
  assert(car_index < car_number());
  unsigned int next_index = (car_index < car_number() - 1) ? car_index + 1 : 0;

  float distance = cars[next_index].location - cars[car_index].location;
  if(distance < 0) distance += length;

  distance -= cars[next_index].length / 2;
  distance -= cars[car_index].length / 2;

  return distance;
}

void OneLaneRoad::location_enforce_boundries()
{
  for (auto &element : cars)
  {
    while (element.location > length)
      element.location -= length;
  }
}

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
float acceleration(float v, float v_next, float distance,
    float v_max, float s_min, float T, float a_max, float b)
{
  float s = s_min + v * T + v * (v - v_next) / (2 * sqrt(a_max * b));
  return a_max * (1 - pow(v / v_max, 4) - pow(s / distance, 2));
}

/* car1 is the car that is supposed to be in the back so
 * ------car1---car2-------------- should return ---
 */
float distance(Car &car1, Car &car2, float const length) {
  float distance = car2.location - car1.location;
  if(distance < 0) distance += length;

  distance -= car1.length / 2;
  distance -= car2.length / 2;

  assert(distance < length);

  if (distance < 0) {
    std::cerr << "distance < 0, locs:\n" << car2.location << " " << car1.location << '\n';
    std::cerr << "lanes: " << car2.lane << " " << car1.lane << '\n';
  }

  assert(distance >= 0);

  return distance;
}

