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

bool checkCollision(sf::FloatRect t_spriteOne, sf::FloatRect t_spriteTwo);
int checkBearBorderCollision(sf::Vector2f t_bearPosition, int t_bearSpeed);
sf::Vector2f followPlayer(sf::Vector2f t_playerPos, float t_slothSpeed, sf::Vector2f t_slothPosition);

//math
sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA);
float vectorLength(const sf::Vector2f t_vectorA);

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "First Graphics in C++");

	int gameScore = 0;

	sf::Sprite crocodileSprite;
	sf::Sprite penguinSprite;
	sf::Sprite bearSprite;
	sf::Sprite slothSprite;

	sf::Texture crocodileTexture;
	sf::Texture penguinTexture;
	sf::Texture bearTexture;
	sf::Texture slothTexture;

	//load text and fonts here
	sf::Text scoreText;
	sf::Font scoreFont;

	if (!scoreFont.loadFromFile("pngs/fonts/Milky Coffee.ttf"))
	{
		std::cout << "failed to load font";
	}
	scoreText.setFont(scoreFont);
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setOutlineColor(sf::Color::White);
	scoreText.setCharacterSize(40U);
	scoreText.setPosition(650, 530);
	scoreText.setString("Score: " + std::to_string(gameScore));

	//load and setup textures here
	if (!crocodileTexture.loadFromFile("pngs/pngs/crocodile.png"))
	{
		std::cout << "Error loading crocodile png image";
	}
	if (!penguinTexture.loadFromFile("pngs/pngs/penguin.png"))
	{
		std::cout << "Error loading crocodile png image";
	}
	if (!bearTexture.loadFromFile("pngs/pngs/bear.png"))
	{
		std::cout << "Error loading bear png image";
	}
	if (!slothTexture.loadFromFile("pngs/pngs/sloth.png"))
	{
		std::cout << "Error loading bear png image";
	}

	crocodileSprite.setTexture(crocodileTexture);
	sf::Vector2u textureSize = crocodileTexture.getSize();
	crocodileSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);

	penguinSprite.setTexture(penguinTexture);
	textureSize = penguinTexture.getSize();
	penguinSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	penguinSprite.setScale(0.5, 0.5);

	bearSprite.setTexture(bearTexture);
	textureSize = bearTexture.getSize();
	bearSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	bearSprite.setScale(0.5, 0.5);

	
	slothSprite.setTexture(slothTexture);
	textureSize = slothTexture.getSize();
	slothSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	slothSprite.setScale(0.5, 0.5);



	sf::RectangleShape simpleRectangle;
	float width = 20;
	float height = 50;

	simpleRectangle.setSize(sf::Vector2f(width, height));

	simpleRectangle.setFillColor(sf::Color::Red);

	simpleRectangle.setPosition(0, 0);
	srand(time(NULL));

	float xPosition = rand() % 800;
	float yPosition = rand() % 600;
	float speed = 10;
	bool crocodileHit = false;
	float crocodileHitTimer = 0;

	float penguiniXPosition = rand() % 800;
	float penguinYPosition = rand() % 600;
	penguinSprite.setPosition(penguiniXPosition, penguinYPosition);

	float randXBearPosition = rand() % 800 ;
	float yBearPosition = 50;
	bearSprite.setPosition(randXBearPosition, yBearPosition);
	int bearSpeed = 10;

	float slothXPosition = rand() % 800;
	float slothYPosition = rand() % 600;
	sf::Vector2f slothPosition = { slothXPosition, slothYPosition };
	slothSprite.setPosition(slothXPosition, slothYPosition);
	int slothSpeed = 1;


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

			window.clear();
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

			//Check collisions here
			if (checkCollision(crocodileSprite.getGlobalBounds(), penguinSprite.getGlobalBounds()))
			{
				float penguiniXPosition = rand() % 800;
				float penguinYPosition = rand() % 600;
				penguinSprite.setPosition(penguiniXPosition, penguinYPosition);
				gameScore++;
				scoreText.setString("Score: " + std::to_string(gameScore));
			}

			if (checkCollision(crocodileSprite.getGlobalBounds(), bearSprite.getGlobalBounds()))
			{
				if (bearSprite.getPosition().x > 400) {
					randXBearPosition = 50;
				}
				else if (bearSprite.getPosition().x < 400)
				{
					randXBearPosition = 749;
				}
				gameScore--;
				scoreText.setString("Score: " + std::to_string(gameScore));
			}

			if (checkCollision(slothSprite.getGlobalBounds(), crocodileSprite.getGlobalBounds()))
			{
				xPosition = rand() % 800;
				yPosition = rand() % 600;
				crocodileHit = true;
				crocodileHitTimer = 0;
				gameScore--;
				scoreText.setString("Score: " + std::to_string(gameScore));
			}

			//bear moving
			randXBearPosition += bearSpeed;
			bearSprite.setPosition(randXBearPosition, bearSprite.getPosition().y);
			bearSpeed = checkBearBorderCollision(bearSprite.getPosition(), bearSpeed);

			//sloth moving
			slothPosition = slothSprite.getPosition();
			sf::Vector2f slothVelocity =  followPlayer(crocodileSprite.getPosition(),slothSpeed ,slothPosition );
			slothSprite.setPosition(slothPosition + slothVelocity);
			

			crocodileSprite.setPosition(xPosition, yPosition);
			
			if (crocodileHit)
			{
				crocodileHitTimer += 0.032;
				crocodileSprite.setColor(sf::Color::Red);
			}

			if(crocodileHitTimer > 2)
			{
				crocodileSprite.setColor(sf::Color::White);
				crocodileHit = false;
			}

		std::cout << "Timer" << crocodileHitTimer;

			window.draw(crocodileSprite);
			window.draw(penguinSprite);
			window.draw(scoreText);
			window.draw(bearSprite);
			window.draw(slothSprite);


			window.display();


			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}


bool checkCollision(sf::FloatRect t_spriteOne, sf::FloatRect t_spriteTwo)
{
	if (t_spriteOne.intersects(t_spriteTwo)){
		return true;
	}
	return false;
}

int checkBearBorderCollision(sf::Vector2f t_bearPosition, int t_bearSpeed)
{
	if (t_bearPosition.x > 770)
	{
		t_bearSpeed = -10;
	}
	if (t_bearPosition.x < 0)
	{
		t_bearSpeed = 10;
	}
	return t_bearSpeed;
}

sf::Vector2f followPlayer(sf::Vector2f t_playerPos, float t_slothSpeed, sf::Vector2f t_slothPosition)
{
	sf::Vector2f veclocity = {0.0f,0.0f};
	sf::Vector2f directionVector = t_playerPos - t_slothPosition;
	sf::Vector2f directionNormal = vectorUnitVector(directionVector);
	veclocity = directionNormal * t_slothSpeed;

	return veclocity;
}

sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA)
{
	sf::Vector2f answer;
	answer.x = t_vectorA.x / vectorLength(t_vectorA);
	answer.y = t_vectorA.y / vectorLength(t_vectorA);
	return answer;
}

float vectorLength(const sf::Vector2f t_vectorA)
{
	float sumOfSquares = (t_vectorA.x * t_vectorA.x) + (t_vectorA.y * t_vectorA.y);
	const float length = std::sqrt(sumOfSquares);
	return length;
}
