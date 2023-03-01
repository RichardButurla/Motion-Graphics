#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();

	void init(sf::Texture const& t_texture);
	void render(sf::RenderWindow& t_window);
	void update();

	sf::Vector2f getPosition() const { return m_position; }
	sf::FloatRect getGlobalBounds() const { return m_playerSprite.getGlobalBounds(); }

	void movePlayer(sf::Vector2f moveVector) { m_position += moveVector; }

	void setHoldingItem(bool t_bool) { holdingItem = t_bool; }
	void setItemHeldID(int t_itemId) { m_itemHeldId = t_itemId; holdingItem = true; }
	void collectCoin() { m_coinsCollected += 1; }

	int getItemHeldID() { return m_itemHeldId; }
	bool isHoldingItem() { return holdingItem; }
	int getNumberOfCoinsCollected() { return m_coinsCollected; }

private:
	sf::Sprite m_playerSprite;
	sf::Texture m_playerTexture;

	sf::Vector2f m_position{ 200,400 };
	float speed{1.5};

	int m_itemHeldId{ -1 };
	bool holdingItem = false;

	int m_coinsCollected{ 0 };
	bool speedBoostActivated{false};
	bool coinDoublerActivated{ false };
	bool armourActivated{ false };
	bool magnetActivated{ false };

};

