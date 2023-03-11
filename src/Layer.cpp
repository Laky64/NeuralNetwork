#include "Layer.h"


Layer::Layer(int NumberOfNeurons, int NumberOfNeuronsreviousLayer, bool isInputLayer)
{
	this->NumberOfNeurons = NumberOfNeurons;
	this->isInputLayer = isInputLayer;
	this->NumberOfNeuronsreviousLayer = NumberOfNeuronsreviousLayer;

	for (int i = 0; i < NumberOfNeurons; i++)
	{
		Neuron _Neuron = Neuron(NumberOfNeuronsreviousLayer);
		Neurons.push_back(_Neuron);
	}
	
}

Layer::~Layer()
{
}
