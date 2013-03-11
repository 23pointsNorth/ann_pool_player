#include "../include/NeuralNetwork.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

NeuralNetwork::NeuralNetwork(int inputVectorSize, int numLayers, vector<int> numberNeurons)
{
    std::cout<< "Creating neural network!\n";
    this->numOfLayers = numLayers;
    this->inputVector = inputVectorSize;

    numberNeurons.insert(numberNeurons.begin(),inputVectorSize);
    for (unsigned int i=1; i < numberNeurons.size(); i++)
    {
        //std::cout << "Adding a layer!\n";
        this->layers.push_back(
            NeuralLayer(numberNeurons[i], numberNeurons[i-1]));
    }
    std::cout << "Neural Network created!" << std::endl;
}

NeuralNetwork::NeuralNetwork(char* filename)
{
    std::ifstream dataFile;
    dataFile.open(filename);

    if (dataFile.is_open())
    {
        std::cout << "Reading from file.\n";
        dataFile >> this->inputVector >> this->numOfLayers;
        //this->layers = vector<NeuralLayer>(numOfLayers);
        vector<int>nInLayer;
        for (int i=0; i<this->numOfLayers; i++)
        {
            double num;
            dataFile >> num;
            nInLayer.push_back(num);
        }
        nInLayer.insert(nInLayer.begin(),this->inputVector);
        //create a layer
        //read nInLayer[i+1] neurons for i-th layer;

        for (unsigned int i=1; i < nInLayer.size(); i++)
        {
            double data;
            vector<Neuron> buildLayer;
            for (int l=0; l<nInLayer[i]; l++)
            {
                vector <double> nWeights;
                for (int n=0; n<=nInLayer[i-1]; n++)
                {
                    dataFile >> data;
                    nWeights.push_back(data);
                }
                buildLayer.push_back(Neuron(nWeights));
            }

            this->layers.push_back(NeuralLayer(buildLayer));
        }
    }
    else
    {
        std::cout << "Error openning file \"" << filename << "\"!";
        return;
    }
    dataFile.close();
    std:: cout << "Finished creating NN!\n";
}

NeuralNetwork::NeuralNetwork()
{

}

NeuralNetwork::~NeuralNetwork()
{
    //dtor
}

vector<double> NeuralNetwork::calculate(vector<double> input)
{
    vector<double> result(input);
    for (unsigned int i=0; i < this->layers.size(); i++)
    {
        result = layers[i].calculate(result);
    }
    return result;
}


void NeuralNetwork::train(vector<trainingCase> data, double maxError, int maxIteration)
{
    //printNetwork();

    std::cout << "Starting training!\n";
    int data_size = data.size();
    vector <double> output;
    double errorInAccuracy;
    double errorInCases;
    double min_error_so_far = 9999999;
    for (int iter = 0; iter<maxIteration; iter++)
    {
        std::cout << "Iterating for " << iter << " time!\n";
        errorInAccuracy = 0; // only for output neurons

        //randomising data
        for (int i=0; i< data_size; i++)
        {
            int randPlace = rand()%(data_size);
            std::swap(data[i],data[randPlace]);
        }

        for (int i=0; i< data_size; i++)
        {
            //printNetwork();
            //getchar();
            //std::cout << "Training " << i << " pattern!\n";

            output = this->calculate(data[i].input);

            int output_size = output.size();
            double deltaError;
            int max_position = 0;
            double max_num = data[i].output[max_position];

            //Set errors for final layer
            for (int j=0; j<output_size; j++)
            {
                double current_output = output[j];
                double e = data[i].output[j] - current_output;
                deltaError = e * current_output * (1 - current_output);
                layers.back().neurons[j].error = deltaError;
                errorInAccuracy += fabs(deltaError);
            }

            if (i % 500 == 0)
            {
                std::cout << "Training " << i << " pattern!\n" << "\t\t SPrev Error -> " << errorInAccuracy << " Min error so far : " << min_error_so_far << "\n";

                for (int outprint = 0; outprint < output_size; outprint++)
                    std::cout << outprint <<" - " << output[outprint] << " -- "<< data[i].output[outprint] <<  "\n";
            }

            //std::cout << "Error for final layer set!\n";
            //Propagate error forward
            for (int j=layers.size()-2; j>=0; --j)
            {
                //std::cout << "Iterating through layer:" << j <<" of the rest of the NN(Not the last layer)!\n";

                for (unsigned int n=0; n < layers[j].neurons.size(); n++)
                {
                    //std::cout << "Finding error for " << n <<" neuron of the prev. layer\n";
                    double errorSum=0;
                    for (unsigned int next_n=0; next_n < layers[j+1].neurons.size() ; next_n++)
                    {
                        errorSum+= layers[j+1].neurons[next_n].error * layers[j+1].neurons[next_n].weights[n+1] ; // last n+1 because 0 is bais
                    }
                    layers[j].neurons[n].error = errorSum * layers[j].neurons[n].lastCalculatedOutput * (1 - layers[j].neurons[n].lastCalculatedOutput);
                    //std::cout << "Setting error Complete!\n";
                }
            }
            //std::cout << "Adjusting weights!\n";
            this->adjustWeights();
        }

        if (errorInAccuracy < min_error_so_far)
        {
            std::cout << "Found new min!" << errorInAccuracy <<"\n";

            min_error_so_far = errorInAccuracy;

            if (errorInAccuracy < maxError)
            {
                std::cout <<"Network is fully trained !\n";\
                break; // or return;
            }

        }
        std::cout << "Iteration complete! Error sum : " << errorInCases << " min_so_far " << min_error_so_far <<" \n\n";
        //getchar();

        //printNetwork();
        //getchar();
    }
    std::cout << "Training end!\n";

    //Print whole network!
    //printNetwork();
}

void NeuralNetwork::printNetwork()
{
    for (unsigned int i=0; i<layers.size(); i++)
    {
        std::cout << "Layer: " << i << "\n";
        for (unsigned int j=0; j<layers[i].neurons.size(); j++)
        {
            std::cout << "Neuron " << j << " weights: ";
            layers[i].neurons[j].printNeuronWeight();
            std::cout << "\n";
        }
    }
}
void NeuralNetwork::adjustWeights()
{
    double deltaWeight;
    Neuron neuron_value;
    Neuron* current_neuron;
    double multiplier;
    int max_layers = layers.size();
    for (int i = 0; i < max_layers; i++)
    {
        int neuron_size = layers[i].neurons.size();
        for (int j = 0; j < neuron_size; j++)
        {
            current_neuron = &(layers[i].neurons[j]);
            neuron_value = *current_neuron;

            multiplier = LEARNING_RATE * neuron_value.error ; //* neuron_value.lastCalculatedOutput * (1 - neuron_value.lastCalculatedOutput);

            int numOfInputs = neuron_value.numInputs;
            for (int k = 0 ; k < numOfInputs; k++)
            {
                deltaWeight = multiplier * neuron_value.lastInputs[k] + MOMENTUM_RATE * neuron_value.lastWeightChanged[k];
                //layers[i].neurons[j].changeWeight(k,deltaWeight);
                (*current_neuron).weights[k] += deltaWeight;
                (*current_neuron).lastWeightChanged[k] = deltaWeight;

            }
        }
    }
}

void NeuralNetwork::saveToFile(char* filename)
{
    std::ofstream dataFile;
    dataFile.open(filename);

    if (dataFile.is_open())
    {
        dataFile << this->inputVector << " " << this->numOfLayers;
        for (unsigned int i=0; i<this->layers.size(); i++)
        {
            dataFile << " " << this->layers[i].neurons.size();
        }
        for (unsigned int l=0; l<this->layers.size(); l++)
        {
            for (unsigned int n=0; n<this->layers[l].neurons.size();n++)
            {
                for (int w=0; w<this->layers[l].neurons[n].numInputs; w++)
                {
                    dataFile << " " << this->layers[l].neurons[n].getWeight(w);
                }
            }
        }

    }
    else
    {
        std::cout << "Error openning file \"" << filename << "\"!";
        return;
    }
    dataFile.close();
}

//WRT ACCURACY ERROR
double NeuralNetwork::train(trainingCase data, vector<double> real_output)
{
    std::cout << "Training Started!";
    double errorInAccuracy=0;

    //std::cout <<"Epoch #" << iter <<"\n";
//output = this->calculate(data[i].input);
    errorInAccuracy=0;
    int output_size = real_output.size();
    double deltaError;

    for (int j=0; j<output_size; j++)
    {
        double current_output = real_output[j];

        deltaError = data.output[j] - current_output;
        layers.back().neurons[j].error = deltaError;
        errorInAccuracy += fabs(deltaError);
    }
    //if (data.output_number != max_position) {errorInCases+=1.0;} // the NN didn't solve it right
    for (int j=layers.size()-2; j>=0; --j)
    {
        //std::cout << "Iterating through layer:" << j <<" of the rest of the NN(Not the last layer)!\n";

        for (unsigned int n=0; n < layers[j].neurons.size(); n++)
        {
            //std::cout << "Finding error for " << n <<" neuron of the prev. layer\n";
            double errorSum=0;
            for (unsigned int next_n=0; next_n < layers[j+1].neurons.size() ; next_n++)
            {
                errorSum+= layers[j+1].neurons[next_n].error * layers[j+1].neurons[next_n].weights[n+1] ; // last n+1 because 0 is bais
            }
            layers[j].neurons[n].error = errorSum;
            //std::cout << "Setting error Complete!\n";
        }
    }
    this->adjustWeights();
    std::cout <<"\tTraining complete with error - " << errorInAccuracy <<"\n";
    return errorInAccuracy;
}



void NeuralNetwork::trainWRTAccuracyError(vector<trainingCase> data, double maxError, int maxIteration)
{
    std::cout << "\t\tStarting training!\n";
    int data_size = data.size();
    vector <double> output;
    double errorInAccuracy;

    for (int iter = 0; iter<maxIteration; iter++)
    {
        errorInAccuracy = 0; // only for output neurons

        //randomising data
        for (int i=0; i< data_size; i++)
        {
            int randPlace = rand()%(data_size);
            std::swap(data[i],data[randPlace]);
        }

        for (int i=0; i< data_size; i++)
        {
            output = this->calculate(data[i].input);

            int output_size = output.size();
            double deltaError;

            for (int j=0; j<output_size; j++)
            {
                double current_output = output[j];

                deltaError = data[i].output[j] - current_output;
                layers.back().neurons[j].error = deltaError;
                errorInAccuracy += fabs(deltaError);
            }

            //std::cout << "Error for final layer set!\n";
            for (int j=layers.size()-2; j>=0; --j)
            {
                //std::cout << "Iterating through layer:" << j <<" of the rest of the NN(Not the last layer)!\n";

                for (unsigned int n=0; n < layers[j].neurons.size(); n++)
                {
                    //std::cout << "Finding error for " << n <<" neuron of the prev. layer\n";
                    double errorSum=0;
                    for (unsigned int next_n=0; next_n < layers[j+1].neurons.size() ; next_n++)
                    {
                        errorSum+= layers[j+1].neurons[next_n].error * layers[j+1].neurons[next_n].weights[n+1] ; // last n+1 because 0 is bais
                    }
                    layers[j].neurons[n].error = errorSum;
                    //std::cout << "Setting error Complete!\n";
                }
            }
            //std::cout << "Adjusting weights!\n";
            this->adjustWeights();
        }
        if (errorInAccuracy < maxError)
        {
            break; // or return;
        }

        //std::cout << "\tIteration complete! Error sum : " << errorInAccuracy << "\n";
     }
}

void NeuralNetwork::train(trainingCase data, double maxError, int maxIteration)
{
    for (int iter=0; iter<maxIteration; iter++)
    {
        double error = train(data,this->calculate(data.input));
        if (error < maxError)
        {
            break;
        }
    }
    std::cout << "Network trained!\n";
}
