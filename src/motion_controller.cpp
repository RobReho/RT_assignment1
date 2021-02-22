#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "motion_controller/Random.h"
#include "math.h"
#define threshold 0.1
#define k 1

/** struct targ containes the variables of the random coordinates 
* assiged in real time
*/
struct coord {
	float x;
	float y;
} targ;

ros::Publisher pub;
ros::ServiceClient client;
motion_controller::Random trg;

/** The callback function compares the current coordinates published 
* by /odom with nav_msgs::Odometry message type to the target 
* coordinates stored in the struct coord.
* If the difference is less than the threshold the server /target 
* is called and new target coordinates as stored in targ.x and targ.y.
* Otherwise it is calculated the linear velocity to publish on /cmd_vel
* in function of the current position.
*
* N.B. In the firs cycle the robot's current position is (0,0) and the 
* target coordinates variables are also initialized as x=0 and y=0,
* so the first iteration will call the /target service.
*/
void positionCallback(const nav_msgs::Odometry::ConstPtr& msg)	
{	
	if((fabs(msg->pose.pose.position.x - targ.x) < threshold)&&(abs(msg->pose.pose.position.y - targ.y) < threshold))
	{
		trg.request.min = -6.0;
		trg.request.min = 6.0;
		client.call(trg);
	   	ROS_INFO("target: [%f %f]", trg.response.x, trg.response.y);
		targ.x = trg.response.x;
		targ.y = trg.response.y;
	}
	else{
		geometry_msgs::Twist vel;
		vel.linear.x = k *(targ.x - msg->pose.pose.position.x);
		vel.linear.y = k *(targ.y - msg->pose.pose.position.y);
		pub.publish(vel);
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motion_controller");
	ros::NodeHandle n;
	pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	ros::Subscriber sub = n.subscribe("odom", 1000, positionCallback);
    client = n.serviceClient<motion_controller::Random>("/target");

	ros::spin();

	return 0;
}
