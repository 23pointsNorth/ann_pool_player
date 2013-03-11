/*
 * Neuron.cpp
 *
 *  Created on: June 11, 2011
 *      Author: Daniel Angelov
 */

#include "../include/Neuron.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Neuron::Neuron(int numOfInputs)
{
    //std::cout << "Creating neuron!\n";
    this->error = 0;
    this->numInputs = numOfInputs+1; //compensate for bias
    for (int i=0; i< numOfInputs +1 ; i++) // add for the bias
    {
        //std::cout << "Generated a weight" << std::endl;
        this->weights.push_back(fRand(-INITAL_WEIGHT,+INITAL_WEIGHT));
        this->lastWeightChanged.push_back(0.0);
        this->lastCalculatedOutput = 0.0;
        this->lastInputs.push_back(0.0);
    }
}
Neuron::Neuron(vector<double> neuronWeightsAndBias)
{
    // bias again at the front of the vector
    this->error = 0;
    this->numInputs = neuronWeightsAndBias.size();
    this->weights = neuronWeightsAndBias;
    this->lastWeightChanged = vector <double>(neuronWeightsAndBias.size(),0);
}
Neuron::Neuron()
{

}
Neuron::~Neuron()
{
    //dtor
}

//w[0] should be the bias
void Neuron::setWeights(vector<double> w)
{
    this->weights = w;
    this->lastWeightChanged = vector <double>(w.size(),0);
}

vector<double> Neuron::getWeights()
{
    return this->weights;
}
double Neuron::getWeight(int atSource)
{
    return this->weights[atSource]; // compensate for bias;
}
void Neuron::setWeight(int inputSource, double newWeight)
{
    //w[0] - bias
    this->lastWeightChanged[inputSource] = newWeight - this->weights[inputSource];
    this->weights[inputSource] = newWeight;
}

void Neuron::changeWeight(int inputSource, double changeW)
{
    this->weights[inputSource] += changeW;
    this->lastWeightChanged[inputSource] = changeW;

}

double Neuron::getLastWeightChagne(int nInput)
{
    return this->lastWeightChanged[nInput];
}

double Neuron::calculate(vector<double> in)
{
    double result = 0.0f;
    in.insert(in.begin(),1.0f); // for bias;
    int in_size = in.size();
    for (int i=0; i< in_size; i++)
    {
        result += in[i]*this->weights[i];
    }

    result = 1.0/(1.0 + exp(-result)); // calculate sigmoid funciton

    //std::cout << "Neuron outputed: " << result << "\n";
    this->lastCalculatedOutput = result;
    this->lastInputs = in;

    return result;
}


void Neuron::setError(double newError)
{
    this->error=newError;
}

double Neuron::getError()
{
    return this->error;
}

double Neuron::getLastOutput()
{
    return this->lastCalculatedOutput;
}

double Neuron::getLastInputOn(int neuronWInput)
{
    //std::cout << this->lastInputs[neuronWInput];
    return this->lastInputs[neuronWInput];
}

void Neuron::printNeuronWeight()
{
    for (unsigned int i=0; i<this->weights.size(); i++)
    {
        std::cout << this->weights[i] <<" ";
    }
}
