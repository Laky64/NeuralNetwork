#include <iostream>
#include <fstream>
#include <string>
#include "NeuralNetwork.h"
#include "Layer.h"
#include <math.h>


NeuralNetwork::NeuralNetwork(const std::initializer_list<int>& LayerSizes, int batchSize)
{

	NumberOfLayers = LayerSizes.size();

	int previosLayerSize = 1;
	for (auto i : LayerSizes)
	{
		Layer currentLayer = Layer(i, previosLayerSize, 1);
		Layers.push_back(currentLayer);
		previosLayerSize = i;

	}


	this->batchSize = batchSize;

	totalError = 0;


	currentActivationFunction = "lr";

	currentOutputActivationFunction = "soft";

	currentRegularizationTerm = "n";

	currentErrorFunction = "ce";

	texture = new sf::RenderTexture();

	texture->create(1100, 850+50);

	texture->clear(sf::Color::White);

}

NeuralNetwork::~NeuralNetwork()
{
}

std::vector<float> NeuralNetwork::offsetNumber(int x, int y, std::vector<float>& data)
{
	for (int _x = 0; _x < abs(x); _x++)
	{
		if (x < 0)
		{
			for (int i = 0; i < 28; i++)
			{
				data.erase(data.begin());
				data.push_back(0);
			}

		}
		else
		{
			for (int i = 0; i < 28; i++)
			{
				data.insert(data.begin(), 0);
				data.pop_back();
			}
		}

	}

	for (int _y = 0; _y < abs(y); _y++)
	{
		if (y < 0)
		{
			for (int i = 0; i < 28; i++)
			{

				data.erase(data.begin() + i * 28);
				data.insert(data.begin() + i * 28 + 27, 0);
			}

		}
		else
		{
			for (int i = 0; i < 28; i++)
			{
				data.erase(data.begin() + i * 28 + 27);
				data.insert(data.begin() + i * 28, 0);
			}
		}
	}

	return data;
}


int NeuralNetwork::calculateSolution(std::vector<float> &inputs)
{
	
	std::vector<float> Outputs = calculateOutputs(inputs);

	int Solution = 0;
	float highestValue = 0;
	for (int i = 0; i < Outputs.size(); i++)
	{

		if (Outputs[i] > highestValue) {
			highestValue = Outputs[i];
			Solution = i;
		}
	}
	return Solution;
	
}

std::vector<float> NeuralNetwork::calculateOutputs(std::vector<float> &inputs, float randNoiseStrenght)
{
	//apply input values to first Layer
	for (int a = 0; a < Layers[0].NumberOfNeurons; a++)
	{
		float value = inputs[a] + ((rand() / RAND_MAX) - 0.5f) * randNoiseStrenght;
		if (value > 1)
			value = 1;
		else if (value < 0)
			value = 0;
		Layers[0].Neurons[a].setValue(value);
	}

	double softmaxSum = 0;

	//calc 
	for (int j = 1; j < NumberOfLayers; j++)
	{



		for (int a = 0; a < Layers[j].NumberOfNeurons; a++)
		{
			float value = 0;

			for (int t = 0; t < Layers[j].Neurons[a].NumberOfNeuronsreviousLayer; t++)
			{

				value += Layers[j].Neurons[a].weights[t] * Layers[j - 1].Neurons[t].value;
				if (std::isnan(value)) {
					std::cout << "NAN2: " << Layers[j].Neurons[a].weights[t] << ", " << Layers[j - 1].Neurons[t].value << "\n";
				}

			}

			//add bias
			value += Layers[j].Neurons[a].bias;

			//decide between activation and softmax
			if (j == NumberOfLayers - 1)
			{
				if (currentOutputActivationFunction == "soft")
				{
					double temp = value;
					softmaxSum += exp(temp);
					Layers[j].Neurons[a].value = value;
				}
				else {
					Layers[j].Neurons[a].value = activationFunction(value, currentOutputActivationFunction);
				}

			}
			else 
			{
				Layers[j].Neurons[a].value = activationFunction(value);
			}
			if (j == 3) {
				//std::cout << activationFunction(value) << "\n";
			}


		}


	}

	if (std::isinf(softmaxSum)) {
		std::cout << "Sum: " << softmaxSum << "\n";
		//softmaxSum = DBL_MAX;

	}


	//return Outputs
	std::vector<float> Outputs;

	for (int l = 0; l < Layers[NumberOfLayers - 1].NumberOfNeurons; l++)
	{
		if (currentOutputActivationFunction == "soft")
		{
			double temp = Layers[NumberOfLayers - 1].Neurons[l].value;
			float softmaxValue = exp(temp);
			if (std::isinf(softmaxValue)) {
				Outputs.push_back(1);
			}
			else {
				Outputs.push_back(softmaxValue / softmaxSum);
			}
			
		}
		else {
			Outputs.push_back(Layers[NumberOfLayers - 1].Neurons[l].value);
		}

		//std::cout << softmaxValue / softmaxSum << "\n";
	}

	return Outputs;
}

std::vector<float> NeuralNetwork::feedForward(int startLayer, std::vector<float>& inputs, float randNoiseStrenght)
{
	
	//apply input values to first Layer
	for (int a = 0; a < Layers[startLayer].NumberOfNeurons; a++)
	{
		float value = inputs[a] + ((rand() / RAND_MAX) - 0.5f) * randNoiseStrenght;
		//if (value > 1)
			//value = 1;
		//else if (value < 0)
			//value = 0;
		Layers[startLayer].Neurons[a].setValue(value);
		std::cout << value << "\n";
	}

	double softmaxSum = 0;


	//calc 
	for (int j = 1 + startLayer; j < NumberOfLayers; j++)
	{

		std::cout << Layers[j].NumberOfNeurons << "\n";

		for (int a = 0; a < Layers[j].NumberOfNeurons; a++)
		{
			float value = 0;
			for (int t = 0; t < Layers[j].Neurons[a].NumberOfNeuronsreviousLayer; t++)
			{

				value += Layers[j].Neurons[a].weights[t] * Layers[j - 1].Neurons[t].value;
				if (std::isnan(value)) {
					std::cout << "NAN2: " << Layers[j].Neurons[a].weights[t] << ", " << Layers[j - 1].Neurons[t].value << "\n";
				}

			}

			//add bias
			value += Layers[j].Neurons[a].bias;

			//decide between activation and softmax
			if (j == NumberOfLayers - 1)
			{
				if (currentOutputActivationFunction == "soft")
				{
					double temp = value;
					softmaxSum += exp(temp);
					Layers[j].Neurons[a].value = value;
				}
				else {
					Layers[j].Neurons[a].value = activationFunction(value, currentOutputActivationFunction);
				}

			}
			else
			{
				Layers[j].Neurons[a].value = activationFunction(value);
			}

			if (j == 3) {
				//std::cout << activationFunction(value) << "\n";
			}

		}


	}

	if (std::isinf(softmaxSum)) {
		std::cout << "Sum: " << softmaxSum << "\n";
		//softmaxSum = DBL_MAX;

	}


	//return Outputs
	std::vector<float> Outputs;

	for (int l = 0; l < Layers[NumberOfLayers - 1].NumberOfNeurons; l++)
	{
		if (currentOutputActivationFunction == "soft")
		{
			double temp = Layers[NumberOfLayers - 1].Neurons[l].value;
			float softmaxValue = exp(temp);
			if (std::isinf(softmaxValue)) {
				Outputs.push_back(1);
			}
			else {
				Outputs.push_back(softmaxValue / softmaxSum);
			}

		}
		else {
			Outputs.push_back(Layers[NumberOfLayers - 1].Neurons[l].value);
		}

		//std::cout << softmaxValue / softmaxSum << "\n";
	}

	return Outputs;
}


int NeuralNetwork::train(std::vector<std::pair<std::vector<float>, int>>& data)
{
	std::pair<std::vector<float>, int> modified;
	for (int i = 0; i < data.size() / batchSize; i++)
	{

		std::vector<std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>>> Gradients;

		for (int j = 0; j < batchSize; j++)
		{

			Gradients.push_back(Backpropagation(data[i * batchSize + j]));
			
		}

		std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> AverageGradient = averageWeightsGradient(Gradients);


		applyWeightsAndBiasGradient(AverageGradient);
	}

	return 0;
}

int NeuralNetwork::trainMultipleSolutions(std::vector<std::pair<std::vector<float>, std::vector<float>>>& data)
{
	std::pair<std::vector<float>, int> modified;
	for (int i = 0; i < data.size() / batchSize; i++)
	{

		std::vector<std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>>> Gradients;

		for (int j = 0; j < batchSize; j++)
		{

			Gradients.push_back(BackpropagationMultipleSolutions(data[i * batchSize + j]));

		}


		std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> AverageGradient = averageWeightsGradient(Gradients);


		applyWeightsAndBiasGradient(AverageGradient);
	}

	return 0;
}



void NeuralNetwork::render(sf::RenderWindow* window, sf::Vector2i size, sf::Vector2i position)
{
	if (renderFrame && !renderWeights) {
		renderFrame = false;
		
		for (int i = 0; i < NumberOfLayers; i++)
		{
			sf::CircleShape circle;
			circle.setRadius(10);
			for (int j = 0; j < Layers[i].NumberOfNeurons; j++)
			{
				
				float val = (activationFunction(abs(Layers[i].Neurons[Layers[i].NumberOfNeurons - j].value), "s") - 0.5f) * 4;
				if (val != 0) {

				

					circle.setPosition((float)size.x / (NumberOfLayers + 1) * (i + 1), (float)size.y / (Layers[i].NumberOfNeurons + 1) * (j));

					circle.setFillColor(sf::Color(253, 197, 92, 255));
					texture->draw(circle);
				}
			}

			for (int j = 0; j < Layers[i].NumberOfNeurons; j++)
			{

				float val = (activationFunction(abs(Layers[i].Neurons[Layers[i].NumberOfNeurons - j].value), "s") - 0.5f) * 4;
				if (val != 0) {

					circle.setPosition((float)size.x / (NumberOfLayers + 1) * (i + 1), (float)size.y / (Layers[i].NumberOfNeurons + 1) * (j));
					val = 1;
					circle.setFillColor(sf::Color(253 - 198 * val, 197 + 55 * val, 92 + 52 * val, 255 * val));
					texture->draw(circle);
				}
			}
		}

		sprite.setTexture(texture->getTexture());
		sprite.setPosition(sf::Vector2f(position.x,position.y-50));
		window->draw(sprite);

	}
	if (renderWeights) {
		renderWeights = false;
		texture->clear(sf::Color::White);
		for (int i = 1; i < NumberOfLayers; i++)
		{
			for (int j = 0; j < Layers[i].NumberOfNeurons; j++) {
				for (int l = 0; l < Layers[i - 1].NumberOfNeurons; l++)
				{
					sf::Vertex line[] =
					{

						sf::Vertex(sf::Vector2f((float)size.x / (NumberOfLayers + 1) * (i + 1) + 10, (float)size.y / (Layers[i].NumberOfNeurons + 1) * (j + 1) + 5)),
						sf::Vertex(sf::Vector2f((float)size.x / (NumberOfLayers + 1) * (i)+10, (float)size.y / (Layers[i - 1].NumberOfNeurons + 1) * (1 + l) + 5))
					};
					int num = (Layers[i].Neurons[j].weights[l]) / abs(Layers[i].Neurons[j].weights[l]) * 0.5 + 0.5;
					line[0].color = sf::Color(255 * abs(num - 1), 0, 255 * (num), 100 * (activationFunction(abs(Layers[i].Neurons[j].weights[l]),"s") - 0.5f));
					line[1].color = sf::Color(255 * abs(num - 1), 0, 255 * (num), 100 * (activationFunction(abs(Layers[i].Neurons[j].weights[l]),"s") - 0.5f));
					if (isnan(Layers[i].Neurons[j].weights[l]))
					{
						line[0].color = sf::Color(0, 250, 0, 255);
						line[1].color = sf::Color(0, 250, 0, 255);
					}
					texture->draw(line, 2, sf::Lines);
				}

			}
		}

		for (int i = 0; i < NumberOfLayers; i++)
		{

			for (int j = 0; j < Layers[i].NumberOfNeurons; j++)
			{
				sf::CircleShape circle;
				circle.setRadius(10);
				circle.setFillColor(sf::Color(253, 197, 92, 255));
				circle.setPosition((float)size.x / (NumberOfLayers + 1) * (i + 1), (float)size.y / (Layers[i].NumberOfNeurons + 1) * (j + 1));
				circle.setOutlineThickness(2);
				circle.setOutlineColor(sf::Color(253, 197, 92, 255));
				texture->draw(circle);

			}
		}
	}


}




float NeuralNetwork::activationFunction(float x, std::string forceType)
{
	if (forceType == "") {
		forceType = currentActivationFunction;
	}

	if (forceType == "s")
	{
		return 1.0 / (1.0 + powf(2.71828, -x));
	}
	else if (forceType == "r")
	{

		if (x < 0)
		{
			return 0;
		}
		else
		{
			return x;
		}
	}
	else if (forceType == "lr")
	{

		if (x < 0)
		{
			return 0.01*x;
		}
		else
		{
			return x;
		}
	}
	else if (forceType == "tr")
	{

		if (x < 0)
		{
			return 0.001 * x;
		}
		else
		{
			return 0.1 * x;
		}
	}
}


float NeuralNetwork::activationFunctionPartialDerivative(float x, std::string forceType)
{
	if (forceType == "") {
		forceType = currentActivationFunction;
	}

	if (forceType == "s")
	{
		return x*(1-x);
	}
	else if (forceType == "r")
	{
		if (x < 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (forceType == "lr")
	{

		if (x < 0)
		{
			return 0.01;
		}
		else
		{
			return 1;
		}
	}
	else if (forceType == "tr")
	{
		if (x < 0)
		{
			return 0.001;
		}
		else
		{
			return 0.1;
		}
	}
}


void NeuralNetwork::saveNetwork()
{
	std::ofstream outfile;
	outfile.open("networkSave.txt");

	outfile << NumberOfLayers << ",";

	for (int _Layers = 0; _Layers < NumberOfLayers; _Layers++)
	{
		outfile << Layers[_Layers].NumberOfNeurons << ",";
	}


	for (int _Layers = 1; _Layers < NumberOfLayers; _Layers++)
	{
		for (int _Neurons = 0; _Neurons < Layers[_Layers].NumberOfNeurons; _Neurons++)
		{
			for (int _Weight = 0; _Weight < Layers[_Layers].Neurons[_Neurons].NumberOfNeuronsreviousLayer; _Weight++)
			{
				outfile << Layers[_Layers].Neurons[_Neurons].weights[_Weight] << ",";
			}
			outfile << ";";
		}
		outfile << ":";
	}
	outfile << "\n";

	for (int _Layers = 1; _Layers < NumberOfLayers; _Layers++)
	{
		for (int _Neurons = 0; _Neurons < Layers[_Layers].NumberOfNeurons; _Neurons++)
		{

			outfile << Layers[_Layers].Neurons[_Neurons].bias << ",";
			
			
		}
		outfile << ":";
	}

	outfile.close();
}

void NeuralNetwork::loadNetwork()
{
	std::fstream txtFile;
	txtFile.open("../assets/Networks/networkSave.txt", std::ios::in);
	std::string strings;
	std::vector<std::string> stringVector;
	if (txtFile.is_open()) {

		while (getline(txtFile, strings)) {  
			stringVector.push_back(strings);
		}
		
	}
	else
	{
		std::cout << "Could not found file!";
	}
	

	txtFile.close();

	std::string Weights = stringVector[0];
	std::string Bias = stringVector[1];



	size_t a = Weights.find(",");
	std::string numLayers = Weights.substr(0, a);

	Weights = Weights.substr(a + 1, Weights.size());

	NumberOfLayers = std::stoi(numLayers);



	int previosLayerSize = 1;

	std::vector<Layer> clearLayers;
	Layers = clearLayers;


	for (int i = 0; i < std::stoi(numLayers); i++)
	{
		size_t a = Weights.find(",");
		std::string s = Weights.substr(0, a);

		Weights = Weights.substr(a + 1, Weights.size());

		
		Layer currentLayer = Layer(std::stoi(s), previosLayerSize, 1);
		Layers.push_back(currentLayer);
		previosLayerSize = std::stoi(s);
	}

	for (int _Layers = 1; _Layers < NumberOfLayers; _Layers++)
	{
		
		size_t a = Weights.find(":");
		std::string currentLayerString = Weights.substr(0, a);
		Weights = Weights.substr(a + 1, Weights.size());
		for (int _Neurons = 0; _Neurons < Layers[_Layers].NumberOfNeurons; _Neurons++)
		{
			
			size_t a = currentLayerString.find(";");
			std::string currentNeuronString = currentLayerString.substr(0, a);
			currentLayerString = currentLayerString.substr(a + 1, currentLayerString.size());
			for (int _Weights = 0; _Weights < Layers[_Layers].Neurons[_Neurons].NumberOfNeuronsreviousLayer; _Weights++)
			{
				size_t a = currentNeuronString.find(",");
				std::string currentWeightString = currentNeuronString.substr(0, a);
				currentNeuronString = currentNeuronString.substr(a + 1, currentNeuronString.size());
				Layers[_Layers].Neurons[_Neurons].weights[_Weights] = std::stof(currentWeightString);
			}
		}
		totalError = 0;
	}
	


	for (int _Layers = 1; _Layers < NumberOfLayers; _Layers++)
	{

		size_t a = Bias.find(":");
		std::string currentLayerString = Bias.substr(0, a);
		Bias = Bias.substr(a + 1, Bias.size());
		for (int _Neurons = 0; _Neurons < Layers[_Layers].NumberOfNeurons; _Neurons++)
		{

			size_t a = currentLayerString.find(",");
			std::string currentNeuronString = currentLayerString.substr(0, a);
			currentLayerString = currentLayerString.substr(a + 1, currentLayerString.size());
			Layers[_Layers].Neurons[_Neurons].bias = std::stof(currentNeuronString);
		}
	}

	renderWeights = true;
}

std::vector<float> NeuralNetwork::calculateNeuronValuesHiddenLayer(int Layer, std::vector<float> NeuronValuesNextLayer) {

	std::vector<float> newNeuronValues;

	for (int i = 0; i < Layers[Layer].NumberOfNeurons; i++)
	{
		float newNeuronValue = 0;
		for (int j = 0; j < NeuronValuesNextLayer.size(); j++)
		{
			newNeuronValue += Layers[Layer + 1].Neurons[j].weights[i] * NeuronValuesNextLayer[j];
		}
		newNeuronValue *= activationFunctionPartialDerivative(Layers[Layer].Neurons[i].value);

		newNeuronValues.push_back(newNeuronValue);
		if (std::isnan(newNeuronValue)) {
			std::cout << "NAN: " << i << ", " << "HiddenLayer" << ", " << 0 << ", " << 0 << "\n";
		}
	}


	return newNeuronValues;
}

std::vector<float> NeuralNetwork::calculateNeuronValuesOutputLayer(std::vector<float> Outputs, std::vector<float> Solutions) {

	std::vector<float> NeuronValues;


	for (int currentNeuron = 0; currentNeuron < Layers[NumberOfLayers - 1].NumberOfNeurons; currentNeuron++)
	{
		if (currentErrorFunction == "mse")
		{
			float first = Outputs[currentNeuron] - Solutions[currentNeuron];
			float second = activationFunctionPartialDerivative(Layers[NumberOfLayers - 1].Neurons[currentNeuron].value, currentOutputActivationFunction);

			NeuronValues.push_back(first * second);
		}
		else if (currentErrorFunction == "ce")
		{
			NeuronValues.push_back(Outputs[currentNeuron] - Solutions[currentNeuron]);
			if (std::isnan(Solutions[currentNeuron] - Outputs[currentNeuron])) {
				std::cout << "Nan3: " << Solutions[currentNeuron] << ", " << Outputs[currentNeuron] << "\n";
			}

		}


		//std::cout << "a  " << Outputs[currentNeuron] << ", " << Layers[NumberOfLayers - 1].Neurons[currentNeuron].value << "\n";
	}
	
	return NeuronValues;
}

std::vector<float> NeuralNetwork::calculateNeuronValuesInputLayer(std::vector<float> NeuronValuesNextLayer)
{
	std::vector<float> newNeuronValues;

	for (int i = 0; i < Layers[0].NumberOfNeurons; i++)
	{
		float newNeuronValue = 0;
		for (int j = 0; j < NeuronValuesNextLayer.size(); j++)
		{
			newNeuronValue += Layers[0 + 1].Neurons[j].weights[i] * NeuronValuesNextLayer[j];
		}
		newNeuronValues.push_back(newNeuronValue);
	}


	return newNeuronValues;
}


std::pair<std::vector<std::vector<float>>, std::vector<float>> NeuralNetwork::calculateGradientsWeightsAndBias(int Layer, std::vector<float> NeuronValues) {

	std::vector<std::vector<float>> weightGradients;

	std::vector<float> biasGradients;

	std::pair<std::vector<std::vector<float>>, std::vector<float>> gradients;
	//std::cout << "----------------------------------------" << "\n";
	for (int i = 0; i < Layers[Layer].NumberOfNeurons; i++)
	{
		//std::cout << NeuronValues[i] << "\n";
		std::vector<float> weightGradientsOneNeuron;
		for (int j = 0; j < Layers[Layer - 1].NumberOfNeurons; j++)
		{
			weightGradientsOneNeuron.push_back(Layers[Layer - 1].Neurons[j].value * NeuronValues[i]);

		}
		weightGradients.push_back(weightGradientsOneNeuron);
		biasGradients.push_back(NeuronValues[i]);
	}
	gradients.first = weightGradients;
	gradients.second = biasGradients;
	return gradients;
}


std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> NeuralNetwork::Backpropagation(std::pair<std::vector<float>, int>& data) {

	std::vector<float> Outputs = calculateOutputs(data.first, 0.3f);
	std::vector<float> Solutions;
	for (int i = 0; i < Outputs.size(); i++)
	{
		if (data.second == i) {
			Solutions.push_back(1);

		}
		else {
			Solutions.push_back(0);
		}   
	}


	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> Gradient;

	std::vector<float> neuronValues = calculateNeuronValuesOutputLayer(Outputs, Solutions);
	std::pair<std::vector<std::vector<float>>, std::vector<float>> gradients = calculateGradientsWeightsAndBias(NumberOfLayers - 1, neuronValues);
	Gradient.push_back(gradients);

	for (int i = NumberOfLayers - 2; i > 0; i--)
	{
		neuronValues = calculateNeuronValuesHiddenLayer(i, neuronValues);
		std::pair<std::vector<std::vector<float>>, std::vector<float>> gradients = calculateGradientsWeightsAndBias(i, neuronValues);
		Gradient.push_back(gradients);
	}

	return Gradient;
}

std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> NeuralNetwork::BackpropagationMultipleSolutions(std::pair<std::vector<float>, std::vector<float>>& data)
{

	std::vector<float> Outputs = calculateOutputs(data.first, 0.3f);
	std::vector<float> Solutions = data.second;



	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> Gradient;

	std::vector<float> neuronValues = calculateNeuronValuesOutputLayer(Outputs, Solutions);
	std::pair<std::vector<std::vector<float>>, std::vector<float>> gradients = calculateGradientsWeightsAndBias(NumberOfLayers - 1, neuronValues);
	Gradient.push_back(gradients);

	for (int i = NumberOfLayers - 2; i > 0; i--)
	{
		neuronValues = calculateNeuronValuesHiddenLayer(i, neuronValues);
		std::pair<std::vector<std::vector<float>>, std::vector<float>> gradients = calculateGradientsWeightsAndBias(i, neuronValues);
		Gradient.push_back(gradients);
	}

	return Gradient;
}




std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> NeuralNetwork::averageWeightsGradient(std::vector<std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>>> &gradients)
{
	std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>> AverageGradient;
	

	
	for (int _Layer = 0; _Layer < gradients[0].size(); _Layer++)
	{
		std::pair<std::vector<std::vector<float>>, std::vector<float>> AverageGradientPair;
		std::vector<std::vector<float>> AverageGradientWeight;
		std::vector<float> AverageGradientBias;
		for (int _Neuron = 0; _Neuron < gradients[0][_Layer].first.size(); _Neuron++)
		{

			std::vector<float> AverageGradientWeight2;
			

			for (int _Weight = 0; _Weight < gradients[0][_Layer].first[_Neuron].size(); _Weight++)
			{
				float AverageGradientWeightSum = 0;
				for (int i = 0; i < gradients.size(); i++)
				{
					AverageGradientWeightSum += gradients[i][_Layer].first[_Neuron][_Weight];
				}
				//std::cout << AverageGradientWeightSum / gradients.size() << "\n";
				AverageGradientWeight2.push_back(AverageGradientWeightSum / gradients.size());
				
			}

			float AverageGradientBiasSum = 0;
			for (int i = 0; i < gradients.size(); i++)
			{
				AverageGradientBiasSum += gradients[i][_Layer].second[_Neuron];
			}

			AverageGradientBias.push_back(AverageGradientBiasSum / gradients.size());

			AverageGradientWeight.push_back(AverageGradientWeight2);
		}
		AverageGradientPair.first = AverageGradientWeight;
		AverageGradientPair.second = AverageGradientBias;

		AverageGradient.push_back(AverageGradientPair);
	}
	


	return AverageGradient;
}

void NeuralNetwork::applyWeightsAndBiasGradient(std::vector<std::pair<std::vector<std::vector<float>>, std::vector<float>>>& gradient)
{



	for (int currentLayer = 1; currentLayer < NumberOfLayers; currentLayer++)
	{
		int currentLayerBack = NumberOfLayers - currentLayer;
		for (int currentNeuron = 0; currentNeuron < Layers[currentLayerBack].NumberOfNeurons; currentNeuron++)
		{
			
			for (int currentWeight = 0; currentWeight < Layers[currentLayerBack].Neurons[currentNeuron].NumberOfNeuronsreviousLayer; currentWeight++)
			{
				float temp = 0;

				if (currentRegularizationTerm == "n")
				{
					temp = Layers[currentLayerBack].Neurons[currentNeuron].weights[currentWeight] - LearningRate * gradient[currentLayer - 1].first[currentNeuron][currentWeight];
					//std::cout << gradient[currentLayer - 1].first[currentNeuron][currentWeight] << "\n";
				}
				else if (currentRegularizationTerm == "l2") 
				{
					float regularizationTerm = ((2 * lambda / batchSize) * Layers[currentLayerBack].Neurons[currentNeuron].weights[currentWeight]);
					temp = Layers[currentLayerBack].Neurons[currentNeuron].weights[currentWeight] - LearningRate * (gradient[currentLayer - 1].first[currentNeuron][currentWeight] + regularizationTerm);
				}
				else if (currentRegularizationTerm == "lim")
				{
					temp = Layers[currentLayerBack].Neurons[currentNeuron].weights[currentWeight] - LearningRate * (gradient[currentLayer - 1].first[currentNeuron][currentWeight]);
					if (temp > 10) 
						temp = 10;
					
				}
				
				Layers[currentLayerBack].Neurons[currentNeuron].weights[currentWeight] = temp;
			}

			Layers[currentLayerBack].Neurons[currentNeuron].bias -= LearningRate * gradient[currentLayer - 1].second[currentNeuron];
		}

	}
}



double NeuralNetwork::calculateAverageError(std::vector<std::pair<std::vector<float>, int>>& data)
{
	double averageCost = 0;
	for (int i = 0; i < data.size(); i++)
	{
		averageCost += calculateError(data[i]);
	}
	return averageCost / data.size();
}

float NeuralNetwork::calculateAccuracy(std::vector<std::pair<std::vector<float>, int>>& data)
{

	int correct = 0;
	for (int i = 0; i < data.size(); i++)
	{
		if(calculateSolution(data[i].first) == data[i].second)
			correct++;
	}
	return  (float) correct / data.size();
}

void NeuralNetwork::addLearningRate(float addition)
{
	LearningRate += addition;
}



double NeuralNetwork::calculateError(std::pair<std::vector<float>, int>& data)
{
	double cost = 0;
	std::vector<float> Outputs = calculateOutputs(data.first);
	std::vector<int> Solutions;
	for (int i = 0; i < Outputs.size(); i++)
	{
		if (data.second == i){
			Solutions.push_back(1);
			
		}
		else
			Solutions.push_back(0);
	}
	
	for (int i = 0; i < Outputs.size(); i++)
	{
		if (currentErrorFunction == "mse")
		{
			double outputCost = Outputs[i] - Solutions[i];

			cost += outputCost * outputCost;
		}
		else if (currentErrorFunction == "ce")
		{
			float temp = Solutions[i] * log10(Outputs[i]);
			if (std::isnan(temp) && Solutions[i] == 0) {
				temp = 0;
			}
			cost += temp;
		}
		
	}
	//std::cout << cost << "\n";
	if (currentErrorFunction == "mse")
	{
		return cost * 0.5f;
	}
	else if (currentErrorFunction == "ce")
	{
		return -cost;
	}
}


double NeuralNetwork::calculateAverageErrorMultipleSolutions(std::vector<std::pair<std::vector<float>, std::vector<float>>>& data)
{
	double averageCost = 0;
	for (int i = 0; i < data.size(); i++)
	{
		averageCost += calculateErrorMultipleSolutions(data[i]);
	}
	return averageCost / data.size();
}

double NeuralNetwork::calculateErrorMultipleSolutions(std::pair<std::vector<float>, std::vector<float>>& data)
{
	double cost = 0;
	std::vector<float> Outputs = calculateOutputs(data.first);
	std::vector<float> Solutions = data.second;


	for (int i = 0; i < Outputs.size(); i++)
	{
		double outputCost = Outputs[i] - Solutions[i];

		cost += outputCost * outputCost;
	}
	//std::cout << cost << "\n";
	return cost * 0.5f;
}