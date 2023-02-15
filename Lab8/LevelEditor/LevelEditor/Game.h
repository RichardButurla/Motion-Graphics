#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

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

	static const int MAX_COLLUMS = 100;
	static const int MAX_ROWS = 17;

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
	void setupGrid();

	void checkHighlightingBlock();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	//Mouse
	sf::Vector2f m_mousePressPos;
	sf::Vector2f m_mouseMovePos;

	sf::RectangleShape m_tile;
	sf::RectangleShape m_highlightTile;
	float tileWidth = 70;
	float tileHeight = 30;
	float m_hudYOffset = 90;
	sf::Vector2f m_gridPositions[MAX_ROWS][MAX_COLLUMS];


	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

