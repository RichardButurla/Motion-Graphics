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
#include <vector>
#include <time.h>
#include <random>


//Color Pallete
sf::Color rustyRed{ 222, 60, 75 };
sf::Color electricBlue{ 135, 245, 251 };
sf::Color paleSilver{ (206, 195, 193) };
sf::Color darkPurple{ (36, 1, 21,255 ) };

bool checkEnemyPlayerCollision(sf::FloatRect t_player, sf::FloatRect t_enemyRect);
bool checkEnemyBulletCollision(sf::FloatRect bullet, sf::FloatRect t_enemyRect);

enum class GameStates
{
	None,
	Game,
	Win,
	Lose
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	void update();
	void move();
	void draw(sf::RenderWindow& t_window);
	sf::RectangleShape getEenmyShape() { return enemyShape; }
	sf::Vector2f getPosition() { return m_position; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void setPosition(int t_x, int t_y) { m_position.x = t_x; m_position.y = m_position.y; }
	void setAlive() { alive = true; }
	void setDead() { alive = false; }
	bool getAlive() const { return alive; }

private:

	sf::RectangleShape enemyShape;
	sf::Vector2f m_position;
	float speed = 10;
	bool alive = false;
};

class Bullet
{
public:
	 Bullet();
	~Bullet();

	void update();
	void move();
	void draw(sf::RenderWindow& t_window);
	sf::RectangleShape getBulletShape() { return bulletShape; }
	sf::Vector2f getPosition() { return m_position; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void fire(sf::Vector2f t_playerPos) { fired = true; m_position = t_playerPos; }
	bool getFired() { return fired; }
	void setFired(bool t_bool) { fired = t_bool; }

private:

	sf::RectangleShape bulletShape;
	sf::Vector2f m_position {300, 0};
	float speed = 10;
	bool fired = false;
	

};

int main()
{
	std:srand(static_cast<unsigned int>(time(nullptr)));

	GameStates currentState = GameStates::Game;
	sf::RenderWindow window(sf::VideoMode(600, 600), "First Graphics in C++");

	sf::Sprite winScreen;
	sf::Sprite loseScreen;

	sf::Texture winTexture;
	sf::Texture loseTexture;

	if (!winTexture.loadFromFile("ASSETS/IMAGES/winScreen.jpg"))
		{
			std::cout << "failed to load win screen";
		}
	if (!loseTexture.loadFromFile("ASSETS/IMAGES/loseScreen.jpg"))
	{
		std::cout << "failed to load lose screen";
	}

	winScreen.setTexture(winTexture);
	loseScreen.setTexture(loseTexture);


	//Bullets
	static const int MAX_BULLETS = 100;
	Bullet bulletArray[MAX_BULLETS];
	int numberOfBulletsFired = 0;

	//Eenemies
	static const int MAX_ENEMIES = 10;
	Enemy enemyArray[MAX_ENEMIES];
	int numberOfEnemiesAlive = 0;
	float spawnTimer = 0;

	sf::Text restartText;
	sf::Font arialFont;

	if (!arialFont.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "failed to load font";
	}
	restartText.setFont(arialFont);
	restartText.setString("PRESS R TO RESTART");
	restartText.setFillColor(sf::Color::White);
	restartText.setOutlineColor(sf::Color::Black);
	restartText.setCharacterSize(40u);
	restartText.setPosition(window.getSize().x / 10, 0 + 50);

	winScreen.setScale(0.55, 0.4);
	loseScreen.setScale(0.8, 0.8);

	sf::RectangleShape simpleRectangle;
	float width = 20;
	float height = 20;

	simpleRectangle.setSize(sf::Vector2f(width, height));

	simpleRectangle.setFillColor(rustyRed);

	simpleRectangle.setPosition(0, 0);
	simpleRectangle.setOrigin(width / 2, height / 2);
	srand(time(NULL));

	bool triggerStop = 0;
	float timer = 0;

	float xPosition = window.getSize().x / 2;
	float yPosition = window.getSize().y - simpleRectangle.getSize().y;
	simpleRectangle.setPosition(xPosition, yPosition);
	float speed = 5;

	float scrollLevelTimer = 0;
	float scrollLevelThreshold = 1;
	float yOffset = 0;

	sf::Vector2f endPoint = { 600, 0 };

	int levelData[]{ 5,3,5,2,5,1,5,2,6,2, 5, 2, 5, 1, 5, 1, 5, 1, 5, 2, 5, 1, 5 ,2,6,2,6,2 ,5 ,3,5,3,6,3,7,2,6,3,5,3,4,2,5,2,5,2,5,2,6,3,6,2,7,2,7,3 };

	sf::RectangleShape wallTile;
	wallTile.setSize(sf::Vector2f(50, 50));
	wallTile.setFillColor(paleSilver);
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
			if (currentState == GameStates::Game)
			{
				sf::Vector2f lastPlayerPos = { xPosition,yPosition };

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					if (!triggerStop)
					{
						numberOfBulletsFired++;
						bulletArray[numberOfBulletsFired].fire(simpleRectangle.getPosition());
						triggerStop = true;
					}
					
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

				timer += 0.032; //every frame
				if (timer > 0.3)
				{
					triggerStop = false;
					timer = 0;
				}
				spawnTimer += 0.032;
				if (spawnTimer > 3)
				{
					enemyArray[numberOfEnemiesAlive].setAlive();
					//Randomise x pos a bit
					int randXPos = rand() % 50 + 25;
					enemyArray[numberOfEnemiesAlive].setPosition(window.getSize().x / 2 - 100 + randXPos + randXPos, 0);
					spawnTimer = 0;
					numberOfEnemiesAlive++;
				}


				while (index < 56)
				{
					if (yOffset < 16)
					{
						yOffset += 0.00115;
					}


					wallStartXPos = 0;
					numColumns = levelData[index];
					wallTile.setFillColor(rustyRed);
					wallTile.setScale(numColumns, 1);
					wallTile.setPosition(wallStartXPos, wallStartYPos + (yOffset * wallTile.getSize().y));
					window.draw(wallTile);

					if (simpleRectangle.getGlobalBounds().intersects(wallTile.getGlobalBounds()))
					{
						currentState = GameStates::Lose;
					}

					numColumns = levelData[index + 1];
					wallTile.setFillColor(electricBlue);
					wallTile.setScale(numColumns, 1);
					wallStartXPos = (levelData[index] * wallTile.getSize().x + 50);
					wallTile.setPosition(wallStartXPos - 50, wallStartYPos + (yOffset * wallTile.getSize().y));
					window.draw(wallTile);

					numColumns = MAX_COLLUMNS - (levelData[index] + levelData[index + 1]);
					wallTile.setFillColor(rustyRed);
					wallTile.setScale(numColumns, 1);
					wallStartXPos = levelData[index] * wallTile.getSize().x + levelData[index + 1] * wallTile.getSize().x;
					wallTile.setPosition(wallStartXPos, wallStartYPos + (yOffset * wallTile.getSize().y));
					window.draw(wallTile);
					if (simpleRectangle.getGlobalBounds().intersects(wallTile.getGlobalBounds()))
					{
						currentState = GameStates::Lose;
					}
					index += 2;

					wallStartYPos -= wallTile.getSize().y;

				}

				if (yPosition <= endPoint.y)
				{
					currentState = GameStates::Win;
				}
				if (yPosition > window.getSize().y)
				{
					currentState = GameStates::Lose;
				}

				simpleRectangle.setPosition(xPosition, yPosition);

				
				int xSize = wallTile.getSize().x;

				//CheckCollisions
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					for (int j = 0; j < MAX_BULLETS; j++)
					{
						if (bulletArray[j].getFired() == true && enemyArray[i].getAlive() == true)
						{
							if (checkEnemyBulletCollision(bulletArray[j].getBulletShape().getGlobalBounds(), enemyArray[i].getEenmyShape().getGlobalBounds()))
							{
								enemyArray[i].setDead();
								bulletArray[j].setFired(false);
							}
						}
						
					}
					
				}

				
				window.draw(simpleRectangle);

				for (int i = 0; i < MAX_BULLETS; i++)
				{
					bulletArray[i].update();
					bulletArray[i].draw(window);

				}
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					enemyArray[i].update();
					enemyArray[i].draw(window);
				}
			}
			if (currentState == GameStates::Lose)
			{
				window.draw(loseScreen);
				window.draw(restartText);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					xPosition = window.getSize().x / 2;
					yPosition = window.getSize().y - simpleRectangle.getSize().y;
					simpleRectangle.setPosition(xPosition, yPosition);
					yOffset = 0;
					currentState = GameStates::Game;
				}
			}
			if (currentState == GameStates::Win)
			{
				window.draw(winScreen);
				window.draw(restartText);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					xPosition = window.getSize().x / 2;
					yPosition = window.getSize().y - simpleRectangle.getSize().y;
					simpleRectangle.setPosition(xPosition, yPosition);
					yOffset = 0;
					currentState = GameStates::Game;
				}
			}
			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}

Bullet::Bullet()
{
	sf::Vector2f bulletSize = { 10,10 };
	bulletShape.setFillColor(paleSilver);
	bulletShape.setSize(bulletSize);
	bulletShape.setOrigin(bulletSize.x / 2.0f, bulletSize.y / 2.0f);
	bulletShape.setPosition(m_position);
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	if (fired)
	{
		move();
	}
	if (bulletShape.getPosition().y < 0)
	{
		fired = false;
	}
	
}

void Bullet::move()
{
	m_position.y -= speed;
	bulletShape.setPosition(m_position);
}

void Bullet::draw(sf::RenderWindow& t_window)
{
	if (fired) {
		t_window.draw(bulletShape);
	}
	
}

Enemy::Enemy()
{
	sf::Vector2f enemySize = { 50,50 };
	enemyShape.setFillColor(paleSilver);
	enemyShape.setSize(enemySize);
	enemyShape.setOrigin(enemySize.x / 2.0f, enemySize.y / 2.0f);
	enemyShape.setPosition(m_position);
}

Enemy::~Enemy()
{
}

void Enemy::update()
{
	if (alive)
	{
		move();	
	}
	
}

void Enemy::move()
{
	m_position.y += speed;
	enemyShape.setPosition(m_position);
}

void Enemy::draw(sf::RenderWindow& t_window)
{
	if(alive)
	t_window.draw(enemyShape);
}

bool checkEnemyPlayerCollision(sf::FloatRect t_player, sf::FloatRect t_enemyRect)
{
	if (t_player.intersects(t_enemyRect))
	{
		return true;
	}
	return false;
}

bool checkEnemyBulletCollision(sf::FloatRect bullet, sf::FloatRect t_enemyRect)
{
	if (bullet.intersects(t_enemyRect))
	{
		return true;
	}
	return false;
}

