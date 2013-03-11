#include "vrep_plugin/vrep_pool_plugin.h"
#include "v_repLib.h"
#include <iostream>
#include <string.h>
#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "sensor_msgs/image_encodings.h"
#include "vrep_plugin/Pool_table_state.h"
#include "vrep_plugin/Turn.h"
#include "std_msgs/Int32.h"

#define PLUGIN_VERSION 1

LIBRARY vrepLib; // the V-REP library that will be dynamically loaded and binded

#define WORLD_FRAME 	-1

#define BALL_COUNT		15

#define TIP_OFFSET_DISTANCE		0.1

#define ANGLE_ROTATION 			3.141529
#define POWER_DISTANCE_RATIO 	0.09 //0.40 // m distance to move forward to hit the ball
#define MIN_POWER_DISTANCE		0.01 // to move to touch the ball
#define RAISE_HEIGHT			1
#define POOL_TABLE_HEIGHT		1.73 // ball z : 1.782
#define CUE_ON_TABLE_HEIGHT		1.8172
#define CUE_RAISED_HEIGHT		2.0

#define STEADY_STATE_DRIFT		0.002

#define WHITE_BALL_POTTED		-8
#define RED_BALL_POTTED			10
#define	RED_BALL_HIT			0.5
#define RED_BALL_MOVEMENT		0.01 //metres

#define WHITE_BALL_X			0
#define WHITE_BALL_Y			1.375
#define WHITE_BALL_Z			1.782

#define PLAYER_NONE				0
#define PLAYER_ROTATE_MOVE		1
#define PLAYER_CUE_DOWN			2
#define PLAYER_EXECUTE_SHOT		4
#define	PLAYER_RAISE_CUE		8

ros::NodeHandle* node;
ros::Subscriber sub;
ros::Subscriber sim_change_sub;
image_transport::ImageTransport* img_transport;
image_transport::Publisher img_pub;
ros::Publisher table_state_pub;
ros::Publisher score_pub;
ros::Subscriber turn_sub; 

vrep_plugin::Pool_table_state table_state, old_table_state, pre_hit_table_state, initial_table_state;
vrep_plugin::Turn player_turn;
int player_turn_state = PLAYER_NONE;
bool steady_state;

int tip_handle, white_ball_handle, balls_handle[15], camera_handle;
float tip_position[3], tip_orientation[3]; //iniatls
float white_ball_initial_position[] = {WHITE_BALL_X, WHITE_BALL_Y, WHITE_BALL_Z};
float pos[3];
unsigned int games_played = 0;

bool new_player_turn = false;

bool equal(geometry_msgs::Point32 a, geometry_msgs::Point32 b)
{
	if ((a.x == b.x) &&
		(a.y == b.y) &&
		(a.z == b.z)) return true;
	else return false;
}

bool closeTo(geometry_msgs::Point32 a, geometry_msgs::Point32 b, double offset)
{
	if ((fabs(a.x - b.x) < offset) &&
		(fabs(a.y - b.y) < offset) &&
		(fabs(a.z - b.z) < offset)) return true;
	else return false;
}

// Player turn
// Power [0..1]
// Angel [0 ..2*PI]
inline void new_player_turn_callback(const vrep_plugin::Turn& msg)
{
	ROS_INFO("New player turn received: %.2f %.2f", msg.power, msg.angle);
	player_turn = msg;
	player_turn_state = PLAYER_ROTATE_MOVE;
	pre_hit_table_state = table_state;
	new_player_turn = true;
}

int calculate_turn_score()
{
	double score = 0;
	//pre_hit_table_state && table_state;
	if (table_state.white_ball.z < POOL_TABLE_HEIGHT)
	{
		score += WHITE_BALL_POTTED;
		std::cout << "[V-REP_ROS_POOL_SIM] White potted\n";
	}

	for (unsigned int i = 0; i < BALL_COUNT; i++)
	{
		if ((table_state.balls[i].z < POOL_TABLE_HEIGHT) && 
			(pre_hit_table_state.balls[i].z >= POOL_TABLE_HEIGHT))
		{
			//if ball is potted
			score += RED_BALL_POTTED;
			std::cout << "[V-REP_ROS_POOL_SIM] RED potted\n";
		}
		else if (!closeTo(table_state.balls[i], pre_hit_table_state.balls[i], RED_BALL_MOVEMENT))
		{
			//ball has be hit and moved!
			score += RED_BALL_HIT;
			std::cout << "[V-REP_ROS_POOL_SIM] RED hit\n";
		}
	}
	return ceil(score);
}

bool move_white_ball()
{
	return false;
}

//
//Get all ball positions
//
bool get_ball_positions()
{
	bool success = simGetObjectPosition(white_ball_handle, WORLD_FRAME, pos);
	table_state.white_ball.x = pos[0];
	table_state.white_ball.y = pos[1];
	table_state.white_ball.z = pos[2];
	
	for (int i=0; i < BALL_COUNT; i++)
	{
		success &= simGetObjectPosition(balls_handle[i], WORLD_FRAME, pos);
		table_state.balls[i].x = pos[0];
		table_state.balls[i].y = pos[1];
		table_state.balls[i].z = pos[2];
	}
	return success;
}

//
// Get cue position and orientation
//
bool get_cue_position_orientation()
{
	bool success = simGetObjectOrientation(tip_handle, WORLD_FRAME, tip_orientation);
	success &= simGetObjectPosition(tip_handle, WORLD_FRAME, tip_position);

	table_state.cue_position.x= tip_position[0];
	table_state.cue_position.y = tip_position[1];
	table_state.cue_position.z = tip_position[2];
	table_state.cue_orientation[0] = tip_orientation[0];
	table_state.cue_orientation[1] = tip_orientation[1];
	table_state.cue_orientation[2] = tip_orientation[2];

	return success;
}
//
//Generate image message
//
void generate_image_message(sensor_msgs::Image& image_msg)
{

	int size[2], data_len;
	float* image_buf = simGetVisionSensorImage(camera_handle);
	simGetVisionSensorResolution(camera_handle, size);

	image_msg.encoding = sensor_msgs::image_encodings::RGB8;
	image_msg.height = size[1];
	image_msg.width  = size[0];
	image_msg.step   = image_msg.width * 3; //image stride in bytes

	data_len = image_msg.step * image_msg.height;
	image_msg.data.resize(data_len);
	image_msg.is_bigendian = 0;

	int msg_idx, buf_idx;
	for(unsigned int i = 0; i < image_msg.height; i++)
	{
		for(unsigned int j = 0; j < image_msg.step; j++)
		{
			msg_idx = (image_msg.height - i - 1) * image_msg.step + j;
			buf_idx = i * image_msg.step + j;
			image_msg.data[msg_idx] = (unsigned char)(image_buf[buf_idx] * 255);
		}
	}
	simReleaseBuffer((char*)image_buf);
}

// This is the plugin start routine (called just once, just after the plugin was loaded):
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{
	// Dynamically load and bind V-REP functions:
	// ******************************************
	// 1. Figure out this plugin's directory:
	char curDirAndFile[1024];

	char * filenptr = getcwd(curDirAndFile, sizeof(curDirAndFile));
	if (filenptr == NULL)
	{
		std::cout << "[V-REP_ROS_POOL_SIM] Cannot get current directory. Failed to start 'ROS' plugin.\n";
		return(0); // Means error, V-REP will unload this plugin
	}

	std::string currentDirAndPath(curDirAndFile);
	// 2. Append the V-REP library's name:
	std::string temp(currentDirAndPath);

	temp+="/libv_rep.so";
	// 3. Load the V-REP library:
	vrepLib=loadVrepLibrary(temp.c_str());
	if (vrepLib == NULL)
	{
		std::cout << "[V-REP_ROS_POOL_SIM]ind or correctly load the V-REP library. Cannot start 'ROS' plugin.\n";
		return (0); // Means error, V-REP will unload this plugin
	}
	if (getVrepProcAddresses(vrepLib)==0)
	{
		std::cout << "[V-REP_ROS_POOL_SIM] Error, could not find all required functions in the V-REP library. Cannot start 'ROS' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return (0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Check the version of V-REP:
	// ******************************************
	int vrepVer;
	simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
	if (vrepVer<20604) // if V-REP version is smaller than 2.06.04
	{
		std::cout << "[V-REP_ROS_POOL_SIM] Sorry, your V-REP copy is somewhat old. Cannot start 'ROS' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return (0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Here you could handle various initializations
	// Here you could also register custom Lua functions or custom Lua constants
	//
	// e.g. you could register following functions here:
	// nodeHandle=simExtRosStart(...)
	// simExtRosFinish(nodeHandle)
	// simExtRosEnableService(nodeHandle,...)
	// etc.

	std::cout << "[V-REP_ROS_POOL_SIM] Initializing v-rep plugin...\n";
	int argc = 0;
	char** argv = NULL;
	ros::init(argc,argv,"v_rep_pool_sim");

	if (!ros::master::check())
	{
		ROS_ERROR("[V-REP_ROS_POOL_SIM] Cannot detect ROS master!");
		return 0;
	}
	ROS_INFO("[V-REP_ROS_POOL_SIM] Creating node...");
	node = new ros::NodeHandle();
	img_transport = new image_transport::ImageTransport(*node);
	img_pub = img_transport->advertise("camera_image", 2);
	table_state_pub = node->advertise<vrep_plugin::Pool_table_state>("pool_table_state", 1);
  	score_pub = node->advertise<std_msgs::Int32>("turn_score", 1);

	ROS_INFO("[V-REP_ROS_POOL_SIM] Subscribing...");
	turn_sub = node->subscribe("player_turn", 1, new_player_turn_callback);
	//pool_table = node->advertiseService("player_turn", evaluate_player_turn_callback);
	//sim_change_sub = node->subscribe("sim_change_status", 2, sim_change_callback);
	
	ROS_INFO("[V-REP_ROS_POOL_SIM] Initialized!");
	return (PLUGIN_VERSION); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
}

// This is the plugin end routine (called just once, when V-REP is ending, i.e. releasing this plugin):
VREP_DLLEXPORT void v_repEnd()
{
	// Here you could handle various clean-up tasks
	//pool_table.shutdown();
	ros::shutdown();
	unloadVrepLibrary(vrepLib); // release the library
}

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
	// Keep following 5 lines at the beginning and unchanged:
	simLockInterface(1);
	int errorModeSaved;
	simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
	simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);
	void* retVal=NULL;

	// Here we can intercept many messages from V-REP (actually callbacks). Only the most important messages are listed here:

	if (message==sim_message_eventcallback_instancepass)
	{ // This message is sent each time the scene was rendered (well, shortly after) (very often)
		// When a simulation is not running, but you still need to execute some commands, then put some code here
	}

	if (message==sim_message_eventcallback_mainscriptabouttobecalled)
	{ // Main script is about to be run (only called while a simulation is running (and not paused!))
		//
		// This is a good location to execute commands (e.g. commands needed to generate ROS messages)
		//
		// e.g. to read all joint positions:
		simLockInterface(1); 

		ros::spinOnce();
		
		sensor_msgs::Image image_msg;

		//
		// Execute player speceific functions
		//
		if ((player_turn_state == PLAYER_NONE) && new_player_turn)
		{
			//ROS_INFO("New player turn received: %f %f", player_turn.power, player_turn.angle);
			ROS_INFO("[PLAYER_NONE] Player turn ended.\n");
			new_player_turn = false;
		}
		else if (player_turn_state == PLAYER_ROTATE_MOVE)
		{    
			tip_position[0] = table_state.white_ball.x - TIP_OFFSET_DISTANCE * sin(player_turn.angle);
			tip_position[1] = table_state.white_ball.y + TIP_OFFSET_DISTANCE * cos(player_turn.angle);
			
			tip_orientation[2] = player_turn.angle;

			std::cout << "[V-REP_ROS_POOL_SIM][PLAYER_ROTATE_MOVE] Orientation success: " << 
				simSetObjectOrientation(tip_handle, WORLD_FRAME, tip_orientation) << std::endl;
			std::cout << "[V-REP_ROS_POOL_SIM][PLAYER_ROTATE_MOVE] Position success: " << 
				simSetObjectPosition(tip_handle, WORLD_FRAME, tip_position) << std::endl;

			player_turn_state = PLAYER_CUE_DOWN;
		}
		else if (player_turn_state == PLAYER_CUE_DOWN)
		{
			tip_position[2] = CUE_ON_TABLE_HEIGHT;
			std::cout << "[V-REP_ROS_POOL_SIM][PLAYER_CUE_DOWN] Position success: " << 
				simSetObjectPosition(tip_handle, WORLD_FRAME, tip_position) << std::endl;	

			player_turn_state = PLAYER_EXECUTE_SHOT;
		}
		else if (player_turn_state == PLAYER_EXECUTE_SHOT)
		{
			double dist = player_turn.power * POWER_DISTANCE_RATIO + MIN_POWER_DISTANCE;
			tip_position[0] += sin(player_turn.angle) * dist; 
			tip_position[1] -= cos(player_turn.angle) * dist;
			
			std::cout << "[V-REP_ROS_POOL_SIM][PLAYER_EXECUTE_SHOT] Position success: " << 
				simSetObjectPosition(tip_handle, WORLD_FRAME, tip_position) << std::endl;

			player_turn_state = PLAYER_RAISE_CUE;
		}
		else if (player_turn_state == PLAYER_RAISE_CUE)
		{
			tip_position[2] = CUE_RAISED_HEIGHT;
			std::cout << "[V-REP_ROS_POOL_SIM][PLAYER_RAISE_CUE] Position success: " << 
				simSetObjectPosition(tip_handle, WORLD_FRAME, tip_position) << std::endl;

			player_turn_state = PLAYER_NONE;
		}

		generate_image_message(image_msg);
		
		//
		// Transfer to old table state and get new state
		//
		old_table_state = table_state;
		get_ball_positions();
		get_cue_position_orientation();

		//
		//Check if table is in steady state
		//
		steady_state = (table_state.white_ball.z < POOL_TABLE_HEIGHT)?
			(true):(closeTo(old_table_state.white_ball, table_state.white_ball, STEADY_STATE_DRIFT));
		for (unsigned int i = 0; (i < BALL_COUNT) && steady_state; i++)
		{
			if (!closeTo(old_table_state.balls[i], table_state.balls[i], STEADY_STATE_DRIFT))
			{
				steady_state = false;
			}
		}
		
		//
		// Publish information
		//
		if (steady_state && (player_turn_state == PLAYER_NONE && new_player_turn == false))
		{
			ROS_INFO("Table is in steady state!\n");
			//calculate turn score and publish it.
			int score = calculate_turn_score();
			ROS_INFO("The score for the last turn is %d", score);
			std_msgs::Int32 score_msg;
			score_msg.data = score;
			score_pub.publish(score_msg);
			usleep(50 * 1000); //50ms
			table_state_pub.publish(table_state); // publish a message only if nothing is moving
		}

		//
		// Move white ball if potted
		//
		if (steady_state && (table_state.white_ball.z < POOL_TABLE_HEIGHT)&& 
			(player_turn_state == PLAYER_NONE && new_player_turn == false))
		{
			std::cout << "[V-REP_ROS_POOL_SIM] Reset: " << simResetDynamicObject(white_ball_handle) << " ";
			std::cout << "[V-REP_ROS_POOL_SIM] Moving white to initial position success: " << 
				simSetObjectPosition(white_ball_handle, WORLD_FRAME, white_ball_initial_position) << std::endl;
			table_state.white_ball.x = white_ball_initial_position[0];
			table_state.white_ball.y = white_ball_initial_position[1];
			table_state.white_ball.z = white_ball_initial_position[2];
		}

		//
		// Move all red balls is all potted
		//
		if (steady_state)
		{
			int balls_in = 0;
			for (unsigned int i = 0; i < BALL_COUNT; i++)
			{
				if (table_state.balls[i].z < POOL_TABLE_HEIGHT)
				{
					balls_in++;
				}
			}
			if (balls_in == BALL_COUNT)
			{
				//Move all balls to the top!
				std::cout << "[V-REP_ROS_POOL_SIM] Reset: " << simResetDynamicObject(white_ball_handle) << " ";
				std::cout << "[V-REP_ROS_POOL_SIM] Moving white to initial position success: " << 
					simSetObjectPosition(white_ball_handle, WORLD_FRAME, white_ball_initial_position) << std::endl;

				for (unsigned int i = 0; i < BALL_COUNT; i++)
				{
					std::cout << "[V-REP_ROS_POOL_SIM] Ball " << i << std::endl;
					float p[3];
					p[0] = initial_table_state.balls[i].x;
					p[1] = initial_table_state.balls[i].y;
					p[2] = initial_table_state.balls[i].z;

					std::cout << "[V-REP_ROS_POOL_SIM] Reset: " << simResetDynamicObject(balls_handle[i]) << std::endl;
					std::cout << "[V-REP_ROS_POOL_SIM] Moving ball position success: " << 
						simSetObjectPosition(balls_handle[i], WORLD_FRAME, p) << std::endl;
				}
				table_state = initial_table_state;
				games_played++;
				ROS_INFO("The are %d games played so far.", games_played);
			}
		}

		simLockInterface(0);

		//Publish messages
		img_pub.publish(image_msg);
		
		// The best would be to start ROS activity when a simulation is running, and stop it when a simulation ended
		// If you allow ROS activity while a simulation is not running, then it becomes a little bit tricky when a scene
		// was switched for example (e.g. the clients would need a way to detect that)
	}

	if (message==sim_message_eventcallback_simulationabouttostart)
	{ // Simulation is about to start
		// Here you could launch the ROS node (if using just one node)
		// If more than one node should be supported (e.g. with different services per node),
		// then it is better to start a node via a custom Lua function
		white_ball_handle = simGetObjectHandle("white_ball");
		std::cout << "[V-REP POOL PLUGIN] White ball handle: " << white_ball_handle << std::endl;
		tip_handle = simGetObjectHandle("tip");

		for (int i=0; i < BALL_COUNT; i++)
		{
			std::stringstream ball_name;
			ball_name << "ball";
			ball_name << i;
			balls_handle[i] = simGetObjectHandle(ball_name.str().c_str()); 
			//std::cout << "Ball " << i << " handle: " << balls_handle[i] << std::endl;
		}
		camera_handle = simGetObjectHandle("camera_sensor");
		std::cout << "[V-REP POOL PLUGIN] Camera handle: " << camera_handle << std::endl;

		//Read all positions and make them as old positions
		get_ball_positions();
		old_table_state = table_state;
		pre_hit_table_state = table_state;
		initial_table_state = table_state;
	}

	if (message==sim_message_eventcallback_simulationended)
	{ // Simulation just ended
		// Here you could kill the ROS node(s) that are still active. There could also be a custom Lua function to kill a specific ROS node.
	}

	// Keep following unchanged:
	simSetIntegerParameter(sim_intparam_error_report_mode, errorModeSaved); // restore previous settings
	simLockInterface(0);
	return (retVal);
}

