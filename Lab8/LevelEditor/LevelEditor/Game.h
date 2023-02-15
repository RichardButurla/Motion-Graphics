#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

enum class EditAction
{
	Placing,
	Removing
};

enum class TileType
{
	Base,
	Hazard,
	Jump,
	Finish
};

class Tile : public sf::RectangleShape
{
public:
	TileType getTileType() { return m_tileType; }
	void setTileType(TileType t_type);

private:
	TileType m_tileType = TileType::Base;
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;

	static const int MAX_COLLUMS = 25;
	static const int MAX_ROWS = 17;

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMousePress(sf::Event t_event);
	void processMouseRelease(sf::Event t_event);
	void processMouseMove(sf::Event t_event);

	void update(sf::Time t_deltaTime);
	void render();
	
	void moveTiles();
	void updatePlayer();
	void checkCollisions();

	void setupFontAndText();
	void setupSprite();
	void setupGrid();

	void checkHighlightingBlock();
	void checkPlacingBlock();
	void checkRemovingBlock();

	sf::RenderWindow m_window; // main SFML window
	sf::View baseView;
	sf::View movingView;
	sf::Font m_ArialBlackfont; // font used by message

	sf::Text m_hudText; // text used for message on screen
	Tile m_hudTile;
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	float m_hudYOffset = 90;
	float m_hudXOffset = 70;
	float gravity = 0.6;

	//Player
	sf::RectangleShape m_playerShape;
	sf::Vector2f m_playerSize{ 30.f,30.f };
	sf::Vector2f m_playerPos{ m_hudYOffset,SCREEN_HEIGHT / 2.f  };
	float playerYVelocity = 0;
	float playerJumpVelocity = -13.f;
	float playerGravity = gravity;

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;
	EditAction m_currentEditingAction = EditAction::Placing;

	//Tiles
	Tile m_tile;
	Tile m_highlightTile;
	float tileWidth = 70;
	float tileHeight = 30;
	sf::Vector2f m_gridPositions[MAX_ROWS][MAX_COLLUMS];
	std::vector<Tile>m_placedTiles;
	std::vector<Tile>m_gameTiles;
	int tileCount = 0;
	float tileSpeed = -3;
	float jumpTileForce = -20;

	bool m_editingLevel{ true };
	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

