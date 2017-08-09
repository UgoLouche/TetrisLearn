#include "BoardGrid.hpp"

#include "Block.hpp"
#include "IBoard.hpp"

BoardGrid::BoardGrid(IBoard & parent) : parent(parent)
{
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			content[j][i] = nullptr;
		}
	}
}

Block ** BoardGrid::bracket_custom(int i)
{
	return content[i];
}

Block * const * BoardGrid::bracket_custom(int i) const
{
	return content[i];
}


bool BoardGrid::postLockRowChecks_custom()
{
	bool lineFound = false;

	//Check for a line
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		if (checkRow(i))
		{
			lineFound = true;

			// Set block to flash mode
			for (int j = 0; j < BOARD_WIDTH; ++j) content[j][i]->setStatus(BlockStatus::Flashing);
		}
	}

	if (lineFound) return true;
	else return false;
}

bool BoardGrid::checkRow(int index)
{
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		if (content[i][index] != nullptr && content[i][index]->getStatus() == BlockStatus::Locked) continue;
		else return false;
	}

	return true;
}

void BoardGrid::removeRows_custom()
{
	int offset = 0;

	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		if (content[0][i] != nullptr && content[0][i]->getStatus() == BlockStatus::Flashing) // Check first block, if flashing, remove the line.
		{
			for (int j = 0; j < BOARD_WIDTH; ++j)
			{
				parent.storeBlock(content[j][i]);
				content[j][i] = nullptr;
			}

			++offset;
		}
		else
		{
			shiftRowDown(i, offset);
		}
	}

	parent.addScore(offset);
}

void BoardGrid::shiftRowDown(int index, int offset)
{
	if (offset > 0)
	{
		for (int i = 0; i < BOARD_WIDTH; ++i)
		{
			if (content[i][index] != nullptr) // Can safely skip empty cells
			{
				content[i][index - offset] = content[i][index];
				content[i][index] = nullptr;

				// /!\ Notify the block of its new position or display might be off.
				content[i][index - offset]->setBoardPosition(sf::Vector2i(i, index - offset));
			}
		}
	}
}

bool BoardGrid::validPos(sf::Vector2i pos) const
{
	return (pos.x >= 0
		&&  pos.x <  BOARD_WIDTH
		&&  pos.y >= 0
		&&  pos.y <  BOARD_HEIGHT
		);
}

Block * BoardGrid::getBlock_custom(sf::Vector2i pos) const
{
	if (validPos(pos)) return content[pos.x][pos.y];
	else return nullptr;
}

void BoardGrid::setBlock_custom(Block * content, sf::Vector2i pos)
{
	if (validPos(pos)) this->content[pos.x][pos.y] = content;
}

bool BoardGrid::isFree_custom(sf::Vector2i pos) const
{
	if (validPos(pos) && content[pos.x][pos.y] == nullptr) return true;
	else return false;
}
