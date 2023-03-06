#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Vector2.h"



/// <summary>
/// default constructor
/// setup the m_window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32U }, "Level Editor" },
	m_exitGame{ false }, //when true game will exit
	m_levelEditor(m_window,m_levelTiles,m_gameTiles)
{
	baseView = m_window.getView();
	movingView = m_window.getView();
	m_window.setView(movingView);

	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupViews();

	loadPreviousLevel();

	using std::placeholders::_1;

	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		if (m_pickupItems[i].getItemType() == ItemTypes::Coin) {
			m_magnetiseCoins = std::bind(&Pickups::coinMagnetTrack, &m_pickupItems[i], _1);
			m_magnetFunctions.push_back(m_magnetiseCoins);
		}
	}
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
		if (m_editingLevel)
		{
			if (sf::Event::MouseWheelMoved == newEvent.type)
			{
				m_levelEditor.processMouseScroll(newEvent);
			}
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
		m_editingLevel = !m_editingLevel;
		timeSinceGameStart.restart();
		saveLevel();
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
	if (m_editingLevel)
	{
		m_levelEditor.update();
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			playerPositions[i] = players[i].getPosition();
			checkPlayerWallTileCollision(players[i]);
		}
		checkPlayerInput(t_deltaTime);
		checkPickupCollision();
		checkBlueShellCollision();
		checkBlueShellWallCollision();
		checkGameTime();

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			players[i].update(m_magnetFunctions);
		}

		for (int i = 0; i < m_pickupItems.size(); i++)
		{
			m_pickupItems[i].update(t_deltaTime);
		}
	}	
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	if (m_editingLevel)
	{
		m_levelEditor.render(m_window);
	}
	else
	{
		if (!m_gameOver)
		{

			left.setCenter(players[playerOne].getPosition());
			right.setCenter(players[playerTwo].getPosition());
			

			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				playerCoinTexts[i].setPosition(players[i].getPosition().x - left.getSize().x / 2, players[i].getPosition().y + left.getSize().y / 2.5);
				playerCoinTexts[i].setString("Coins Collected: " + std::to_string(players[i].getNumberOfCoinsCollected()));
			}
			m_window.draw(m_gameTimeText);
			
			renderPlayerOneScreen();

			renderPlayerTwoScreen();

			m_window.setView(fixed); // Draw 'GUI' elements with fixed positions
			//m_gameTimeText.setPosition()
			m_window.draw(m_gameTimeText);


			//m_window.setView(minimap); // Draw minimap

			//for (int i = 0; i < m_gameTiles.size(); i++)
			//{
			//	m_window.draw(m_gameTiles[i]);
			//}

			//for (int i = 0; i < MAX_PLAYERS; i++)
			//	players[i].render(m_window);		

			//m_window.draw(m_gameTimeText);

		}
		else
		{
			m_window.draw(m_gameWinText);
			m_window.draw(m_gameLoseText);
		}
	}

	m_window.display();
}

void Game::renderPlayerOneScreen()
{
	m_window.setView(left);
	
	for (int i = 0; i < m_gameTiles.size(); i++)
	{
		m_window.draw(m_gameTiles[i]);
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
		players[i].render(m_window);

	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		m_window.draw(m_pickupItems[i]);
	}

	m_window.draw(playerCoinTexts[playerOne]);
	//m_window.draw(player1);
	//m_window.draw(bulletPlayer1);
	//m_window.draw(bulletPlayer2);
	//m_window.draw(player2);
}

void Game::renderPlayerTwoScreen()
{
	m_window.setView(right);

	for (int i = 0; i < m_gameTiles.size(); i++)
	{
		m_window.draw(m_gameTiles[i]);
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
		players[i].render(m_window);

	

	m_window.draw(playerCoinTexts[playerTwo]);

	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		m_window.draw(m_pickupItems[i]);
	}

	/*m_window.draw(bulletPlayer1);

	m_window.draw(bulletPlayer2);
	m_window.draw(player1);
	m_window.draw(player2);*/
}

void Game::checkPlayerInput(sf::Time& t_deltaTime)
{
	checkPlayerOneInput(t_deltaTime);
	checkPlayerTwoInput(t_deltaTime);
}

void Game::checkPlayerOneInput(sf::Time& t_deltaTime)
{
	sf::Vector2f moveVector;
	float direction = 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
			moveVector = { 0,-1 * t_deltaTime.asSeconds()};
			players[playerOne].movePlayer(moveVector);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
			moveVector = { 0,1 * t_deltaTime.asSeconds() };
			players[playerOne].movePlayer(moveVector);		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{	
			moveVector = { -1 * t_deltaTime.asSeconds(),0 };
			players[playerOne].movePlayer(moveVector);
	}	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{		
			moveVector = { 1 * t_deltaTime.asSeconds(),0 };
			players[playerOne].movePlayer(moveVector);
	}
	//Pick Up/Drop Item
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		if (players[playerOne].isHoldingItem())
		{
			int itemId = players[playerOne].getItemHeldID();
			players[playerOne].setHoldingItem(false);
			m_pickupItems[itemId].dropPickup();
		}
	}
	//Use Item
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		if (players[playerOne].isHoldingItem())
		{
			int itemId = players[playerOne].getItemHeldID();
			players[playerOne].setHoldingItem(false);
			m_pickupItems[itemId].useItem();
			ItemTypes itemType = m_pickupItems[itemId].getItemType();
			switch (itemType)
			{
				// coin is not used since it is added to count

				//BlueShell does not get erased
			case ItemTypes::SpeedBoost:
				players[playerOne].usePowerUp(itemType);
				m_pickupItems.erase(itemId);
				break;
			case ItemTypes::Armour:
				m_pickupItems.erase(itemId);
				players[playerOne].usePowerUp(itemType);
				break;
			case ItemTypes::Magnet:
				m_pickupItems.erase(itemId);
				players[playerOne].usePowerUp(itemType);
				break;
			case ItemTypes::CoinDoubler:
				m_pickupItems.erase(itemId);
				players[playerOne].usePowerUp(itemType);
				break;
			default:
				break;
			}
		}
	}
}

void Game::checkPlayerTwoInput(sf::Time& t_deltaTime)
{
	sf::Vector2f moveVector;
	float direction = 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		moveVector.y = -direction * t_deltaTime.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		moveVector.x = -direction * t_deltaTime.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		moveVector.y = direction * t_deltaTime.asSeconds();

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		moveVector.x = direction * t_deltaTime.asSeconds();
	}
	//Pick Up/Drop Item
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (players[playerTwo].isHoldingItem())
		{
			int itemId = players[playerTwo].getItemHeldID();
			players[playerTwo].setHoldingItem(false);
			m_pickupItems[itemId].dropPickup();
		}
	}
	//Use Item
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (players[playerTwo].isHoldingItem())
		{
			int itemId = players[playerTwo].getItemHeldID();
			players[playerTwo].setHoldingItem(false);
			m_pickupItems[itemId].useItem();

			ItemTypes itemType = m_pickupItems[itemId].getItemType();
			switch (itemType)
			{
				// coin is not used since it is added to count

				//BlueShell does not get erased
			case ItemTypes::SpeedBoost:
				players[playerTwo].usePowerUp(itemType);
				m_pickupItems.erase(itemId);
				break;
			case ItemTypes::Armour:
				players[playerTwo].usePowerUp(itemType);
				m_pickupItems.erase(itemId);
				break;
			case ItemTypes::Magnet:
				players[playerTwo].usePowerUp(itemType);
				m_pickupItems.erase(itemId);
				break;
			case ItemTypes::CoinDoubler:
				players[playerTwo].usePowerUp(itemType);
				m_pickupItems.erase(itemId);
				break;
			default:
				break;
			}
		}
	}
	players[playerTwo].movePlayer(moveVector);
}

void Game::checkPickupCollision()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		for (int j = 0; j < m_pickupItems.size(); j++)
		{
			if (m_pickupItems[j].getGlobalBounds().intersects(players[i].getGlobalBounds()))
			{
				ItemTypes itemType = m_pickupItems[j].getItemType();
				if (itemType == ItemTypes::Coin)
				{
					m_pickupItems.erase(j);
					players[i].collectCoin();
					break;
				}
				if (!players[i].isHoldingItem())
				{
					switch (itemType)
					{
					case ItemTypes::BlueShell:
						if (!m_pickupItems[j].isPickedUp())
						{
							m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
							players[i].setItemHeldID(m_pickupItems[j].getItemId());
						}
						break;
					case ItemTypes::SpeedBoost:
						if (!m_pickupItems[j].isPickedUp())
						{
							m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
							players[i].setItemHeldID(m_pickupItems[j].getItemId());
						}
						break;
					case ItemTypes::Armour:
						if (!m_pickupItems[j].isPickedUp())
						{
							m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
							players[i].setItemHeldID(m_pickupItems[j].getItemId());
						}
						break;
					case ItemTypes::Magnet:
						if (!m_pickupItems[j].isPickedUp())
						{
							m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
							players[i].setItemHeldID(m_pickupItems[j].getItemId());
						}
						break;
					case ItemTypes::CoinDoubler:
						if (!m_pickupItems[j].isPickedUp())
						{
							m_pickupItems[j].pickUp(static_cast<PlayerID>(i));
							players[i].setItemHeldID(m_pickupItems[j].getItemId());
						}
						break;
					default:
						break;
					}
				}				
			}
		}
	}
}

void Game::checkBlueShellCollision()
{
	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		if (m_pickupItems[i].getItemType() == ItemTypes::BlueShell)
		{
			Pickups blueShell = m_pickupItems[i];
			if (blueShell.isUsed())
			{
				PlayerID playerID = blueShell.getPlayerID();

				switch (playerID)
				{
				case PlayerID::PlayerOne:
					if (blueShell.getGlobalBounds().intersects(players[playerTwo].getGlobalBounds()))
					{
						if (!players[playerTwo].hasArmour())
						{
							std::cout << "Collision with Player Two";
							takeAwayCoins(playerTwo);
						}
						m_pickupItems.erase(blueShell.getItemId());
					}
					break;
				case PlayerID::PlayerTwo:
					if (blueShell.getGlobalBounds().intersects(players[playerOne].getGlobalBounds()))
					{
						if (!players[playerOne].hasArmour())
						{
							std::cout << "Collision with Player One";
							takeAwayCoins(playerOne);
						}	
						m_pickupItems.erase(blueShell.getItemId());
					}
					break;
				default:
					break;
				}
			}

		}
	}
}

bool Game::checkPlayerWallTileCollision(Player & t_player)
{
	for (int j = 0; j < m_levelTiles.size(); j++)
	{
		if (m_levelTiles[j].getTileType() == TileType::Wall)
		{
			if (t_player.getGlobalBounds().intersects(m_levelTiles[j].getGlobalBounds()))
			{
				sf::Vector2f pushBackVector = t_player.getPosition() - m_levelTiles[j].getPosition();
				if (abs(pushBackVector.x) > abs(pushBackVector.y))
				{
					t_player.setPosition({ (t_player.getPosition().x + pushBackVector.x  / 4), t_player.getPosition().y });
				}
				else
				{
					t_player.setPosition({ t_player.getPosition().x , (t_player.getPosition().y + pushBackVector.y  / 4)});
				}
			}
		}
			
	}
	return false;
}

void Game::checkBlueShellWallCollision()
{
	for (int i = 0; i < m_pickupItems.size(); i++)
	{
		if (m_pickupItems[i].getItemType() == ItemTypes::BlueShell)
		{
			Pickups & blueShell = m_pickupItems[i];
			if (blueShell.isUsed())
			{
				std::cout << "\nblueShell Velocity X: " << blueShell.getVelocity().x << "Velocity Y : " << blueShell.getVelocity().y;
				for (int j = 0; j < m_gameTiles.size(); j++)
				{
					if (m_gameTiles[j].getTileType() == TileType::Wall)
					{
						if (blueShell.getGlobalBounds().intersects(m_gameTiles[j].getGlobalBounds()))
						{
							
							//Now apply math
							sf::Vector2f shellVelocity = blueShell.getVelocity();

							sf::Vector2f shellPosition = blueShell.getPosition();
							sf::Vector2f tilePosition = m_gameTiles[j].getPosition();


							/*sf::Vector2f pushBack;
							pushBack.x = shellPosition.x - shellVelocity.x * 6;
							pushBack.y = shellPosition.y - shellVelocity.y * 6;
							blueShell.setPositionVector(pushBack);*/

							sf::Vector2f wallNormalComponent = (shellPosition - tilePosition );
							sf::Vector2f greenShellNormalComponent = (tilePosition - shellPosition);

							sf::Vector2f projectionOne = vectorProjection({ 0,0 }, wallNormalComponent);

							sf::Vector2f rejectionOne = vectorRejection(shellVelocity, greenShellNormalComponent);

							sf::Vector2f velocityOne = projectionOne + rejectionOne;

							blueShell.setPositionVector({ blueShell.getPosition().x + (-velocityOne.x * 10) , blueShell.getPosition().y + (-velocityOne.y * 10) });
							//velocityOne.x *= 1.3;
							//velocityOne.y *= 1.3;
							blueShell.setVelocity(- velocityOne );
							
						}
					}
				}
				
			}

		}
	}
}

void Game::checkGameTime()
{
	int timeLeft = gameDuration.asSeconds() - timeSinceGameStart.getElapsedTime().asSeconds();
	m_gameTimeText.setString("Time Left: " + std::to_string(timeLeft));
	if (timeSinceGameStart.getElapsedTime() > gameDuration)
	{
		m_gameOver = true;
		sf::Vector2f playerOneTextPos{ m_window.getSize().x / 3.5f - m_gameTimeText.getGlobalBounds().width / 2 , m_window.getSize().y / 2.f };
		sf::Vector2f playerTwoTextPos{ m_window.getSize().x / 1.5f - m_gameTimeText.getGlobalBounds().width / 2 , m_window.getSize().y / 2.f };
		if (players[playerOne].getNumberOfCoinsCollected() > players[playerTwo].getNumberOfCoinsCollected())
		{
			m_gameWinText.setPosition(playerOneTextPos);
			m_gameLoseText.setPosition(playerTwoTextPos);
		}
		else
		{
			m_gameWinText.setPosition(playerTwoTextPos);
			m_gameLoseText.setPosition(playerOneTextPos);
		}
	}
}

void Game::takeAwayCoins(int t_playerNumber)
{
	int numberOfCoinsLost = 6;
	int numberOfCoinsCollected = players[t_playerNumber].getNumberOfCoinsCollected();
	if (numberOfCoinsCollected > 0)
	{
		//if player has more than the coin penalty just take away that amount, otherwise take all his coins away
		if (numberOfCoinsCollected > numberOfCoinsLost)
		{
			players[t_playerNumber].removeCoins(numberOfCoinsLost);
			dropCoins(playerPositions[t_playerNumber], numberOfCoinsLost);
		}
		else
		{
			players[t_playerNumber].removeCoins(numberOfCoinsCollected);
			dropCoins(playerPositions[t_playerNumber], numberOfCoinsCollected);
		}
	}
}

void Game::dropCoins(sf::Vector2f t_playerPos, int t_numberOfCoinsLost)
{
	//put coins around player and add onto map.
	float dropAngle = 20;
	float dropDistance = 150;
	float dropAngleRadians = dropAngle * (PI / 180);
	dropAngleRadians = dropAngleRadians;
	float dropAngleRadianDecrement = (dropAngleRadians * 10) / t_numberOfCoinsLost;

	sf::Vector2f initialDropPos = t_playerPos;
	initialDropPos.y += dropDistance;
	sf::Vector2f unitVector = vectorUnitVector(initialDropPos);
	unitVector *= dropDistance;

	sf::Vector2f rotatedPoint;


	Pickups pickup;
	for (int i = 0; i  < t_numberOfCoinsLost; i++)
	{
		pickup.init(m_pickupsTextures[static_cast<int>(ItemTypes::Coin)], ItemTypes::Coin, playerPositions);

		rotatedPoint = vectorRotateBy(unitVector, dropAngleRadians);
		rotatedPoint += t_playerPos;

		pickup.setPositionVector(rotatedPoint);
		m_pickupItems[pickup.getItemId()] = pickup;

		dropAngleRadians -= dropAngleRadianDecrement;
		std::cout << "Angle in Radians" << dropAngleRadians;
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

	sf::Vector2f playerCoinTextPositions[MAX_PLAYERS];

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		playerCoinTextPositions[i] = players[i].getPosition();
	}

	for (size_t i = 0; i < MAX_PLAYERS; i++)
	{
		playerCoinTexts[i].setFont(m_ArialBlackfont);
		playerCoinTexts[i].setString("Coins Collected: ");
		playerCoinTexts[i].setStyle(sf::Text::Italic | sf::Text::Bold);
		playerCoinTexts[i].setPosition(playerCoinTextPositions[i]);
		playerCoinTexts[i].setCharacterSize(30U);
		playerCoinTexts[i].setOutlineColor(sf::Color::Black);
		playerCoinTexts[i].setFillColor(sf::Color::Yellow);
		playerCoinTexts[i].setOutlineThickness(3.0f);
		playerCoinTexts[i].setScale(0.1, 0.1);
	}
	m_gameTimeText.setFont(m_ArialBlackfont);
	m_gameTimeText.setString("Time Remaining: ");
	m_gameTimeText.setStyle(sf::Text::Italic | sf::Text::Bold);
	m_gameTimeText.setPosition({ m_window.getSize().x / 2.f - m_gameTimeText.getGlobalBounds().width / 2,0 + 0});
	m_gameTimeText.setCharacterSize(30U);
	m_gameTimeText.setOutlineColor(sf::Color::Black);
	m_gameTimeText.setFillColor(sf::Color::White);
	m_gameTimeText.setOutlineThickness(3.0f);

	m_gameLoseText.setFont(m_ArialBlackfont);
	m_gameLoseText.setString("You Lose!");
	m_gameLoseText.setStyle(sf::Text::Italic | sf::Text::Bold);
	m_gameLoseText.setPosition({ m_window.getSize().x / 2.f - m_gameTimeText.getGlobalBounds().width / 2 , m_window.getSize().y / 2.f });
	m_gameLoseText.setCharacterSize(30U);
	m_gameLoseText.setOutlineColor(sf::Color::Black);
	m_gameLoseText.setFillColor(sf::Color::White);
	m_gameLoseText.setOutlineThickness(3.0f);

	m_gameWinText.setFont(m_ArialBlackfont);
	m_gameWinText.setString("You Win!");
	m_gameWinText.setStyle(sf::Text::Italic | sf::Text::Bold);
	m_gameWinText.setPosition({ m_window.getSize().x / 4.f - m_gameTimeText.getGlobalBounds().width / 2 , m_window.getSize().y / 2.f });
	m_gameWinText.setCharacterSize(30U);
	m_gameWinText.setOutlineColor(sf::Color::Black);
	m_gameWinText.setFillColor(sf::Color::White);
	m_gameWinText.setOutlineThickness(3.0f);

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
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::SpeedBoost)].loadFromFile("ASSETS\\IMAGES\\speedBoost.png"))
	{
		std::cout << "problem loading SpeedBoost png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::Armour)].loadFromFile("ASSETS\\IMAGES\\armourIcon.png"))
	{
		std::cout << "problem loading Armour png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::Magnet)].loadFromFile("ASSETS\\IMAGES\\magnetIcon.png"))
	{
		std::cout << "problem loading Magnet png" << std::endl;
	}
	if (!m_pickupsTextures[static_cast<int>(ItemTypes::CoinDoubler)].loadFromFile("ASSETS\\IMAGES\\2xCoin.png"))
	{
		std::cout << "problem loading CoinDoubler png" << std::endl;
	}
	if (!m_tileTexture.loadFromFile("ASSETS\\IMAGES\\tileSheet2.png"))
	{
		std::cout << "problem loading TileSheet png" << std::endl;
	}

	m_levelEditor.init(m_tileTexture,m_ArialBlackfont);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].init(playerTexture);
	}	

	map.setTexture(texMap);
}

void Game::setupViews()
{
	fixed = m_window.getView(); // The 'fixed' view will never change
	standard = fixed; // The 'standard' view will be the one that gets always displayed

	size = 200; // The 'minimap' view will show a smaller picture of the map
	minimap = sf::View(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size), static_cast<float>(m_window.getSize().y * size / m_window.getSize().x)));
	minimap.setViewport(sf::FloatRect(1.f - static_cast<float>(minimap.getSize().x) / m_window.getSize().x - 0.1f, 1.f - static_cast<float>(minimap.getSize().y) / m_window.getSize().y - 0.1f,
		static_cast<float>(minimap.getSize().x) / m_window.getSize().x, static_cast<float>(minimap.getSize().y) / m_window.getSize().y));
	minimap.zoom(4.f);
	minimap.setCenter(static_cast<sf::Vector2f>(m_window.getSize()));
	// The 'left' and the 'right' view will be used for splitscreen displays
	left = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(m_window.getSize().x / 2), static_cast<float>(m_window.getSize().y)));
	left.setViewport(sf::FloatRect(0.f, 0.f, 0.5, 1.f));
	left.zoom(0.2);
	right = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(m_window.getSize().x / 2), static_cast<float>(m_window.getSize().y)));
	right.setViewport(sf::FloatRect(0.5, 0.f, 0.5, 1.f));
	right.zoom(0.2);

	// We want to draw a rectangle behind the minimap
	/*miniback.setPosition(minimap.getViewport().left * m_window.getSize().x - 5, minimap.getViewport().top * m_window.getSize().y - 5);
	miniback.setSize(sf::Vector2f(minimap.getViewport().width * m_window.getSize().x + 10, minimap.getViewport().height * m_window.getSize().y + 10));
	miniback.setFillColor(sf::Color(160, 8, 8));*/

	/*left.setCenter(player1.getPosition());
	right.setCenter(player2.getPosition());*/
}

void Game::loadPreviousLevel()
{
	std::ifstream file("tileData.txt");
	if (file.is_open())
	{
		Tile tile;
		int x;
		int y;
		int tileType;

		Pickups pickup;
		ItemTypes itemType;
		bool itemsPlaced[MAX_ITEM_TYPES]{ false };
		itemsPlaced[static_cast<int>(ItemTypes::Coin)] = true;

		while (file >> x >> y >> tileType) {
			tile.setPosition(sf::Vector2f{ static_cast<float>(x),static_cast<float>(y) });
			tile.setTexture(m_tileTexture);
			tile.setTileType(static_cast<TileType>(tileType));

			//This is superflawed since we dont create mroe instances
			if (tileType == static_cast<int>(TileType::PlayerSpawn))
			{
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					players[i].setPosition(tile.getPosition());
				}				
			}
			if (tileType == static_cast<int>(TileType::CoinSpawn))
			{
				pickup.init(m_pickupsTextures[static_cast<int>(ItemTypes::Coin)], ItemTypes::Coin, playerPositions);
				pickup.setPositionVector(tile.getPosition());
				m_pickupItems[pickup.getItemId()] = pickup;
				
			}
			if (tileType == static_cast<int>(TileType::PowerUpSpawn))
			{
				int count = 0;
				for (int i = 0; i < MAX_ITEM_TYPES; i++) //checks if all powerup types have been placed and resets array
				{
					if (itemsPlaced[i])
						count++;

					if (count == MAX_ITEM_TYPES)
					{
						for (int j = 0; j < MAX_ITEM_TYPES; j++)
						{
							itemsPlaced[j] = false;
						}
						itemsPlaced[static_cast<int>(ItemTypes::Coin)] = true; //have to make coin never used since i made items types hold coin...
					}
										
				}

				int randItem = rand() % MAX_ITEM_TYPES;

				while (itemsPlaced[randItem] == true)
				{
					randItem = rand() % MAX_ITEM_TYPES;

				}
				if (itemsPlaced[randItem] == false)
				{
					itemType = static_cast<ItemTypes>(randItem);
					pickup.init(m_pickupsTextures[static_cast<int>(itemType)], itemType, playerPositions);
					pickup.setPositionVector(tile.getPosition());
					m_pickupItems[pickup.getItemId()] = pickup;

					itemsPlaced[randItem] = true;
				}
			}
			m_levelTiles.push_back(tile);
			if (tile.getTileType() != TileType::Wall && tile.getTileType() != TileType::Floor)
			{
				tile.setTileType(TileType::Floor);//This is so the special tiles arent drawn in game
			}
			m_gameTiles.push_back(tile);
		}
		file.close();
	}

	
}

void Game::saveLevel()
{
	std::ofstream file("tileData.txt");

	for (int i = 0; i < m_levelTiles.size(); i++) {
		sf::Vector2f tilePos = m_levelTiles[i].getPosition();
		int tileType = static_cast<int>(m_levelTiles[i].getTileType());
		file << tilePos.x << " " << tilePos.y << " " << tileType << std::endl;
	}
	file.close();
}
