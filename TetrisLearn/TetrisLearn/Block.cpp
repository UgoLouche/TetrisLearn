#include "Block.hpp"

#include "const.hpp"
#include "Settings.hpp"

Block::Block() :
	status(BlockStatus::Inactive),
	parentBoard(nullptr),
	boardPosition(0, 0),
	flashColorSwitch(false),
	animationDelay(sf::milliseconds(Settings::get().flashAnimDelay))
{
}

void Block::setStatus(BlockStatus status)
{
	this->status = status;
	setSpriteColor();
}

BlockStatus Block::getStatus() const
{
	return status;
}

void Block::setBoardPosition(sf::Vector2i pos)
{
	boardPosition = pos;

	setPosition(sf::Vector2f(boardPosition.x * BLOCK_SIZE,  -1 * boardPosition.y * BLOCK_SIZE ));
}

const sf::Vector2i& Block::getBoardPosition() const
{
	return boardPosition;
}

void Block::setParentBoard(IBoard* parent)
{
	parentBoard = parent;
}

void Block::setSprite(sf::Sprite sprite)
{
	sprite_origin = sprite;
	sprite_origin.setTextureRect(sf::IntRect(0, 0, (int)BLOCK_SIZE, (int)BLOCK_SIZE));
	this->sprite = sprite_origin;
	setSpriteColor();
}

void Block::notifyGraphicUpdate_custom(sf::Time deltaTime)
{
	if (status == BlockStatus::Flashing)
	{
		while (deltaTime > sf::Time::Zero)
		{
			if (animationDelay > deltaTime)
			{
				animationDelay -= deltaTime;
				deltaTime = sf::Time::Zero;
			}
			else
			{
				deltaTime -= animationDelay;
				animationDelay = sf::milliseconds(Settings::get().flashAnimDelay);

				flashColorSwitch = !flashColorSwitch;
				setSpriteColor();
			}
		}
	}
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (status != BlockStatus::Inactive) target.draw(sprite, states.transform * getTransform());
}

void Block::setSpriteColor()
{
	sprite = sprite_origin;

	switch (status)
	{
	case BlockStatus::Locked:
		sprite.setColor(Settings::get().lockedSprite_color);
		break;
	case BlockStatus::Flashing:
		if (flashColorSwitch) sprite.setColor(Settings::get().flashingSprite_color);
		else sprite.setColor(Settings::get().flashingSprite_altColor);
		break;
	case BlockStatus::Ghost:
		sprite.setColor(Settings::get().ghostSprite_color);
		break;
	case BlockStatus::Inactive:
	case BlockStatus::Active:
	default:
		break;
	}
}

