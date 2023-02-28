#pragma once
#include <SFML/Graphics.hpp>
class Player
{
public:
	Player();

	void init(sf::Texture const& t_texture);
	void render(sf::RenderWindow& t_window);
	void update();

	sf::Vector2f getPosition() const { return m_position; }

	void movePlayer(sf::Vector2f moveVector) { m_position += moveVector; }


private:
	sf::Sprite m_playerSprite;
	sf::Texture m_playerTexture;

	sf::Vector2f m_position{400,400};
	float speed = 1.5;

};

