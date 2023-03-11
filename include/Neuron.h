#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>


class Neuron
{
public:
	Neuron(int NumberOfNeuronsreviousLayer);
	virtual ~Neuron();
	int NumberOfNeuronsreviousLayer;
	float value;	
	std::vector<float> weights;
	float bias;
	void setWeight(int pos, float weight);
	void setValue(float value);


};

