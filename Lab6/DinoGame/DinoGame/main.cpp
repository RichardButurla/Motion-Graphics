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

static const int SCREEN_WIDTH = 602;
static const int SCREEN_HEIGHT = 140;

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
	void init(sf::Texture& t_texture);

	void randomiseObstacle();
	void setCactusObstacle();
	void setBirdObstacle();


private:
	ObstacleType m_obstacleType;
	sf::Sprite m_obstacleSprite;
	sf::Texture m_obstacleTexture;
	sf::IntRect m_currentFrame; 

	sf::Vector2f m_position{300,263 };
	sf::Vector2f m_velocity{-355,0};

};

class Obstacles
{
public:
	Obstacles(sf::Texture& t_texture);
	~Obstacles();

	void update(double t_deltaTime);
	void render(sf::RenderWindow& t_window);

private:
	static const int MAX_OBSTACLES = 1;
	Obstacle m_obstacles[MAX_OBSTACLES];

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

	sf::Clock m_groundClock;
	sf::Time m_timeSinceLastSpeedIncrease = sf::seconds(1);

	float speedIncrease = -0.1;

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

	static constexpr double GRAVITY = 40;

private:
	sf::Sprite m_dinoSprite;
	sf::Texture m_dinoTexture;

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

	sf::Texture dinotexture;
	if (!dinotexture.loadFromFile("ASSETS/IMAGES/dino-spritesheet.png"))
	{
		std::cout << "failed to load spritesheet Texture";
	}
	Dino dinosaur(dinotexture);

	sf::Texture groundTexture = dinotexture;
	Scene ground(groundTexture);

	Obstacles obstacles(dinotexture);

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
			window.clear(sf::Color::White);
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
			}

			//Check collision
			

			//Update here
			dinosaur.update(timePerFrame);
			ground.update(timePerFrame);
			obstacles.update(timePerFrame.asSeconds());

			//Draw here
			dinosaur.render(window);
			ground.render(window);
			obstacles.render(window);



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
		m_dinoSprite.setTextureRect(sf::IntRect(940 + (m_duckingCurrentFrame * 59), 0, 59, 50));
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
	if (m_groundClock.getElapsedTime() > m_timeSinceLastSpeedIncrease)
	{
		m_groundClock.restart();
		for (int i = 0; i < MAX_GROUND_SPRITES; i++)
		{
			m_groundVelocities[i].x += speedIncrease;
		}
	}
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		if (m_groundPositions[i].x < 0 - SCREEN_WIDTH)
		{
			m_groundPositions[i].x = SCREEN_WIDTH - 7;
		}
		m_groundPositions[i].x += m_groundVelocities[i].x * t_deltaTime.asSeconds();
		m_groundSprites[i].setPosition(m_groundPositions[i]);
	}
	for (int i = 0; i < MAX_CLOUD_SPRITES; i++)
	{
		
		if (m_cloudPositions[i].x < 0 - 50)
		{
			m_cloudPositions[i].x = SCREEN_WIDTH - 7;		
		}
		std::cout << "\nPos X for Cloud " << i << " is: " << m_cloudPositions[i].x;
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
	m_obstacleSprite.setTextureRect(m_currentFrame);
	t_window.draw(m_obstacleSprite);
}

void Obstacle::update(double t_deltaTime)
{
	if (m_position.x + m_currentFrame.width < 0)
	{
		m_position.x = SCREEN_WIDTH;
		randomiseObstacle();
	}

	m_position.x += m_velocity.x * t_deltaTime;
	m_obstacleSprite.setPosition(m_position);
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
	int obstacleType = std::rand() % 2 + 1;
	obstacleType = 2;

	switch (static_cast<ObstacleType>(obstacleType))
	{
	case ObstacleType::None:
			break;

	case ObstacleType::Cactus:
		setCactusObstacle();
		break;

	case ObstacleType::Bird:
		setBirdObstacle();
		break;
	default:
		break;
	}
	
}

void Obstacle::setCactusObstacle()
{
	int randCactusType = std::rand() % 3 + 1; //this will deicde wether we use the big or small cactus parts of the texture
	int randNumberOfCacti; //number of cacti together
	int randStartCactusOffset; //from where in the png do we start the texture rect. max of 4 to ensure 3 cacti together is possible

	

	if (randCactusType == 1) //small Cactus
	{
		randNumberOfCacti = std::rand() % 3 + 1;
		randStartCactusOffset = std::rand() % 4;
		m_currentFrame = (sf::IntRect(225 + (18 * randStartCactusOffset), 0, (18 * randNumberOfCacti), 50));
		m_position.y = 96;
	}
	else if(randCactusType == 2)//big cactus which will have a max of 2 bundles
	{
		randNumberOfCacti = std::rand() % 2 + 1;
		randStartCactusOffset = std::rand() % 3; 
		m_currentFrame = (sf::IntRect(333 + (randStartCactusOffset * 25), 0, (randNumberOfCacti * 25), 55));
		m_position.y = 82; 
	}
	else //triple cactus
	{
		m_currentFrame = (sf::IntRect(408, 0, 75, 55));
		m_position.y = 82;
	} 

	
}

void Obstacle::setBirdObstacle()
{
	m_currentFrame = sf::IntRect(180, 0, 48, 50);
}

/////////////////////////////////


Obstacles::Obstacles(sf::Texture& t_texture)
{
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].init(t_texture);
	}
}

Obstacles::~Obstacles()
{
}

void Obstacles::update(double t_deltaTime)
{
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].update(t_deltaTime);
	}
}

void Obstacles::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < MAX_OBSTACLES; i++)
	{
		m_obstacles[i].render(t_window);
	}
}
