#pragma once
#include <SFML/Graphics.hpp>
#include "GameEnums.h"
#include <functional>

class Player
{
public:
	Player();

	void init(sf::Texture const& t_texture);
	void render(sf::RenderWindow& t_window);
	void update(std::vector <std::function<void(sf::Vector2f)> >& t_magnetiseCoinFunctions);

	void checkPowerup();

	sf::Vector2f getPosition() const { return m_position; }
	sf::FloatRect getGlobalBounds() const { return m_playerSprite.getGlobalBounds(); }

	void movePlayer(sf::Vector2f moveVector) { m_position += moveVector * m_currentSpeed; }

	void setHoldingItem(bool t_bool) { holdingItem = t_bool; }
	void setItemHeldID(int t_itemId) { m_itemHeldId = t_itemId; holdingItem = true; }
	void usePowerUp(ItemTypes t_type);
	void collectCoin() { if (coinDoublerActivated) { m_coinsCollected += 2; } else { m_coinsCollected += 1; } }

	int getItemHeldID() { return m_itemHeldId; }
	bool isHoldingItem() { return holdingItem; }
	int getNumberOfCoinsCollected() { return m_coinsCollected; }
	bool isMagnetising() { return magnetActivated; }

private:
	sf::Sprite m_playerSprite;
	sf::Texture m_playerTexture;

	sf::Vector2f m_position{ 200,400 };
	float m_currentSpeed{3};
	float boostedSpeed{ 6 };
	float normalSpeed{ 3 };
	int m_itemHeldId{ -1 };
	bool holdingItem = false;

	int m_coinsCollected{ 0 };

	sf::Time powerUpTime = sf::seconds(5);
	sf::Clock timeSincePowerUpUsed;
	bool speedBoostActivated{false};
	bool coinDoublerActivated{ false };
	bool armourActivated{ false };
	bool magnetActivated{ false };

};

