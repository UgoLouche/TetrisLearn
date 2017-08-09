#include "TetraThumbnail.hpp"

#include "TetraminoMoves.hpp"

TetraThumbnail::TetraThumbnail(BlocksPool & pool) :
	pool(pool)
{
	anchor = getPosition() + sf::Vector2f(THUMBNAIL_BORDER, THUMBNAIL_BORDER);

	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		blocks[i] = pool.getBlock();
		blocks[i]->setStatus(BlockStatus::Active);
		blocks[i]->setScale(sf::Vector2f(0.5f, 0.5f));
	}

	boundingBox.setPosition(getPosition());
	boundingBox.setDimensions(sf::Vector2f(5 * BLOCK_SIZE / 2 + 2 * THUMBNAIL_BORDER, 4 * BLOCK_SIZE / 2 + 2 * THUMBNAIL_BORDER));
	boundingBox.setBorderProperties(sf::Color::White, THUMBNAIL_BORDER);

	changeType(TetraminoTypes::I, true);
}

TetraThumbnail::~TetraThumbnail()
{
}

void TetraThumbnail::changeType(TetraminoTypes newType, bool isEmpty)
{
	type = newType;

	sf::Vector2f positionOffset = getPosition() + sf::Vector2f(THUMBNAIL_BORDER, THUMBNAIL_BORDER);
	float block = BLOCK_SIZE / 2;

	switch (newType)
	{
	case TetraminoTypes::I:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(2 * block, 0 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 1 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(2 * block, 3 * block));
		break;
	case TetraminoTypes::J:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 1 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(1 * block, 2 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(3 * block, 2 * block));
		break;
	case TetraminoTypes::L:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 2 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(3 * block, 2 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(3 * block, 1 * block));
		break;
	case TetraminoTypes::O:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 1 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 1 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(1 * block, 2 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		break;
	case TetraminoTypes::S:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 2 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(2 * block, 1 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(3 * block, 1 * block));
		break;
	case TetraminoTypes::T:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 1 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 1 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(3 * block, 1 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		break;
	case TetraminoTypes::Z:
		blocks[0]->setPosition(positionOffset + sf::Vector2f(1 * block, 1 * block));
		blocks[1]->setPosition(positionOffset + sf::Vector2f(2 * block, 1 * block));
		blocks[2]->setPosition(positionOffset + sf::Vector2f(2 * block, 2 * block));
		blocks[3]->setPosition(positionOffset + sf::Vector2f(3 * block, 2 * block));
		break;
	default:
		break;
	}

	this->isEmpty = isEmpty;
}

void TetraThumbnail::changeColor(sf::Color color)
{
	boundingBox.setBorderColor(color);
}

void TetraThumbnail::destroyLater()
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		pool.storeBlock(blocks[i]);
		blocks[i] = nullptr;
	}
}

void TetraThumbnail::notifyGraphicUpdate_custom(sf::Time deltaTime)
{
}

void TetraThumbnail::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::Transform transform = getTransform();

	target.draw(boundingBox, transform);

	if (!isEmpty)
	{
		for (int i = 0; i < TETRAMINO_SIZE; ++i)
		{
			target.draw(*(blocks[i]), states);
		}
	}

}


