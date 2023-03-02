#include "LevelEditor.h"

LevelEditor::LevelEditor(sf::RenderWindow & t_window) : m_levelWindow(t_window)
{

	m_currentView = m_levelWindow.getView();
}

void LevelEditor::init(std::vector<Tile>& t_gameLevel, std::vector<sf::Texture> t_tileTextures)
{
	m_placedTiles = t_gameLevel;
	m_tileTextures = t_tileTextures;
	sf::Vector2u textureSize = t_tileTextures[0].getSize(); //tiles will be square so we can just grab the first texture
	tileWidth = textureSize.x;
	tileHeight = textureSize.y;

	setupSprite();
	setupGrid();
}

void LevelEditor::update()
{
	processKeys();
	processMouse();

	checkHighlightingBlock();
}

void LevelEditor::render(sf::RenderWindow& t_window)
{
	m_levelWindow.setView(m_currentView);

	//Draw Grid for placement
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			m_gridTile.setPosition(m_gridPositions[row][col]);
			t_window.draw(m_gridTile);
		}
	}

	for (int i = 0; i < m_placedTiles.size(); i++)
	{
		t_window.draw(m_placedTiles[i]);
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
	}
}

void LevelEditor::processMouse()
{
	processMouseMove();
	processMousePress();	
	if (sf::Event::MouseButtonReleased)
	{
		//processMouseRelease();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
		sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		processMouseRelease();
	}
}

void LevelEditor::setupSprite()
{

	m_gridTile.setFillColor(sf::Color::Black);
	m_gridTile.setSize({ tileWidth, tileHeight });
	m_gridTile.setOutlineColor(sf::Color::White);
	m_gridTile.setOutlineThickness(10.f);

	m_tile.setColor(sf::Color::Red);
	m_tile.setTexture(m_tileTextures[static_cast<int>(TileType::Wall)]);
	m_tile.setTextureRect(sf::IntRect(0, 0, tileWidth, tileHeight));

	m_highlightTile.setTexture(m_tileTextures[static_cast<int>(TileType::Wall)]);
	m_highlightTile.setTextureRect(sf::IntRect(0, 0, tileWidth, tileHeight));

	//Set Positiuons?

}

void LevelEditor::setupGrid()
{
	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			m_gridPositions[row][col] = sf::Vector2f{ col * tileWidth, row * tileHeight + m_hudYOffset };
		}
	}
	m_placedTiles.reserve(MAX_COLLUMS * MAX_ROWS);
}

void LevelEditor::checkHighlightingBlock()
{
	sf::Vector2f tilePos;

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			tilePos = m_gridPositions[row][col];
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

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			tilePos = m_gridPositions[row][col];
			if (m_mousePressPos.x > tilePos.x && m_mousePressPos.x < tilePos.x + tileWidth &&
				m_mousePressPos.y > tilePos.y && m_mousePressPos.y < tilePos.y + tileHeight)
			{
				sf::Vector2f placedTilePos;
				if (m_placedTiles.size() == 0) //first block to be placed
				{
					Tile newTile = m_highlightTile;
					newTile.setPosition(tilePos);
					m_placedTiles.push_back(newTile);
					tileCount++;
				}
				else
				{
					for (int i = 0; i < m_placedTiles.size(); i++)
					{
						placedTilePos = m_placedTiles[i].getPosition();
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
						m_placedTiles.push_back(newTile);
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

	for (int row = 0; row < MAX_ROWS; row++)
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			tilePos = m_gridPositions[row][col];
			if (m_mousePressPos.x > tilePos.x && m_mousePressPos.x < tilePos.x + tileWidth &&
				m_mousePressPos.y > tilePos.y && m_mousePressPos.y < tilePos.y + tileHeight)
			{
				sf::Vector2f placedTilePos;
				for (int i = 0; i < m_placedTiles.size(); i++)
				{
					placedTilePos = m_placedTiles[i].getPosition();
					if (tilePos.x == placedTilePos.x &&
						tilePos.y == placedTilePos.y)
					{
						occupied = true;
						tileIndex = i;
					}
				}
				if (occupied == true)
				{
					m_placedTiles.erase(m_placedTiles.begin() + tileIndex);
					tileCount--;
				}


			}
		}
	}
}

void Tile::setTileType(TileType t_type)
{
	switch (t_type)
	{
	case TileType::PlayerSpawn:
		m_tileType = TileType::PlayerSpawn;

		break;
	case TileType::Wall:
		m_tileType = TileType::Wall;

		break;
	case TileType::Floor:
		m_tileType = TileType::Floor;

		break;
	case TileType::PowerUpSpawn:
		m_tileType = TileType::PowerUpSpawn;

		break;
	default:
		break;
	}
}
