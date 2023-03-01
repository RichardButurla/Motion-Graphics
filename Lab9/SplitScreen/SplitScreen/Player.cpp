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
	checkPowerup();

	m_playerSprite.setPosition(m_position);
}

void Player::checkPowerup()
{
	if (timeSincePowerUpUsed.getElapsedTime() > powerUpTime)
	{
		speedBoostActivated = false;
		m_currentSpeed = normalSpeed;
		coinDoublerActivated = false;
		armourActivated = false;
		magnetActivated = false;
	}

	if (speedBoostActivated)
	{
		m_currentSpeed = boostedSpeed;
	}
}

void Player::usePowerUp(ItemTypes t_type)
{
	switch (t_type)
	{
		//Coin and Blue Shell are not PoweUps
	case ItemTypes::SpeedBoost:
		speedBoostActivated = true;
		timeSincePowerUpUsed.restart();
		break;
	case ItemTypes::Armour:
		armourActivated = true;
		timeSincePowerUpUsed.restart();
		break;
	case ItemTypes::Magnet:
		magnetActivated = true;
		timeSincePowerUpUsed.restart();
		break;
	case ItemTypes::CoinDoubler:
		coinDoublerActivated = true;
		timeSincePowerUpUsed.restart();
		break;
	default:
		break;
	}
}
