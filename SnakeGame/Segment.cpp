#include "Segment.h"

Segment::Segment(sf::Vector2f position, float size, Segment::Type type, Direction::Type direction)
{
	rect_ = sf::RectangleShape({ size, size });

	originOffset_ = size / 2;
	rect_.setOrigin(originOffset_, originOffset_);
	setPosition(position);

	type_ = type;
	direction_ = direction;
}

Segment::Segment(const Segment& segment)
{
	rect_ = sf::RectangleShape(segment.rect_);
	originOffset_ = segment.originOffset_;
	type_ = segment.type();
	direction_ = segment.direction();
}

Segment& Segment::setType(Segment::Type newType)
{
	type_ = newType;
	updateTextureRect_();
	return *this;
}

Segment& Segment::setDirection(Direction::Type direction)
{
	direction_ = direction;
	return *this;
};

sf::Vector2f Segment::position() const
{
	return rect_.getPosition() - sf::Vector2f{ originOffset_, originOffset_ };
}

Segment& Segment::setPosition(const sf::Vector2f& pos)
{
	rect_.setPosition(pos + sf::Vector2f{ originOffset_, originOffset_ });
	return *this;
}

Segment& Segment::setRotation(float rotation)
{
	rect_.setRotation(rotation);
	return *this;
}

Segment& Segment::setTexture(sf::Texture* texture)
{
	rect_.setTexture(texture);
	updateTextureRect_();
	return *this;
}

void Segment::updateTextureRect_()
{
	switch (type_)
	{
	case Segment::Type::Head:
		return rect_.setTextureRect({ 0, 0, 300, 300 });
	case Segment::Type::Body:
		return rect_.setTextureRect({ 300, 0, 300, 300 });
	case Segment::Type::Tail:
		return rect_.setTextureRect({ 300, 300, 300, 300 });
	case Segment::Type::Turn:
		return rect_.setTextureRect({ 0, 300, 300, 300 });
	}
}