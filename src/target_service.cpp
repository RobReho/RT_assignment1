#include "ros/ros.h"
#include "motion_controller/Random.h"

/** Function that returns a random value between M and N
* M: minimum value
* N: maximum value
*/
double randMToN(double M, double N)
{   
	return M + ( rand() / ( N - M + 1 ) ) ; 
}

/** The callback funtion assigns a random value to the variables 
* x and y between the values min and max every time the service 
* server /target is called.
*/
bool Callback (motion_controller::Random::Request &req, motion_controller::Random::Response &res)
{ 
	res.x = randMToN(req.min, req.max)/100000000;
	res.y = randMToN(req.min, req.max)/100000000;
	return true;
}

int main(int argc, char**argv)
{
	ros::init(argc, argv, "target_service");	
	ros::NodeHandle n;
	ros::ServiceServer service= n.advertiseService("/target", Callback);
	ros::spin();

	return 0;
}
