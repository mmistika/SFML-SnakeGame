#include "Direction.h"

#include <cstdlib>

Direction::Type Direction::fromSfmlKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Left:
		return Direction::Left;
	case sf::Keyboard::Right:
		return Direction::Right;
	case sf::Keyboard::Up:
		return Direction::Up;
	case sf::Keyboard::Down:
		return Direction::Down;
	default:
		return Direction::None;
	}
}

Direction::Type Direction::opposite(Direction::Type dir)
{
	switch (dir)
	{
	case Direction::Left:
		return Direction::Right;
	case Direction::Right:
		return Direction::Left;
	case Direction::Up:
		return Direction::Down;
	case Direction::Down:
		return Direction::Up;
	}
	return Direction::None;
}

Direction::Type operator-(Direction::Type dir, int num)
{ return operator-(num, dir); }

Direction::Type operator-(int num, Direction::Type dir)
{
	if (dir and num != 0)
	{
		if (num < 0) return operator+(std::abs(num), dir);
		return (Direction::Type)(5 - (int)operator+(num, (Direction::Type)(5 - (int)dir)));
	}
	return dir;
}

Direction::Type operator+(Direction::Type dir, int num)
{ return operator+(num, dir); }

Direction::Type operator+(int num, Direction::Type dir)
{
	if (dir and num != 0)
	{
		if (num < 0) return operator-(std::abs(num), dir);

		int res = num - num / 4 * 4 + (int)dir;
		if (res > 4) return (Direction::Type)(res % 4);
		else return (Direction::Type)res;
	}
	return dir;
}