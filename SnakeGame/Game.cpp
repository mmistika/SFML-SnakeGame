#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

#include "Direction.h"
#include "Game.h"

Game::Game(sf::RenderWindow* window)
	: window_(window),
	tileEdgeLength_{ 20.f }
{
	// Window height and width should be multiple of tile edge length
	assert(!(window_->getSize().y % (int)tileEdgeLength_) and
		!(window_->getSize().x % (int)tileEdgeLength_));
	window_->setFramerateLimit(60);

	// Field texture
	textures_[0] = new sf::Texture();
	textures_[0]->loadFromFile("fieldTexture.png");

	int normilizedWidth = window_->getSize().x / (int)tileEdgeLength_;
	int normilizedHeight = window_->getSize().y / (int)tileEdgeLength_;

	field_ = new GameField(normilizedWidth, normilizedHeight,(int)tileEdgeLength_, textures_[0]);

	// Food texture
	textures_[1] = new sf::Texture();
	textures_[1]->loadFromFile("foodTexture.png");

	food_ = new FoodSpawner(field_, textures_[1]);

	// Snake texture
	textures_[2] = new sf::Texture();
	textures_[2]->loadFromFile("snakeTexture.png");

	snake_ = new Snake(field_, textures_[2]);
	
}

Game::~Game()
{
	delete snake_;
	delete food_;
	delete field_;

	for (auto pointer : textures_)
		delete pointer;
}

void Game::loop()
{
	Direction::Type directionChosen = Direction::None;

	while (window_->isOpen())
	{
		sf::Event event;
		while (window_->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window_->close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code >= sf::Keyboard::Left and event.key.code <= sf::Keyboard::Down)
					directionChosen = Direction::fromSfmlKey(event.key.code);
				break;
			}
		}

		window_->clear();

		snake_->update(directionChosen, food_->currentPos());
		if (snake_->isFoodEaten())
		{
			food_->respawn();
			score_++;
			window_->setTitle(std::string("SnakeGame | Score: ") + std::to_string(score_));
			if (score_ + 3 == field_->tileSum())
			{
				std::cout << "The end! You won..." << std::endl;
				std::cout << "\nPress any key to close the game..." << std::endl;
				char _;
				std::cin >> _;
				break;
			}
		}
		
		if (snake_->isDead())
		{
			std::cout << "The end! You are dead..." << std::endl;
			std::cout << "\nPress any key to close the game..." << std::endl;
			char _;
			std::cin >> _;
			break;
		}

		window_->draw(*field_->getRectToDraw());

		for (auto drawable : snake_->getRectsToDraw())
			window_->draw(*drawable);

		window_->draw(*food_->getRectToDraw());
		
		window_->display();
	}
}