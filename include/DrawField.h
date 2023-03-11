#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
//#include <SFML/OpenGL.hpp>

class DrawField
{
public:
	DrawField(int size);
	virtual ~DrawField();
	bool draw(sf::Vector2i mousePos);
	std::vector<float> getImage();
	void clear();
	void render(sf::RenderWindow* window, sf::Vector2i mousePos);
	void changeBrushSize(int dir);
private:
	std::vector<float> image;
	std::vector<float> lastRenderedImage;
	std::pair<std::vector<int>, std::vector<int>> lastDrawnPoints;
	int size;
	int changedBrushSize = 0;
	int brushSize = 50;
	int brushSizeMaxDist = 0;
	sf::Image img;
	sf::Sprite sprite;
	sf::Texture texture;
	float colorToBrightness(sf::Color color);
	sf::Color brightnessToColor(float brightness);

	sf::Color backgroundColor = sf::Color::Black;

};
