/*
 * NeuralLayer.h
 *
 *  Created on: June 11, 2011
 *      Author: Daniel Angelov
 */

#ifndef NEURALLAYER_H
#define NEURALLAYER_H

#include <vector>
#include "Neuron.h"

using std::vector;

class NeuralLayer
{
    public:
        NeuralLayer(int numNeuronsInCurrentLayer, int numInPreviousLayer);
        NeuralLayer(vector<Neuron> n);
        NeuralLayer();
        ~NeuralLayer();
        vector<double> calculate(vector<double> inputs);

		//not private due to optimisations
        vector<Neuron> neurons;
    protected:
    private:
};

#endif // NEURALLAYER_H
