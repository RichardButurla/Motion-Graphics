#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

enum class EditAction
{
	Placing,
	Removing
};

enum class TileType
{
	PlayerSpawn,
	Wall,
	Floor,
	PowerUpSpawn
};

class Tile : public sf::Sprite
{
public:
	TileType getTileType() { return m_tileType; }
	void setTileType(TileType t_type);

private:
	TileType m_tileType = TileType::PlayerSpawn;
};

class LevelEditor
{
public:
	LevelEditor(sf::RenderWindow& t_window);
	void init(std::vector<Tile>& t_gameLevel, std::vector<sf::Texture> t_tileTextures);
	void update();
	void render(sf::RenderWindow& t_window);

	void processKeys();
	void processMousePress();
	void processMouseRelease();
	void processMouseMove();
	void processMouseScroll(sf::Event t_event);
	void processMouse();

	void setupSprite();
	void setupGrid();

	void checkHighlightingBlock();
	void checkPlacingBlock();
	void checkRemovingBlock();

private:
	Tile m_tile;
	Tile m_highlightTile;
	float tileWidth;
	float tileHeight;
	float m_hudYOffset = 90;
	float m_hudXOffset = 70;
	int tileCount = 0;
	sf::Vector2f m_gridPositions[MAX_ROWS][MAX_COLLUMS];
	std::vector<Tile>m_placedTiles;
	std::vector<sf::Texture> m_tileTextures;

	//
	float currentZoom = 1;
	sf::View m_currentView;
	sf::RenderWindow& m_levelWindow;

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;
	EditAction m_currentEditingAction = EditAction::Placing;

};

