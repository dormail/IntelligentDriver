/* road.cpp */

#include <road.h>
#include <car.h>
#include <vector>
#include <assert.h>
#include <cmath> // pow

#include <iostream>

/**
 * @brief Construct a new Multi Lane Road:: Multi Lane Road object
 * 
 * Sets the cars to congestion_at_start()
 * 
 * @param length length of the road
 * @param lane_num amount of lanes
 * @param car_num amount of cars
 */
MultiLaneRoad::MultiLaneRoad(float length, unsigned int const lane_num, unsigned int const car_num) : OneLaneRoad(car_num, length), lane_num(lane_num)
{
  congestion_at_start();
}

/**
 * @brief Creates a traffice around location = 0
 * 
 * Sets the location of cars as low as possible (as low as min_distance allows)
 * 
 */
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

void MultiLaneRoad::fill_right_lanes()
{
  float location = 0;
  unsigned int lane = 0;

  for (auto& iter : cars)
  {
    assert(lane < lane_num);
    assert(location < length);

    iter.velocity = 0;
    iter.location = location;
    iter.lane = lane;

    location += iter.length;
    location += iter.min_distance;

    if (location >= length)
    {
      location = 0;
      ++lane;
    }
  }
}

/**
 * @brief returns cars[car_index].lane
 * 
 * @param car_index index of car
 * @return unsigned int lane of cars[car_index]
 */
unsigned int MultiLaneRoad::lane(unsigned int const car_index)
{
  return cars[car_index].lane;
}

/**
 * @brief returns the index of the car in front of a specfic car
 * 
 * @param car_index index of the car 
 * @return unsigned int index of the car in front
 */
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

/**
 * @brief gets the car in front of Car& car
 * 
 * @param car Car the returned car should be infront of
 * @return Car& reference to the car in front
 */
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

/**
 * @brief returns the distance between two cars
 * 
 * The method is sensitive to the order specifically
 * distance(car1, car2) = length - distance(car2, car1)
 * 
 * Generally car1 is though to be the car in the back,
 * ------car1---car2-------------- should return ---
 * 
 * 
 * @param car1 car in the back
 * @param car2 car in the front
 * @return float the distance between the two
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

  // if (distance < 0)
  //{
  //   std::cerr << "distance: " << distance << '\n';
  //   std::cerr << "locs: " << car1.location << " " << car2.location << '\n';
  //   std::cerr << "lanes: " << car1.lane << " " << car2.lane << '\n';
  // }

  // assert(distance >= 0);
  // assert(distance < length);
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

/**
 * @brief gets the amount of cars
 * 
 * @return unsigned int amount of cars = cars.size()
 */
unsigned int OneLaneRoad::car_number()
{
  return cars.size();
}

/**
 * @brief makes a time integration where the speed doesnt change
 * 
 * @param dt time step in seconds
 */
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

/**
 * @brief Returns the velocity indexed by car_index
 *
 * @param car_index
 * @return cars[car_index].velocity
 */
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
    assert(iter.lane < lane_num);

    // check for lane change possibility
    if (should_change(iter, iter.lane + 1))
    {
      iter.lane++;
      std::cerr << "Lane change\n";
    }
    else
    {
      if (should_change(iter, iter.lane - 1) && iter.lane > 0)
      {
        iter.lane--;
        std::cerr << "Lane change\n";
      }
    }

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
 * It does not check if it fits at this spot, only if the lane exists.
 *
 * @param car[in, out] The car the lane change should be applied to
 * @param lane_change[in] Lane change direction +1=left -1=left
 *
 * @return 0 if it worked, -1 if it fails
 */
int MultiLaneRoad::change_lane(Car &car, int const lane_change)
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

/**
 * @brief A method calculating if a car should change a lane
 *
 *  It uses the incentive criterion from some paper and also checks if the params are correct.
 *
 * @param car car to change lane, using its location, speed, etc.
 * @param lane lane where the change shall be checked
 * @return true  car can safely change
 * @return false  car can not safely change or bad params
 */
bool MultiLaneRoad::should_change(Car &car, unsigned int const lane)
{
  if (lane >= lane_num)
  {
    // std::cerr << "Specified to high lane number " << lane << '\n';
    return false;
  }
  if (lane != car.lane - 1 && lane != car.lane + 1)
  {
    std::cerr << "Proposed lane " << lane << " for car one lane " << car.lane << '\n';
    return false;
  }

  // the two cars most in front and back to the spot of car on the future lane
  Car *car_front_old_lane = &car;
  Car *car_back_old_lane = &car;
  Car *car_front_new_lane = &car;
  Car *car_back_new_lane = &car;

  for (auto &iter : cars)
  {
    // new lane search
    if (iter.lane == lane)
    {
      if (distance(iter, car) < distance(*car_back_new_lane, car))
        car_back_new_lane = &iter;
      if (distance(car, iter) < distance(car, *car_front_new_lane))
        car_front_new_lane = &iter;
      continue;
    }
    // old lane
    if (iter.lane == car.lane)
    {
      if (distance(iter, car) < distance(*car_back_old_lane, car))
        car_back_old_lane = &iter;
      if (distance(car, iter) < distance(car, *car_front_old_lane))
        car_front_old_lane = &iter;
      continue;
    }
  }
  // see if it fits on new lane
  if (distance(car, *car_front_new_lane) < car.length + car_front_new_lane->length + car.min_distance)
    return false;
  if (distance(*car_back_new_lane, car) < car.length + car_back_new_lane->length + car.min_distance)
    return false;

  // calculate the accelerations for car, old and new follow each before and after change
  // first old/new => is the new or old acceleration meant
  // second old/new => is the old or new follower meant
  float accel_old = acceleration_car(car, *car_front_old_lane);
  float accel_new = acceleration_car(car, *car_front_new_lane);
  float accel_old_old_follower = acceleration_car(*car_back_old_lane, car);
  float accel_new_old_follower = acceleration_car(*car_back_old_lane, *car_front_old_lane);
  float accel_old_new_follower = acceleration_car(*car_back_new_lane, *car_front_new_lane);
  float accel_new_new_follower = acceleration_car(*car_back_new_lane, car);

  // calculation for the criteria
  float lhs = accel_new - accel_old + politeness_factor * (accel_new_new_follower - accel_old_new_follower + accel_new_old_follower - accel_old_old_follower);

  return (lhs > switching_threshhold);
}

/**
 * @brief Calculate the acceleration of a car based on the intelligent driver model differential equation
 *
 * @param v         velocity of the car
 * @param v_next    velocity of the car in front
 * @param distance  net distance to the car in front
 * @param v_max     Desired velocity
 * @param s_min     Minimum net distance to the car in front
 * @param T         Safe time head away
 * @param a_max     Max acceleration
 * @param b         Comformatable breaking
 * @return float acceleration of the vehicle described by params
 */
float acceleration(float v, float v_next, float distance,
                   float v_max, float s_min, float T, float a_max, float b)
{
  float s = s_min + v * T + v * (v - v_next) / (2 * sqrt(a_max * b));
  return a_max * (1 - pow(v / v_max, 4) - pow(s / distance, 2));
}

/**
 * @brief Calculating the acceleration utilizing float acceleration(**params)
 *
 * It does not check if they are on the same lane, which we use in the lane change evaluation
 *
 * @param car Car to calculate the acceleration for
 * @param car_front Car in front of the target of the calculation
 * @return float acceleration of Car& car
 */
float MultiLaneRoad::acceleration_car(const Car &car, const Car &car_front)
{
  return acceleration(car.velocity,
                      car_front.velocity,
                      distance(car, car_front),
                      car.desired_velocity,
                      car.min_distance,
                      car.safe_time_headaway,
                      car.max_acceleration,
                      car.comfortable_deceleration);
}
