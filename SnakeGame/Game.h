#ifndef SNAKE_GAME_GAME_H
#define SNAKE_GAME_GAME_H

#include <SFML/Graphics.hpp>

#include "GameField.h"
#include "Snake.h"
#include "FoodSpawner.h"

class Game
{
public:
	Game(sf::RenderWindow *window);
	~Game();

	void loop();

private:
	sf::RenderWindow* window_;
	sf::Texture* textures_[3]; // 0 - field, 1 - food, 2 - snake

	GameField* field_;
	float tileEdgeLength_;

	Snake* snake_;
	FoodSpawner* food_;
	int score_ = 0;
};

#endif // SNAKE_GAME_GAME_H