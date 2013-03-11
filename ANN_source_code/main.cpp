//============================================================================
// Name        : main.cpp
// Author      : Daniel Angelov
// Version     :
// Description : Implementation of an artificial neural network
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "include/NeuralNetwork.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
using namespace std;

#define MAX_TRAIN_EPOCHS    10000
#define MIN_TRAIN_ERROR     0.1


void getTrainingData(vector<trainingCase>& lData)
{
    //Read a file or generate data
    //Generating XOR data
    vector<double> input(2); // consists of 2 elements
    vector<double> true_output(1);
    for (int i = 0; i < 4; i++)
    {
        input[0] = ((i&2)>>1);
        input[1] = (i & 1);
        true_output[0] = (int)input[0] ^ (int)input[1];
        lData.push_back(trainingCase(input, true_output));
    }
}


void loadTestData(vector<trainingCase>& tData)
{

}

int main()
{
    vector<trainingCase> testData;
    vector<trainingCase> trainingData;

    getTrainingData(trainingData);
    // loadTestData(testData);
    // with the xor problem, the trainig data is the test data
    testData = trainingData;

    vector<int> neuronNumbers;
    neuronNumbers.push_back(2); // first layer has 2 neurons
    neuronNumbers.push_back(1); // second layer has 1 neuron

    NeuralNetwork xor_nn;

    cout << "Creating NN! " << endl;
    srand(time(NULL));

    //xor_nn = NeuralNetwork("xor_problem.txt"); //load the ANN from a file

    xor_nn = NeuralNetwork(2, neuronNumbers.size(), neuronNumbers); // input vector size, num of layers, number of neurons in layer
    xor_nn.train(trainingData, MIN_TRAIN_ERROR, MAX_TRAIN_EPOCHS); // vector with training data, train until maxError is reached or maxIteration - number of epochs are undergone.
    xor_nn.saveToFile("xor_problem.txt");

    //Test the ANN
    for (int i=0; i< testData.size(); i++)
    {
        cout <<"TEST data #" << i << "\n";
        vector <double> output = xor_nn.calculate(testData[i].input);
        for (int j=0; j<output.size(); j++)
        {
            cout << "Netowrk output : " << output[j] << " -- Real: " << testData[i].output[j]<<"\n";
        }
    }
    return 0;
}
