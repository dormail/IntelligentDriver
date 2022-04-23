/* OneOnLaneTest.cpp 
*  Test script which checks for the bug crashing the program when there is only one car one a lane
*/

#include <iostream>
#include <road.h>

int main () {
    unsigned int car_count = 3;
    float road_length = 200.;
    MultiLaneRoad myRoad(road_length, 3, car_count);
    myRoad.congestion_at_start();

    float dt = .1;
    myRoad.euler(dt);

    return 0;
}
