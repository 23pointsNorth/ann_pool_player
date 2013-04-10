#include <iostream>
#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/Point32.h>
#include <vrep_plugin/Turn.h>
#include <vrep_plugin/Pool_table_state.h>

using namespace ros;
using namespace cv;

cv_bridge::CvImageConstPtr img;
bool img_received = false; 
vrep_plugin::Turn player_turn;
vrep_plugin::Pool_table_state table_state;

inline void imageReceiveCallback(const sensor_msgs::ImageConstPtr& msg)
{
	img = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
	img_received = true;
}
inline void poolTableStateCallback(const vrep_plugin::Pool_table_state& msg)
{
	//table_state = msg;
	ROS_INFO("White ball position: x=%f y=%f z=%f", msg.white_ball.x, msg.white_ball.y, msg.white_ball.z);
}

int main(int argc, char** argv)
{
	init(argc, argv, "pool_table");
	
	NodeHandle node;	
	image_transport::ImageTransport img_transport(node);

	image_transport::Subscriber img_sub = img_transport.subscribe("camera_image", 1, imageReceiveCallback);
	ros::Subscriber table_state_sub = node.subscribe("pool_table_state", 1, poolTableStateCallback);
	//Publisher turn_pub = node.advertise<vrep_plugin::Turn>("player_turn", 1);
	//Publisher line_pub = node.advertise<line_tracer_vision::Vector>("full_control_vector", 2);
	Rate img_refresh(30);

	Mat threshold_img;
	Point target;
	geometry_msgs::Point32 control_vector;
	ROS_INFO("Node started. Awaiting Images and Pool Table state messages.");
	while(ok())
	{
		if (img_received)
		{
			img_received = false;
			imshow("Image", img->image);
			waitKey(2);
		}
		spinOnce();
		img_refresh.sleep();
	}
	shutdown();
	
}



