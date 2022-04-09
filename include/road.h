/* road.h */

#ifndef ROAD_H
#define ROAD_H

#include <car.h>

class Road {
    protected:
    Car* cars;
    unsigned int car_number = 0;
    float length = 1000.;

    public:
    Road() = default;
    Road(unsigned int const car_num, float const len);
    ~Road();

    int set_car_number(unsigned int new_car_number);

    void RK4(float dt);
};

#endif