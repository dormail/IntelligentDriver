# IntelligentDriver/include

The include directory with files which implement the intelligent driver model. It consists of a
`struct Car` and classes for a road. The `class MultiLaneRoad` inherits some stuff from the 
`class OneLaneRoad` which was a play ground in test stages.

The MultiLaneRoad consists of 
 - a `std::vector<Car>` which stores the state of each car
 - A diferential equation solver (currently Euler's method)
 - methods for initializing in specific states like
   - `congestion_at_start` putting all cars at the begining of the road at standing start
 - accessing car specific details like position, velocity

## Regarding the multi lane implementation:
`struct Car` stores the current lane as an `unsigned int`. Lanes are zero indexed; 0 refers to the lane to the most 
right, so with `n` lanes `n-1` is the road which is on the far left (right, left from a drivers POV).