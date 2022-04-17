/* MultiLaneTest.cpp */

#include <iostream>
#include <road.h>

int main () {
    unsigned int car_count = 6;
    float road_length = 100.;
    MultiLaneRoad myRoad(road_length, 3, car_count);
    myRoad.congestion_at_start();
    // x km/h = .278 * x m/s
    float mean_desired_speed = 120 * .278;
    float stddev_desired_speed = 20 * .278;
    myRoad.desired_speed_gaussian(mean_desired_speed, stddev_desired_speed);

    // creating csv header
    std::cout << "t";
    for(unsigned int i = 0; i<car_count; ++i) {
        std::cout << ",x" << i << ",l" << i;
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
            std::cout << myRoad.lane(i) << ",";
        }
        std::cout << '\n';

        myRoad.euler(dt);

        time += dt;
    }

    return 0;
}
