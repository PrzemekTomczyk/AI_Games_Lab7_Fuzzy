#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <random>

class Game
{
private:
	enum class EntityType
	{
		Ally,
		Enemy
	};

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
	void update(sf::Time t_deltaTime);
	void render();
	void renderVector(std::vector<sf::Sprite>& t_container);

	void setupFontAndText();
	void loadTexture(std::string t_filename, EntityType t_type);
	void setupSprite(std::vector<sf::Sprite>& t_container, EntityType t_type);
	int randomNum(int t_min, int t_max);
	void allyLayout();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used

	sf::Text m_instructionsText; // text used for message on screen

	sf::Texture m_allyTexture;
	sf::Texture m_enemyTexture;

	std::vector<sf::Sprite> m_allySprites;
	std::vector<sf::Sprite> m_enemySprites;

	bool m_exitGame; // control exiting game
};

#endif // !GAME_HPP

