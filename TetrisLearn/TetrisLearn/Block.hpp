#pragma once

#include "SFML\Graphics.hpp"

#include "enum.hpp"

#include "IGraphicObject.hpp"

class IBoard;

class Block : public IGraphicObject
{
public:
	Block();

	void setStatus(BlockStatus status);
	BlockStatus getStatus() const;

	void setBoardPosition(sf::Vector2i pos);
	const sf::Vector2i& getBoardPosition() const;

	void setParentBoard(IBoard* parent);

	void setSprite(sf::Sprite sprite);

private: 
	void notifyGraphicUpdate_custom(sf::Time deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setSpriteColor();


	BlockStatus status;

	IBoard* parentBoard;
	sf::Vector2i boardPosition;

	// UI.
	sf::Sprite sprite;
	sf::Sprite sprite_origin;
	bool flashColorSwitch;
	sf::Time animationDelay;
};
