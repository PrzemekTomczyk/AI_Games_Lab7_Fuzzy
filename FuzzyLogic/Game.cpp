#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1280U, 720U, 32U }, "AI Lab 7 - Fuzzy Logic" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText();
	loadTexture("enemy.png", EntityType::Enemy);
	loadTexture("ally.png", EntityType::Ally);
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
	else if (sf::Keyboard::Space == t_event.key.code)
	{
		m_allySprites.clear();

		int allyCount = rollEnemyForces();
		for (size_t i = 0; i < allyCount; i++)
		{
			setupSprite(m_allySprites, EntityType::Ally);
		}
		allyLayout();
	}
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
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.draw(m_instructionsText);

	renderVector(m_allySprites);
	renderVector(m_enemySprites);

	m_window.display();
}

void Game::renderVector(std::vector<sf::Sprite>& t_container)
{
	for (auto& sprite : t_container)
	{
		m_window.draw(sprite);
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
	m_instructionsText.setFont(m_ArialBlackfont);
	m_instructionsText.setString("Press SPACE to deploy forces");
	m_instructionsText.setStyle(sf::Text::Bold);
	m_instructionsText.setCharacterSize(60U);
	m_instructionsText.setOrigin(sf::Vector2f(m_instructionsText.getGlobalBounds().width / 2.0f, m_instructionsText.getGlobalBounds().height / 2.0f));
	m_instructionsText.setPosition(static_cast<sf::Vector2f>(m_window.getSize()) / 2.0f);
	m_instructionsText.setFillColor(sf::Color::White);
}

void Game::loadTexture(std::string t_filename, EntityType t_type)
{
	sf::Texture* texture = nullptr;
	switch (t_type)
	{
	case Game::EntityType::Ally:
		texture = &m_allyTexture;
		break;
	case Game::EntityType::Enemy:
		texture = &m_enemyTexture;
		break;
	default:
		break;
	}
	if (texture && !texture->loadFromFile("ASSETS\\IMAGES\\" + t_filename))
	{
		// simple error message if previous call fails
		printf("Problem loading texture %s\n", t_filename.c_str());
	}
}

void Game::setupSprite(std::vector<sf::Sprite>& t_container, EntityType t_type)
{
	sf::Texture* texture = nullptr;

	switch (t_type)
	{
	case Game::EntityType::Ally:
	{
		texture = &m_allyTexture;
		break;
	}
	case Game::EntityType::Enemy:
	{
		texture = &m_enemyTexture;
		break;
	}
	default:
		return;
	}

	sf::Sprite sprite;
	sprite.setTexture(*texture);

	if (t_type == EntityType::Enemy)
	{
		sprite.setScale(0.075f, 0.075f);
		sf::Vector2f pos;
		pos.x = randomNum(sprite.getGlobalBounds().width, m_window.getSize().x - sprite.getGlobalBounds().width);
		pos.y = randomNum(sprite.getGlobalBounds().height, (m_window.getSize().y / 3.0f) - sprite.getGlobalBounds().height);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2.0f, sprite.getGlobalBounds().height / 2.0f);
		sprite.setPosition(pos);
		t_container.push_back(sprite);
	}
	else if (t_type == EntityType::Ally)
	{
		sprite.setScale(0.1f, 0.1f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2.0f, sprite.getGlobalBounds().height / 2.0f);
		t_container.push_back(sprite);
	}
}

int Game::randomNum(int t_min, int t_max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(t_min, t_max);
	return dist(rng);
}

void Game::allyLayout()
{
	/// <summary>
	/// TO-DO:
	/// 1. Fix spacing
	/// </summary>
	int alliesLeft = m_allySprites.size();
	int spriteWidth = m_allySprites[0].getGlobalBounds().width;
	int spriteHeight = m_allySprites[0].getGlobalBounds().height;
	int maxPerRow = alliesLeft / ((m_window.getSize().y / 3) / spriteHeight);

	int rowCount = 0;
	int index = 0;
	while (alliesLeft > 0)
	{
		sf::Vector2f pos;
		int spacing;
		int count;
		int currentX;
		if (alliesLeft > maxPerRow)
		{
			spacing = (m_window.getSize().x - spriteWidth) / (maxPerRow);
			count = maxPerRow;
			alliesLeft -= maxPerRow;
			currentX = spriteWidth;
		}
		else
		{
			spacing = (m_window.getSize().x - spriteWidth * 2) / (alliesLeft + 1);
			count = alliesLeft;
			alliesLeft = 0;
			currentX = spacing + spriteWidth;
		}

		for (size_t i = 0; i < count; i++)
		{
			pos.x = currentX;
			currentX += spacing;
			pos.y = m_window.getSize().y - (spriteHeight * (rowCount + 1));

			m_allySprites.at(index).setPosition(pos);
			index++;
		}

		rowCount++;
	}
}

int Game::rollEnemyForces()
{
	int enemyForce = randomNum(1, 30);
	int enemyDist = randomNum(0, 70);
	system("CLS");
	std::cout << "########################" << std::endl;
	std::cout << "\tEnemy Intel!" << std::endl;
	std::cout << "Enemy units:\t" << enemyForce << std::endl;
	std::cout << "Enemy klicks:\t" << enemyDist << std::endl;

	m_enemySprites.clear();

	for (size_t i = 0; i < enemyForce; i++)
	{
		setupSprite(m_enemySprites, EntityType::Enemy);
		float scale = m_enemySprites[i].getScale().x * enemyDist / 70;
		if (scale < 0.025)
		{
			scale = 0.025f;
		}
		m_enemySprites[i].setScale(scale, scale);
	}

	std::cout << "########################" << std::endl;
	std::cout << "\tFuzzy forces" << std::endl;
	m_tiny = FuzzyLogic::Triangle(enemyForce, -10, 0, 10);
	std::cout << "Tiny:\t" << m_tiny << std::endl;
	m_small = FuzzyLogic::Trapezoid(enemyForce, 2.5, 10, 15, 20);
	std::cout << "Smol:\t" << m_small << std::endl;
	m_moderate = FuzzyLogic::Trapezoid(enemyForce, 15, 20, 25, 30);
	std::cout << "Mod:\t" << m_moderate << std::endl;
	m_large = FuzzyLogic::Grade(enemyForce, 25, 30);
	std::cout << "Large:\t" << m_large << std::endl;

	std::cout << "########################" << std::endl;
	std::cout << "\tFuzzy ranges" << std::endl;
	m_close = FuzzyLogic::Triangle(enemyDist, -30, 0, 30);
	std::cout << "Close:\t" << m_close << std::endl;
	m_medium = FuzzyLogic::Trapezoid(enemyDist, 10, 30, 50, 70);
	std::cout << "Medium:\t" << m_medium << std::endl;
	m_far = FuzzyLogic::Grade(enemyDist, 50, 70);
	std::cout << "Far:\t" << m_far << std::endl;


	double low =
		FuzzyLogic::OR(
			FuzzyLogic::OR(
				FuzzyLogic::AND(m_medium, m_tiny),
				FuzzyLogic::AND(m_medium, m_small)
			),
			FuzzyLogic::AND(m_far, FuzzyLogic::NOT(m_large))
		);

	double medium =
		FuzzyLogic::OR(
			FuzzyLogic::OR(
				FuzzyLogic::AND(m_close, m_tiny),
				FuzzyLogic::AND(m_medium, m_moderate)
			),
			FuzzyLogic::AND(m_far, m_large)
		);

	double high =
		FuzzyLogic::OR(
			FuzzyLogic::AND(m_close, FuzzyLogic::NOT(m_tiny)),
			FuzzyLogic::AND(m_medium, m_large)
		);

	std::cout << "########################" << std::endl;
	std::cout << "\tFuzzied values" << std::endl;
	std::cout << "Low\t" << low << std::endl;
	std::cout << "Med\t" << medium << std::endl;
	std::cout << "High\t" << high << std::endl;
	std::cout << "########################" << std::endl;

	int deploy = std::ceil(low * 10 + medium * 30 + high * 50) / (low + medium + high);
	std::cout << "Deploying units: " << deploy << std::endl;

	return deploy;
}