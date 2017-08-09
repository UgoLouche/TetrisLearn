#pragma once

#include "const.hpp"

#include "ProtoEng\Box.h"

#include "IGraphicObject.hpp"
#include "Block.hpp"
#include "Tetramino.hpp"
#include "BlockPool.hpp"

class TetraThumbnail : public IGraphicObject
{
public:
	TetraThumbnail(BlocksPool& pool);
	~TetraThumbnail();

	void changeType(TetraminoTypes newType, bool isEmpty = false);
	void changeColor(sf::Color color);

	void destroyLater();

private:
	TetraminoTypes type;
	
	sf::Vector2f anchor;

	Block* blocks[TETRAMINO_SIZE];
	BlocksPool& pool;

	ProtoEng::Box boundingBox;

	bool isEmpty;

	//Override
	void notifyGraphicUpdate_custom(sf::Time deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};