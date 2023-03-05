#include "Pickups.h"
#include "Vector2.h"
#include <iostream>

void Pickups::update()
{
	if (pickupUsed)
	{
		if (m_itemType == ItemTypes::BlueShell)
		{
			//blueShellTrack(); if we want tracking 
			m_position.x += m_velocity.x * 5;
			m_position.y += m_velocity.y * 5;
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
	}
}

void Pickups::init(sf::Texture const& t_texture, ItemTypes itemType, sf::Vector2f* playerPositions)
{
	itemId = newItemID();
	sf::Vector2u textureSize = t_texture.getSize();
	m_itemType = itemType;
	pickUpPositions = playerPositions;

	switch (itemType)
	{
	case ItemTypes::Coin:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x / 6, textureSize.y / 2));
		textureSize.x /= 6;
		textureSize.y /= 2;
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setScale(0.05f, 0.05f);
		m_position = sf::Vector2f{ 600,200 };
		this->setPosition(m_position);
		break;
	case ItemTypes::BlueShell:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x / 5, textureSize.y));
		textureSize.x /= 5;
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setScale(0.5f, 0.5f);
		m_position = { 600,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::SpeedBoost:
		this->setTexture(t_texture);
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
		this->setScale(0.04f, 0.04f);
		m_position = { 800,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::Armour:
		this->setTexture(t_texture);
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
		this->setScale(0.03f, 0.03f);
		m_position = { 1000,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::Magnet:
		this->setTexture(t_texture);
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
		this->setScale(0.02f, 0.02f);
		m_position = { 1200,400 };
		this->setPosition(m_position);
		break;
	case ItemTypes::CoinDoubler:
		this->setTexture(t_texture);
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
		this->setScale(0.05f, 0.05f);
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
