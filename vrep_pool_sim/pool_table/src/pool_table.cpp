#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <iostream>
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
/*
bool isOnLine(Mat& img, Point start, Point end, int level)
{
	bool center_point = (img.at<uchar>((start+end)*0.5) == 0); 
	bool result = (level == 0)? center_point && (img.at<uchar>(end) == 0) : center_point;
	if (level == MAX_RECURSION) 
	{
		return result;
	}
	else
	{ 				
		return result && isOnLine(img, start, (start+end)*0.5, level+1) && isOnLine(img, (start+end)*0.5, end, level+1);
	}
}

Point findLine(Mat& img, Point start)
{
	for (int i=0; i < start.y; i++)
	{
		for (int j=0; j< img.cols; j++)
		{
			//std::cout <<"Point -> " << j << " " << i << "\n";
			if (isOnLine(img, start, Point(j,i), 0)) 
			{	
				//std::cout <<"\tFound Point -> " << j << " " << i << "\n";
				return Point(j,i);
			}
		}
	}
	return Point(-1,-1);
}
*/
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

	//Point center(128,BOTTOM_LINE);
	Mat threshold_img;
	Point target;
	geometry_msgs::Point32 control_vector;
	ROS_INFO("Node started. Awaiting Images and Pool Table state messages.");
	while(ok())
	{
		if (img_received)
		{
		/*
			cvtColor(img->image, threshold_img, CV_BGR2GRAY);
			threshold(threshold_img, threshold_img, 77, 255, THRESH_BINARY); 
			
			if (!onStartLine(threshold_img, center.x))
			{
				center = find_center_point(threshold_img);
				target = findLine(threshold_img, center);	
				
				control_vector.x = 1.5 * (center.x - 128) + (target.x - 128);
				control_vector.y = -(target.y - BOTTOM_LINE); //Take into account the inverse y-axis
				
				circle(threshold_img, center, 6, Scalar(128), -1);	
				line(threshold_img, center, target, Scalar(200), 2);
			}
			else
			{
				circle(threshold_img, Point(center.x - OFFSET, BOTTOM_LINE), 6, Scalar(200), -1);	
				circle(threshold_img, Point(center.x + OFFSET, BOTTOM_LINE), 6, Scalar(200), -1);	
			}
			
			// If the line is visible
			if (target.x != -1 && target.y != -1)
			{ 
				vector_pub.publish(control_vector);
			
				full_vector.start.x = center.x;
				full_vector.start.y = center.y;
				full_vector.end.x = target.x;
				full_vector.end.y = target.y;
				line_pub.publish(full_vector);
				
			}
			else
			{
				//control_vector.x *= 0.95;
				//control_vector.y *= 0.95;
			}
			
			line(threshold_img, Point(128, BOTTOM_LINE), Point(control_vector.x + 128, -control_vector.y + BOTTOM_LINE), Scalar(200), 1);
			*/
			img_received = false;
			imshow("Image", img->image);
			waitKey(2);
		}
		spinOnce();
		img_refresh.sleep();
	}
	shutdown();
	
}



