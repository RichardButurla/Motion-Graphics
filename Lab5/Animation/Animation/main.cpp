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
#include <vector>

enum class RobotAnimationState
{
	None,
	RunningRight,
	RunningLeft,
	GoingUp,
	GoingDown
};

class RainDrop
{
public:
	RainDrop(sf::Vector2f& t_position, sf::Vector2f& t_velocity,sf::Color& t_color, sf::Vector2f& t_size);
	~RainDrop();

	void update();
	void render(sf::RenderWindow& t_window);

	void setPosition(sf::Vector2f t_position) { m_position = t_position; }
	sf::Vector2f getPosition() { return m_position; }
	sf::FloatRect getBoundingBox() { return m_raindropShape.getGlobalBounds(); }

private:
	sf::RectangleShape m_raindropShape;

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
};

class Rain
{
public:

	Rain();
	~Rain();

	void update();
	void render(sf::RenderWindow& t_window);

	void setPosition(sf::Vector2f t_pos) { m_rainPosition = t_pos; }
	void startRaining() { m_raining = true; }

	bool checkRainCollision(sf::FloatRect t_otherBoundingBox);

	static const int MAX_RAINDROPS = 100;
private:
	
	RainDrop* raindrops[MAX_RAINDROPS];
	sf::Clock m_rainSpawnClock;
	sf::Time m_rainSpawnTime{ sf::seconds(0.05) };
	unsigned int m_currentRainDrop = 0;
	unsigned int m_numberOfRaindrops = 0;
	bool m_raining{ false };

	sf::Vector2f m_rainPosition{ 800,200 };
	float maxYDistance = 500;
	sf::Vector2f raindropVel{ 0,5 };
	sf::Vector2f raindropSize{ 2,10 };
	sf::Color raindropColor{ sf::Color::Blue };
};

class Flower
{
public:
	Flower(sf::Texture& t_texture);
	~Flower();

	void update();
	void updateAnimation();
	void render(sf::RenderWindow& t_window);

	void setupAnimation();

	void setGrowing(bool t_boolean) { m_growing = true; }
	sf::FloatRect getBoundingBox() { return m_flowerSprite.getGlobalBounds(); }


	static const int MAX_ANIMATIONS = 1;
	static const int MAX_FRAMES = 4;

private:
	sf::Sprite m_flowerSprite;
	sf::Texture m_flowerTexture;

	sf::Vector2f m_position{ 800,500 };

	sf::IntRect m_animationFrames[MAX_ANIMATIONS][MAX_FRAMES];

	int frameWidth;
	int frameHeight;
	int currentCollumn = 0;
	int m_currentFrame = 0;

	int cols;
	int rows;

	sf::Clock m_clock;

	bool m_growing{ false };

};

class Robot
{
public:
	Robot(sf::Texture & t_texture);
	~Robot();

	void update();
	void updateAnimation();
	void render(sf::RenderWindow& t_window);

	void setupAnimations();

	void setVelocity(sf::Vector2f t_velocity) { m_velocity = t_velocity; }
	void setAnimationState(RobotAnimationState t_state) { m_currentAnimationState = t_state; }
	void checkAnimationState();

	//Animation Methods
	void animateRunRight();
	void animateRunLeft();
	void animateIdle();
	void animateClimbUp();
	void animateClimbDown();

	static const int MAX_ANIMATIONS = 3;
	static const int MAX_FRAMES = 9;


private:
	sf::Sprite m_robotSprite;
	sf::Texture m_robotTexture;

	sf::Vector2f m_position{300,500};
	sf::Vector2f m_velocity;

	sf::IntRect m_animationFrames[MAX_ANIMATIONS][MAX_FRAMES];


	RobotAnimationState m_currentAnimationState = RobotAnimationState::None;

	int cols;
	int rows;
	
	int frameWidth;
	int frameHeight;

	sf::Clock m_clock;
	int currentCollumn = 0;
	int currentRow = 4;

	int m_currentFrame = 0;


};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 800), "First Graphics in C++");

	std::srand(static_cast<unsigned int>(time(nullptr)));
	sf::Font font;

	if (!font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "failed to load font";
	}



	sf::Texture robotTexture;
	float robotSpeed = 5;
	sf::Vector2f robotVelocity = { 0,0 };
	if (!robotTexture.loadFromFile("ASSETS/IMAGES/character_robot_sheet.png"))
	{
		std::cout << "failed to load robot Texture";
	}
	Robot robot(robotTexture);

	sf::Texture flowerTexture;
	if (!flowerTexture.loadFromFile("ASSETS/IMAGES/flowerAnimation.png"))
	{
		std::cout << "failed to load flower Texture";
	}
	Flower flower(flowerTexture);

	sf::Texture cloudTexture;
	if (!cloudTexture.loadFromFile("ASSETS/IMAGES/cloud.png"))
	{
		std::cout << "failed to load cloud Texture";
	}

	sf::Sprite rainCloud;
	rainCloud.setTexture(cloudTexture);
	sf::Vector2u cloudTextureSize = cloudTexture.getSize();
	rainCloud.setOrigin(cloudTextureSize.x / 2, cloudTextureSize.y / 2);
	rainCloud.setPosition(800, 100);
	Rain rain;
	rain.setPosition(rainCloud.getPosition());


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
			robotVelocity = { 0,0 };
			//Handle Key Input
			if (sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					robot.setAnimationState(RobotAnimationState::None);
				}

				if (event.key.code == sf::Keyboard::Right)
				{
					robot.setAnimationState(RobotAnimationState::None);
				}
			}

			if (sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					robot.setAnimationState(RobotAnimationState::GoingUp);
					robotVelocity.y = -robotSpeed;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					robot.setAnimationState(RobotAnimationState::GoingDown);
					robotVelocity.y = robotSpeed;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					robot.setAnimationState(RobotAnimationState::RunningLeft);
					robotVelocity.x = -robotSpeed;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					robot.setAnimationState(RobotAnimationState::RunningRight);
					robotVelocity.x = robotSpeed;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					rain.startRaining();
				}
			}
			

			


			robot.setVelocity(robotVelocity);

			//Check collision
			if (rain.checkRainCollision(flower.getBoundingBox()))
			{
				flower.setGrowing(true);
			}

			//Update here
			robot.update();
			flower.update();
			rain.update();

			//Draw here
			
			robot.render(window);
			flower.render(window);
			window.draw(rainCloud);
			rain.render(window);

			

			window.display();
			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}

Robot::Robot(sf::Texture& t_texture) : m_robotTexture(t_texture)
{
	m_robotSprite.setTexture(m_robotTexture);
	m_robotSprite.setPosition(m_position);
	cols = 9;
	rows = 5;

	frameWidth = m_robotTexture.getSize().x / cols;
	frameHeight = m_robotTexture.getSize().y / rows;

	m_robotSprite.setOrigin(frameWidth / 2, frameHeight / 2);

	setupAnimations();

}

Robot::~Robot()
{
}

void Robot::update()
{
	m_position += m_velocity;
	m_robotSprite.setPosition(m_position);

	checkAnimationState();
	updateAnimation();
}

void Robot::updateAnimation()
{
	
}

void Robot::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_robotSprite);
}

void Robot::setupAnimations()
{
	currentRow = 4;
	int animationState = 0;
	animationState = static_cast<int>(RobotAnimationState::RunningLeft);
	for (int i = 0; i < 8; i++)
	{
		m_animationFrames[animationState][i] = sf::IntRect{frameWidth * i, frameHeight * currentRow, frameWidth , frameHeight};
	}
	animationState = static_cast<int>(RobotAnimationState::RunningRight);
	for (int i = 0; i < 8; i++)
	{
		m_animationFrames[animationState][i] = sf::IntRect{ frameWidth * i, frameHeight * currentRow, frameWidth , frameHeight };
	}
	animationState = static_cast<int>(RobotAnimationState::None);
	currentRow = 0;
	for (int i = 7; i < 9; i++)
	{
		m_animationFrames[animationState][i] = sf::IntRect{ frameWidth * i, frameHeight * currentRow, frameWidth , frameHeight };
	}
}


void Robot::checkAnimationState()
{
	switch (m_currentAnimationState)
	{
	case RobotAnimationState::None:
		animateIdle();
		break;
	case RobotAnimationState::RunningRight:
		animateRunRight();
		break;
	case RobotAnimationState::RunningLeft:
		animateRunLeft();
		break;
	case RobotAnimationState::GoingUp:
		//animateClimbUp();
		break;
	case RobotAnimationState::GoingDown:
		//animateClimbDown();
		break;
	default:
		break;
	}
}

void Robot::animateRunRight()
{
	int animationState = 0;
	animationState = static_cast<int>(RobotAnimationState::RunningRight);

	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		m_currentFrame++;
		m_clock.restart();
	}
	if (m_currentFrame > 7)
	{
		m_currentFrame = 0;
	}
	m_robotSprite.setTextureRect(m_animationFrames[animationState][m_currentFrame]);
	m_robotSprite.setScale(1, 1);
}

void Robot::animateRunLeft()
{
	int animationState = 0;
	animationState = static_cast<int>(RobotAnimationState::RunningLeft);

	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		m_currentFrame++;
		m_clock.restart();
	}
	if (m_currentFrame > 7)
	{
		m_currentFrame = 0;
	}
	m_robotSprite.setTextureRect(m_animationFrames[animationState][m_currentFrame]);
	m_robotSprite.setScale(-1, 1);
}

void Robot::animateIdle()
{
	int animationState = 0;
	animationState = static_cast<int>(RobotAnimationState::None);

	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		m_currentFrame++;
		m_clock.restart();
	}
	if (m_currentFrame < 7)
	{
		m_currentFrame = 7;
	}
	if (m_currentFrame > 8)
	{
		m_currentFrame = 7;
	}
	m_robotSprite.setTextureRect(m_animationFrames[animationState][m_currentFrame]);
	m_robotSprite.setScale(1, 1);
}

void Robot::animateClimbUp()
{
	currentRow = 0;
	currentCollumn = 5;
	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		currentCollumn++;
		m_clock.restart();
	}
	if (currentCollumn > 6)
	{
		currentCollumn = 5;
	}

	//Walking.
	sf::IntRect Walkingrect = { frameWidth * currentCollumn, frameHeight * currentRow, frameWidth , frameHeight };
	m_robotSprite.setTextureRect(Walkingrect);
	m_robotSprite.setScale(-1, 1);
}

void Robot::animateClimbDown()
{
	currentRow = 0;
	currentCollumn = 6;
	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		currentCollumn--;
		m_clock.restart();
	}
	if (currentCollumn < 5)
	{
		currentCollumn = 6;
	}

	//Walking.
	sf::IntRect Walkingrect = { frameWidth * currentCollumn, frameHeight * currentRow, frameWidth , frameHeight };
	m_robotSprite.setTextureRect(Walkingrect);
	m_robotSprite.setScale(-1, 1);
}





Flower::Flower(sf::Texture& t_texture) : m_flowerTexture(t_texture)
{
	m_flowerSprite.setTexture(m_flowerTexture);
	m_flowerSprite.setPosition(m_position);
	cols = 4;
	rows = 1;

	frameWidth = m_flowerTexture.getSize().x / 4.f;
	frameHeight = m_flowerTexture.getSize().y / 1.f;

	m_flowerSprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
	m_flowerSprite.setPosition(m_position);

	setupAnimation();
}

Flower::~Flower()
{
}

void Flower::update()
{
	updateAnimation();
}

void Flower::updateAnimation()
{
	int animationState = 0;
	if (m_growing)
	{
		if (m_clock.getElapsedTime().asSeconds() > 1.5)
		{
			if (m_currentFrame < MAX_FRAMES - 1)
			{
				m_currentFrame++;
				m_clock.restart();
			}
		}
	}
	
	m_flowerSprite.setTextureRect(m_animationFrames[animationState][m_currentFrame]);
}

void Flower::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_flowerSprite);
}

void Flower::setupAnimation()
{

	int animationState = 0;
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		m_animationFrames[animationState][i] = sf::IntRect{ (frameWidth - 3) * i, 0, frameWidth - 3 , frameHeight };
	}
	
}

RainDrop::RainDrop(sf::Vector2f& t_position, sf::Vector2f& t_velocity, sf::Color& t_color, sf::Vector2f& t_size) :
	m_position(t_position),
	m_velocity(t_velocity)
{
	m_raindropShape.setSize(t_size);
	m_raindropShape.setFillColor(t_color);
	m_raindropShape.setPosition(m_position);
}

RainDrop::~RainDrop()
{
	std::cout << "RaindDrop Destructor called";
}

void RainDrop::update()
{
	m_position += m_velocity;
	m_raindropShape.setPosition(m_position);
}

void RainDrop::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_raindropShape);
}

Rain::Rain()
{

}

Rain::~Rain()
{
	
}

void Rain::update()
{
	if (m_raining)
	{
		if (m_rainSpawnClock.getElapsedTime() > m_rainSpawnTime)
		{
			sf::Vector2f raindropPosition;
			int xOffset = 200;
			if (m_currentRainDrop < MAX_RAINDROPS)
			{
				raindropPosition.x = std::rand() % (xOffset) + (m_rainPosition.x - xOffset / 2);
				raindropPosition.y = m_rainPosition.y;
				raindrops[m_currentRainDrop] = new RainDrop(raindropPosition, raindropVel, raindropColor, raindropSize);
				m_numberOfRaindrops++;
				m_currentRainDrop++;
			}
		}

		for (int i = 0; i < m_numberOfRaindrops; i++)
		{
			if (raindrops[i]->getPosition().y > m_rainPosition.y + maxYDistance)
			{
				sf::Vector2f raindropPosition;
				int xOffset = 200;
				raindropPosition.x = std::rand() % (xOffset)+(m_rainPosition.x - xOffset / 2);
				raindropPosition.y = m_rainPosition.y;
				raindrops[i]->setPosition(raindropPosition);
			}
		}
	}

	for (int i = 0; i < m_numberOfRaindrops; i++)
	{
		raindrops[i]->update();
	}
}

void Rain::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_numberOfRaindrops; i++)
	{
		raindrops[i]->render(t_window);
	}
}

bool Rain::checkRainCollision(sf::FloatRect t_otherBoundingBox)
{
	for (int i = 0; i < m_numberOfRaindrops; i++)
	{
		if (t_otherBoundingBox.intersects(raindrops[i]->getBoundingBox()))
		{
			return true;
		}
	}
	return false;
}

