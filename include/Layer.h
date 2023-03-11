#pragma once



#include "Neuron.h"

class Layer
{
public:
	Layer(int NumberOfNeurons, int NumberOfNeuronsreviousLayer, bool isInputLayer = false);
	virtual ~Layer();
	bool isInputLayer;
	int NumberOfNeurons;
	int NumberOfNeuronsreviousLayer;

	std::vector<Neuron> Neurons;
};

