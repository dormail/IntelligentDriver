/* EulerCongestionToCSV.cpp */

#include <car.h>
#include <road.h>
#include <iostream>

int main() {
    unsigned int car_count = 10;
    OneLaneRoad myRoad(car_count, 200.);
    myRoad.congestion_at_start();

    // creating csv header
    std::cout << "t";
    for(unsigned int i = 0; i<car_count; ++i) {
        std::cout << ",x" << i;// << ",v" << i;
    }
    std::cout << '\n';

    //
    float time = 0.;
    float dt = 1./30.;
    float time_end = 90;
    while(time <= time_end) {
        std::cout << time << ',';
        for (unsigned int i = 0; i < car_count; ++i)
        {
            std::cout << myRoad.location(i) << ",";
            //std::cout << myRoad.velocity(i) << ",";
        }
        std::cout << '\n';

        myRoad.euler(dt);

        time += dt;
    }

    return 0;
}
