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
	sf::RenderWindow window(sf::VideoMode(1200, 800), "First Graphics in C++");

	std::srand(static_cast<unsigned int>(time(nullptr)));
	sf::Font font;

	sf::Texture dinotexture;
	if (!dinotexture.loadFromFile("ASSETS/IMAGES/dino-spritesheet.png"))
	{
		std::cout << "failed to load cloud Texture";
	}
	Dino dinosaur(dinotexture);

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

			//Draw here
			dinosaur.render(window);



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

	if (m_position.y > t_window.getSize().y - 200)
	{
		m_velocity.y = 0;
		m_position.y = t_window.getSize().y - 199;
	}
}
