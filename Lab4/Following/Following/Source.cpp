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

//My own vector library

float vectorLength(const sf::Vector2f t_vectorA);  // root x2 + y2
float vectorLengthSquared(const sf::Vector2f t_vectorA); // x2 + y2
sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA);// length of ans is one, null in null out
float vectorCrossProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB);  // Vx * Uy - Vy * Ux
float vectorDotProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB);  // Vx * Uy + Vy * Ux
float vectorAngleBetween(const sf::Vector2f t_vectorA, const sf::Vector2f t_vectorB); // result always 0>= && <=180
sf::Vector2f vectorRotateBy(const sf::Vector2f t_vectorA, const float t_angleRadians); // anti-clockwise ({1,0},PI/2) = {0,1}
sf::Vector2f vectorProjection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// ans parallel to second vector
sf::Vector2f vectorRejection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// ans perpendicular to second vector
float vectorScalarProjection(const sf::Vector2f t_vectorA, const sf::Vector2f t_onto);// scalar resolute

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

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "First Graphics in C++");

	
	std::srand(static_cast<unsigned int>(time(nullptr)));

	int playerSpeed = 10;

	//Load Textures
	sf::Texture playerTexture;

	if (!playerTexture.loadFromFile("ASSETS/IMAGES/goat.png"))
	{
		std::cout << "player texture failed to load";
	}



	//Player
	Player player(playerTexture);
	sf::Vector2f mousePos;

	//Enemies

	static const int MAX_ENEMIES = 3;
	EnemyEntity enemyArray[MAX_ENEMIES];
	
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemyArray[i].init(playerTexture);
		enemyArray[i].setAlive(true);
	}

	//Bullets
	static const int MAX_BULLETS = 50;
	Bullet bulletArray[MAX_BULLETS];
	int numberOfBulletsFired = 0;
	bool triggerStop = false;
	float timeSinceLastBulletFired = 0;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bulletArray[i].init(playerTexture);
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

			window.clear();
			if (sf::Event::MouseMoved == event.type)
			{ //always gets the mouse position when mouse is moved
				
				mousePos.x = static_cast<float>(event.mouseMove.x);
				mousePos.y = static_cast<float>(event.mouseMove.y);
				player.rotatePlayer(mousePos);
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
					if (numberOfBulletsFired < MAX_BULLETS - 1)
					{
						sf::Vector2f bulletVelocity = vectorUnitVector(mousePos - player.getPos()) * bulletArray[numberOfBulletsFired].getSpeed(); //set bullet in that direction
						bulletArray[numberOfBulletsFired].setVelocity(bulletVelocity); //we give it unit vector because in bullet class it multiplies by speed
						bulletArray[numberOfBulletsFired].fire(player.getPos());
						numberOfBulletsFired++;
						triggerStop = true;
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
	sf::Vector2f directionNormal = vectorUnitVector(directionVector);
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

void Bullet::rotateBulletSprite(sf::Vector2f t_mousePos, sf::Vector2f t_playerPos)
{
	float rotation = 0.0f;
	sf::Vector2f rotationVector;
	rotationVector.x = t_mousePos.x - t_playerPos.x;
	rotationVector.y = t_mousePos.y - t_playerPos.y;

	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;

	bulletSprite.setRotation(-rotation);
}



//Name: Richard Buturla


/// <summary>
/// get length of vector using sqrt of the sum of the squares
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(const sf::Vector2f t_vectorA)
{
	float sumOfSquares = (t_vectorA.x * t_vectorA.x) + (t_vectorA.y * t_vectorA.y);
	const float length = std::sqrt(sumOfSquares);
	return length;
}
/// <summary>
/// gets length squared of a vector using vectoe length
/// </summary>
/// <param name="t_vectorA"> input vector</param>
/// <returns>length squared</returns>
float vectorLengthSquared(const sf::Vector2f t_vectorA)
{
	const float lengthSquared = (t_vectorA.x * t_vectorA.x) + (t_vectorA.y * t_vectorA.y);
	return lengthSquared;
}
/// <summary>
/// gets the cross product of 2 vectors
/// </summary>
/// <param name="t_vectorA"> first input vector</param>
/// <param name="t_VectorB"> second input vector</param>
/// <returns> Cross product</returns>
float vectorCrossProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	const float crossProduct = (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);
	return crossProduct;
}
/// <summary>
/// gets the dot product of 2 vectors
/// </summary>
/// <param name="t_vectorA"> input vector 1</param>
/// <param name="t_VectorB"> input vector 2</param>
/// <returns> dotProduct</returns>
float vectorDotProduct(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	const float dotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);
	return dotProduct;
}
/// <summary>
/// Finds the angle between 2 vectors
/// </summary>
/// <param name="t_vectorA"> input vector 1</param>
/// <param name="t_VectorB"> input vector 2</param>
/// <returns> angle between in degrees</returns>
float vectorAngleBetween(const sf::Vector2f t_vectorA, const sf::Vector2f t_VectorB)
{
	float theta = (vectorDotProduct(t_vectorA, t_VectorB)) / (vectorLength(t_vectorA) * (vectorLength(t_VectorB)));
	const float angleBetween = (acos(theta)) * (180 / PI); // multiply by 180/PI to get degrees. calculations are in radians.
	return angleBetween;
}
/// <summary>
/// Rotates a vector with radians
/// </summary>
/// <param name="t_vectorA"> input vector</param>
/// <param name="t_angleRadians"> angle in radians</param>
/// <returns> the rotated vector</returns>
sf::Vector2f vectorRotateBy(const sf::Vector2f t_vectorA, const  float t_angleRadians)
{
	sf::Vector2f rotatedVector;
	rotatedVector.x = ((t_vectorA.x * cos(t_angleRadians)) - (t_vectorA.y * sin(t_angleRadians)));
	rotatedVector.y = ((t_vectorA.x * sin(t_angleRadians)) + (t_vectorA.y * cos(t_angleRadians)));
	return rotatedVector;
}

/// <summary>
/// Projects one vector onto another vector
/// </summary>
/// <param name="t_vectorA"> input vector that is projected</param>
/// <param name="t_onto"> input vector that is projected onto</param>
/// <returns> vector resolute of vector A in the direction of vector B</returns>
sf::Vector2f vectorProjection(const sf::Vector2f t_vectorA, const  sf::Vector2f t_onto)
{
	sf::Vector2f answer;
	answer.x = (((vectorDotProduct(t_vectorA, t_onto)) / vectorLength(t_onto))) * (t_onto.x / vectorLength(t_onto));
	answer.y = (((vectorDotProduct(t_vectorA, t_onto)) / vectorLength(t_onto))) * (t_onto.y / vectorLength(t_onto));
	return answer;
}
/// <summary>
/// Gets the orthogonal projection of vector a onto the plane orthogonal to b
/// </summary>
/// <param name="t_vectorA"> input vector that is rejected</param>
/// <param name="t_onto"> input vector that is rejected from </param>
/// <returns> vector resolute of vector A perpendicular to vector B </returns>
sf::Vector2f vectorRejection(const sf::Vector2f t_vectorA, const  sf::Vector2f t_onto)
{
	sf::Vector2f answer;
	answer = t_vectorA - vectorProjection(t_vectorA, t_onto);
	return answer;
}
/// <summary>
/// Gets the length of the vector Projection
/// </summary>
/// <param name="t_vector"> input vector that is projected</param>
/// <param name="t_onto"> input vector that is projected onto</param>
/// <returns> length</returns>
float vectorScalarProjection(const sf::Vector2f t_vector, const  sf::Vector2f t_onto)
{
	const float length = (vectorDotProduct(t_vector, t_onto)) / (vectorLength(t_onto));
	return length;
}

/// <summary>
/// Gets the unit vector of a vector
/// </summary>
/// <param name="t_vectorA">input vector</param>
/// <returns> the unit vector </returns>
sf::Vector2f vectorUnitVector(const sf::Vector2f t_vectorA)
{
	sf::Vector2f answer;
	answer.x = t_vectorA.x / vectorLength(t_vectorA);
	answer.y = t_vectorA.y / vectorLength(t_vectorA);
	return answer;
}


