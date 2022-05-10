/* road.cpp */


#include <road.h>
#include <car.h>
#include <vector>
#include <assert.h>
#include <cmath> // pow
#include <algorithm> // std::min
#include <string>
#include <fstream> // std::ofstream
#include <errno.h>

#include <iostream>

#define LOG(message) std::cerr << message << std::endl;

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
 * @brief A method printing all car's locations and lanes sorted by lane
 * 
 */
void MultiLaneRoad::print_locations()
{
  std::cerr << "MultiLaneRoad::print_locations\n"
            << "\tLocation\tvelocity\tlane\n";

  for (unsigned int i = 0; i < lane_num; ++i)
  {
    for (auto &car : cars)
    {
      if (car.lane != i)
        continue;
      std::cerr << '\t' << car.location << "\t\t" << car.velocity << "\t\t" << car.lane << '\n';
    }
  }
  std::cerr << std::endl;
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

/**
 * @brief Puts all cars as far right, lowest location as possible
 *
 * Fills up lowest indexed lanes first
 *
 */
void MultiLaneRoad::fill_right_lanes()
{
  float location = 0;
  unsigned int lane = 0;

  for (auto &iter : cars)
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
 * @brief Sets all the car.front pointers
 *
 */
void MultiLaneRoad::set_car_front()
{
  for (auto &iter : cars)
  {
    iter.front = &(car_in_front(iter));
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
 * If the car is alone on the lane it returns the car it self. This method makes no assumtions about features beaing set at struct Car.
 * It always returns the correct car but it linear in time complexity
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

/**
 * @brief Returns the car in the back of the car it assumes car.front to be set correctly
 * 
 * @param car 
 * @return Car& follower, if car is alone on the lane it returns the car itself
 */
Car& MultiLaneRoad::follower(Car& car)
{
  for (auto &iter : cars)
  {
    if (iter.front == &car)
    {
      assert(iter.lane == car.lane);
      return iter;
    }

  }
  return car;
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
 * If the cars are not touching each other, this method should always return something positive
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

  // assert(distance >= -1.5 * (car1.length + car2.length));
  //  assert(distance < length);
  return distance;
}

/**
 * @brief Construct a new One Lane Road:: One Lane Road object
 *
 * It creates car_num cars on a new Road with velocity 10m/s and as close to the start as possible.
 *
 * @param car_num Amount of cars to set on the road
 * @param len Length of the road
 */
OneLaneRoad::OneLaneRoad(unsigned int const car_num, float const len) : cars(car_num), length(len), generator()
{
  assert(cars.size() == car_num);

  for (unsigned int i = 0; i < car_num; ++i)
  {
    cars[i].velocity = 10.;
    cars[i].location = i * float(len / car_num);
  }
}

void OneLaneRoad::print_locations()
{
  std::cerr << "cars.location\n";
  for (auto &car : cars)
  {
    std::cerr << "\t" << car.location << '\n';
  }
  std::cerr << std::endl;
}

/**
 * @brief Creates a traffic at the start of the road with cars at full stop and min distance between each other.
 *
 */
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

/**
 * @brief Sets the desired_velocity for Cars as a Gaussian distribution
 *
 * @param mean
 * @param stddev
 */
void OneLaneRoad::desired_speed_gaussian(float const mean, float const stddev)
{
  std::normal_distribution<double> distribution(mean, stddev);

  for (auto &car : cars)
  {
    car.desired_velocity = distribution(generator);
  }
}

/**
 * @brief Sets the car's parameters to an all-car configuration from the MOBIL Paper
 *
 * This function sets all cars as cars as defined in the MOBIL Paper (length, etc.)
 * The desired speed is a uniform distribution (in the paper they use mean+/-20%, here the are independent)
 *
 * @param mean Mean of the generated desired velocity
 * @param variation Maximum deviation from mean
 */
void OneLaneRoad::MOBIL_all_cars(float const mean, float const variation)
{
  std::uniform_real_distribution<float> distribution(mean - variation, mean + variation);

  for (auto &car : cars)
  {
    car.desired_velocity = distribution(generator);
    set_MOBIL(car);
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

/**
 * @brief Makes a time integration step using Euler's method
 *
 * @param dt Time step length
 */
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

/**
 * @brief Gets the position of a car by index
 *
 * @param car_index
 * @return float cars[car_index].location
 */
float OneLaneRoad::location(unsigned int const car_index)
{
  assert(car_index < car_number());
  return cars[car_index].location;
}

/**
 * @brief Calculates the net distance to the car in front
 *
 * This method takes the length of the cars in to account, so it returns the actual
 * bumper to bumper distance.
 *
 * @param car_index
 * @return float Net distance
 */
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
 * @brief Calculates the average speed of the cars
 *
 * @return float
 */
float OneLaneRoad::average_speed()
{
  float average = 0.;
  for (auto &car : cars)
  {
    average += car.velocity;
  }
  return average / float(car_number());
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

  for (auto &iter : cars)
  {
    assert(iter.lane < lane_num);
    euler_single_car(iter, dt);
  }

  for (auto &iter : cars)
  {
    iter.location += dt * iter.velocity;
    iter.velocity += dt * iter.accel;
  }
  location_enforce_boundries();
  set_car_front();
}

/**
 * @brief Euler integration step for a car assuming US rules
 *
 * @param car car to do the integration
 * @param dt time step length
 */
void MultiLaneRoad::euler_single_car(Car &car, float const dt)
{
  // check for lane change possibility
  if (should_change(car, car.lane + 1))
  {
    car.lane++;
  }
  else
  {
    if (should_change(car, car.lane - 1) && car.lane > 0)
    {
      car.lane--;
    }
  }

  float distance_front = distance(car, *(car.front));
  assert(distance_front >= 0);

  car.accel = acceleration_car(car, *(car.front));
}

/**
 * @brief Computes the time evolution of the road and outputs the whole data to a file in CSV format
 *
 * The CSV file contains a column for the time (starting at 0) and 3 columns for each Car with
 * lane, velocity and location.
 *
 * It does not catch any exceptions from std::ofstream, though it does check if it could open the file.
 *
 * @param dt Step length
 * @param steps Amount of integration steps
 * @param filename Name of the output file
 * @return 0 Everything good
 * @return -1 Could not open the file
 */
int MultiLaneRoad::euler_to_CSV(float const dt, unsigned int const steps, std::string filename)
{
  std::ofstream output;
  output.open(filename);

  if (!(output.is_open()))
  {
    std::cerr << "Error opening file " << filename << '\n';
    return -1;
  }
  // create csv header
  output << 't';
  for (unsigned int i = 0; i < car_number(); ++i)
  {
    output << ",x" << i << ",v" << i << ",l" << i;
  }
  output << std::endl;

  float time = 0.;
  // time integration and data output
  for (unsigned int i = 0; i < steps; ++i)
  {
    output << time << ',';
    for (auto &iter : cars)
    {
      output << iter.location << ",";
      output << iter.velocity << ",";
      output << iter.lane << ",";
    }
    // output << std::endl;
    output << '\n';

    euler(dt);
    time += dt;
  }

  output.close();
  return 0;
}

int MultiLaneRoad::euler_to_CSV_EU(float const dt, unsigned int const steps, std::string filename)
{
  std::ofstream output;
  output.open(filename);

  if (!(output.is_open()))
  {
    std::cerr << "Error opening file " << filename << '\n';
    return -1;
  }
  // create csv header
  output << 't';
  for (unsigned int i = 0; i < car_number(); ++i)
  {
    output << ",x" << i << ",v" << i << ",l" << i;
  }
  output << std::endl;

  float time = 0.;
  // time integration and data output
  for (unsigned int i = 0; i < steps; ++i)
  {
    output << time << ',';
    for (auto &iter : cars)
    {
      output << iter.location << ",";
      output << iter.velocity << ",";
      output << iter.lane << ",";
    }
    // output << std::endl;
    output << '\n';

    euler_eu(dt);
    time += dt;
  }

  output.close();
  return 0;
}

/**
 * @brief A method calculating if a car should change a lane
 *
 *  It uses the incentive criterion from some paper and also checks if the params are correct. It is based on
 *  <a href="https://doi.org/10.3141%2F1999-10">this paper</a>.
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
      if (distance(iter, car) < 0)
        return false;
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
  // see if it fits
  if (distance(*car_back_new_lane, car) < 0)
    return false;
  if (distance(car, *car_front_new_lane) < 0)
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

  // safety criterion
  if (accel_new_new_follower < -1 * safety_break)
    return false;
  if (accel_new < -1 * safety_break)
    return false;

  // calculation for the incentive criterion
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
  double s = s_min + v * T + v * (v - v_next) / (2. * sqrt(a_max * b));

  double ret = a_max * (1. - pow(double(v / v_max), 4) - pow(double(s / distance), 2));
  if (errno == ERANGE || ret > a_max)
  {
    std::cerr << "Acceleration overflowed" << '\n';
  }
  return ret;
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

/**
 * @brief Time step with the right lane obligation
 *
 * This method assumes that the Cars[i].front are set correctly. It implements the Passing rule and lane usage rule
 * according to MOBIL page 7f.
 *
 * @param dt step length
 */
void MultiLaneRoad::euler_eu(float dt)
{
  for (Car &car : cars)
  {
    car.accel = acceleration_car(car, *(car.front));
  }
  for (Car &car : cars)
  {
    car.velocity += dt * car.accel;
    car.location += dt * car.velocity;
  }

  // now changing lanes
  for (Car &car : cars)
  {
    offer_lane_change(car);
    correct_front();
  }

  for (auto &car : cars)
    float a = acceleration_on_the_left(car);
}

/**
 * @brief Offers a EU lane change and performs it
 * 
 * If there is no car on the lane to the left it ignores the change for that lane (like politeness_factor=0).
 *
 * @param car
 */
void MultiLaneRoad::offer_lane_change(Car &car)
{
  float a_c_eur = acceleration_on_the_left(car);
  int change_right = 0;
  int change_left = 0;

  Car old_follower = follower(car);
  Car* left_follower;
  
  if (car.lane > 0)
    change_right = (a_c_eur - car.accel + politeness_factor * (acceleration_car(old_follower, *(car.front)) - acceleration_car(old_follower, car)) > switching_threshhold - a_bias);
  if (car.lane < lane_num - 1)
  {
    Car tmp;
    tmp.location = -3 * length;
    left_follower = nullptr;
    float min_dist = 3. * length;
    for (auto &iter : cars)
    {
      if (iter.lane != car.lane + 1)
        continue;
      if (distance(iter, car) < min_dist)
      {
        left_follower = &iter;
        min_dist = distance(iter, car);
      }
    }
    if (left_follower == nullptr)
    {
      change_left = (car.max_acceleration - a_c_eur > switching_threshhold + a_bias);
      if (change_left)
      {
        ++(car.lane);
        return;
      }
    }
    else
    {
      change_left = (acceleration_car(car, *(left_follower->front)) - a_c_eur + politeness_factor * (acceleration_car(*left_follower, *(car.front)) - acceleration_car(*left_follower, car)) > switching_threshhold + a_bias);
    }
  }

  if (change_left && distance(*left_follower, car) > 0 && distance(car, *(left_follower->front)) > 0)
  {
    ++(car.lane);
    return;
  }
  // find right follower

  if (change_right)
  {
    Car *right_follower = nullptr;
    float tmp_dist = length;
    for (auto &iter : cars)
    {
      if (iter.lane != car.lane - 1)
        continue;
      if (distance(iter, car) < tmp_dist)
      {
        right_follower = &iter;
        tmp_dist = distance(*right_follower, car);
      }
    }
    if (!(right_follower == nullptr))
    {
      change_right *= (distance(*right_follower, car) > 0);
      change_right *= (distance(car, *(right_follower->front)) > 0);
    }
    if (change_right)
    {
      assert( (distance(*right_follower, car) > 0));
      assert( (distance(car, *(right_follower->front)) > 0));

      --(car.lane);
    }

    return;
  }
}

/**
 * @brief It calculates the a_c^eur acceleration as defined in the MOBIL paper
 *
 * Usefull for EU traffic law simulation
 *
 * @param car
 * @return float a_c^eur as defined in MOBIL paper
 */
float MultiLaneRoad::acceleration_on_the_left(Car &car)
{
  // if car is on the left lane it is car.accel
  if (car.lane == lane_num - 1)
    return car.accel;

  // looking for car in front on the left lane
  Car tmp; // dummy car to initilize the pointers
  tmp.location = 3 * length;
  tmp.accel = car.accel; // if there is no car on the left -> use car's acceleration
  Car *front_left = &tmp;
  for (auto &iter : cars)
  {
    if (iter.lane != car.lane + 1)
      continue;
    if (distance(car, iter) < distance(car, *front_left))
      front_left = &iter;
  }

  // case of no congestion
  if (car.velocity > front_left->velocity && front_left->velocity > v_crit)
  {
    // looking for car most close on the left lane
    Car *closest = &tmp;
    float min_distance;
    for (auto &iter : cars)
    {
      if (iter.lane != car.lane + 1)
        continue;
      if (distance(car, iter) < min_distance || distance(iter, car) < min_distance)
        closest = &iter;
    }
    return std::min(car.accel, closest->accel);
  }
  return car.accel;
}

/**
 * @brief Corrects the car.front entry
 *
 * Since car_in_front uses linear time this function alone is O(car_number^2)
 *
 */
void MultiLaneRoad::correct_front()
{
  for (auto &iter : cars)
  {
    iter.front = &(car_in_front(iter));
  }
}