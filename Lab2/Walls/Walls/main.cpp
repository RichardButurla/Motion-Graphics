#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <fstream>

void checkCollision(int t_levelDataArray[], float& t_Xpos, float& t_yPos, sf::RectangleShape& t_player);

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "First Graphics in C++");

	sf::RectangleShape simpleRectangle;
	float width = 20;
	float height = 20;

	simpleRectangle.setSize(sf::Vector2f(width, height));

	simpleRectangle.setFillColor(sf::Color::Red);

	simpleRectangle.setPosition(0, 0);
	simpleRectangle.setOrigin(width / 2, height / 2);
	srand(time(NULL));

	float xPosition = window.getSize().x / 2;
	float yPosition = window.getSize().y - simpleRectangle.getSize().y;
	simpleRectangle.setPosition(xPosition, yPosition);
	float speed = 5;

	float scrollLevelTimer = 0;
	float scrollLevelThreshold = 1;
	float yOffset = 0;

	int levelData[]{ 4,5,5,2,6,1,5,2, 6, 4, 5, 3, 5, 4, 7, 5, 6, 4, 4, 3, 3, 6, 5 ,5,2,6,3,5 ,3 ,5,5,3,6,3,7,2,6,3,5,3,4,2,5,2,5,2,5,2,6,3,6,2,7,2,7,3 };

	sf::RectangleShape wallTile;
	wallTile.setSize(sf::Vector2f(50, 50));
	wallTile.setFillColor(sf::Color::Magenta);
	float wallStartYPos = window.getSize().y - wallTile.getSize().y;

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;


	sf::Clock clockForFrameRate;

	clockForFrameRate.restart();

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		timeSinceLastUpdate += clockForFrameRate.restart();


		if (timeSinceLastUpdate > timePerFrame)
		{


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				yPosition -= speed;

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				yPosition += speed;

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				xPosition -= speed;


			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				xPosition += speed;


			}

			sf::Text scoreText;
			sf::Font scoreFont;
			int gameScore = 0;

			/*if (!scoreFont.loadFromFile("ASSETS/fonts/Milky Coffee.ttf"))
			{
				std::cout << "failed to load font";
			}*/
			scoreText.setFont(scoreFont);
			scoreText.setFillColor(sf::Color::Red);
			scoreText.setOutlineColor(sf::Color::White);
			scoreText.setCharacterSize(20U);
			scoreText.setPosition(500, 20);
			scoreText.setString("Score: " + std::to_string(gameScore));


			window.clear();
			int levelArraySize = 32;
			int index = 0;

			wallStartYPos = window.getSize().y - wallTile.getSize().y;
			int wallStartXPos = 0;
			int startYpos = window.getSize().y - 50;
			const int MAX_COLLUMNS = 12;

			int wallBumpBack = 5;
			int numColumns = 0;

			while (index < 56)
			{
				//yOffset += 0.0005;
				scrollLevelTimer = 0;


				wallStartXPos = 0;
				numColumns = levelData[index];
				wallTile.setFillColor(sf::Color::Magenta);
				wallTile.setScale(numColumns, 1);
				wallTile.setPosition(wallStartXPos, wallStartYPos + (yOffset * wallTile.getSize().y));
				window.draw(wallTile);

				numColumns = levelData[index + 1];
				wallTile.setFillColor(sf::Color::Cyan);
				wallTile.setScale(numColumns, 1);
				wallStartXPos = (levelData[index] * wallTile.getSize().x + 50);
				wallTile.setPosition(wallStartXPos - 50, wallStartYPos + (yOffset * wallTile.getSize().y));
				window.draw(wallTile);

				numColumns = MAX_COLLUMNS - (levelData[index] + levelData[index + 1]);
				wallTile.setFillColor(sf::Color::Magenta);
				wallTile.setScale(numColumns, 1);
				wallStartXPos = levelData[index] * wallTile.getSize().x + levelData[index + 1] * wallTile.getSize().x;
				wallTile.setPosition(wallStartXPos, wallStartYPos + (yOffset * wallTile.getSize().y));
				window.draw(wallTile);
				index += 2;

				wallStartYPos -= wallTile.getSize().y;

			}
			checkCollision(levelData, xPosition, yPosition, simpleRectangle);
			simpleRectangle.setPosition(xPosition, yPosition);


			int xSize = wallTile.getSize().x;

			std::cout << "X Coord: " << simpleRectangle.getPosition().x << "\n";

			window.draw(simpleRectangle);
			//window.draw(scoreText);


			window.display();


			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}

void checkCollision(int t_levelDataArray[], float& t_Xpos, float& t_yPos, sf::RectangleShape& t_player)
{
	int maxWidth = 600;
	int maxHeight = 600;
	sf::Vector2f playerPos = t_player.getPosition();

	for (int i = 0; i < 2; i += 2) //56 is size of array
	{
		 
		if (playerPos.x > t_levelDataArray[i] * 50) {
			
			
		}
		
	}
}
