#include "Pickups.h"

Pickups::Pickups(sf::Texture const& t_texture, ItemTypes itemType, sf::Vector2f * playerPositions) : pickUpPositions(playerPositions)
{
	sf::Vector2u textureSize = t_texture.getSize();
	type = itemType;

	switch (itemType)
	{
	case ItemTypes::Coin:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x / 6, textureSize.y / 2));
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setScale(0.2f, 0.2f);
		this->setPosition(400, 400);
		break;
	case ItemTypes::BlueShell:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x / 5, textureSize.y));
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setScale(2.f, 2.f);
		this->setPosition(600, 400);
		break;
	case ItemTypes::SpeedBoost:

		break;
	case ItemTypes::Armour:

		break;
	case ItemTypes::Magnet:

		break;
	case ItemTypes::CoinDoubler:

		break;
	default:
		break;
	}
}

void Pickups::update()
{
	if (pickedUp)
	{
		this->setPosition(pickUpPositions[static_cast<int>(playerID)]);
	}
}
