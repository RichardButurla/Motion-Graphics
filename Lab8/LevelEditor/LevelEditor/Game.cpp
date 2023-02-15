#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32U }, "Level Editor" },
	m_exitGame{false} //when true game will exit
{
	baseView = m_window.getView();
	movingView = m_window.getView();
	m_window.setView(movingView);
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupGrid();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseMoved == newEvent.type)
		{
			processMouseMove(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMousePress(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseRelease(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Enter == t_event.key.code)
	{
		m_gameTiles = m_placedTiles;
		m_editingLevel = !m_editingLevel; //playing game
		movingView = baseView;
	}
	if (m_editingLevel)
	{
		if (sf::Keyboard::Left == t_event.key.code)
		{
			movingView.move({ -10,0 });
		}
		if (sf::Keyboard::Right == t_event.key.code)
		{
			movingView.move({ 10,0 });
		}
		if (sf::Keyboard::Q == t_event.key.code)
		{
			m_highlightTile.setTileType(TileType::Base);
		}
		if (sf::Keyboard::W == t_event.key.code)
		{
			m_highlightTile.setTileType(TileType::Hazard);
		}
		if (sf::Keyboard::E == t_event.key.code)
		{
			m_highlightTile.setTileType(TileType::Jump);
		}
	}
	else
	{
		if (sf::Keyboard::Space == t_event.key.code && playerYVelocity == 0)
		{
			playerYVelocity = playerJumpVelocity;
			std::cout << "jump called";
		}
	}
}


void Game::processMousePress(sf::Event t_event)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

	m_mousePressPos.x = worldPos.x;
	m_mousePressPos.y = worldPos.y;
}

void Game::processMouseRelease(sf::Event t_event)
{
	if (m_editingLevel)
	{
		checkPlacingBlock();
	}	
}

void Game::processMouseMove(sf::Event t_event)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

	m_mouseMovePos.x = worldPos.x;
	m_mouseMovePos.y = worldPos.y;
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	if (m_editingLevel)
	{
		checkHighlightingBlock();
	}
	else
	{
		moveTiles();
		updatePlayer();
		checkCollisions();
	}

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.setView(movingView);

	
	if (m_editingLevel)
	{
		for (int row = 0; row < MAX_ROWS; row++)
		{
			for (int col = 0; col < MAX_COLLUMS; col++)
			{
				m_tile.setPosition(m_gridPositions[row][col]);
				m_window.draw(m_tile);
			}
		}
		for (int i = 0; i < m_placedTiles.size(); i++)
		{
			m_window.draw(m_placedTiles[i]);
		}
		m_window.draw(m_highlightTile);
	}
	else
	{
		for (int i = 0; i < m_gameTiles.size(); i++)
		{
			m_window.draw(m_gameTiles[i]);
		}
		m_window.draw(m_playerShape);
	}
	

	m_window.display();
}

void Game::moveTiles()
{
	for (int i = 0; i < m_gameTiles.size(); i++)
	{
		m_gameTiles[i].move(tileSpeed,0);
	}
}

void Game::updatePlayer()
{
	playerYVelocity = playerYVelocity + playerGravity;
	m_playerShape.move(0, playerYVelocity);
	playerGravity = gravity;
}

void Game::checkCollisions()
{
	if (playerYVelocity >= 0)
	{
		
		for (int i = 0; i < m_gameTiles.size(); i++)
		{
			if (m_playerShape.getGlobalBounds().intersects(m_gameTiles[i].getGlobalBounds()))
			{
				if (m_playerShape.getPosition().y < m_gameTiles[i].getPosition().y)
				{
					playerGravity = 0;
					playerYVelocity = 0;
					m_playerShape.setPosition(m_playerShape.getPosition().x, m_gameTiles[i].getPosition().y);
					m_playerShape.move(0, -m_playerShape.getGlobalBounds().height);
					break;
				}
				else {
					m_editingLevel = true;
				}

			}
		}
		
	}
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_tile.setFillColor(sf::Color::White);
	m_tile.setSize({tileWidth, tileHeight});
	m_tile.setOutlineColor(sf::Color::Black);
	m_tile.setOutlineThickness(3.f);
	
	m_highlightTile.setFillColor(sf::Color::Red);
	m_highlightTile.setOutlineColor(sf::Color::Black);
	m_highlightTile.setOutlineThickness(3.f);
	m_highlightTile.setSize({ tileWidth - 3, tileHeight - 3});
	
	m_playerShape.setFillColor(sf::Color::Yellow);
	m_playerShape.setSize(m_playerSize);
	m_playerShape.setPosition(m_playerPos);
	
}

void Game::setupGrid()
{
	for (int row = 0; row < MAX_ROWS; row++  )
	{
		for (int col = 0; col < MAX_COLLUMS; col++)
		{
			m_gridPositions[row][col] = sf::Vector2f{ col * 70.f + m_hudXOffset, row * 30.f + m_hudYOffset };
		}
	}
	m_placedTiles.reserve(MAX_COLLUMS * MAX_ROWS);
}

void Game::checkHighlightingBlock()
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

void Game::checkPlacingBlock()
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
							std::cout << "already placed a block here!";
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

void Tile::setTileType(TileType t_type)
{
	switch (t_type)
	{
	case TileType::Base:
		this->setFillColor(sf::Color::Red);
		break;
	case TileType::Hazard:
		this->setFillColor(sf::Color::Blue);
		break;
	case TileType::Jump:
		this->setFillColor(sf::Color::Green);
		break;
	default:
		break;
	}
}
