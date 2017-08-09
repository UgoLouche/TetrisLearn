#include "ITetraminoManager.hpp"

ITetraminoManager::~ITetraminoManager()
{
}

Tetramino * ITetraminoManager::current() const
{
	return current_custom();
}

void ITetraminoManager::activate_next()
{
	activate_next_custom();
}

void ITetraminoManager::holdTetramino()
{
	holdTetramino_custom();
}

TetraminoTypes ITetraminoManager::getNextType(size_t index) const
{
	return getNextType_custom(index);
}

TetraminoTypes ITetraminoManager::getHoldType() const
{
	return getHoldType_custom();
}

bool ITetraminoManager::isHolding() const
{
	return isHolding_custom();
}
