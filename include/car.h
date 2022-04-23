/* car.h */

#ifndef CAR_H
#define CAR_H

/**
 * @brief Struct storing information about a car
 * 
 */
struct Car {
  float location{0.};
  float velocity{10.};

  float desired_velocity{30.};
  float safe_time_headaway{1.5};
  float comfortable_deceleration{.67};
  float max_acceleration{.5};
  float min_distance{2.};

  float length{5.};

  unsigned int lane = 0;
};

void move_unaccelerated(Car *target, float dt);

#endif