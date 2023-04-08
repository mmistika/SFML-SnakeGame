#include "snake.h"

Direction SFMLKeyToSnakeDirection(sf::Keyboard::Key key)
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

Snake::Snake(GameField* field, sf::Texture* textures, float speed)
	: field_(field),
	tileEdgeLength_((float)field_->tileEdgeLength()),
	speed_(speed),
	textures_(textures)
{
	timer_ = sf::Clock();

	float originOffset = tileEdgeLength_ / 2;

	// Head segment
	Segment* bufferSegm = new Segment();
	bufferSegm->rect.setOrigin({ originOffset, originOffset });
	bufferSegm->rect.setPosition(
		{
			tileEdgeLength_ * 2 + originOffset,
			0.f + originOffset
		});
	bufferSegm->rect.setSize({ tileEdgeLength_ , tileEdgeLength_ });
	
	bufferSegm->rect.setTexture(textures_);
	setTextureByName_(bufferSegm->rect, SegmentName::Head);

	bufferSegm->direction = Direction::Right;
	bufferSegm->rect.setRotation(getRotation_(Direction::Right));

	segmentList_.push_back(bufferSegm);

	field_->setSnakeBlockAt((int)tileEdgeLength_ * 2, 0);

	// Body segment
	bufferSegm = new Segment();
	bufferSegm->rect.setOrigin({ originOffset, originOffset });
	bufferSegm->rect.setPosition(
		{
			tileEdgeLength_ + originOffset,
			0.f + originOffset
		});
	bufferSegm->rect.setSize({ tileEdgeLength_ , tileEdgeLength_ });

	bufferSegm->rect.setTexture(textures_);
	setTextureByName_(bufferSegm->rect, SegmentName::Body);

	bufferSegm->direction = Direction::Right;
	bufferSegm->rect.setRotation(getRotation_(Direction::Right));

	segmentList_.push_back(bufferSegm);

	field_->setSnakeBlockAt((int)tileEdgeLength_, 0);

	// Tail segment
	bufferSegm = new Segment();
	bufferSegm->rect.setOrigin({ originOffset, originOffset });
	bufferSegm->rect.setPosition(
		{
			0.f + originOffset,
			0.f + originOffset
		});
	bufferSegm->rect.setSize({ tileEdgeLength_ , tileEdgeLength_ });
	
	bufferSegm->rect.setTexture(textures_);
	setTextureByName_(bufferSegm->rect, SegmentName::Tail);
	
	bufferSegm->direction = Direction::Right;
	bufferSegm->rect.setRotation(getRotation_(Direction::Right));
	
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

		int originOffset = tileEdgeLength_ / 2;

		// Head movement
		Segment* head = segmentList_[0];
		if (direction != getOppositeDirection_(head->direction) and
			direction != Direction::None)
		{
			head->direction = direction;
			head->rect.setRotation(getRotation_(direction));
		}

		Segment* bufferSegm = new Segment(*head);

		sf::Vector2f newPosOffset = getNewPositionOffset_(head->direction);
		sf::Vector2f newPos = head->rect.getPosition() + newPosOffset;

		if (!field_->isPosInField((int)newPos.x - originOffset, (int)newPos.y - originOffset) or isPosOnSnake_(newPos))
		{
			isDead_ = true;
			head->rect.setFillColor(sf::Color::Red);
			return;
		}

		head->rect.setPosition(newPos);
		field_->setSnakeBlockAt((int)newPos.x - originOffset, (int)newPos.y - originOffset);

		// Segment after the head movement
		if (newPos.x - originOffset == foodPos.x and
			newPos.y - originOffset == foodPos.y)
		{
			isFoodEaten_ = true;
		}
		else
		{
			sf::Vector2f pos = bufferSegm->rect.getPosition();
			delete bufferSegm;

			bufferSegm = new Segment(*segmentList_[segmentList_.size() - 2]);
			bufferSegm->direction = head->direction;
			bufferSegm->rect.setPosition(pos);
		}

		segmentList_.insert(segmentList_.begin() + 1, bufferSegm);

		if (head->direction == segmentList_[2]->direction)
		{
			bufferSegm->rect.setRotation(getRotation_(head->direction));
			setTextureByName_(bufferSegm->rect, SegmentName::Body);
		}
		else
		{
			bufferSegm->rect.setRotation(getRotation_(head->direction, segmentList_[2]->direction));
			setTextureByName_(bufferSegm->rect, SegmentName::Turn);
		}

		// Tail movement
		if (!isFoodEaten_)
		{
			bufferSegm = segmentList_.back();
			segmentList_.pop_back();

			sf::Vector2f pos = bufferSegm->rect.getPosition();
			field_->removeSnakeBlockAt((int)pos.x - originOffset, (int)pos.y - originOffset);

			Segment* segmentToDelete = segmentList_.back();
			pos = segmentToDelete->rect.getPosition();
			delete segmentToDelete;

			bufferSegm->rect.setPosition(pos);
			bufferSegm->direction = getDirectionFromPositions_(
				bufferSegm->rect.getPosition(),
				segmentList_[segmentList_.size() - 2]->rect.getPosition());
			bufferSegm->rect.setRotation(getRotation_(bufferSegm->direction));
			

			segmentList_[segmentList_.size() - 1] = bufferSegm;
		}
	}
}

void Snake::setTextureByName_(sf::RectangleShape& segment, SegmentName name)
{
	switch (name)
	{
	case Snake::SegmentName::Head:
		segment.setTextureRect(sf::IntRect(0, 0, 300, 300));
		break;
	case Snake::SegmentName::Body:
		segment.setTextureRect(sf::IntRect(300, 0, 300, 300));
		break;
	case Snake::SegmentName::Tail:
		segment.setTextureRect(sf::IntRect(300, 300, 300, 300));
		break;
	case Snake::SegmentName::Turn:
		segment.setTextureRect(sf::IntRect(0, 300, 300, 300));
		break;
	}
}

sf::Vector2f Snake::getNewPositionOffset_(Direction direction) const
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

Direction Snake::getOppositeDirection_(Direction direction) const
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
	}
	return Direction::None;
}

Direction Snake::getDirectionFromPositions_(sf::Vector2f position, sf::Vector2f position2) const
{
	float dx = position2.x - position.x;
	float dy = position2.y - position.y;

	if (std::abs(dx) >= std::abs(dy)) {
		return (dx > 0) ? Direction::Right : Direction::Left;
	}
	else {
		return (dy > 0) ? Direction::Down : Direction::Up;
	}
}

float Snake::getRotation_(Direction direction, Direction direction2) const
{
	float rotation = 0;

	if (direction2 != Direction::None)
	{
		if ((int)direction - 1 == (int)direction2 or
			(direction == Direction::Up and direction2 == Direction::Left))
			rotation += 180;

		if ((int)direction + 1 == (int)direction2 or
			(direction == Direction::Left and direction2 == Direction::Up))
			rotation += 90;
	}

	switch (direction)
	{
	case Direction::Left:
		rotation += 270;
		break;
	case Direction::Right:
		rotation += 90;
		break;
	case Direction::Up:
		rotation += 0;
		break;
	case Direction::Down:
		rotation += 180;
		break;
	}

	return ((int)rotation >= 360) ? rotation - 360 : rotation;
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