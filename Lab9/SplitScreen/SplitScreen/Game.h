#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "LevelEditor.h"
#include "Player.h"
#include "GameEnums.h"
#include "Pickups.h"
#include <functional>
#include <list>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMousePress(sf::Event t_event);
	void processMouseRelease(sf::Event t_event);
	void processMouseMove(sf::Event t_event);

	void update(sf::Time t_deltaTime);
	void render();

	void renderPlayerOneScreen();
	void renderPlayerTwoScreen();

	void checkPlayerInput(sf::Time& t_deltaTime);
	void checkPlayerOneInput(sf::Time& t_deltaTime);
	void checkPlayerTwoInput(sf::Time& t_deltaTime);


	void checkStartGame();
	void checkPickupCollision();
	void checkBlueShellCollision();
	void checkPlayerWallTileCollision(Player & t_player);
	void checkBlueShellWallCollision();
	void calculateBlueShellDeflect(Pickups& t_blueShell, Tile& t_wall);
	void checkGameTime();

	void takeAwayCoins(int t_playerNumber);
	void dropCoins(sf::Vector2f t_playerPos, int t_numberOfCoinsLost);

	void setupFontAndText();
	void setupSprite();
	void setupViews();

	void loadPreviousLevel();
	void saveLevel();
	void swapToGameTextures();
	void swapToEditorTextures();

	sf::RenderWindow m_window; // main SFML window
	sf::View baseView;
	sf::View movingView;
	sf::Font m_ArialBlackfont; // font used by message

	//Level
	LevelEditor m_levelEditor;
	std::map<TileType, std::list<Tile>> m_gameTiles;
	sf::Texture m_tileTexture;
	bool m_editingLevel = true;
	Tile playerSpawnTile;

	sf::Texture texMap; // Create the world texture and sprite

	sf::Sprite map;

	sf::View fixed;
	sf::View standard;
	sf::View playerMiniMaps[MAX_PLAYERS];
	unsigned int size = 200; // The 'minimap' view will show a smaller picture of the map

	//Time
	sf::Clock timeSinceGameStart;
	sf::Time startCountdown = sf::seconds(4);
	sf::Time gameDuration = (sf::seconds(9) + startCountdown);
	
	sf::Text m_gameTimeText;
	sf::Text m_countdownText;
	bool m_startOfGame = true;
	sf::Text m_gameLoseText;
	sf::Text m_gameWinText;
	bool m_gameOver;

	// The 'left' and the 'right' view will be used for splitscreen displays
	sf::View left;
	sf::View right;
	sf::RectangleShape miniMapBackgrounds[MAX_PLAYERS]; // We want to draw a rectangle behind the minimap

	//Players
	sf::Texture playerTexture;
	sf::Text playerCoinTexts[MAX_PLAYERS];
	Player players[MAX_PLAYERS];
	sf::Vector2f playerPositions[MAX_PLAYERS];
	int playerOne = static_cast<int>(PlayerID::PlayerOne);
	int playerTwo = static_cast<int>(PlayerID::PlayerTwo);

	//Pickups
	std::map<ItemID,Pickups> m_pickupItems;
	sf::Texture m_pickupsTextures[MAX_ITEM_TYPES];
	sf::Vector2f m_coinPositions[MAX_COINS];
	std::function<void(sf::Vector2f)> m_magnetiseCoins;
	std::vector <std::function<void(sf::Vector2f)> > m_magnetFunctions;

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

