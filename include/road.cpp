/* road.cpp */

#include <road.h>
#include <car.h>
#include <vector>
#include <assert.h>
#include <cmath> // pow

#include <iostream>

MultiLaneRoad::MultiLaneRoad(float length, unsigned int const lane_num, unsigned int const car_num) : OneLaneRoad(car_num, length), lane_num(lane_num)
{
  congestion_at_start();
}

void MultiLaneRoad::congestion_at_start()
{
  unsigned int current_lane = 0;
  float *location = new float[lane_num]();

  for (unsigned int i = 0; i < car_number(); ++i)
  {
    cars[i].velocity = 0.;
    cars[i].location = location[current_lane];
    cars[i].lane = current_lane;

    location[current_lane] += cars[i].length;
    location[current_lane] += cars[i].min_distance;

    ++current_lane;
    current_lane = (current_lane == lane_num) ? 0 : current_lane;
  }
  delete[] location;
}

unsigned int MultiLaneRoad::lane(unsigned int const car_index)
{
  return cars[car_index].lane;
}

// returns an index
unsigned int MultiLaneRoad::car_in_front(unsigned int const car_index)
{
  unsigned int closest = 0;
  float best_distance = length;
  float tmp_dist;
  for (unsigned int i = 0; i < car_number(); ++i)
  {
    if (i == car_index)
      continue;
    if (cars[i].lane != cars[car_index].lane)
      continue;
    tmp_dist = distance(car_index, i);
    if (tmp_dist < best_distance)
    {
      best_distance = tmp_dist;
      closest = i;
    }
  }
  return closest;
}

Car &MultiLaneRoad::car_in_front(Car &car)
{
  Car *closest = &car;
  float min_distance = length;

  for (auto &iter : cars)
  {
    if (&iter == &car)
      continue;
    if (iter.lane != car.lane)
      continue;
    float tmp_dist = distance(car, iter);
    if (tmp_dist < min_distance)
    {
      min_distance = tmp_dist;
      closest = &iter;
    }
  }
  return *closest;
}

/* car1 is the car that is supposed to be in the back so
 * ------car1---car2-------------- should return ---
 */
float MultiLaneRoad::distance(const Car &car1, const Car &car2)
{
  if (&car1 == &car2)
    return length - car1.length;

  float distance = car2.location - car1.location;
  if (distance < 0)
    distance += length;

  distance -= car1.length / 2;
  distance -= car2.length / 2;

  if (distance < 0)
  {
    std::cerr << "distance: " << distance << '\n';
    std::cerr << "locs: " << car1.location << " " << car2.location << '\n';
    std::cerr << "lanes: " << car1.lane << " " << car2.lane << '\n';
  }

  if(distance < 0) throw(-1);

  assert(distance >= 0);
  assert(distance < length);
  return distance;
}

OneLaneRoad::OneLaneRoad(unsigned int const car_num, float const len) : cars(car_num), length(len), generator()
{
  assert(cars.size() == car_num);

  for (unsigned int i = 0; i < car_num; ++i)
  {
    cars[i].velocity = 10.;
    cars[i].location = i * float(len / car_num);
  }
}

void OneLaneRoad::congestion_at_start()
{
  float location = 0;

  for (auto &element : cars)
  {
    element.location = location;
    element.velocity = 0;

    location += element.length;
    location += element.min_distance;
  }
}

void OneLaneRoad::desired_speed_gaussian(float const mean, float const stddev)
{
  std::normal_distribution<double> distribution(mean, stddev);

  for (auto &car : cars)
  {
    car.desired_velocity = distribution(generator);
  }
}

unsigned int OneLaneRoad::car_number()
{
  return cars.size();
}

void OneLaneRoad::constant_speed(float const dt)
{
  for (auto &element : cars)
  {
    element.location += +element.velocity * dt;
  }
  location_enforce_boundries();
}

void OneLaneRoad::euler(float const dt)
{
  unsigned int const car_num = car_number();
  float accel;
  for (unsigned int i = 0; i < car_num; ++i)
  {
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
  if (car_index == car_number())
    return cars[0].velocity;
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
  if (distance < 0)
    distance += length;

  distance -= cars[next_index].length / 2;
  distance -= cars[car_index].length / 2;

  return distance;
}

/**
 * @brief Enforces the boundries so resets cars to the start when they go beyond length
 * 
 */
void OneLaneRoad::location_enforce_boundries()
{
  for (auto &element : cars)
  {
    while (element.location > length)
      element.location -= length;
  }
}

/**
 * @brief A method doing one integration step on the system with Euler's method
 * 
 * @param dt Time step in seconds
 */
void MultiLaneRoad::euler(float const dt)
{
  Car *car_front_ptr;
  float accel;

  for (auto &iter : cars)
  {
    iter.location += iter.velocity * dt;

    car_front_ptr = &(car_in_front(iter));

    accel = acceleration(iter.velocity, car_front_ptr->velocity, distance(iter, *car_front_ptr),
                         iter.desired_velocity,
                         iter.min_distance,
                         iter.safe_time_headaway,
                         iter.max_acceleration,
                         iter.comfortable_deceleration);
    iter.velocity += accel * dt;
    location_enforce_boundries();
  }
}

/**
 * @brief Changes a car's lane, checking if parameters allow
 * 
 * @param car[in, out] The car the lane change should be applied to
 * @param lane_change[in] Lane change direction +1=left -1=left
 * 
 * @return 0 if it worked, -1 if it fails
 */
int MultiLaneRoad::change_lane(Car& car, int const lane_change)
{
  if (lane_change != 1 && lane_change != -1)
    return -1; // bad param

  if (lane_change == 1 && car.lane == lane_num)
    return -1; // car already on the far left

  if (lane_change == -1 && car.lane == 0)
    return -1; // car already on the far right

  car.lane += lane_change;
  return 0;
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

