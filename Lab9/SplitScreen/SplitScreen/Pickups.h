#pragma once
#include <SFML/Graphics.hpp>

enum class ItemTypes
{
	Coin,
	Gun,
	SpeedBoost,
	Armour,
	Magnet,
	CoinDoubler
};

class Pickups : public sf::Sprite
{
public:
	Pickups(sf::Texture const& t_texture,ItemTypes itemType);
	ItemTypes getItemType() const { return type; }
private:
	ItemTypes type = ItemTypes::Coin;
};

