#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <geometry_msgs/Point32.h>
#include <vrep_plugin/Turn.h>
#include <vrep_plugin/Pool_table_state.h>
#include <vector>
#include <fstream>

#include "NeuralNetwork.h"

using namespace ros;
using namespace cv;
using namespace std;

#define _MAX(a,b) ((a)>(b)?(a):(b))
#define _MIN(a,b) ((a)<(b)?(a):(b))

#define BALL_COUNT			15

#define SCORE_THRESHOLD		3 //10
#define EXPERT_ON			true
#define EXPERTISE_SCORE		-6

#define TABLE_WIDTH			3 //m
#define TABLE_LENGTH		6 //m
#define TABLE_DIAG			sqrt(TABLE_LENGTH*TABLE_LENGTH + TABLE_WIDTH*TABLE_WIDTH)
#define TABLE_HEIGHT		1.73 // ball z : 1.78

#define PI 					3.14159265
#define _2PI 				6.2831853

#define STANDARD_MEAN		0.0f
#define STANDARD_DEVIATION	1.0f
#define RAND_SAMPLE			24

#define INPUT_VECTOR_SIZE 	(3+2)*15 // 3*15 + 2 //256*128

#define ANN_NAME			"/var/neural_nets/ANN_PLAYER_V0.4"
#define ANN_DATA			"/var/neural_nets/games_played_V0.4"

#define MAX_TRAINING_EPOCHS	50
const double MIN_ERROR_LEARN = 0.01;

ros::Publisher turn_pub;
cv_bridge::CvImageConstPtr img;
bool img_received = false; 
bool table_state_received = false;
vrep_plugin::Turn player_turn;
vrep_plugin::Pool_table_state table_state;
NeuralNetwork* nn_player;
vector<trainingCase> training_data_vector;
trainingCase training_data;
vector<double> input;
vector<double> nn_output;
vector<double> turn_output;

unsigned int balls_potted;
unsigned int turns_played;
unsigned int games_played;
unsigned int total_score;
unsigned int white_ball_potted;

inline double getNormalDistribution(double mean, double sigma)
{
	double val = 0;
	for (unsigned int i = 0; i < RAND_SAMPLE; i++)
	{
		val += (double)rand()/RAND_MAX;
	}
	val /= RAND_SAMPLE; //now val in range [0..1]
	val = val - 0.5f + mean;
	//std::cout << "[] VAL = " << val << std::endl;
	return val * sigma;
}

inline void imageReceiveCallback(const sensor_msgs::ImageConstPtr& msg)
{
	img = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
	img_received = true;
}

inline void poolTableStateCallback(const vrep_plugin::Pool_table_state& msg)
{
	table_state = msg;
	table_state_received = true;
	//ROS_INFO("White ball position: x=%f y=%f z=%f", msg.white_ball.x, msg.white_ball.y, msg.white_ball.z);
	ROS_INFO("New table state received!");
}

inline void save_games()
{
	std::ofstream dataFile;
    dataFile.open(ANN_DATA, std::ios_base::out|std::ios_base::trunc);

    if (dataFile.is_open())
    {
    	dataFile << games_played << " " << total_score << " " << turns_played << " " << white_ball_potted;
	}
	else
	{
		std::cout << "[ANN_POOL_PLAYER] Cannot save player data." << std::endl;
	}
	dataFile.close();
}

inline void load_games()
{
	std::ifstream dataFile;
    dataFile.open(ANN_DATA);

    if (dataFile.is_open())
    {
    	dataFile >> games_played >> total_score >> turns_played >> white_ball_potted;
    	std::cout << "[ANN_POOL_PLAYER] The NeuralNetwork has played " << games_played << 
    		" games so far. Score: " << total_score << 
    		" Total turns played: " << turns_played << 
    		" Total white ball potted: " << white_ball_potted << std::endl;
	}
	else
	{
		std::cout << "[ANN_POOL_PLAYER] Cannot load player data." << std::endl;
		games_played = 0;
		total_score = 0;
		turns_played = 0;
		white_ball_potted = 0;
	}
	dataFile.close();
}

void correct_ann_output(vector<double>& o)
{
	//choose a ball
	for (unsigned int i = 0; i < BALL_COUNT; i++)
	{
		if (table_state.balls[i].z > TABLE_HEIGHT)
		{
			double a = (table_state.balls[i].x - table_state.white_ball.x + TABLE_WIDTH/2) / TABLE_WIDTH;
			double b = (table_state.balls[i].y - table_state.white_ball.y + TABLE_LENGTH/2) / TABLE_LENGTH;
			double c = sqrt(a*a + b*b);
			o[0] = c;
			// o[1] = ((b / c) + 1) / 2;
			// o[2] = ((a / c) + 1) / 2;
			o[1] = ((a / c) + 1) / 2;
			o[2] = ((b / c) + 1) / 2;
			ROS_INFO("Expert values: %.4f %.4f %.4f", o[0], o[1], o[2]);
			return;
		}
	}
}

inline void turn_score_callback(const std_msgs::Int32& msg)
{
	if (msg.data >= SCORE_THRESHOLD)
	{
		ROS_INFO("Score(%d) >= threshold!!!", msg.data);
		training_data.input = input;
		training_data.output = turn_output;

		training_data_vector.clear();
		training_data_vector.push_back(training_data);
		nn_player->trainWRTAccuracyError(training_data_vector, MIN_ERROR_LEARN, MAX_TRAINING_EPOCHS);
		ROS_INFO("Neural Network trained!");
		nn_player->saveToFile(ANN_NAME);
		ROS_INFO("Network saved to file(\"%s\").", ANN_NAME);

		total_score += msg.data;
		balls_potted++;
		if (balls_potted >= BALL_COUNT)
		{
			games_played++;
			balls_potted = 0;
		}
	}
	else
	{
		ROS_INFO("Score(%d) below threshold (%d) to train.", msg.data, SCORE_THRESHOLD);
		if (msg.data < 0) white_ball_potted++;

		//if (EXPERT_ON && (msg.data <= EXPERTISE_SCORE))
		if (EXPERT_ON && (msg.data <= 0))
		{
			ROS_INFO("Giving expertise knowledge.");
			training_data.input = input;
			correct_ann_output(turn_output);
			training_data.output = turn_output;

			training_data_vector.clear();
			training_data_vector.push_back(training_data);
			nn_player->trainWRTAccuracyError(training_data_vector, MIN_ERROR_LEARN, MAX_TRAINING_EPOCHS);
			ROS_INFO("Neural Network trained!");
		}
	}

	save_games();
}

int main(int argc, char** argv)
{
	init(argc, argv, "ann_player");
	if (!ros::master::check())
	{
		ROS_ERROR("[ANN_POOL_PLAYER] Cannot detect ROS master!");
		return 1;
	}
	ROS_INFO("Subscribing and Advertising.");
	NodeHandle* node = new ros::NodeHandle();	
	turn_pub = node->advertise<vrep_plugin::Turn>("player_turn", 1);  	

	image_transport::ImageTransport img_transport(*node);
	image_transport::Subscriber img_sub = img_transport.subscribe("camera_image", 1, imageReceiveCallback);
	ros::Subscriber table_state_sub = node->subscribe("pool_table_state", 1, poolTableStateCallback);
	ros::Subscriber score_sub = node->subscribe("turn_score", 1, turn_score_callback);
	Rate img_refresh(10); //10Hz

	srand(time(NULL));

	std::cout << "[ANN_PLAYER] Read from (f)ile or create a (n)ew network? ";
	char choice;
	std::cin >> choice;
	choice = tolower(choice);

	turn_output = vector<double>(3, 1.f); 
	training_data.output = vector<double>(3, 1.f); 

	if (choice == 'n')
	{
		ROS_INFO("Creating a new Artificial Neural Network.");
		std::vector<int> neuronNumbers;
		neuronNumbers.push_back(150);
		neuronNumbers.push_back(80);
		neuronNumbers.push_back(20);
		//neuronNumbers.push_back(600);
		//neuronNumbers.push_back(1900);
		//neuronNumbers.push_back(500);
		neuronNumbers.push_back(3); //Neurons in layers
				
		//input vector size, number of layers, neurons in layers 
		nn_player = new NeuralNetwork(INPUT_VECTOR_SIZE, neuronNumbers.size(), neuronNumbers);
		
		//
		//Give expertise knowledge -shoot straight ahead
		//
		training_data.output[0] = 0.8;
		training_data.output[1] = 0;
		training_data.output[2] = 1;

		training_data.input = input;
		training_data.output = turn_output;

		training_data_vector.clear();
		training_data_vector.push_back(training_data);
		nn_player->trainWRTAccuracyError(training_data_vector, MIN_ERROR_LEARN, 2);

		games_played = 0;
		total_score = 0;
		turns_played = 0;
		white_ball_potted = 0;
		save_games();
	}
 	else if (choice == 'f')
	{
		nn_player = new NeuralNetwork(ANN_NAME);
		load_games();
	}
	else
	{
		ROS_WARN("NeuralNetwork cannot be created!");
		return 1;
	}
	
	balls_potted = 0;

	srand(time(NULL));
	ROS_INFO("Awaiting table state messages...");

	while(ros::ok())
	{
		spinOnce();

		if (img_received )
		{
			img_received = false;
			imshow("Image", img->image);
			waitKey(5);
		}

		if (table_state_received)
		{
			input.clear();
			//input.push_back((table_state.white_ball.x + TABLE_WIDTH/2) / TABLE_WIDTH);
			//input.push_back((table_state.white_ball.y + TABLE_LENGTH/2) / TABLE_LENGTH);
			
			//Function to choose the next ball to hit or add all balls
			for (unsigned int i = 0; i < BALL_COUNT; i++)
			{
				//White ball
				input.push_back((table_state.white_ball.x + TABLE_WIDTH/2) / TABLE_WIDTH);
				input.push_back((table_state.white_ball.y + TABLE_LENGTH/2) / TABLE_LENGTH);
				//Red ball[i]
				input.push_back((table_state.balls[i].x + TABLE_WIDTH/2) / TABLE_WIDTH);
				input.push_back((table_state.balls[i].y + TABLE_LENGTH/2) / TABLE_LENGTH);
				input.push_back((table_state.balls[i].z > TABLE_HEIGHT)?(1):(0)); // On table. not on table
			}

			nn_output = nn_player->calculate(input);
			ROS_INFO("NN output: %.4f %.4f %.4f", nn_output[0], nn_output[1], nn_output[2]);
			
			turn_output = nn_output;
			// Player turn 
			// Power [0..1]
			// Angel [0..2*PI]

			//Add gausian variance to the output proportional to score
			turn_output[0] += getNormalDistribution(STANDARD_MEAN, STANDARD_DEVIATION);
			turn_output[1] += getNormalDistribution(STANDARD_MEAN, 
				(0.5 + ((double)(2)/(games_played + 1))) * STANDARD_DEVIATION);
			turn_output[2] += getNormalDistribution(STANDARD_MEAN, 
				(0.5 + ((double)(2)/(games_played + 1))) * STANDARD_DEVIATION);

			turn_output[0] = _MAX(turn_output[0], 0); // make sure it's above 0
			turn_output[0] = _MIN(turn_output[0], 1); // make sure it's below 1
			//make them wrappable
			while (turn_output[1] < 0) // make sure it's above 0
			{
				turn_output[1] = 1.0 - turn_output[1];
			}
			while (turn_output[1] > 1) // make sure it's below 1
			{
				turn_output[1] -= 1.0;	
			}
			while (turn_output[2] < 0) // make sure it's above 0
			{
				turn_output[2] = 1.0 - turn_output[2];
			}
			while (turn_output[2] > 1) // make sure it's below 1
			{
				turn_output[2] -= 1.0;	
			}
			ROS_INFO("New value: %.4f %.4f %.4f", turn_output[0], turn_output[1], turn_output[2]);
			
			double a = (turn_output[1] - 0.5) * 2;
			double b = (turn_output[2] - 0.5) * 2; // [-1 .. 1]
			double angle = acos(a /	sqrt(a*a + b*b)); // [0 .. PI]
			if (b < 0) angle = _2PI - angle; // [0..2PI]
			ROS_INFO("Angle: %.4f", angle);

			player_turn.power = turn_output[0];
			player_turn.angle = angle;
			
			ROS_INFO("Sent msg: %.2f %.2f", player_turn.power, player_turn.angle);
			
			turns_played++;
			turn_pub.publish(player_turn);
			ROS_INFO("Turns played: %d White ball potted: %d\n", turns_played, white_ball_potted);
			table_state_received = false;
		}

		img_refresh.sleep();
	}
	ros::shutdown();
	return 0;
}
