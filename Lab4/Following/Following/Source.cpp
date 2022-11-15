//Richard Buturla
//C00272345

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
#include <vector>
#include "VectorAlgebra2D.h"


const float PI = 3.14159265359f;


class EnemyEntity
{
public:
	EnemyEntity();
	void update();
	void render(sf::RenderWindow& t_window);
	void init(sf::Texture& t_zombieTexture);

	void chasePlayer(sf::Vector2f t_playerLocation);
	void friendlyCollision(sf::Vector2f t_intersectPos);

	sf::Sprite getBody() { return m_body; }
	sf::Vector2f getPosition() { return m_location; }
	void setPosition(sf::Vector2f t_position) { m_location = t_position; }
	void setPosition(float xPos, float yPos) { m_location.x = xPos; m_location.y = yPos; }
	bool getAlive() { return m_alive; }
	void setAlive(bool t_alive) { m_alive = t_alive; }
	sf::Vector2f getVelocity() { return m_velocity; }
	void setSpawnPos(sf::Vector2f t_spawnlocation) { m_spawnLocation = t_spawnlocation; }
	int getDamage() { return m_damage; }

protected:
	sf::Sprite m_body;
	sf::Texture m_zombieTexture;
	sf::Vector2f m_sizeOfBody{ 32.0f,32.0f };
	sf::Vector2f m_location;
	sf::Vector2f m_spawnLocation;
	sf::Vector2f directionVector;
	sf::Vector2f m_velocity;
	float m_speed = 5.0f;
	int m_damage = 1;
	bool m_alive{ true };
};


class Bullet
{
public:
	Bullet();
	~Bullet();

	void update();
	void draw(sf::RenderWindow& t_window);
	sf::Sprite getBulletShape() { return bulletSprite; }
	sf::Vector2f getPosition() { return m_position; }
	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }
	void fire(sf::Vector2f t_playerPos) { fired = true; m_position = t_playerPos; }
	bool getFired() { return fired; }
	void init(sf::Texture& t_bulletTexture);
	void setFired(bool t_bool) { fired = t_bool; }
	float getSpeed() { return m_speed; }
	void checkBounds();

	void setVelocity(sf::Vector2f t_velocity) { m_velocity = t_velocity; }
	void rotateBulletSprite(sf::Vector2f t_mousePos, sf::Vector2f t_playerPos);

private:

	sf::Sprite bulletSprite;
	sf::Texture bulletTexture;
	sf::Vector2f m_position{ 300, 0 };
	sf::Vector2f m_velocity{ 1,-1 };
	float m_speed = 10;
	bool fired = false;


};

class Player
{
public:
	Player();
	Player(sf::Texture &t_playerTexture);
	~Player();

	void update();
	void render(sf::RenderWindow &t_window);

	void rotatePlayer(sf::Vector2f t_mousePos);

	void addHorisontalSpeed(float speed) { m_location.x += speed; }
	void addVerticalSpeed(float speed) { m_location.y += speed; }

	sf::Vector2f getPos() { return m_location; }


private:
	sf::Sprite m_playerSprite;
	sf::Texture m_playerTexture;

	sf::Vector2f m_velocity = {0.f,0.f};
	sf::Vector2f m_location;


};

sf::Vector2f screenSize = { 800,600 };

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "First Graphics in C++");
	
	std::srand(static_cast<unsigned int>(time(nullptr)));

	int playerSpeed = 10;

	//Load Textures
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture bulletTexture;

	if (!playerTexture.loadFromFile("ASSETS/IMAGES/goat.png"))
	{
		std::cout << "player texture failed to load";
	}
	if (!enemyTexture.loadFromFile("ASSETS/IMAGES/Mace.png"))
	{
		std::cout << "player texture failed to load";
	}
	if (!bulletTexture.loadFromFile("ASSETS/IMAGES/projectile.png"))
	{
		std::cout << "player texture failed to load";
	}





	//Player
	Player player(playerTexture);
	sf::Vector2i mousePos;

	//Enemies

	static const int MAX_ENEMIES = 3;
	EnemyEntity enemyArray[MAX_ENEMIES];
	
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemyArray[i].init(enemyTexture);
		enemyArray[i].setAlive(true);
	}

	//Bullets
	static const int MAX_BULLETS = 10;
	Bullet bulletArray[MAX_BULLETS];
	int numberOfBulletsFired = 0;
	bool triggerStop = false;
	float timeSinceLastBulletFired = 0;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bulletArray[i].init(bulletTexture);
	}


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
			 mousePos = sf::Mouse::getPosition(window);
			window.clear();
			if (sf::Event::MouseMoved == event.type)
			{ //always gets the mouse position when mouse is moved
				player.rotatePlayer(static_cast<sf::Vector2f>(mousePos));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.addVerticalSpeed(-playerSpeed);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player.addVerticalSpeed(playerSpeed);

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player.addHorisontalSpeed(-playerSpeed);


			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player.addHorisontalSpeed(playerSpeed);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!triggerStop) //if off firing cooldown
				{
					for (int i = 0; i < MAX_BULLETS; i++)
					{
						if (bulletArray[i].getFired() == false) {
							sf::Vector2f bulletVelocity = thor::unitVector(static_cast<sf::Vector2f>(mousePos) - player.getPos()) * bulletArray[i].getSpeed(); //set bullet in that direction
							bulletArray[i].setVelocity(bulletVelocity); //we give it unit vector because in bullet class it multiplies by speed
							bulletArray[i].fire(player.getPos());
							triggerStop = true;
							break;

						}
					}				
				}

			}

			//update stuff here
			timeSinceLastBulletFired += 0.032; //every frame
			if (timeSinceLastBulletFired > 0.5)
			{
				triggerStop = false;
				timeSinceLastBulletFired = 0;
			}

			player.update();

			for (int i = 0; i < MAX_ENEMIES; i++)
			{
				enemyArray[i].update();
			}
			for (int i = 0; i < MAX_BULLETS; i++)
			{
				bulletArray[i].update();
			}

			//draw player here
			player.render(window);

			for (int i = 0; i < MAX_ENEMIES; i++)
			{
				enemyArray[i].chasePlayer(player.getPos());
				enemyArray[i].render(window);
			}
			for (int i = 0; i < MAX_BULLETS; i++)
			{
				bulletArray[i].draw(window);
			}

			window.display();
			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}

Player::Player()
{
}

Player::Player(sf::Texture& t_playerTexture)
{
	m_playerTexture = t_playerTexture;
	m_playerSprite.setTexture(t_playerTexture);
	m_playerSprite.setScale(0.5, 0.5);
	sf::Vector2u textureSize = m_playerTexture.getSize();
	m_playerSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

Player::~Player()
{
}

void Player::update()
{
	m_playerSprite.setPosition(m_location);
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_playerSprite);
}

void Player::rotatePlayer(sf::Vector2f t_mousePos)
{
	float rotation = 0.0f;
	sf::Vector2f shapeLocation = m_playerSprite.getPosition();
	sf::Vector2f rotationVector;
	rotationVector.x = t_mousePos.x - shapeLocation.x;
	rotationVector.y = t_mousePos.y - shapeLocation.y;

	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;
	rotation = -rotation; //sfml window is upside down so negative rotation

	m_playerSprite.setRotation(rotation);
}

EnemyEntity::EnemyEntity()
{

	int randomSpawn = rand() % 4 + 1;
	switch (randomSpawn)
	{
	case 1:
		m_spawnLocation = { 50.0f,50.0f };
		break;
	case 2:
		m_spawnLocation = { 800, 20.0f };
		break;
	case 3:
		m_spawnLocation = { 50.0f, 600 };
		break;
	case 4:
		m_spawnLocation = { 800, 600 };
	default:
		break;
	}

	m_body.setPosition(m_spawnLocation);
	m_body.setScale(0.5, 0.5);
}

void EnemyEntity::update()
{
	if (!m_alive) //respawn
	{
		m_body.setPosition(m_spawnLocation);
		m_alive = true;
	}

	m_body.move(m_location); //moves enemy to a location which is changed constantly by chasePlayer function
}

void EnemyEntity::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

void EnemyEntity::init(sf::Texture& t_zombieTexture)
{
	m_zombieTexture = t_zombieTexture;
	m_body.setTexture(m_zombieTexture);
	sf::Vector2u textureSize = m_zombieTexture.getSize();
	m_body.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void EnemyEntity::chasePlayer(sf::Vector2f t_playerLocation)
{
	sf::Vector2f directionVector = t_playerLocation - m_body.getPosition();
	sf::Vector2f directionNormal = thor::unitVector(directionVector);
	m_velocity = directionNormal * m_speed;
	m_location = m_velocity;

	//rotate zombie
	float rotation = 0.0f;
	sf::Vector2f shapeLocation = m_body.getPosition();
	sf::Vector2f rotationVector;
	rotationVector.x = t_playerLocation.x - shapeLocation.x;
	rotationVector.y = t_playerLocation.y - shapeLocation.y;

	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;
	rotation = -rotation; //sfml window is upside down so negative rotation

	m_body.setRotation(rotation);
}

void EnemyEntity::friendlyCollision(sf::Vector2f t_intersectPos)
{

}

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	checkBounds();
	m_position += m_velocity;
	bulletSprite.setPosition(m_position);
}


void Bullet::draw(sf::RenderWindow& t_window)
{
	t_window.draw(bulletSprite);
}

void Bullet::init(sf::Texture& t_bulletTexture)
{
	bulletTexture = t_bulletTexture;
	sf::Vector2u textureSize = bulletTexture.getSize();
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
	bulletSprite.setScale(0.2, 0.2);
}

void Bullet::checkBounds()
{
	if (m_position.y > screenSize.y || m_position.x > screenSize.x || m_position.x < 0 || m_position.y < 0)
	{
		fired = false;
	}
}

void Bullet::rotateBulletSprite(sf::Vector2f t_mousePos, sf::Vector2f t_playerPos)
{
	float rotation = 0.0f;
	sf::Vector2f rotationVector;
	rotationVector.x = t_mousePos.x - t_playerPos.x;
	rotationVector.y = t_mousePos.y - t_playerPos.y;

	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;

	bulletSprite.setRotation(-rotation);
}

