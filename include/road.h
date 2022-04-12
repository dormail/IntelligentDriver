/* road.h */

#ifndef ROAD_H
#define ROAD_H

#include <car.h>
#include <vector>

class Road {
    protected:
    Car* cars;
    unsigned int car_number = 0;
    float length = 1000.;

    public:
    Road() = default;
    Road(unsigned int const car_num, float const len);
    ~Road();


    float distance(unsigned int car1, unsigned int car2);
    int find_car_in_front(unsigned int const input);

    int set_car_number(unsigned int new_car_number);

    void RK4(float dt);
};

class OneLaneRoad {
    protected:
    std::vector<Car> cars;
    float length = 1000.;
    
    public:
    OneLaneRoad(unsigned int const car_num, float const len);
    ~OneLaneRoad() {}

    unsigned int car_number();
    
    float location(unsigned int const car_index);
    float velocity(unsigned int const car_index);
    
    // diff eq solvers
    void constant_speed(float const dt);
    void RK4(float const dt);
};

#endif