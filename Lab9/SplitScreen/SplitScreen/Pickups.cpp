#include "Pickups.h"

Pickups::Pickups(sf::Texture const& t_texture, ItemTypes itemType)
{
	sf::Vector2u textureSize = t_texture.getSize();

	switch (itemType)
	{
	case ItemTypes::Coin:
		this->setTexture(t_texture);
		this->setTextureRect(sf::IntRect(0, 0, textureSize.x / 6, textureSize.y / 2));
		this->setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
		this->setScale(0.5f, 0.5f);
		this->setPosition(400, 400);
		break;
	case ItemTypes::Gun:

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
