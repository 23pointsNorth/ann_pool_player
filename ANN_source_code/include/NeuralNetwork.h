/*
 * NeuralNetwork.h
 *
 *  Created on: June 11, 2011
 *      Author: Daniel Angelov
 */
 
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include "NeuralLayer.h"

#define LEARNING_RATE 0.2
#define MOMENTUM_RATE 0.5

using std::vector;

typedef struct trainingCase{
    vector<double> input;
    vector<double> output;
    int output_number;
    trainingCase(){}
    trainingCase(vector<double> i, vector<double> o, int o_num)
    {
        this->input = i;
        this->output = o;
        this->output_number = o_num;
    }
    trainingCase(vector<double> i, vector<double> o)
    {
        this->input = i;
        this->output = o;
        this->output_number = -1;
    }
};

//feed-forward back-propagation network
class NeuralNetwork
{
    public:
        NeuralNetwork(int inputVectorSize, int numLayers, vector<int> numberNeurons);
        NeuralNetwork(char* filename);
        NeuralNetwork();
        ~NeuralNetwork();
        vector<double> calculate(vector<double> input);
        void train(vector<trainingCase> data, double maxError, int maxIteration);

        void printNetwork();
        void saveToFile(char* filename);

        //Special training functions
        double train(trainingCase data, vector<double> real_output);
        void train(trainingCase data, double maxError, int maxIteration);
        void trainWRTAccuracyError(vector<trainingCase> data, double maxError, int maxIteration);

    protected:
    private:
        vector<NeuralLayer> layers;
        void adjustWeights();
        int numOfLayers;
        int inputVector;
        vector<int> numOfNeuronsInLayer;

};

#endif // NEURALNETWORK_H
