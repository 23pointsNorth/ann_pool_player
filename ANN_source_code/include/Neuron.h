/*
 * Neuron.h
 *
 *  Created on: June 11, 2011
 *      Author: Daniel Angelov
 */

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <math.h>

#define INITAL_WEIGHT 0.5
//the range [-INITAL_WEIGHT; INITAL_WEIGHT] that the value of the neurons is randomized

using std::vector;

class Neuron
{
    public:
        Neuron(int NumOfInputs);
        Neuron(vector<double> neuronWeightsAndBias);
        Neuron();
        ~Neuron();
        void setWeights(vector<double> w);
        vector<double> getWeights();
        double getWeight(int atSource);
        void setWeight(int inputSource, double newWeight);
        void changeWeight(int inputSource, double changeW);
        double calculate(vector<double> in);
        void setError(double newError);
        double getError();
        double getLastOutput();
        double getLastInputOn(int neuronWInput);
        double getLastWeightChagne(int nInput);
        void printNeuronWeight();

        int numInputs;

        //not private due to acceleration issues
        vector <double> weights;
        vector<double> lastWeightChanged;
        double lastCalculatedOutput;
        vector <double> lastInputs;
        double error;
    protected:
    private:

};

#endif // NEURON_H
