/* car.cpp */

#include <car.h>

void move_unaccelerated(Car *target, float dt) {
    target->location += target->velocity * dt;
}
