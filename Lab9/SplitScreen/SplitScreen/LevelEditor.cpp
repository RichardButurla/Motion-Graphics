#include "LevelEditor.h"
#include <iostream>

LevelEditor::LevelEditor(sf::RenderWindow & t_window, std::vector<Tile>& m_levelEditorTiles, std::vector<Tile>& m_gameTiles) :
	m_levelWindow(t_window),
	m_levelEditorTiles(m_levelEditorTiles),
	m_gameTiles(m_gameTiles)
{

	m_currentView = m_levelWindow.getView();
}

void LevelEditor::init(sf::Texture & t_tileTexture, sf::Font& t_font)
{
	m_tileTexture = t_tileTexture;

	setupSprite();
	setupGrid();
}

void LevelEditor::update()
{
	processKeys();
	processMouse();

	
	//hudTilePosition.y += m_levelWindow.getViewport(m_currentView).height;
	for (int i = 0; i < MAX_TILE_TYPES; i++)
	{
		TileType tileType = m_hudTile[i].getTileType();
		sf::Vector2f hudTilePosition = m_currentView.getCenter();
		switch (tileType)
		{
		case TileType::PlayerSpawn:
			hudTilePosition.x += m_hudTile[i].getGlobalBounds().width * 3;
			break;
		case TileType::Wall:
			hudTilePosition.x += m_hudTile[i].getGlobalBounds().width;
			break;
		case TileType::Floor:
			hudTilePosition.x += m_hudTile[i].getGlobalBounds().width * 2;
			break;
		case TileType::PowerUpSpawn:
			hudTilePosition.x += m_hudTile[i].getGlobalBounds().width * 4;
			break;
		case TileType::CoinSpawn:
			hudTilePosition.x += m_hudTile[i].getGlobalBounds().width * 5;
			break;
		default:
			break;
		}
		m_hudTile[i].setPosition(hudTilePosition);
	}

	checkHighlightingBlock();
}

void LevelEditor::render(sf::RenderWindow& t_window)
{
	m_levelWindow.setView(m_currentView);

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			m_gridTile.setPosition(m_gridPositions[row * MAX_ROWS + col]);
			t_window.draw(m_gridTile);
		}
	}
	
	for (int i = 0; i < m_levelEditorTiles.size(); i++)
	{
		t_window.draw(m_levelEditorTiles[i]);
	}

	for (int i = 0; i < MAX_TILE_TYPES; i++)
	{
		t_window.draw(m_hudTile[i]);
	}

	t_window.draw(m_highlightTile);
}

void LevelEditor::processKeys()
{
	float offset = 10;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_currentView.move({ 0,-offset });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_currentView.move({ -offset,0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_currentView.move({ 0,offset });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_currentView.move({ offset,0 });
	}
}

void LevelEditor::processMousePress()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_levelWindow);
	sf::Vector2f worldPos = m_levelWindow.mapPixelToCoords(pixelPos);

	m_mousePressPos.x = worldPos.x;
	m_mousePressPos.y = worldPos.y;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_currentEditingAction = EditAction::Placing;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_currentEditingAction = EditAction::Removing;
	}
}

void LevelEditor::processMouseRelease()
{	
	if (m_currentEditingAction == EditAction::Placing)
	{
		checkPlacingBlock();
	}
	if (m_currentEditingAction == EditAction::Removing)
	{
		checkRemovingBlock();
	}
}

void LevelEditor::processMouseMove()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_levelWindow);
	sf::Vector2f worldPos = m_levelWindow.mapPixelToCoords(pixelPos);

	m_mouseMovePos.x = worldPos.x;
	m_mouseMovePos.y = worldPos.y;
}

void LevelEditor::processMouseScroll(sf::Event t_event)
{
	if(t_event.type == sf::Event::MouseWheelMoved) // Zomm in or out if the mouse wheel moves
	{
		m_currentView.zoom(1.f - t_event.mouseWheel.delta * 0.1f);
		switch (t_event.mouseWheel.delta)
		{
		case -1:
			currentZoom += (1.f + t_event.mouseWheel.delta * 0.1f) * 0.1f;
			break;

		case 1:
			currentZoom += (1.f + t_event.mouseWheel.delta * 0.1f) * 0.1f;
			break;
		default:
			break;
		}
		std::cout << "current Zoom: " << currentZoom;
		for (int i = 0; i < MAX_TILE_TYPES; i++)
		{
			m_hudTile[i].setScale(currentZoom, currentZoom);
		}
	}
}

void LevelEditor::processMouse()
{
	processMouseMove();
	processMousePress();	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
		sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		bool hudTileSelected = false;

		for (int i = 0; i < MAX_TILE_TYPES; i++)
		{
			sf::Vector2f tilePos = m_hudTile[i].getPosition();
			if (m_mousePressPos.x > tilePos.x && m_mousePressPos.x < tilePos.x + tileWidth * currentZoom &&
				m_mousePressPos.y > tilePos.y && m_mousePressPos.y < tilePos.y + tileHeight * currentZoom)
			{
				std::cout << "tile selected";
				m_highlightTile.setTileType(static_cast<TileType>(i));
				hudTileSelected = true;
			}

		}
		if(!hudTileSelected)
		processMouseRelease();
	}
}

void LevelEditor::setupSprite()
{
	sf::Vector2u textureSize;
	textureSize.x = m_tileTexture.getSize().x / 7;
	textureSize.y = m_tileTexture.getSize().y / 3;
	tileWidth = textureSize.x;
	tileHeight = textureSize.y;

	m_gridTile.setFillColor(sf::Color::Black);
	m_gridTile.setSize({ tileWidth, tileHeight });
	m_gridTile.setOutlineColor(sf::Color::White);
	m_gridTile.setOutlineThickness(1.f);

	m_tile.setColor(sf::Color::Red);
	m_tile.setTexture(m_tileTexture);
	m_tile.setTextureRect(sf::IntRect(0, 0, tileWidth, tileHeight));

	m_highlightTile.setTexture(m_tileTexture);
	m_highlightTile.setTextureRect(sf::IntRect(0, 0, tileWidth, tileHeight));

	TileType tileType;

	for (int i = 0; i < MAX_TILE_TYPES; i++)
	{
		tileType = static_cast<TileType>(i);
		m_hudTile[i].setTexture(m_tileTexture);
		m_hudTile[i].setTileType(tileType);
	}

	m_highlightTile = m_hudTile[static_cast<int>(TileType::Wall)];
	//Set Positiuons?

}

void LevelEditor::setupGrid()
{	
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			m_gridPositions[row * MAX_ROWS + col] = sf::Vector2f{ col * tileWidth, row * tileHeight + m_hudYOffset };
		}
	}
	m_levelEditorTiles.reserve(MAX_COLLUMS * MAX_ROWS);
	m_gameTiles.reserve(MAX_COLLUMS * MAX_ROWS);
}

void LevelEditor::setupFontAndText()
{
	std::string hudTexts[4]
	{
		"Wall Block: ",
		"Floor Block: ",
		"Player Spawn Block: ",
		"Power Up Spawn Block: "
	};

	for (int i = 0; i < 4; i++)
	{
		m_hudText.setFont(m_font);
		m_hudText.setString(hudTexts[i]);
		m_hudText.setStyle(sf::Text::Italic | sf::Text::Bold);
		m_hudText.setPosition(70.0f, 20.0f);
		m_hudText.setCharacterSize(30U);
		m_hudText.setOutlineColor(sf::Color::Black);
		m_hudText.setFillColor(sf::Color::White);
		m_hudText.setOutlineThickness(3.0f);
	}
}

void LevelEditor::checkHighlightingBlock()
{
	sf::Vector2f tilePos;
	int index = 0;
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			index = row * MAX_ROWS + col;
			tilePos = m_gridPositions[index];
			if (m_mouseMovePos.x > tilePos.x && m_mouseMovePos.x < tilePos.x + tileWidth &&
				m_mouseMovePos.y > tilePos.y && m_mouseMovePos.y < tilePos.y + tileHeight)
			{
				m_highlightTile.setPosition(tilePos);
			}
		}
	}

	index = 0;
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			index = row * MAX_ROWS + col;
			tilePos = m_gridPositions[index];
			if (m_mouseMovePos.x > tilePos.x && m_mouseMovePos.x < tilePos.x + tileWidth &&
				m_mouseMovePos.y > tilePos.y && m_mouseMovePos.y < tilePos.y + tileHeight)
			{
				m_highlightTile.setPosition(tilePos);
			}
		}
	}
}

void LevelEditor::checkPlacingBlock()
{
	sf::Vector2f tilePos;
	bool freeSpace = true;
	int index = 0;

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			index = row * MAX_ROWS + col;
			tilePos = m_gridPositions[index];
			if (m_mousePressPos.x > tilePos.x && m_mousePressPos.x < tilePos.x + tileWidth &&
				m_mousePressPos.y > tilePos.y && m_mousePressPos.y < tilePos.y + tileHeight)
			{
				sf::Vector2f placedTilePos;
				if (m_levelEditorTiles.size() == 0) //first block to be placed
				{
					Tile newTile = m_highlightTile;
					newTile.setPosition(tilePos);
					m_levelEditorTiles.push_back(newTile);
					m_gameTiles.push_back(newTile);
					tileCount++;
				}
				else
				{
					for (int i = 0; i < m_levelEditorTiles.size(); i++)
					{
						placedTilePos = m_levelEditorTiles[i].getPosition();
						if (tilePos.x == placedTilePos.x &&
							tilePos.y == placedTilePos.y)
						{
							freeSpace = false;
						}
					}
					if (freeSpace == true)
					{
						Tile newTile = m_highlightTile;
						newTile.setPosition(tilePos);
						m_levelEditorTiles.push_back(newTile);
						m_gameTiles.push_back(newTile);
						tileCount++;
					}
				}

			}
		}
	}
}

void LevelEditor::checkRemovingBlock()
{
	sf::Vector2f tilePos;
	bool occupied = false;
	int tileIndex = -1;
	int index = 0;

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			index = row * MAX_ROWS + col;
			tilePos = m_gridPositions[index];
			if (m_mousePressPos.x > tilePos.x && m_mousePressPos.x < tilePos.x + tileWidth &&
				m_mousePressPos.y > tilePos.y && m_mousePressPos.y < tilePos.y + tileHeight)
			{
				sf::Vector2f placedTilePos;
				for (int i = 0; i < m_levelEditorTiles.size(); i++)
				{
					placedTilePos = m_levelEditorTiles[i].getPosition();
					if (tilePos.x == placedTilePos.x &&
						tilePos.y == placedTilePos.y)
					{
						occupied = true;
						tileIndex = i;
					}
				}
				if (occupied == true)
				{
					m_levelEditorTiles.erase(m_levelEditorTiles.begin() + tileIndex);
					m_gameTiles.erase(m_gameTiles.begin() + tileIndex);
					tileCount--;
				}


			}
		}
	}
}

void Tile::setTileType(TileType t_type)
{
	sf::Vector2u textureSize = this->getTexture()->getSize();
	textureSize.x = textureSize.x / 7;
	textureSize.y = textureSize.y / 3;

	switch (t_type)
	{
	case TileType::PlayerSpawn:
		m_tileType = TileType::PlayerSpawn;
		this->setTextureRect(sf::IntRect(textureSize.x * 1, 0, textureSize.x, textureSize.y));
		break;
	case TileType::Wall:
		m_tileType = TileType::Wall;
		this->setTextureRect(sf::IntRect(textureSize.x * 5, textureSize.y * 2, textureSize.x, textureSize.y));
		break;
	case TileType::Floor:
		m_tileType = TileType::Floor;

		this->setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
		break;
	case TileType::PowerUpSpawn:
		m_tileType = TileType::PowerUpSpawn;
		this->setTextureRect(sf::IntRect(textureSize.x * 2,0, textureSize.x, textureSize.y));
		break;
	case TileType::CoinSpawn:
		m_tileType = TileType::CoinSpawn;
		this->setTextureRect(sf::IntRect(textureSize.x * 3, 0, textureSize.x, textureSize.y));
		break;
	default:
		break;
	}

	m_tileType = t_type;
}
