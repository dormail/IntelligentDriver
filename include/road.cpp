/* road.cpp */

#include <road.h>
#include <car.h>
#include <vector>
#include <assert.h>

Road::Road(unsigned int const car_num, float const len) : car_number(car_num), length(len) {
    cars = new Car[car_num];

    float car_distance = len / car_num;
    for(unsigned int i = 0; i < car_num; ++i) {
        cars[i].location = car_distance * i;

        // some default values
        cars[i].length = 5;
        cars[1].desired_velocity = 30;
        cars[i].safe_time_headaway = 1.5;
        cars[i].max_acceleration = .73;
        cars[i].comfortable_deceleration = 1.67;
        cars[i].min_distance = 2;

        cars[i].velocity = 0; // standing start
    }
}

/* car1 and car2 are indices 
* returns the distance between car1 and car2
*  positive if 
* ----------car1---car2----------------------
* or
* --car2------------------------------car1---
* negative if switched
*/
//float Road::distance(unsigned int car1, unsigned int car2) {
//    float distance = cars[car2].location - cars[car1].location;
//
//    // case ---------car1--car2-------
//    if(0 < distance <= length/2.) {
//        return distance;
//    }
//    // case --car1---------------car2--
//    if(0 < distance) {
//        return -1 * distance;
//    }
//    // case ---------car2--car1-------
//    if(0 > distance >= -1 * length/2.) {
//        return distance;
//    }
//    // case --car2---------------car1--
//    if(0 > distance) {
//        return distance + length;
//    }
//
//
//}
    
/* i index of a car in cars
*  return: index of the car closest in front 
*/
//int Road::find_car_in_front(unsigned int const input) {
//    unsigned int best_index;
//    float min_distance = length;
//    for(unsigned int i = 0; i < car_number; ++i) {
//        if(cars[i].location < cars[input].location)
//            continue;
//        
//    }
//}

Road::~Road() {
    delete[] cars;
}


OneLaneRoad::OneLaneRoad(unsigned int const car_num, float const len) : cars(car_num), length(len)  {
    assert(cars.size() == car_num);

    for(unsigned int i = 0; i<car_num; ++i) {
        cars[i].velocity = 10.;
        cars[i].location = i * float(len / car_num);
    }
}

unsigned int OneLaneRoad::car_number() {
    return cars.size();
}

void OneLaneRoad::constant_speed(float const dt) {
    for (unsigned int i = 0; i < car_number(); ++i) {
        cars[i].location = cars[i].location + cars[i].velocity * dt;
    }
}

float OneLaneRoad::velocity(unsigned int const car_index) {
    assert(car_index < car_number());
    return cars[car_index].velocity;
}

float OneLaneRoad::location(unsigned int const car_index) {
    assert(car_index < car_number());
    return cars[car_index].location;
}
