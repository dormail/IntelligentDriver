/* car.cpp */

#include <car.h>

void move_unaccelerated(Car *target, float dt) {
    target->location += target->velocity * dt;
}

/**
 * @brief Sets a cars parameters to the values they used in MOBIL_TRB
 * 
 * Only the non randomized parameters get applied
 * 
 * @param target Car the function should be applied to
 */
void set_MOBIL(Car &target) {
  target.safe_time_headaway = 2;
  target.max_acceleration = 1.5;
  target.comfortable_deceleration = 2.;
  target.min_distance = 2.;
  target.length = 4.;
}

/**
 * @brief Transforms the parameters of the car to make it a truck
 * 
 * It uses the parameters as used in MOBIL, so the probabibility distribution for desired_speed
 * gets assumed here.
 * 
 * @param car 
 */
void make_truck(Car &car)
{
  car.desired_velocity *= 2./3.;
  car.length = 12;
}
