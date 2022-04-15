/* main.cpp */

#include <car.h>
#include <road.h>
#include <iostream>

int main() {
    unsigned int car_count = 10;
    OneLaneRoad myRoad(car_count, 1000.);
    std::cout << myRoad.car_number();

    // creating csv header
    std::cout << "t";
    for(unsigned int i = 0; i<car_count; ++i) {
        std::cout << ",x" << i;// << ",v" << i;
    }
    std::cout << '\n';

    //
    float time = 0.;
    float dt = 1./30.;
    float time_end = 10;
    while(time <= time_end) {
        std::cout << time << ',';
        for (unsigned int i = 0; i < car_count; ++i)
        {
            std::cout << myRoad.location(i) << ",";
            //std::cout << myRoad.velocity(i) << ",";
        }
        std::cout << '\n';

        myRoad.constant_speed(dt);

        time += dt;
    }

    return 0;
}
