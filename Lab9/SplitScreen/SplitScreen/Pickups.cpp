#include "Pickups.h"
#include "Vector2.h"
#include <iostream>

void Pickups::update(sf::Time& t_deltaTime)
{
	if (pickupUsed)
	{
		if (m_itemType == ItemTypes::BlueShell)
		{
			//blueShellTrack();
			m_position.x += (m_velocity.x * 90) * t_deltaTime.asSeconds();
			m_position.y += (m_velocity.y * 90) * t_deltaTime.asSeconds();
		}
		this->setPosition(m_position);
	}
	else if (pickedUp )
	{
		m_position = (pickUpPositions[static_cast<int>(playerID)]);
		this->setPosition(m_position);
	}
	else
	{
		this->setPosition(m_position);
		if (m_itemType == ItemTypes::Coin)
		{
			animateCoin();
		}
	}
}

void Pickups::init(sf::Texture const& t_texture, ItemTypes itemType, sf::Vector2f* playerPositions)
{
	m_texture = t_texture;
	itemId = newItemID();
	m_textureSize = t_texture.getSize();
	m_itemType = itemType;
	pickUpPositions = playerPositions;
	sf::Vector2f scale{ 0.05,0.05 };

	switch (itemType)
	{
	case ItemTypes::Coin:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x / 6, m_textureSize.y / 2));
		m_textureSize.x /= 6;
		m_textureSize.y /= 2;
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setScale(scale);
		m_position = sf::Vector2f{ 600,200 };
		this->setPosition(m_position);
		break;
	case ItemTypes::BlueShell:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x / 5, m_textureSize.y));
		m_textureSize.x /= 5;
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setScale(scale);
		m_textureSize.x *= scale.x;
		m_textureSize.y *= scale.y;
		m_position = { 600,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::SpeedBoost:
		this->setTexture(t_texture);
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x, m_textureSize.y));
		scale = { 0.04,0.04 };
		this->setScale(scale);
		m_textureSize.x *= scale.x;
		m_textureSize.y *= scale.y;
		m_position = { 800,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::Armour:
		this->setTexture(t_texture);
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x, m_textureSize.y));
		scale = { 0.03,0.03 };
		this->setScale(scale);
		m_textureSize.x *= scale.x;
		m_textureSize.y *= scale.y;
		m_position = { 1000,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::Magnet:
		this->setTexture(t_texture);
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x, m_textureSize.y));
		scale = { 0.02,0.02 };
		this->setScale(scale);
		m_textureSize.x *= scale.x;
		m_textureSize.y *= scale.y;
		m_position = { 1200,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::CoinDoubler:
		this->setTexture(t_texture);
		this->setOrigin(m_textureSize.x / 2.f, m_textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, m_textureSize.x, m_textureSize.y));
		this->setScale(scale);
		m_textureSize.x *= scale.x;
		m_textureSize.y *= scale.y;
		m_position = { 1400,400 };
		this->setPosition(m_position);
		break;
	default:
		break;
	}

}

void Pickups::blueShellTrack()
{
	int enemyPlayerId;
	if (static_cast<int>(playerID) == 0)
		enemyPlayerId = 1;
	else
		enemyPlayerId = 0;

	sf::Vector2f enemyPos = pickUpPositions[enemyPlayerId];
	setTrackingVelocity(enemyPos);
}

/// <summary>
/// used for when player is magnetising
/// </summary>
void Pickups::coinMagnetTrack(sf::Vector2f t_target)
{	
	sf::Vector2f directionVector = t_target - m_position;
	float magnetRadius = 40;
	if (vectorLength(directionVector) < magnetRadius)
	{
		sf::Vector2f unitDir = vectorUnitVector(directionVector);
		m_position.x += unitDir.x * 5;
		m_position.y += unitDir.y * 5;
	}
}

void Pickups::setTrackingVelocity(sf::Vector2f target)
{
	sf::Vector2f directionVector = target - m_position;
	sf::Vector2f unitDir = vectorUnitVector(directionVector);
	m_velocity = unitDir;
}

void Pickups::animateCoin()
{
	if (m_timeSinceFrameChange.getElapsedTime() > m_coinFrameTime)
	{
		m_timeSinceFrameChange.restart();
		if (m_currentCoinFrames < MAX_COIN_FRAMES)
		{
			m_currentCoinFrames++;
		}
		if (m_currentCoinFrames >= MAX_COIN_FRAMES)
		{
			m_currentCoinFrames = 0;
		}
		
		
	}

	std::cout << "Frame: " << m_currentCoinFrames;
	this->setTextureRect(sf::IntRect(m_textureSize.x * m_currentCoinFrames, 0,
		m_textureSize.x, m_textureSize.y));

	/*this->setTextureRect(sf::IntRect(m_textureSize.x * m_currentCoinFrames, m_textureSize.y * m_currentCoinFrames,
		m_textureSize.x + m_textureSize.x * m_currentCoinFrames, m_textureSize.y + m_textureSize.y * m_currentCoinFrames));*/
}
