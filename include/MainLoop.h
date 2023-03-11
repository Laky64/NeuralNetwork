#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include "DrawField.h"


class MainLoop
{
public:
	MainLoop();
	virtual ~MainLoop();

	//Called by main every frame
	void update();
	void render();

	// Accessors
	const bool running() const;

private:

	//-Network-//
	int datasetSize = 100;
	NeuralNetwork network = NeuralNetwork({ 28 * 28, 450, 250, 100, 30, 10 }, 10);
	//NeuralNetwork network2 = NeuralNetwork({ 28 * 28, 25, 10 }, 10);

	//-Window-//
	sf::RenderWindow* window;

	void renderText();

	//-Event-//
	sf::Event event;


	//-initilize-//
	void initVariables();
	void initWindow();

	//-Data-//
	std::ifstream fileData;
	std::ifstream fileTestData;
	std::ifstream fileData2;
	void drawNumber(std::vector<float>& data, sf::Vector2f offset);

	std::vector<float> offsetNumber(int x, int y, std::vector<float>& data);
	std::vector<float> scaleNumber(float scaleFactor, std::vector<float>& data);
	std::vector<float> scaleImg(int newSize, std::vector<float>& data);

	//-Events-//
	void pollEvents();



	//--Private Variables --//

	std::vector<std::pair<std::vector<float>, int>> allData;
	std::vector<std::pair<std::vector<float>, std::vector<float>>> dataMultipleSolutions;
	std::vector<std::pair<std::vector<float>, int>> testData;
	std::vector<std::pair<std::vector<float>, std::vector<float>>> testDataMultipleSolutions;
	int currentTestNumber = 0;
	bool learn = false;
	DrawField drawField = DrawField(560);
	std::vector<float> downScaledImg;
	sf::RenderTexture* numberTexture;
	std::vector<float> lastNumberTexture;
	sf::Sprite sprite;
	int solution = 0;
	std::vector<float> solutions;
	bool loopData = true;
	int loopDataIndex = 0;


	//-inputs-//
	bool leftMouseButton;
	sf::Vector2i mousePos;

	//-timing-fps-//
	float fps;
	sf::Clock clock;
	sf::Time previousTime;
	sf::Time currentTime;



};
