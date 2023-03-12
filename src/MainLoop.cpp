#include "mainLoop.h"
#include <iostream>
#include <ctime>
#include <math.h>

MainLoop::MainLoop()
{
    // initialize
    // this->initVariables();
    this->initWindow();

    for (int i = 0; i < 10; i++)
    {
        solutions.push_back(0);
    }

    numberTexture = new sf::RenderTexture();

    numberTexture->create(28 * 7, 28 * 7);

    numberTexture->clear(sf::Color::Black);

    lastNumberTexture = std::vector<float>(28 * 28);
}

MainLoop::~MainLoop()
{
    delete this->window;
    delete this->numberTexture;
}

// Accessors
const bool MainLoop::running() const
{
    return this->window->isOpen();
}

//------Functions------//
//---Voids---//

void MainLoop::drawNumber(std::vector<float> &data, sf::Vector2f offset)
{

    int size = sqrt(data.size());
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if (data[y * size + x] != lastNumberTexture[y * size + x])
            {
                sf::RectangleShape rect(sf::Vector2f(1, 1));
                rect.setSize(sf::Vector2f(7, 7));
                rect.setPosition((x)*7, (size - y) * 7);

                rect.setFillColor(sf::Color(data[y * size + x] * 255, data[y * size + x] * 255, data[y * size + x] * 255));
                numberTexture->draw(rect);
            }
        }
    }
    sprite.setTexture(numberTexture->getTexture());
    sprite.setPosition(sf::Vector2f(offset.x, offset.y));
    window->draw(sprite);
    lastNumberTexture = data;
}

std::vector<float> MainLoop::offsetNumber(int x, int y, std::vector<float> &data)
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

std::vector<float> MainLoop::scaleNumber(float scaleFactor, std::vector<float> &data)
{
    int newScale = scaleFactor * 28;
    std::vector<float> newImg;
    for (int i = 0; i < newScale * newScale; i++)
    {
        int row = i / newScale;
        int column = (i - row) - row * (newScale - 1);

        float val1 = data[row * 28 + column / scaleFactor];
        float val2 = data[row * 28 + column / scaleFactor - 1];
        newImg.push_back(0.5 * (val1 + val2));
    }
    for (int i = 0; i < ((28 - newScale) * newScale); i++)
    {
        newImg.push_back(0);
    }

    for (int j = 0; j < 28 - newScale; j++)
    {
        for (int i = 0; i < 28; i++)
        {
            newImg.insert(newImg.begin() + i * (newScale + j + 1), 0);
        }
    }

    newImg = offsetNumber((28 - newScale) / 2, -(28 - newScale), newImg);

    return newImg;
}

std::vector<float> MainLoop::scaleImg(int newSize, std::vector<float> &data)
{
    int oldScale = sqrt(data.size());
    int newScale = newSize;

    int pixelSize = oldScale / newScale;
    std::vector<float> newImg;

    for (int y = 0; y < newScale; y++)
    {
        for (int x = 0; x < newScale; x++)
        {

            float sum = 0;

            for (int j = 0; j < pixelSize; j++)
            {
                for (int i = 0; i < pixelSize; i++)
                {
                    sum += data[x * pixelSize + i + y * oldScale * pixelSize + j * oldScale];
                }
            }

            newImg.push_back(sum / (pixelSize * pixelSize));
        }
    }

    return newImg;
}

//--Events--//
void MainLoop::pollEvents()
{

    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::MouseButtonPressed:
            if (this->event.key.code == sf::Mouse::Left)

                leftMouseButton = true;
            break;
        case sf::Event::MouseButtonReleased:
            if (this->event.key.code == sf::Mouse::Left)
            {

                leftMouseButton = false;
            }
            else if (this->event.key.code == sf::Mouse::Right)
            {

                // std::cout << "AverageError: " << network.calculateAverageErrorMultipleSolutions(dataMultipleSolutions) << "\n";
                downScaledImg = network.calculateOutputs(testDataMultipleSolutions[currentTestNumber].first);

                currentTestNumber++;
            }
            break;
        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Up)
            {

                network.addLearningRate(0.01f);
                std::cout << network.LearningRate << std::endl;
            }
            else if (this->event.key.code == sf::Keyboard::Down)
            {

                network.addLearningRate(-0.01f);
                std::cout << network.LearningRate << std::endl;
            }
            else if (this->event.key.code == sf::Keyboard::Space)
            {

                network.renderWeights = true;
                if (learn)
                    learn = false;
                else
                    learn = true;
            }
            else if (this->event.key.code == sf::Keyboard::S)
            {
                network.saveNetwork();
            }
            else if (this->event.key.code == sf::Keyboard::C)
            {

                drawField.clear();
                std::vector<float> img(28 * 28);
                downScaledImg = img;
                network.calculateOutputs(downScaledImg);
                network.renderWeights = true;
            }
            else if (this->event.key.code == sf::Keyboard::L)
            {
                network.loadNetwork();
            }
            else if (this->event.key.code == sf::Keyboard::R)
            {
                std::vector<float> input;

                for (int i = 0; i < 4; i++)
                {
                    float value = ((float)rand() / RAND_MAX) * 20;

                    input.push_back(value);
                }

                downScaledImg = network.feedForward(3, input);
            }

            break;
        case sf::Event::MouseWheelMoved:
            drawField.changeBrushSize(event.mouseWheel.delta);
        }
    }
    mousePos = sf::Mouse::getPosition(*window);
    if (leftMouseButton && !learn)
    {

        if (drawField.draw(mousePos))
        {
            std::vector<float> img = drawField.getImage();
            downScaledImg = scaleImg(28, img);
            solutions = network.calculateOutputs(downScaledImg);
            downScaledImg = solutions;
            float highestValue = 0;
            for (int i = 0; i < solutions.size(); i++)
            {

                if (solutions[i] > highestValue)
                {
                    highestValue = solutions[i];
                    solution = i;
                }
            }
        }
    }

    // calc FPS
    currentTime = clock.getElapsedTime();
    fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
    previousTime = currentTime;
}

//--initialize--//
void MainLoop::initVariables()
{

    this->window = nullptr;

    previousTime = clock.getElapsedTime();

    srand(time(NULL));

    // Load the MNIST Data

    /*

    //Path for CSV File of MNIST Digits (Train)
    fileData2.open("../Data/MNIST/trainingImages/mnist_train_modified.csv");


    for (int size = 0; size < 60000; size++)
    {

        std::pair<std::vector<float>, int> data;
        std::vector<float> inputs;



        std::string line;
        fileData2 >> line;

        size_t a = line.find(",");
        std::string s = line.substr(0, a);

        data.second = std::stof(s);


        for (int i = 0; i < 28 * 28; i++)
        {
            size_t a = line.find(",");
            std::string s = line.substr(0, a);
            line = line.substr(a + 1, line.size());
            inputs.push_back(std::stof(s) / 255);

        }


        data.first = inputs;
        this->allData.push_back(data);

    }

    //Path for CSV File of MNIST Digits (Test)
    fileTestData.open("../Data/MNIST/trainingImages/mnist_test.csv");

    for (int size = 0; size < 10000; size++)
    {

        std::pair<std::vector<float>, int> data;
        std::vector<float> inputs;



        std::string line;
        fileTestData >> line;

        size_t a = line.find(",");
        std::string s = line.substr(0, a);

        data.second = std::stof(s);


        for (int i = 0; i < 28 * 28; i++)
        {
            size_t a = line.find(",");
            std::string s = line.substr(0, a);
            line = line.substr(a + 1, line.size());
            inputs.push_back(std::stof(s) / 255);

        }


        data.first = inputs;
        this->testData.push_back(data);

    }
*/
}

void MainLoop::initWindow()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    this->window = new sf::RenderWindow(sf::VideoMode(1600, 900), "Main", sf::Style::Close, settings);
}

//--Update--//

//-Called by main--//
void MainLoop::update()
{
    // Events

    this->pollEvents();

    // drawNumber(data[0].first);

    // network.train2(this->data);
    // network.train(data);
    // can only be trained if data is Loaded
    if (learn)
    {
        std::vector<std::pair<std::vector<float>, int>> data;
        if (!loopData)
        {
            for (int i = 0; i < datasetSize; i++)
            {
                data.push_back(allData[i]);
            }
        }
        else
        {
            for (int i = 0; i < datasetSize; i++)
            {
                data.push_back(allData[i + loopDataIndex]);
                loopDataIndex++;
                if (loopDataIndex >= 60000 - datasetSize)
                {
                    loopDataIndex = 0;
                }
            }
        }
        network.train(data);
    }

    // window->display();
}

void MainLoop::renderText()
{
    sf::Text text;
    sf::Font font;
    // if (!font.loadFromFile("font.ttf"))
    if (!font.loadFromFile("../assets/font.ttf"))
        std::cout << "Coud not load font!"
                  << "\n";

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    for (int i = 0; i < solutions.size(); i++)
    {
        text.setPosition(sf::Vector2f(20, 560 + i * 30));
        text.setString(std::to_string(i));
        window->draw(text);

        text.setPosition(sf::Vector2f(80, 560 + i * 30));
        text.setString(std::to_string((double)solutions[i]));
        window->draw(text);
    }

    text.setCharacterSize(100);
    text.setStyle(sf::Text::Bold);
    text.setPosition(sf::Vector2f(250, 560));
    text.setString(std::to_string(solution));
    window->draw(text);
}

//--render--// -> Called by main
void MainLoop::render()
{

    // std::cout << fps << "\n";
    if (!learn)
    {
        window->clear(sf::Color::White);

        renderText();

        network.renderFrame = true;
        network.render(this->window, sf::Vector2i(1100, 850), sf::Vector2i(600, 25));
        drawField.render(window, mousePos);
        drawNumber(downScaledImg, sf::Vector2f(560, 168));
        window->display();
    }
}