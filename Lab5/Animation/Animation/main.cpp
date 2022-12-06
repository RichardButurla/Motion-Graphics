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

enum class AnimationState
{
	None,
	RunningRight,
	RunningLeft,
	GoingUp,
	GoingDown
};

class Robot
{
public:
	Robot(sf::Texture & t_texture);
	~Robot();

	void update();
	void render(sf::RenderWindow& t_window);

	void setVelocity(sf::Vector2f t_velocity) { m_velocity = t_velocity; }
	void setAnimationState(AnimationState t_state) { m_currentAnimationState = t_state; }
	void checkAnimationState();

	//Animation Methods
	void animateRunRight();
	void animateRunLeft();
	void animateIdle();
	void animateClimbUp();
	void animateClimbDown();


private:
	sf::Sprite m_robotSprite;
	sf::Texture m_robotTexture;

	sf::Vector2f m_position{300,300};
	sf::Vector2f m_velocity;


	AnimationState m_currentAnimationState = AnimationState::None;

	int cols;
	int rows;
	
	int frameWidth;
	int frameHeight;

	sf::Clock m_clock;
	int currentCollumn = 0;
	int currentRow = 4;


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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				robot.setAnimationState(AnimationState::GoingUp);
				robotVelocity.y = -robotSpeed;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				robot.setAnimationState(AnimationState::GoingDown);
				robotVelocity.y = robotSpeed;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				robot.setAnimationState(AnimationState::RunningLeft);
				robotVelocity.x = -robotSpeed;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				robot.setAnimationState(AnimationState::RunningRight);
				robotVelocity.x = robotSpeed;
			}
			robot.setVelocity(robotVelocity);


			//Update here
			robot.update();

			//Draw here
			robot.render(window);

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

}

Robot::~Robot()
{
}

void Robot::update()
{
	m_position += m_velocity;
	m_robotSprite.setPosition(m_position);

	checkAnimationState();
}

void Robot::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_robotSprite);
}


void Robot::checkAnimationState()
{
	switch (m_currentAnimationState)
	{
	case AnimationState::None:

		break;
	case AnimationState::RunningRight:
		animateRunRight();
		break;
	case AnimationState::RunningLeft:
		animateRunLeft();
		break;
	case AnimationState::GoingUp:
		animateClimbUp();
		break;
	case AnimationState::GoingDown:
		animateClimbDown();
		break;
	default:
		break;
	}
}

void Robot::animateRunRight()
{
	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		currentCollumn++;
		m_clock.restart();
	}
	if (currentCollumn > 7)
	{
		currentCollumn = 0;
	}

	//Walking Right
	sf::IntRect Walkingrect = { frameWidth * currentCollumn, frameHeight * currentRow, frameWidth , frameHeight };
	m_robotSprite.setTextureRect(Walkingrect);
	m_robotSprite.setScale(1, 1);
}

void Robot::animateRunLeft()
{

	if (m_clock.getElapsedTime().asSeconds() > 0.1)
	{
		currentCollumn++;
		m_clock.restart();
	}
	if (currentCollumn > 7 )
	{
		currentCollumn = 0;
	}

	//Walking.
	sf::IntRect Walkingrect = { frameWidth * currentCollumn, frameHeight * currentRow, frameWidth , frameHeight };
	m_robotSprite.setTextureRect(Walkingrect);
	m_robotSprite.setScale(-1, 1);
}

void Robot::animateIdle()
{
	
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