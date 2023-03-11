#include <iostream>
#include "Neuron.h"

Neuron::Neuron(int NumberOfNeuronsreviousLayer)
{
	this->NumberOfNeuronsreviousLayer = NumberOfNeuronsreviousLayer;
	this->value = 0;



	for (int i = 0; i < NumberOfNeuronsreviousLayer; i++)
	{
		float randVal = ((float)rand() / RAND_MAX - 0.5f) / 5;
		if (randVal == 0)
			randVal = 0.001f;
		weights.push_back(randVal);
	
	}
	bias = 0;
}

Neuron::~Neuron()
{
}

void Neuron::setWeight(int pos, float weight)
{
	weights[pos] = weight;
}

void Neuron::setValue(float value)
{
	this->value = value;

}
