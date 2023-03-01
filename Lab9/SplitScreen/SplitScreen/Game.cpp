#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>



/// <summary>
/// default constructor
/// setup the m_window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32U }, "Level Editor" },
	m_exitGame{ false } //when true game will exit
{
	baseView = m_window.getView();
	movingView = m_window.getView();
	m_window.setView(movingView);

	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupViews();
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
		if (sf::Event::Closed == newEvent.type) // window message
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
	
}


void Game::processMousePress(sf::Event t_event)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

	m_mousePressPos.x = worldPos.x;
	m_mousePressPos.y = worldPos.y;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		
	}


}

void Game::processMouseRelease(sf::Event t_event)
{
	


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

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		playerPositions[i] = players[i].getPosition();
	}

	checkPlayerInput();
	checkPickupCollision();

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].update();
	}
	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		m_pickupItems[i].update();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	left.setCenter(players[playerOne].getPosition());
	right.setCenter(players[playerTwo].getPosition());
	
	m_window.clear(sf::Color::Black);
	//minimap.setCenter(sf::Vector2f(player2.getPosition().x + (player1.getPosition().x) / 2, player2.getPosition().y + (player1.getPosition().y) / 2));
	
	renderPlayerOneScreen();

	renderPlayerTwoScreen();

	m_window.setView(fixed); // Draw 'GUI' elements with fixed positions

	//m_window.draw(miniback);
	//m_window.setView(minimap); // Draw minimap
	//m_window.draw(map);

	m_window.display();
}

void Game::renderPlayerOneScreen()
{
	m_window.setView(left);
	m_window.draw(map);
	for (int i = 0; i < MAX_PLAYERS; i++)
		players[i].render(m_window);

	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		m_window.draw(m_pickupItems[i]);
	}
	//m_window.draw(player1);
	//m_window.draw(bulletPlayer1);
	//m_window.draw(bulletPlayer2);
	//m_window.draw(player2);
}

void Game::renderPlayerTwoScreen()
{
	m_window.setView(right);
	m_window.draw(map);

	for (int i = 0; i < MAX_PLAYERS; i++)
		players[i].render(m_window);

	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		m_window.draw(m_pickupItems[i]);
	}
	/*m_window.draw(bulletPlayer1);

	m_window.draw(bulletPlayer2);
	m_window.draw(player1);
	m_window.draw(player2);*/
}

void Game::checkPlayerInput()
{
	//Player One
	sf::Vector2f moveVector[MAX_PLAYERS]
	{
		{0,0},
		{0,0}
	};
	float speed = 3;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		moveVector[0].y = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveVector[0].x = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		moveVector[0].y = speed;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveVector[0].x = speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		if (players[playerOne].isHoldingItem())
		{
			int itemId = players[playerOne].getItemHeldID();
			players[playerOne].setHoldingItem(false);
			m_pickupItems[itemId].dropPickup();
		}		
	}

	//Player Two
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		moveVector[1].y = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		moveVector[1].x = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		moveVector[1].y = speed;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		moveVector[1].x = speed;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (players[playerTwo].isHoldingItem())
		{
			int itemId = players[playerTwo].getItemHeldID();
			players[playerTwo].setHoldingItem(false);
			m_pickupItems[itemId].dropPickup();
		}	
	}

	players[playerOne].movePlayer(moveVector[0]);
	players[playerTwo].movePlayer(moveVector[1]);
}

void Game::checkPickupCollision()
{
	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		std::cout << "\n Item Id: " << m_pickupItems[i].getItemId();
	}
	std::cout << "\n";
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		for (int j = 0; j < m_pickupItems.size(); j++)
		{
			if (m_pickupItems[j].getGlobalBounds().intersects(players[i].getGlobalBounds()))
			{
				ItemTypes itemType = m_pickupItems[j].getItemType();
				switch (itemType)
				{
				case ItemTypes::Coin:
					m_pickupItems.erase(j);
					break;
				case ItemTypes::BlueShell:
					std::cout << i;
					m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
					players[i].setItemHeldID(m_pickupItems[j].getItemId());
					break;
				case ItemTypes::SpeedBoost:
					break;
				case ItemTypes::Armour:
					break;
				case ItemTypes::Magnet:
					break;
				case ItemTypes::CoinDoubler:
					break;
				default:
					break;
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

	/*m_hudText.setFont(m_ArialBlackfont);
	m_hudText.setString(hudTexts[i]);
	m_hudText.setStyle(sf::Text::Italic | sf::Text::Bold);
	m_hudText.setPosition(70.0f, 20.0f);
	m_hudText.setCharacterSize(30U);
	m_hudText.setOutlineColor(sf::Color::Black);
	m_hudText.setFillColor(hudTileColors[i]);
	m_hudText.setOutlineThickness(3.0f);*/
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!texMap.loadFromFile("ASSETS\\IMAGES\\world.png"))
	{
		std::cout << "problem loading WORLD PNG" << std::endl;
	}
	if (!playerTexture.loadFromFile("ASSETS\\IMAGES\\mario.png"))
	{
		std::cout << "problem loading player png" << std::endl;
	}

	//Item Textures
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::Coin)].loadFromFile("ASSETS\\IMAGES\\coin.png"))
	{
		std::cout << "problem loading Coin png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::BlueShell)].loadFromFile("ASSETS\\IMAGES\\blueShell.png"))
	{
		std::cout << "problem loading Gun png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::SpeedBoost)].loadFromFile("ASSETS\\IMAGES\\mario.png"))
	{
		std::cout << "problem loading SpeedBoost png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::Armour)].loadFromFile("ASSETS\\IMAGES\\mario.png"))
	{
		std::cout << "problem loading Armour png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::Magnet)].loadFromFile("ASSETS\\IMAGES\\mario.png"))
	{
		std::cout << "problem loading Magnet png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::CoinDoubler)].loadFromFile("ASSETS\\IMAGES\\mario.png"))
	{
		std::cout << "problem loading CoinDoubler png" << std::endl;
	}
	
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].init(playerTexture);
	}

	Pickups pickup;
	for (int i = 0; i < MAX_COINS; i++)
	{
		
		pickup.init(m_pickupsTextures[static_cast<int>(ItemTypes::Coin)], ItemTypes::Coin, playerPositions);
		m_pickupItems[pickup.getItemId()] = pickup;
	}

	for (int i = 0; i < MAX_BLUE_SHELLS; i++)
	{
		pickup.init(m_pickupsTextures[static_cast<int>(ItemTypes::BlueShell)], ItemTypes::BlueShell, playerPositions);
		m_pickupItems[pickup.getItemId()] = pickup;
	}

	

	map.setTexture(texMap);
}

void Game::setupViews()
{
	fixed = m_window.getView(); // The 'fixed' view will never change
	standard = fixed; // The 'standard' view will be the one that gets always displayed

	size = 200; // The 'minimap' view will show a smaller picture of the map
	minimap = sf::View(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size), static_cast<float>(m_window.getSize().y * size / m_window.getSize().x)));
	minimap.setViewport(sf::FloatRect(1.f - static_cast<float>(minimap.getSize().x) / m_window.getSize().x - 0.02f, 1.f - static_cast<float>(minimap.getSize().y) / m_window.getSize().y - 0.02f,
		static_cast<float>(minimap.getSize().x) / m_window.getSize().x, static_cast<float>(minimap.getSize().y) / m_window.getSize().y));
	minimap.zoom(4.f);

	// The 'left' and the 'right' view will be used for splitscreen displays
	left = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(m_window.getSize().x / 2), static_cast<float>(m_window.getSize().y)));
	left.setViewport(sf::FloatRect(0.f, 0.f, 0.5, 1.f));
	right = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(m_window.getSize().x / 2), static_cast<float>(m_window.getSize().y)));
	right.setViewport(sf::FloatRect(0.5, 0.f, 0.5, 1.f));

	// We want to draw a rectangle behind the minimap
	miniback.setPosition(minimap.getViewport().left * m_window.getSize().x - 5, minimap.getViewport().top * m_window.getSize().y - 5);
	miniback.setSize(sf::Vector2f(minimap.getViewport().width * m_window.getSize().x + 10, minimap.getViewport().height * m_window.getSize().y + 10));
	miniback.setFillColor(sf::Color(160, 8, 8));







	/*left.setCenter(player1.getPosition());
	right.setCenter(player2.getPosition());*/
}
