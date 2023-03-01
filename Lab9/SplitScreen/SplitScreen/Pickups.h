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

typedef int ItemID;

inline ItemID newItemID() {
	static ItemID id = 0;
	return id++;
}

class Pickups : public sf::Sprite
{
public:
	Pickups();

	void update();
	void init(sf::Texture const& t_texture, ItemTypes itemType, sf::Vector2f* playerPositions);

	ItemTypes getItemType() const { return m_itemType; }
	ItemID getItemId() const { return itemId; }
	bool isPickedUp() { return pickedUp; }

	void setItemType(ItemTypes t_type) { m_itemType = t_type; }

	void pickUp(PlayerID t_playerID) {  pickedUp = true; playerID = t_playerID; }
	void dropPickup() { pickedUp = false; }

	
private:
	ItemTypes m_itemType = ItemTypes::Coin;
	bool pickedUp = false;
	sf::Vector2f * pickUpPositions;
	PlayerID playerID;
	ItemID itemId;
};



