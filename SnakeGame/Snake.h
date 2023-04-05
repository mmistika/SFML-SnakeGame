#ifndef SNAKE_GAME_SNAKE_SNAKE_H_
#define SNAKE_GAME_SNAKE_SNAKE_H_

#include <vector>

#include <SFML/Graphics.hpp>

#include "GameField.h"

enum class Direction
{
	None,
	Left = sf::Keyboard::Left,
	Right,
	Up,
	Down
};

class Snake
{
public:
	Snake(GameField* field, float speed = 0.2f, sf::Color color = sf::Color::Green);
	~Snake();

	void update(Direction direction, sf::Vector2f foodPos);
	bool isFoodEaten() const { return isFoodEaten_; }
	bool isDead() const { return isDead_; }

	std::vector<sf::Drawable*> getRectsToDraw() const;

private:
	struct Segment
	{
		sf::RectangleShape rect;
		Direction direction = Direction::None;

		Segment() : rect(sf::RectangleShape()) {};
		Segment(const Segment& segment) : rect(segment.rect) {};
	};

	sf::Vector2f getNewPositionOffset_(Direction direction);
	bool isPosOnSnake_(sf::Vector2f pos) const;

	GameField* field_;
	float tileEdgeLength_;

	sf::Clock timer_;
	float deltaTime_ = 0;
	float speed_;

	sf::Color color_;
	
	std::vector<Segment*> segmentList_;

	bool isFoodEaten_{ false };
	bool isDead_ { false };
};

#endif // SNAKE_GAME_SNAKE_SNAKE_H_