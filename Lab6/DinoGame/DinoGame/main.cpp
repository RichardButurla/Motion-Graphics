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
static const int SCREEN_HEIGHT = 400;

class Ground
{
public:
	Ground(sf::Texture& t_texture);
	~Ground();

	void render(sf::RenderWindow& t_window);
	void update(sf::Time t_deltaTime);

	static const int MAX_GROUND_SPRITES = 2;
private:
	sf::Sprite m_groundSprites[MAX_GROUND_SPRITES];
	sf::Texture m_groundTexture;

	sf::Vector2f m_positions[MAX_GROUND_SPRITES]
	{
		{0,SCREEN_HEIGHT - 100},
		{SCREEN_WIDTH,SCREEN_HEIGHT - 100}
	};
	sf::Vector2f m_velocities[MAX_GROUND_SPRITES]
	{
		{-2,0},
		{-2,0}
	};

};

class Dino
{
public:

	Dino(sf::Texture &t_texture);
	~Dino();

	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow& t_window);
	
	void jump() { m_velocity.y = -10; }

	static constexpr double GRAVITY = 38.2;

private:
	sf::Sprite m_dinoSprite;
	sf::Texture m_dinoTexture;

	sf::Vector2f m_position{100,400};
	sf::Vector2f m_velocity;

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
	Ground ground(groundTexture);

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
				
			}

			if (sf::Event::KeyPressed)
			{
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					dinosaur.jump();
				}
			}

			//Check collision
			

			//Update here
			dinosaur.update(timePerFrame);
			ground.update(timePerFrame);

			//Draw here
			dinosaur.render(window);
			ground.render(window);



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
	m_velocity.y += GRAVITY * t_deltaTime.asSeconds();;
	m_position += m_velocity;
	m_dinoSprite.setPosition(m_position);
}

void Dino::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_dinoSprite);

	if (m_position.y > (SCREEN_HEIGHT / 3 ) * 2)
	{
		m_velocity.y = 0;
		m_position.y = (SCREEN_HEIGHT / 3) * 2;
	}
}

Ground::Ground(sf::Texture& t_texture) : m_groundTexture(t_texture)
{
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		m_groundSprites[i].setTexture(t_texture);
	}
	sf::Vector2f textureSize = static_cast<sf::Vector2f>(t_texture.getSize());
	m_groundSprites[0].setTextureRect(sf::IntRect(0,(textureSize.y / 6) * 4.7, textureSize.x / 2, textureSize.y));
	m_groundSprites[1].setTextureRect(sf::IntRect(textureSize.x / 2, (textureSize.y / 6) * 4.7, textureSize.x / 2, textureSize.y));

	
}

Ground::~Ground()
{
}

void Ground::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		t_window.draw(m_groundSprites[i]);
	}
}

void Ground::update(sf::Time t_deltaTime)
{
	
	for (int i = 0; i < MAX_GROUND_SPRITES; i++)
	{
		if (m_positions[i].x < 0 - SCREEN_WIDTH)
		{
			m_positions[i].x = SCREEN_WIDTH - 7;
		}
		m_positions[i].x += m_velocities[i].x;
		m_groundSprites[i].setPosition(m_positions[i]);
	}
}
