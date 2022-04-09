/* car.h */

#ifndef CAR_H
#define CAR_H

struct Car {
  float location;
  float velocity;

  float desired_velocity;
  float safe_time_headaway;
  float comfortable_deceleration;
  float min_distance;

  float length;
};

void move_unaccelerated(Car *target, float dt);

#endif