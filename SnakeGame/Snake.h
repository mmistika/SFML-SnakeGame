#ifndef SNAKE_GAME_SNAKE_SNAKE_H_
#define SNAKE_GAME_SNAKE_SNAKE_H_

#include <vector>

#include <SFML/Graphics.hpp>

#include "Direction.h"
#include "GameField.h"

class Snake
{
public:
	Snake(GameField* field, sf::Texture* textures, float speed = 0.2f);
	~Snake();

	void update(Direction::Type direction, sf::Vector2f foodPos);
	bool isFoodEaten() const { return isFoodEaten_; }
	bool isDead() const { return isDead_; }

	std::vector<sf::Drawable*> getRectsToDraw() const;

private:
	struct Segment
	{
		sf::RectangleShape rect;
		Direction::Type direction = Direction::None;

		Segment() : rect(sf::RectangleShape()) {};
		Segment(const Segment& segment)
			: rect(segment.rect), direction(segment.direction) {};
	};

	enum class SegmentName { Head, Body, Tail, Turn };
	void setTextureByName_(sf::RectangleShape& segment, SegmentName name);

	sf::Vector2f getNewPositionOffset_(Direction::Type direction) const;
	Direction::Type getDirectionFromPositions_(sf::Vector2f position, sf::Vector2f position2) const;

	// direction2 (direction of previous segment) should be set only if rotation is needed for turn segment.
	float getRotation_(Direction::Type direction, Direction::Type direction2 = Direction::None) const;
	
	bool isPosOnSnake_(sf::Vector2f pos) const;

	GameField* field_;
	float tileEdgeLength_;

	sf::Clock timer_;
	float deltaTime_ = 0;
	float speed_;

	sf::Texture* textures_;
	
	std::vector<Segment*> segmentList_;

	bool isFoodEaten_{ false };
	bool isDead_ { false };
};

#endif // SNAKE_GAME_SNAKE_SNAKE_H_