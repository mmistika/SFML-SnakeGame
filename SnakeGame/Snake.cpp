#include "snake.h"

Direction getOppositeDirection(Direction direction);

Snake::Snake(GameField* field, float speed, sf::Color color)
	: field_(field),
	tileEdgeLength_((float)field_->tileEdgeLength()),
	speed_(speed),
	color_(color)
{
	timer_ = sf::Clock();

	// Head segment
	Segment* bufferSegm = new Segment();
	bufferSegm->rect.setPosition({ float(tileEdgeLength_), 0.f });
	bufferSegm->rect.setSize({ tileEdgeLength_ , tileEdgeLength_ });
	bufferSegm->rect.setFillColor(color_);
	bufferSegm->direction = Direction::Right;
	segmentList_.push_back(bufferSegm);

	field_->setSnakeBlockAt((int)tileEdgeLength_, 0);

	// Tail segment
	bufferSegm = new Segment();
	bufferSegm->rect.setFillColor(color_);
	bufferSegm->rect.setSize({ tileEdgeLength_ , tileEdgeLength_ });
	segmentList_.push_back(bufferSegm);

	field_->setSnakeBlockAt(0, 0);
}

Snake::~Snake()
{
	for (auto segmentPointer : segmentList_)
		delete segmentPointer;
}

void Snake::update(Direction direction, sf::Vector2f foodPos)
{
	deltaTime_ += timer_.getElapsedTime().asSeconds();
	timer_.restart();

	isFoodEaten_ = false;

	if (deltaTime_ >= speed_)
	{
		deltaTime_ = 0;

		Segment* head = segmentList_[0];
		if (direction != getOppositeDirection(head->direction) and
			direction != Direction::None)
			head->direction = direction;

		Segment* bufferSegm = new Segment(*head);

		sf::Vector2f newPosOffset = getNewPositionOffset_(head->direction);
		sf::Vector2f newPos = head->rect.getPosition() + newPosOffset;

		if (!field_->isPosInField((int)newPos.x, (int)newPos.y) or isPosOnSnake_(newPos))
		{
			isDead_ = true;
			head->rect.setFillColor(color_.Red);
			return;
		}

		head->rect.setPosition(newPos);
		field_->setSnakeBlockAt((int)newPos.x, (int)newPos.y);

		if (newPos == foodPos)
		{
			isFoodEaten_ = true;
			segmentList_.insert(segmentList_.begin() + 1, bufferSegm);
		}
		else
		{
			sf::Vector2f pos = bufferSegm->rect.getPosition();

			bufferSegm = segmentList_.back();
			field_->removeSnakeBlockAt((int)bufferSegm->rect.getPosition().x,
									   (int)bufferSegm->rect.getPosition().y);
			
			bufferSegm->rect.setPosition(pos);
			segmentList_.pop_back();
			segmentList_.insert(segmentList_.begin() + 1, bufferSegm);
		}
	}
}

sf::Vector2f Snake::getNewPositionOffset_(Direction direction)
{
	sf::Vector2f offset;
	switch (direction)
	{
	case Direction::Left:
		offset.x -= tileEdgeLength_;
		break;
	case Direction::Right:
		offset.x += tileEdgeLength_;
		break;
	case Direction::Up:
		offset.y -= tileEdgeLength_;
		break;
	case Direction::Down:
		offset.y += tileEdgeLength_;
		break;
	}
	return offset;
}

bool Snake::isPosOnSnake_(sf::Vector2f pos) const
{
	for (auto segm : segmentList_)
		if (pos == segm->rect.getPosition()) return true;
	return false;
}

std::vector<sf::Drawable*> Snake::getRectsToDraw() const
{
	std::vector<sf::Drawable*> vect;
	for (auto segment : segmentList_)
		vect.push_back(&segment->rect);

	return vect;
}

Direction getOppositeDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::Left:
		return Direction::Right;
	case Direction::Right:
		return Direction::Left;
	case Direction::Up:
		return Direction::Down;
	case Direction::Down:
		return Direction::Up;
	default:
		return Direction::None;
	}
}