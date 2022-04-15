/* road.h */

#ifndef ROAD_H
#define ROAD_H

#include <car.h>
#include <vector>

class OneLaneRoad {
    protected:
    std::vector<Car> cars;
    float length = 1000.;
    
    public:
    OneLaneRoad(unsigned int const car_num, float const len);
    ~OneLaneRoad() {}

    // set methods
    void congestion_at_start(); // create a traffic at the start of the road

    
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

float acceleration(float v, float v_next, float distance, float v_max, float s_min, float T, float a_max, float b);

#endif