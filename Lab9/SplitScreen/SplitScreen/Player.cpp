#include "Player.h"

Player::Player()
{
}

void Player::init(sf::Texture const& t_texture)
{
	m_playerTexture = t_texture;
	m_playerSprite.setTexture(m_playerTexture);
	sf::Vector2u textureSize = m_playerTexture.getSize();
	m_playerSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	m_playerSprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
	m_playerSprite.setScale(0.2f, 0.2f);
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_playerSprite);
}

void Player::update()
{	
	m_playerSprite.setPosition(m_position);
}
