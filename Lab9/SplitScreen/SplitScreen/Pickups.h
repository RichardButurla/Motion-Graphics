#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "GameEnums.h"

enum class ItemTypes
{
	Coin,
	BlueShell,
	SpeedBoost,
	Armour,
	Magnet,
	CoinDoubler
};

class Pickups : public sf::Sprite
{
public:
	Pickups(sf::Texture const& t_texture,ItemTypes itemType,sf::Vector2f * playerPositions);
	ItemTypes getItemType() const { return type; }
	void pickUp(PlayerID t_playerID) { if (!pickedUp) { pickedUp = true; playerID = t_playerID; } }
	void dropPickup() { pickedUp = false; }

	void update();
private:
	ItemTypes type = ItemTypes::Coin;
	bool pickedUp = false;
	sf::Vector2f * pickUpPositions;
	PlayerID playerID;
};

