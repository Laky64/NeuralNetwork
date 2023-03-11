#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Layer.h"
#include "Neuron.h"

class NeuralNetwork
{
public:
	NeuralNetwork(const std::initializer_list<int>& LayerSizes , int batchSize);
	virtual ~NeuralNetwork();

	int calculateSolution(std::vector<float> &inputs);
	std::vector<float> calculateOutputs(std::vector<float> &inputs, float randNoiseStrenght = 0);

	std::vector<float> feedForward(int startLayer, std::vector<float>& inputs, float randNoiseStrenght = 0);


	int train(std::vector<std::pair<std::vector<float>, int>>& data);
	int trainMultipleSolutions(std::vector<std::pair<std::vector<float>, std::vector<float>>>& data);

	void render(sf::RenderWindow *window,sf::Vector2i size, sf::Vector2i position);

	double calculateAverageError(std::vector<std::pair<std::vector<float>, int>>& data);

	float calculateAccuracy(std::vector<std::pair<std::vector<float>, int>>& data);

	double calculateAverageErrorMultipleSolutions(std::vector<std::pair<std::vector<float>, std::vector<float>>>& data);

	void addLearningRate(float addition);
	
	float LearningRate = 0.0001f;
	bool renderFrame = false;
	bool renderWeights = true;
	float totalError;

	void saveNetwork();
	
	void loadNetwork();


private:

	sf::RenderTexture* texture;
	sf::Sprite sprite;

	std::vector<float> offsetNumber(int x, int y, std::vector<float>& data);

	std::vector<float> calculateNeuronValuesHiddenLayer(int Layer, std::vector<float> NeuronValuesNextLayer);

	std::vector<float> calculateNeuronValuesOutputLayer(std::vector<float> Outputs, std::vector<float> Solutions);

	std::vector<float> calculateNeuronValuesInputLayer(std::vector<float> NeuronValuesNextLayer);

	std::pair<std::vector<std::vector<float>>, std::vector<float>> calculateGradientsWeightsAndBias(int Layer, std::vector<float> NeuronValues);

	int NumberOfLayers = 3;

	std::vector<Layer> Layers;


	int batchSize;



	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> Backpropagation(std::pair<std::vector<float>, int>& data);

	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> BackpropagationMultipleSolutions(std::pair<std::vector<float>, std::vector<float>>& data);

	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> averageWeightsGradient(std::vector<std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>>>& gradients);

	void applyWeightsAndBiasGradient(std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> &gradient);



	float activationFunction(float x, std::string forceType = "");
	float activationFunctionPartialDerivative(float x, std::string forceType = "");
	std::string currentActivationFunction;
	std::string currentOutputActivationFunction;
	std::string currentRegularizationTerm;
	std::string currentErrorFunction;
	float lambda = 0.001f;


	double calculateError(std::pair<std::vector<float>, int>& data);

	double calculateErrorMultipleSolutions(std::pair<std::vector<float>, std::vector<float>>& data);
};

