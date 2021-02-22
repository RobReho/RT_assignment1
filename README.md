# Motion Controller package
> The package controls a holonomic robot in 2D space by assigning random coordinates and the proper linear velocity to reach them.

## Service node
The service node `/target_service` creates the service server `/target` that provides random x and y coordinates. It relies on a custom service message `motion_controller/Random`.

## Random service
Custom service /Random requests `float32 min` and `float32 max` interval values and responds with `float32 x` and `float32 y`.

## Controller node
The node `/motion_controller` is subscribed to `/odom` topic, which publishes the robot's current coordinates. The callback function compares these coordinates to the random assigned target and calls the service server `/target` to get a new target if the difference is below a threshold of 0.1, otherwise the appropriate linear velocities for x and y are calculated and in turn published to the topic `/cmd_vel`.

## Nodes Graph
![](rosgraph.png)

## How to run

download this folder in your working space:

```sh
git clone kghyfnytfn
```
build it with `catkin_make`, then run the following nodes:

```sh
rosrun stage_ros stageros $(rospack find assignment1)/wor/exercise.world
rosrun motion_controller target_service 
rosrun motion_controller motion_controller 
```


