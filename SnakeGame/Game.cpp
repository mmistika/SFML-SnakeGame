#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

#include "Game.h"

Game::Game(sf::RenderWindow* window)
	: window_(window),
	tileEdgeLength_{ 20.f }
{
	// Window height and width should be multiple of tile edge length
	assert(!(window_->getSize().y % (int)tileEdgeLength_) and
		!(window_->getSize().x % (int)tileEdgeLength_));
	window_->setFramerateLimit(60);

	backgroundColor_ = sf::Color::Black;

	int normilizedWidth = window_->getSize().x / (int)tileEdgeLength_;
	int normilizedHeight = window_->getSize().y / (int)tileEdgeLength_;

	field_ = new GameField(normilizedWidth, normilizedHeight,(int)tileEdgeLength_);
	snake_ = new Snake(field_);
	food_ = new FoodSpawner(field_);
}

Game::~Game()
{
	delete snake_;
	delete food_;
	delete field_;
}

void Game::loop()
{
	Direction directionChosen = Direction::None;

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
					directionChosen = static_cast<Direction>(event.key.code);
				break;
			}
		}

		window_->clear(backgroundColor_);

		snake_->update(directionChosen, food_->currentPos());
		if (snake_->isFoodEaten())
		{
			food_->respawn();
			score_++;
			window_->setTitle(std::string("SnakeGame | Score: ") + std::to_string(score_));
			if (score_ + 2 == field_->tileSum())
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

		for (auto drawable : snake_->getRectsToDraw())
			window_->draw(*drawable);

		window_->draw(*food_->getRectToDraw());
		
		window_->display();
	}
}