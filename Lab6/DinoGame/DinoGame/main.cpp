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
#include <sstream>
#include <iomanip>

static const int SCREEN_WIDTH = 602;
static const int SCREEN_HEIGHT = 140;
static double SPEED_INCREASE = -0.1;
bool DEBUG_MODE = false;

enum class ObstacleType
{
	None,
	Bird,
	Cactus
};

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	void render(sf::RenderWindow& t_window);
	void update(double t_deltaTime); 
	void updateBirdAnimation();
	void init(sf::Texture& t_texture);

	void randomiseObstacle();
	void setCactusObstacle();
	void setBirdObstacle();

	void stop() { m_velocity.x = 0; }
	void release() { m_velocity.x = m_speed; }
	void setPosition(sf::Vector2f t_position) { m_position = t_position; }

	float getXPosition() { return m_position.x; }
	sf::FloatRect getBoundingBox() { return m_obstacleSprite.getGlobalBounds(); }


private:
	ObstacleType m_obstacleType;
	sf::Sprite m_obstacleSprite;
	sf::Texture m_obstacleTexture;
	sf::IntRect m_currentFrame; 

	sf::RectangleShape m_hitbox;

	sf::Vector2f m_position{SCREEN_WIDTH,193 };
	float m_speed = -355;
	sf::Vector2f m_velocity{-355,0};

	sf::Clock birdAnimationClock;
	sf::Time birdFrameTime = sf::seconds(0.3);
	int m_currentBirdFrame = 0;

};

class Obstacles
{
public:
	Obstacles(sf::Texture& t_texture);
	~Obstacles();

	void update(double t_deltaTime);
	void render(sf::RenderWindow& t_window);

	void checkObstaclePositions();
	bool checkCollision(const sf::FloatRect& t_other);

private:
	static const int MAX_OBSTACLES = 2;
	Obstacle m_obstacles[MAX_OBSTACLES];
	bool chanceOfSecondObject = false;

	sf::Clock m_clock;
	sf::Time secondObstacleSpawnTime = sf::seconds(5);

 };


class Scene
{
public:
	Scene(sf::Texture& t_texture);
	~Scene();

	void render(sf::RenderWindow& t_window);
	void update(sf::Time t_deltaTime);

	static const int MAX_GROUND_SPRITES = 2;
	static const int MAX_CLOUD_SPRITES = 4;
private:
	sf::Sprite m_groundSprites[MAX_GROUND_SPRITES];
	sf::Sprite m_cloudSprites[MAX_CLOUD_SPRITES];
	sf::Texture m_groundTexture;

	sf::Vector2f m_groundPositions[MAX_GROUND_SPRITES]
	{
		{0,SCREEN_HEIGHT - 20},
		{SCREEN_WIDTH,SCREEN_HEIGHT - 20}
	};
	sf::Vector2f m_groundVelocities[MAX_GROUND_SPRITES]
	{
		{-355,0},
		{-355,0}
	};

	sf::Vector2f m_cloudPositions[MAX_CLOUD_SPRITES]
	{
		{SCREEN_WIDTH,20},
		{SCREEN_WIDTH + 170,40},
		{SCREEN_WIDTH + (170 * 2),30},
		{SCREEN_WIDTH + (170 * 3),60}
	};
	float m_cloudSpeed = -40;
	

};

class Dino
{
public:

	Dino(sf::Texture &t_texture);
	~Dino();

	void update(sf::Time t_deltaTime);
	void updateDinoAnimation();
	void render(sf::RenderWindow& t_window);
	
	void jump() { if (!inAir)m_velocity.y = -11; inAir = true; }
	void duck() { isDucking = true; }
	void stand() { isDucking = false; }

	const sf::FloatRect getBoundingBox() const { return m_dinoSprite.getGlobalBounds(); }

	static constexpr double GRAVITY = 40;

private:
	sf::Sprite m_dinoSprite;
	sf::Texture m_dinoTexture;
	sf::RectangleShape hitBox;

	bool inAir{ false };
	bool isDucking{ false };

	sf::Vector2f m_position{30,400};
	sf::Vector2f m_velocity;

	sf::Clock m_runningClock;
	sf::Time m_runningFrameTime = sf::seconds(0.05);
	int m_runningCurrentFrame = 0;

	sf::Clock m_duckingClock;
	sf::Time m_duckingFrameTime = sf::seconds(0.075);
	int m_duckingCurrentFrame = 0;

};

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "First Graphics in C++");

	std::srand(static_cast<unsigned int>(time(nullptr)));
	sf::Font font;
	if (!font.loadFromFile("ASSETS/FONTS/PressStart2P-Regular.ttf"))
	{
		std::cout << "failed to load font";
	}

	sf::Texture spriteSheetTexture;
	if (!spriteSheetTexture.loadFromFile("ASSETS/IMAGES/dino-spritesheet.png"))
	{
		std::cout << "failed to load spritesheet Texture";
	}
	Dino dinosaur(spriteSheetTexture);

	Scene scene(spriteSheetTexture);

	Obstacles obstacles(spriteSheetTexture);

	sf::Sprite gameOverTextSprite;
	gameOverTextSprite.setTexture(spriteSheetTexture);
	gameOverTextSprite.setTextureRect(sf::IntRect(483, 13, 193, 30));
	gameOverTextSprite.setPosition(SCREEN_WIDTH / 2.9, SCREEN_HEIGHT / 4);

	sf::Sprite replayButtonSprite;
	replayButtonSprite.setTexture(spriteSheetTexture);
	replayButtonSprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
	replayButtonSprite.setPosition(SCREEN_WIDTH / 2.13, SCREEN_HEIGHT / 2);

	sf::Color Grey{ 50,50,50 };
	sf::Text currentScoreText;
	currentScoreText.setFont(font);
	currentScoreText.setFillColor(Grey);
	currentScoreText.setOutlineColor(sf::Color::Black);
	currentScoreText.setCharacterSize(12U);
	currentScoreText.setString("00000");
	currentScoreText.setPosition(SCREEN_WIDTH - currentScoreText.getGlobalBounds().width - 10, 0);

	sf::Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setFillColor(sf::Color{50,50,50,150});
	highScoreText.setOutlineColor(sf::Color::Black);
	highScoreText.setCharacterSize(12U);
	highScoreText.setString("HI: ");
	highScoreText.setPosition(SCREEN_WIDTH - highScoreText.getGlobalBounds().width - 10, 0);

	sf::Clock scoreAnimationDurationClock;
	sf::Clock scoreAnimationClock;
	bool playingScoreAnimation = false;

	sf::Clock scoreClock;
	sf::Time scoreTime = sf::seconds(0.05);
	float currentScore = 0;
	int scoreIncreaseCap = 100;
	float scoreIncrement = 0.45;
	float currentScoreMultiplier = 1;
	int highScore = 0;

	sf::Clock speedClock;
	sf::Time m_timeSinceLastSpeedIncrease = sf::seconds(1);

	
	bool gameOver = false;
	bool replayedAtLeastOnce = false;

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock clockForFrameRate;
	clockForFrameRate.restart();

	/*sf::View view = window.getView();
	view.zoom(2);
	window.setView(view);*/

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
			window.clear(sf::Color::White);
			//Handle Mouse Input
			if (sf::Event::MouseButtonReleased)
			{
				if (gameOver)
				{
					sf::Vector2f buttonPos = replayButtonSprite.getPosition();
					sf::FloatRect buttonRect = replayButtonSprite.getGlobalBounds();
					sf::Vector2i mousePos;
					mousePos.x = event.mouseButton.x;
					mousePos.y = event.mouseButton.y;

					if (mousePos.x > buttonPos.x && mousePos.x < buttonPos.x + buttonRect.width
						&& mousePos.y > buttonPos.y && mousePos.y < buttonPos.y + buttonRect.height)
					{
						//Create fresh Objects
						dinosaur = Dino(spriteSheetTexture);
						scene = Scene(spriteSheetTexture);
						obstacles =Obstacles(spriteSheetTexture);

						//reset variables
						SPEED_INCREASE = -0.1;
						highScore = currentScore;
						currentScore = 0;
						scoreIncrement = 0.5;
						currentScoreMultiplier = 1;

						//reset Timers
						speedClock.restart();
						scoreClock.restart();

						gameOver = false;
						replayedAtLeastOnce = true;
					}
				}
				
			}

			//Handle Key Input
			if (sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Down)
				{
					dinosaur.stand();
				}
			}

			if (sf::Event::KeyPressed)
			{
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					dinosaur.jump();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					dinosaur.duck();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				{
					gameOver = true;
				}
			}
			

			//Update Clocks
			if (scoreClock.getElapsedTime() > scoreTime)
			{
				if (!gameOver)
				{
					if (static_cast<int>(currentScore) / scoreIncreaseCap == 1)
					{
						playingScoreAnimation = true;
						scoreAnimationDurationClock.restart();
						scoreAnimationClock.restart();
						currentScoreMultiplier += 0.1;
						scoreIncreaseCap += 100;
					}
					scoreClock.restart();
					currentScore += scoreIncrement * currentScoreMultiplier;
				}				
			}


			if (speedClock.getElapsedTime() > m_timeSinceLastSpeedIncrease)
			{
				speedClock.restart();
				//SPEED_INCREASE -= 0.1;
			}
			if (playingScoreAnimation)
			{
				if (scoreAnimationDurationClock.getElapsedTime() < sf::seconds(3))
				{
					if (scoreAnimationClock.getElapsedTime() < sf::seconds(0.25))
					{				
						currentScoreText.setFillColor(Grey);
						currentScoreText.setOutlineColor(sf::Color::Black);
					}
					if (scoreAnimationClock.getElapsedTime() > sf::seconds(0.5))
					{
						
						currentScoreText.setFillColor(sf::Color::White);
 						currentScoreText.setOutlineColor(sf::Color::White);
						
					}
					if (scoreAnimationClock.getElapsedTime() > sf::seconds(1))
					{
						scoreAnimationClock.restart();
					}
				}
				else
				{
					playingScoreAnimation = false;
				}

			}
			
			

			if (!playingScoreAnimation)
			{
				
				currentScoreText.setFillColor(Grey);
				currentScoreText.setOutlineColor(sf::Color::Black);
				std::ostringstream scoreString;
				std::ostringstream highScoreString;
				if (replayedAtLeastOnce)
				{
					highScoreString << "HI: " << std::setw(5) << std::setfill('0') << static_cast<int>(highScore) << " ";
					scoreString  <<  std::setw(5) << std::setfill('0') << static_cast<int>(currentScore);
					currentScoreText.setString(scoreString.str());
					highScoreText.setString(highScoreString.str());
					currentScoreText.setPosition(SCREEN_WIDTH - currentScoreText.getGlobalBounds().width - 10, 0);
					highScoreText.setPosition(SCREEN_WIDTH - highScoreText.getGlobalBounds().width - currentScoreText.getGlobalBounds().width - 10, 0);
				}
				else
				{
					scoreString << std::setw(5) << std::setfill('0') << static_cast<int>(currentScore);
					currentScoreText.setString(scoreString.str());
				}
				
			}
			

			//Check Collisions
			if (obstacles.checkCollision(dinosaur.getBoundingBox()))
			{
				gameOver = true;
			}
			

			//Update here
			if (!gameOver)
			{
				dinosaur.update(timePerFrame);
				scene.update(timePerFrame);
				obstacles.update(timePerFrame.asSeconds());
			}
			

			//Draw here
			scene.render(window);
			obstacles.render(window);
			dinosaur.render(window);
			window.draw(currentScoreText);
			if (replayedAtLeastOnce)
			{
				window.draw(highScoreText);
			}
			
			if (gameOver)
			{
				window.draw(gameOverTextSprite);
				window.draw(replayButtonSprite);
			}

			window.display();
			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}

Dino::Dino(sf::Texture& t_texture) : m_dinoTexture(t_texture)
{
	m_dinoSprite.setTexture(t_texture);
	m_dinoSprite.setPosition(m_position);
	m_dinoSprite.setTextureRect(sf::IntRect(675, 0, 48, 50));
}

Dino::~Dino()
{
}

void Dino::update(sf::Time t_deltaTime)
{
	if(!inAir)
	updateDinoAnimation();

	m_velocity.y += GRAVITY * t_deltaTime.asSeconds();;
	m_position += m_velocity;
	m_dinoSprite.setPosition(m_position);
}

void Dino::updateDinoAnimation()
{
	if (isDucking)
	{
		if (m_duckingClock.getElapsedTime() > m_duckingFrameTime)
		{
			m_duckingClock.restart();
			m_duckingCurrentFrame++;
			if (m_duckingCurrentFrame > 1)
			{
				m_duckingCurrentFrame = 0;
			}
		}
		m_dinoSprite.setTextureRect(sf::IntRect(940 + (m_duckingCurrentFrame * 59), 20, 59, 30));
		m_position.y += 20;
	}
	else
	{
		if (m_runningClock.getElapsedTime() > m_runningFrameTime)
		{
			m_runningClock.restart();
			m_runningCurrentFrame++;
			if (m_runningCurrentFrame == 1) //useless frame for animation
			{
				m_runningCurrentFrame++;
			}
			if (m_runningCurrentFrame > 3)
			{
				m_runningCurrentFrame = 0;
			}
		}
		m_dinoSprite.setTextureRect(sf::IntRect(675 + (m_runningCurrentFrame * 44), 0, 48, 50));
	}
	
}

void Dino::render(sf::RenderWindow& t_window)
{
	if (DEBUG_MODE)
	{
		hitBox.setFillColor(sf::Color::White);
		hitBox.setOutlineColor(sf::Color::Red);
		hitBox.setOutlineThickness(3);
		hitBox.setSize({ m_dinoSprite.getGlobalBounds().width,m_dinoSprite.getGlobalBounds().height });
		hitBox.setPosition(m_position);
		t_window.draw(hitBox);
	}

	t_window.draw(m_dinoSprite);

	if (m_position.y > SCREEN_HEIGHT - 56 )
	{
		inAir = false;
		m_velocity.y = 0;
		m_position.y = SCREEN_HEIGHT - 56;
	}
}

Scene::Scene(sf::Texture& t_texture) : m_groundTexture(t_texture)
{
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		m_groundSprites[i].setTexture(t_texture);
	}
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(t_texture.getSize());
	m_groundSprites[0].setTextureRect(sf::IntRect(0,(textureSize.y / 6) * 4.7, textureSize.x / 2, textureSize.y));
	m_groundSprites[1].setTextureRect(sf::IntRect(textureSize.x / 2, (textureSize.y / 6) * 4.7, textureSize.x / 2, textureSize.y));

	for (int i = 0; i < MAX_CLOUD_SPRITES; i++)
	{
		m_cloudSprites[i].setTexture(t_texture);
		m_cloudSprites[i].setTextureRect(sf::IntRect(85, 0,48,50));
	}
	
}

Scene::~Scene()
{
}

void Scene::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		t_window.draw(m_groundSprites[i]);
	}
	for (int i = 0; i < MAX_CLOUD_SPRITES; i++)
	{
		t_window.draw(m_cloudSprites[i]);
	}
}

void Scene::update(sf::Time t_deltaTime)
{
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		if (m_groundPositions[i].x < 0 - SCREEN_WIDTH)
		{
			m_groundPositions[i].x = SCREEN_WIDTH - 7;
		}
		m_groundPositions[i].x += (m_groundVelocities[i].x + SPEED_INCREASE) * t_deltaTime.asSeconds();
		m_groundSprites[i].setPosition(m_groundPositions[i]);
	}
	for (int i = 0; i < MAX_CLOUD_SPRITES; i++)
	{
		
		if (m_cloudPositions[i].x < 0 - 50)
		{
			m_cloudPositions[i].x = SCREEN_WIDTH - 7;		
		}
		m_cloudPositions[i].x += m_cloudSpeed * t_deltaTime.asSeconds();
		m_cloudSprites[i].setPosition(m_cloudPositions[i]);
	}
}

Obstacle::Obstacle()
{

}

Obstacle::~Obstacle()
{
}

void Obstacle::render(sf::RenderWindow& t_window)
{
	if (DEBUG_MODE)
	{
		m_hitbox.setFillColor(sf::Color::White);
		m_hitbox.setOutlineColor(sf::Color::Red);
		m_hitbox.setOutlineThickness(3);
		m_hitbox.setSize({ m_obstacleSprite.getGlobalBounds().width,m_obstacleSprite.getGlobalBounds().height });
		m_hitbox.setPosition(m_position);
		t_window.draw(m_hitbox);
	}

	m_obstacleSprite.setTextureRect(m_currentFrame);
	t_window.draw(m_obstacleSprite);
}

void Obstacle::update(double t_deltaTime)
{
	if (m_obstacleType == ObstacleType::Bird)
	{
		updateBirdAnimation();
	}
	m_position.x += (m_velocity.x + SPEED_INCREASE) * t_deltaTime;
	m_obstacleSprite.setPosition(m_position);
}

void Obstacle::updateBirdAnimation()
{
	if (birdAnimationClock.getElapsedTime() > birdFrameTime )
	{
		birdAnimationClock.restart();
		m_currentBirdFrame++;
	}
	if (m_currentBirdFrame > 1)
	{
		m_currentBirdFrame = 0;
	}
	m_currentFrame = sf::IntRect(132 + (m_currentBirdFrame * 48), 10, 48, 30);
}

void Obstacle::init(sf::Texture& t_texture)
{
	m_obstacleTexture = t_texture;
	m_obstacleSprite.setTexture(t_texture);

	m_currentFrame = sf::IntRect(180, 0, 48, 50);
	m_obstacleSprite.setTextureRect(m_currentFrame);

}

void Obstacle::randomiseObstacle()
{
	int randNum = std::rand() % 10 + 1;

	if (randNum == 1 || randNum <= 8)
	{
		m_obstacleType = ObstacleType::Cactus;
		setCactusObstacle();
	}
	else
	{
		m_obstacleType = ObstacleType::Bird;
		setBirdObstacle();
	}

}

void Obstacle::setCactusObstacle()
{
	//this will deicde wether we use the big or small cactus parts of the texture and has a preference to the middle
	int randCactusType = std::rand() % 10 + 1; 
	int randNumberOfCacti; //number of cacti together
	int randStartCactusOffset; //from where in the png do we start the texture rect. max of 4 to ensure 3 cacti together is possible
	
	//60 percent chance
	if (randCactusType >= 1 && randCactusType <= 6) //small Cactus
	{
		randNumberOfCacti = std::rand() % 3 + 1;
		randStartCactusOffset = std::rand() % 4;
		m_currentFrame = (sf::IntRect(225 + (18 * randStartCactusOffset), 0, (18 * randNumberOfCacti), 50));
		m_position.y = 96;
	}
	//20 percent
	else if(randCactusType >= 7 && randCactusType <= 9)//big cactus which will have a max of 2 bundles
	{
		randNumberOfCacti = std::rand() % 2 + 1;
		randStartCactusOffset = std::rand() % 3; 
		m_currentFrame = (sf::IntRect(333 + (randStartCactusOffset * 25), 0, (randNumberOfCacti * 25), 55));
		m_position.y = 82; 
	}
	//10 percent
	else //triple cactus
	{
		m_currentFrame = (sf::IntRect(408, 0, 75, 55));
		m_position.y = 82;
	} 

	
}

void Obstacle::setBirdObstacle()
{
	int randPossiblePos = std::rand() % 2 + 1;

	if (randPossiblePos == 1)
	{
		m_position.y = 86;
	}
	else
	{
		m_position.y = 70;
	}

	m_currentFrame = sf::IntRect(132, 0, 48, 50);
}

/////////////////////////////////


Obstacles::Obstacles(sf::Texture& t_texture)
{
	m_obstacles[0].setPosition({ SCREEN_WIDTH, 90 });
	m_obstacles[1].setPosition({ SCREEN_WIDTH + SCREEN_WIDTH / 2 + 50, 90 });

	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].init(t_texture);
		m_obstacles[i].randomiseObstacle();
	}
	
}

Obstacles::~Obstacles()
{
}

void Obstacles::update(double t_deltaTime)
{
	checkObstaclePositions();
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].update(t_deltaTime);
	}
}

void Obstacles::checkObstaclePositions()
{
	int xOffset = 30;

	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		if (m_obstacles[i].getXPosition() + m_obstacles[i].getBoundingBox().width < 0)
		{		
			if (i == 0)
			{
				m_obstacles[i].setPosition({ SCREEN_WIDTH + 7,90 });
			}
			if (i == 1)
			{
				m_obstacles[i].setPosition({ SCREEN_WIDTH + 7,90 });
			}
			m_obstacles[i].randomiseObstacle();
		}
	}
}

bool Obstacles::checkCollision(const sf::FloatRect& t_other)
{
	sf::FloatRect newBox = t_other;
	newBox.width -= 20;
	newBox.height -= 5;
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		if (newBox.intersects(m_obstacles[i].getBoundingBox())) {
			return true;
		}
	}
	return false;
}

void Obstacles::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].render(t_window);
	}
}
