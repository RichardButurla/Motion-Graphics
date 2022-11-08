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


sf::Color lightBrown{ 244, 164, 96 };

bool checkCollision(sf::FloatRect t_floatRectOne, sf::FloatRect t_floatRectTwo);

enum class GameStates
{
	None,
	Game,
	Win,
	Lose
};

class FloatingLife
{
public:
	FloatingLife();
	~FloatingLife();

	void update();
	void move();
	void draw(sf::RenderWindow& t_window);

	void sendToTop() { m_position.y = 0 - 50; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void setPosition(int t_x, int t_y) { m_position.x = t_x; m_position.y = m_position.y; }
	void setCollected(bool t_bool) { collected = t_bool; }
	bool getCollected() { return collected; }

	sf::FloatRect getLifeBounds() { return lifeShape.getGlobalBounds(); }

	void checkOutofBounds(sf::RenderWindow& t_window);
	void setTexture(sf::Texture& t_texture) { lifeTex = t_texture; lifeShape.setTexture(lifeTex); }

private:
	sf::Sprite lifeShape;
	sf::Texture lifeTex;
	sf::Vector2f m_position;
	float verticalSpeed = 10;
	bool collected = true;
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	void update();
	void move();
	void draw(sf::RenderWindow& t_window);
	sf::Vector2f getPosition() { return m_position; }
	void checkOutofBounds(sf::RenderWindow& t_window);

	void sendToTop() { m_position.y = 0 - 50; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void setPosition(int t_x, int t_y) { m_position.x = t_x; m_position.y = m_position.y; }
	void setAlive() { alive = true; }
	void setDead() { alive = false; }
	void setTexture(sf::Texture& t_texture) { enemyTex = t_texture; enemyShape.setTexture(enemyTex); }
	bool getAlive() const { return alive; }
	sf::FloatRect getEnemyBounds() { return enemyShape.getGlobalBounds(); }

private:

	sf::Sprite enemyShape;
	sf::Texture enemyTex;
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
	sf::Sprite getBulletShape() { return bulletSprite; }
	sf::Vector2f getPosition() { return m_position; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void fire(sf::Vector2f t_playerPos) { fired = true; m_position = t_playerPos; }
	bool getFired() { return fired; }
	void setTexture(sf::Texture& t_bulletTexture) { bulletTexture = t_bulletTexture; bulletSprite.setTexture(bulletTexture);}
	void setFired(bool t_bool) { fired = t_bool; }
	void setXVelocity(int t_horisontalSpeed) { horisontalSpeed = t_horisontalSpeed; }

private:

	sf::Sprite bulletSprite;
	sf::Texture bulletTexture;
	sf::Vector2f m_position{ 300, 0 };
	float verticalSpeed = 10;
	float horisontalSpeed = 0;
	bool fired = false;


};

int main()
{
std:srand(static_cast<unsigned int>(time(nullptr))); //seed for randomisation

	GameStates currentState = GameStates::Game; //will control wether we are in game, wether we lost/won
	sf::RenderWindow window(sf::VideoMode(1200, 800), "First Graphics in C++");
	
	sf::Font scoreFont;
	sf::Text scoreText;
	sf::Text messageText;
	sf::Text livesText;

	sf::Texture goatTex;
	sf::Texture maceTex;
	sf::Texture lifeTex;
	sf::Texture bulletTex;

	//Load textures
	if (!scoreFont.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "failed to load font";
	}
	if (!goatTex.loadFromFile("ASSETS/IMAGES/goat.png"))
	{
		std::cout << "failed to load goat";
	}
	if (!maceTex.loadFromFile("ASSETS/IMAGES/Mace.png"))
	{
		std::cout << "failed to load Mace";
	}
	if (!lifeTex.loadFromFile("ASSETS/IMAGES/heart.png"))
	{
		std::cout << "failed to load Mace";
	}
	if (!bulletTex.loadFromFile("ASSETS/IMAGES/projectile.png"))
	{
		std::cout << "failed to load projectile";
	}

	//Bullets
	static const int MAX_BULLETS = 1000;
	Bullet bulletArray[MAX_BULLETS];
	int numberOfBulletsFired = 0;
	sf::Vector2f bulletFiringOffset = { 40,0 };
	int horisontalBulletSpeed = 0;

	//Floating Lives
	static const int MAX_FLOATING_LIVES = 10;
	FloatingLife floatingLives[MAX_FLOATING_LIVES];
	float timeSinceLastLifeSpawned = 0;
	int numberOfLivesSpawned = 0;

	//Enemies
	static const int MAX_ENEMIES = 100;
	int numberOfEnemiesAlive = 0;
	Enemy enemyArray[MAX_ENEMIES];


	//Player
	sf::Sprite playerSprite;
	int playerLives = 3;
	sf::Vector2u textureSize = goatTex.getSize();
	float width = 50;
	float height = 50;
	float playerXPos = window.getSize().x / 2;
	float playerYPos = window.getSize().y - textureSize.y / 2;
	int playerSpeed = 5;

	playerSprite.setTexture(goatTex);
	playerSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	playerSprite.setScale(0.5, 0.5);
	playerSprite.setPosition(playerXPos, playerYPos);


	//Set textrues for all Sprites
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemyArray[i].setTexture(maceTex);
	}
	for (int i = 0; i < MAX_FLOATING_LIVES; i++)
	{
		floatingLives[i].setTexture(lifeTex);
	}
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bulletArray[i].setTexture(bulletTex);
	}
	

	//Set texts
	scoreText.setFont(scoreFont);
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setOutlineColor(sf::Color::White);
	scoreText.setCharacterSize(20U);
	scoreText.setPosition(500, 20);

	messageText.setFont(scoreFont);
	messageText.setFillColor(sf::Color::Red);
	messageText.setOutlineColor(sf::Color::White);
	messageText.setCharacterSize(20U);
	messageText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2);

	livesText.setFont(scoreFont);
	livesText.setFillColor(sf::Color::Red);
	livesText.setOutlineColor(sf::Color::White);
	livesText.setCharacterSize(20U);
	livesText.setPosition(10 , 20);
	livesText.setString(std::to_string(playerLives));

	//Game dependant variables
	bool triggerStop = 0;
	float timeSinceLastBulletFired = 0;
	float timeSinceLastEnemySpawned = 0;
	int enemiesShot = 0;

	//GameLoop Clock/Time variables
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
			if (currentState == GameStates::Game) //if we are in the game
			{
				sf::Vector2f lastPlayerPos = { playerXPos,playerYPos };
				horisontalBulletSpeed = 0;

				//check all keypresses

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					playerYPos -= playerSpeed;

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					playerYPos += playerSpeed;

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					playerXPos -= playerSpeed;


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					playerXPos += playerSpeed;


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					horisontalBulletSpeed = -8;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					horisontalBulletSpeed = 8;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					if (!triggerStop) //if off firing cooldown
					{
						numberOfBulletsFired++;
						bulletArray[numberOfBulletsFired].setXVelocity(horisontalBulletSpeed); //either sets the X Velocity to 0, or any minus or plus digit 
						bulletArray[numberOfBulletsFired].fire(playerSprite.getPosition() + bulletFiringOffset); //fires with left offest

						numberOfBulletsFired++;
						bulletArray[numberOfBulletsFired].setXVelocity(horisontalBulletSpeed); //either sets the X Velocity to 0, or any minus or plus digit
						bulletArray[numberOfBulletsFired].fire(playerSprite.getPosition() + -bulletFiringOffset); //fires with right offset
						triggerStop = true;
					}

				}
				//update Player and texts
				playerSprite.setPosition(playerXPos, playerYPos);
				scoreText.setString("Score: " + std::to_string(enemiesShot));
				livesText.setString("Lives Left: " + std::to_string(playerLives));

				window.clear(lightBrown);
				
				timeSinceLastBulletFired += 0.032; //every frame
				if (timeSinceLastBulletFired > 0.5)
				{
					triggerStop = false;
					timeSinceLastBulletFired = 0;
				}
				timeSinceLastEnemySpawned += 0.032;
				if (timeSinceLastEnemySpawned > 0.3) //spawns enemies
				{
					if (numberOfEnemiesAlive < MAX_ENEMIES)
						numberOfEnemiesAlive += 1;

					enemyArray[numberOfEnemiesAlive].setAlive();
					//Randomise x pos a bit
					int randXPos = rand() % (window.getSize().x - 50) + 50;
					if (numberOfEnemiesAlive != MAX_ENEMIES ) //if we havent reached our last enemy, spawn at the top
					{
						enemyArray[numberOfEnemiesAlive].setPosition(window.getSize().x / 2 - 100 + randXPos + randXPos, 0);
					}
					
					timeSinceLastEnemySpawned = 0;
					
				}

				timeSinceLastLifeSpawned += 0.032;
				if (timeSinceLastLifeSpawned > 10)
				{
					floatingLives[numberOfLivesSpawned].setCollected(false);
					if (numberOfLivesSpawned < MAX_FLOATING_LIVES)
						numberOfLivesSpawned++;

					timeSinceLastLifeSpawned = 0;
				}

				

				//CheckCollisions
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					//check player collision with enemy
					if (checkCollision(playerSprite.getGlobalBounds(), enemyArray[i].getEnemyBounds()) == true)
					{
						enemyArray[i].sendToTop();
						playerLives--;
					}
					//check bullet collision with enemy
					for (int j = 0; j < MAX_BULLETS; j++)
					{
						if (bulletArray[j].getFired() == true && enemyArray[i].getAlive() == true)
						{
							if (checkCollision(bulletArray[j].getBulletShape().getGlobalBounds(), enemyArray[i].getEnemyBounds()))
							{
								enemyArray[i].setDead();
								enemiesShot++;
								bulletArray[j].setFired(false);
							}
						}

					}

				}
				//check player collision with lives
				for (int i = 0; i < MAX_FLOATING_LIVES; i++)
				{
					if (floatingLives[i].getCollected() == false)
					{
						if (checkCollision(playerSprite.getGlobalBounds(), floatingLives[i].getLifeBounds())) {
							floatingLives[i].setCollected(true);
							playerLives++;
						}
					}				
				}
				
				window.draw(playerSprite);
				window.draw(scoreText);
				window.draw(livesText);

				//Update and draw
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
				for (int i = 0; i < MAX_FLOATING_LIVES; i++)
				{
					floatingLives[i].update();
					floatingLives[i].draw(window);
				}
			}


			//Check Game ending scenarios
			if (enemiesShot == MAX_ENEMIES - 1)
			{
				currentState = GameStates::Win;
			}
			if (playerLives <= 0)
			{
				currentState = GameStates::Lose;
			}
			

			if (currentState == GameStates::Win)
			{
				window.clear(lightBrown);
				messageText.setString("YOU WIN!");
				window.draw(messageText);
			}
			if (currentState == GameStates::Lose)
			{
				window.clear(lightBrown);
				messageText.setString("YOU Lose!");
				window.draw(messageText);
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
	bulletSprite.setOrigin(bulletSize.x / 2.0f, bulletSize.y / 2.0f);
	bulletSprite.setPosition(m_position);
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
	if (bulletSprite.getPosition().x < 0)
	{
		fired = false;
	}

}

void Bullet::move()
{
	m_position.y -= verticalSpeed;
	m_position.x += horisontalSpeed; //usually zero unless we move sideways and fire
	bulletSprite.setPosition(m_position);
}

void Bullet::draw(sf::RenderWindow& t_window)
{
	if (fired) {
		t_window.draw(bulletSprite);
	}

}

Enemy::Enemy()
{
	sf::Vector2f enemySize = { 50,50 };
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
	checkOutofBounds(t_window);

	if (alive)
		t_window.draw(enemyShape);
}

void Enemy::checkOutofBounds(sf::RenderWindow& t_window)
{
	if (this->m_position.y > t_window.getSize().y)
	{
		m_position.y = 0 - 100; //sets it to the top and up more based on its height.
		int endXCoord = (t_window.getSize().x - 50);
		m_position.x = rand() % endXCoord + 50; //random x coord
	}
}

bool checkCollision(sf::FloatRect t_floatRectOne, sf::FloatRect t_floatRectTwo)
{
	if (t_floatRectOne.intersects(t_floatRectTwo))
	{
		return true;
	}
	return false;
}

FloatingLife::FloatingLife()
{
	sf::Vector2f life = { 50.f,50.f };
	lifeShape.setScale(0.1, 0.1);
	lifeShape.setOrigin(life.x / 2.0f, life.y / 2.0f);
	lifeShape.setPosition(m_position);
}

FloatingLife::~FloatingLife()
{
}

void FloatingLife::update()
{
	if (!collected)
	{
		move();
	}
}

void FloatingLife::move()
{
	m_position.y += verticalSpeed;
	lifeShape.setPosition(m_position);
}

void FloatingLife::draw(sf::RenderWindow& t_window)
{
	checkOutofBounds(t_window);

	if(!collected)
	t_window.draw(lifeShape);
}

void FloatingLife::checkOutofBounds(sf::RenderWindow& t_window)
{
	if (this->m_position.y > t_window.getSize().y)
	{
		m_position.y = 0 - 100; //sets it to the top and up more based on its height.
		int endXCoord = (t_window.getSize().x - 50);
		m_position.x = rand() % endXCoord + 50; //random x coord
	}
}
