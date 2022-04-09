/* road.cpp */

#include <road.h>

Road::Road(unsigned int const car_num, float const len) : car_number(car_num), length(len) {
    cars = new Car[car_num];
}

Road::~Road() {
    delete[] cars;
}
