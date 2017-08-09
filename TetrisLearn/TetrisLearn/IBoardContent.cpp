#include "IBoardContent.hpp"

IBoardContent::~IBoardContent()
{
}

bool IBoardContent::postLockRowChecks()
{
	return postLockRowChecks_custom();
}

void IBoardContent::removeRows()
{
	removeRows_custom();
}

Block ** IBoardContent::operator[](int i)
{
	return bracket_custom(i);
}

Block * const * IBoardContent::operator[](int i) const
{
	return bracket_custom(i);
}

bool IBoardContent::isFree(sf::Vector2i pos) const
{
	return isFree_custom(pos);
}

Block * IBoardContent::getBlock(sf::Vector2i pos) const
{
	return getBlock_custom(pos);
}

void IBoardContent::setBlock(Block* content, sf::Vector2i pos)
{
	setBlock_custom(content, pos);
}
