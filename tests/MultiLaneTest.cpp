/* MultiLaneTest.cpp */

#include <iostream>
#include <road.h>

int main () {
    unsigned int car_count = 20;
    float road_length = 1000.;
    MultiLaneRoad myRoad(road_length, 3, car_count);
    myRoad.fill_right_lanes();
    // x km/h = .278 * x m/s
    float mean_desired_speed = 130 * .278;
    //float stddev_desired_speed = 20 * .278;
    myRoad.MOBIL_all_cars(mean_desired_speed, .2 * mean_desired_speed);

    // creating csv header
    std::cout << "t";
    for(unsigned int i = 0; i<car_count; ++i) {
        std::cout << ",x" << i << ",l" << i;
    }
    std::cout << '\n';

    //
    float time = 0.;
    float dt = 1./30.;
    float time_end = 120;
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
