#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <list>
#include <map>

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
	PowerUpSpawn,
	CoinSpawn
};

class Tile : public sf::Sprite
{
public:
	TileType getTileType() { return m_tileType; }
	void setTileType(TileType t_type);

private:
	TileType m_tileType = TileType::Wall;
};

class LevelEditor
{
public:
	LevelEditor(sf::RenderWindow& t_window, std::vector<Tile>& m_levelEditorTiles, std::shared_ptr<std::map<TileType, std::list<Tile>>> t_gameTiles);
	void init(sf::Texture & t_tileTexture,sf::Font & t_font);
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
	void setupFontAndText();

	void checkHighlightingBlock();
	void checkPlacingBlock();
	void checkRemovingBlock();

private:
	sf::Text m_hudText; // text used for message on screen
	sf::Font m_font;

	Tile m_tile;
	sf::RectangleShape m_gridTile;
	Tile m_highlightTile;
	Tile m_hudTile[MAX_TILE_TYPES];
	float tileWidth;
	float tileHeight;
	float m_hudYOffset = 90;
	float m_hudXOffset = 70;
	int tileCount = 0;
	sf::Vector2f m_gridPositions[MAX_ROWS * MAX_COLLUMS];
	std::vector<Tile> & m_levelEditorTiles;
	std::shared_ptr<std::map<TileType,std::list<Tile>>> m_gameTiles;
	sf::Texture m_tileTexture;

	//
	float currentZoom = 1;
	sf::View m_currentView;
	sf::RenderWindow& m_levelWindow;

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;
	EditAction m_currentEditingAction = EditAction::Placing;

};

