#include "DrawField.h"

DrawField::DrawField(int size)
{
	for (int i = 0; i < size * size; i++)
	{
		image.push_back(0);
		lastRenderedImage.push_back(0);
	}
	for (int i = 0; i < brushSize * brushSize; i++)
	{
		lastDrawnPoints.first.push_back(0);
		lastDrawnPoints.second.push_back(0);
	}

	this->size = size;

	brushSizeMaxDist = ((brushSize / 2) * (brushSize / 2)) * 2;
	img.create(size, size, backgroundColor);
	//glEnable(GL_TEXTURE_2D);
	changedBrushSize = false;
}

DrawField::~DrawField()
{

}

bool DrawField::draw(sf::Vector2i mousePos)
{
	bool changed = true;


	lastDrawnPoints.second = lastDrawnPoints.first;
	std::vector<int> temp;
	lastDrawnPoints.first = temp;
	if (mousePos.x > 0 && mousePos.x < this->size && mousePos.y > 0 && mousePos.y < this->size) {
		if (colorToBrightness(img.getPixel(mousePos.x, mousePos.y)) < 1.99f) {
			for (int y = -(brushSize / 2); y < brushSize / 2; y++)
			{

				

				for (int x = -(brushSize / 2); x < brushSize / 2; x++)
				{
					int pixelPos = mousePos.x + x + this->size * mousePos.y + y * this->size;
					if (pixelPos > 0 && pixelPos < this->size * this->size) {
						//std::cout << brushSizeMaxDist << ",  " << (x * x + y * y) << "\n";
						float addition = 1 - ((x * x + y * y) / (float)brushSizeMaxDist) * 2;
						if (addition < 0) {
							addition = 0;
						}
						float value = image[pixelPos] + addition * 2.5f;
						if (value > 1) {
							image[pixelPos] = 1;
							value = 1;
						}
						image[mousePos.x + x + this->size * mousePos.y + y * this->size] = value;
						sf::Color newColor = brightnessToColor(value);
						img.setPixel(mousePos.x + x, mousePos.y + y, newColor);

						//std::cout << image[mousePos.x + x + this->size * mousePos.y + y * this->size] << "\n";


						//image[mousePos.x + x + this->size * mousePos.y + y * this->size] = ceilf(image[mousePos.x + x + this->size * mousePos.y + y * this->size]);
					}
				}
				
			}
		}
		else {
			changed = false;
		}

	}
	else {
		changed = false;
	}
	return changed;
}

std::vector<float> DrawField::getImage()
{
	return image;
}

void DrawField::clear()
{
	std::vector<float> img;
	for (int i = 0; i < size * size; i++)
	{
		img.push_back(0);
	}
	image = img;

	this->img.create(size, size, backgroundColor);

}

void DrawField::render(sf::RenderWindow* window, sf::Vector2i mousePos)
{

	texture.loadFromImage(img);
	sprite.setTexture(texture);
	window->draw(sprite);

	if (changedBrushSize > 0) {
		changedBrushSize--;
		//std::cout << mousePos.x << "\n";
		sf::CircleShape circle;
		circle.setRadius(brushSize / 2);

		circle.setPosition(sf::Vector2f(mousePos.x - brushSize / 2, mousePos.y - brushSize / 2));

		circle.setFillColor(sf::Color(0, 0, 0, 0));
		int tempFadeTimer = changedBrushSize;
		if (changedBrushSize > 100)
			tempFadeTimer = 100;
		circle.setOutlineColor(sf::Color(230, 230, 230, tempFadeTimer*2.55));
		circle.setOutlineThickness(3);
		window->draw(circle);
	}

}

void DrawField::changeBrushSize(int dir)
{
	changedBrushSize = 200;
	brushSize += dir * 5;
	if (brushSize < 10)
		brushSize = 10;
	else if (brushSize > 300)
		brushSize = 300;
	brushSizeMaxDist = ((brushSize / 2) * (brushSize / 2)) * 2;

}

float DrawField::colorToBrightness(sf::Color color)
{
	float sum = (color.r + color.g + color.b) / 255.0;
	return sum / 3;
}

sf::Color DrawField::brightnessToColor(float brightness)
{

	return sf::Color(255 * brightness, 255 * brightness, 255 * brightness,255);
}
