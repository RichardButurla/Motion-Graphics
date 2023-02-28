#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameEnums.h"

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

	static const int MAX_PLAYERS = 2;

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMousePress(sf::Event t_event);
	void processMouseRelease(sf::Event t_event);
	void processMouseMove(sf::Event t_event);

	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();
	void setupSprite();
	void setupViews();

	sf::RenderWindow m_window; // main SFML window
	sf::View baseView;
	sf::View movingView;
	sf::Font m_ArialBlackfont; // font used by message

	sf::Texture texMap; // Create the world texture and sprite

	sf::Sprite map;

	sf::View fixed;
	sf::View standard;
	sf::View minimap;
	unsigned int size = 200; // The 'minimap' view will show a smaller picture of the map

	// The 'left' and the 'right' view will be used for splitscreen displays
	sf::View left;
	sf::View right;
	sf::RectangleShape miniback; // We want to draw a rectangle behind the minimap

	//Players
	sf::Texture playerTexture;
	Player players[MAX_PLAYERS];
	PlayerID m_playerID = PlayerID::PlayerOne;

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

