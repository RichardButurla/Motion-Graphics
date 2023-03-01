#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "GameEnums.h"
#include <memory>
#include <iostream>


typedef int ItemID;

inline ItemID newItemID() {
	static ItemID id = 0;
	return id++;
}

class Pickups : public sf::Sprite
{
public:
	Pickups() = default;

	void update();
	void init(sf::Texture const& t_texture, ItemTypes itemType, sf::Vector2f* playerPositions);

	ItemTypes getItemType() const { return m_itemType; }
	ItemID getItemId() const { return itemId; }
	bool isPickedUp() { return pickedUp; }

	void setItemType(ItemTypes t_type) { m_itemType = t_type; }
	void setPositionVector(sf::Vector2f t_pos) { m_position = t_pos; }

	void pickUp(PlayerID t_playerID) {  pickedUp = true; playerID = t_playerID; }
	void useItem() { pickupUsed = true; if (m_itemType == ItemTypes::BlueShell) { blueShellTrack(); } }
	void dropPickup() { pickedUp = false; m_position.y += 100; this->setPosition(m_position); }

	void blueShellTrack();
	void coinMagnetTrack(sf::Vector2f t_target);
	void setTrackingVelocity(sf::Vector2f target);

	
private:
	ItemTypes m_itemType = ItemTypes::Coin;
	bool pickedUp = false;
	bool pickupUsed = false;
	sf::Vector2f m_position{400,400};
	sf::Vector2f m_velocity;
	std::shared_ptr<sf::Vector2f> pickUpPositions;
	PlayerID playerID;
	ItemID itemId;
};



