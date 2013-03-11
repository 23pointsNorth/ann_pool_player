/*
 * NeuralLayer.cpp
 *
 *  Created on: June 11, 2011
 *      Author: Daniel Angelov
 */

#include "../include/NeuralLayer.h"
#include <iostream>

NeuralLayer::NeuralLayer(int numNeuronsInCurrentLayer, int numInPreviousLayer)
{
    //std::cout <<"Creating neural layer!\n";
    for (int i=0; i < numNeuronsInCurrentLayer; i++)
    {
        this->neurons.push_back(Neuron(numInPreviousLayer));
    }
}

NeuralLayer::NeuralLayer(vector<Neuron> n)
{
    this->neurons = n;
}
NeuralLayer::NeuralLayer()
{

}
NeuralLayer::~NeuralLayer()
{
    //dtor
}

vector<double> NeuralLayer::calculate(vector<double> inputs)
{
    vector<double> result;
    int neurons_size = this->neurons.size();
    for (unsigned int i=0; i < neurons_size; i++)
    {
        result.push_back(neurons[i].calculate(inputs));
    }
    return result;
}
