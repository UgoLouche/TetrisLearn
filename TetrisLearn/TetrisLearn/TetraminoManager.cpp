#include "TetraminoManager.hpp"

#include "Tetramino.hpp"
#include "IBoard.hpp"
#include "IBlocksPool.hpp"
#include "TetraminosBag.hpp"

TetraminoManager::TetraminoManager(IBoard & parent, IBlocksPool& pool) : 
	parent(parent), bag(parent, pool)
{
	nexts.push(bag.getNext());
	nexts.push(bag.getNext());
	nexts.push(bag.getNext());
}

void TetraminoManager::updateThumbnails()
{
	//Update thumbnails
	for (int i = 0; i < THUMBNAIL_PREVIEW_NUMBER; ++i)
	{
		parent.setThumbnail(i, nexts.peek(i)->getType());
	}
}


Tetramino * TetraminoManager::current_custom() const
{
	return current_tetramino.get();
}

void TetraminoManager::activate_next_custom()
{
	current_tetramino = nexts.pop();
	current_tetramino->activate();

	nexts.push(bag.getNext());
	updateThumbnails();


	// Reactivate hold
	holdAvailable = true;

	if (!current_tetramino->canStepDown())
		parent.gameOver();
}

void TetraminoManager::holdTetramino_custom()
{
	if (!holdAvailable) return;
	holdAvailable = false;

	std::shared_ptr<Tetramino> tmp = hold_tetramino;

	hold_tetramino = current_tetramino;
	parent.newHoldTetramino(hold_tetramino->getType());

	if (tmp != nullptr)
	{
		current_tetramino = tmp;
		current_tetramino->resetPosition();
	}
	else
		activate_next(); //purposefully set holdAvailable on true.
}

TetraminoTypes TetraminoManager::getNextType_custom(size_t index) const
{
	return nexts.peek(index)->getType();
}

TetraminoTypes TetraminoManager::getHoldType_custom() const
{
	return isHolding() ? hold_tetramino->getType() : TetraminoTypes::I;
}

bool TetraminoManager::isHolding_custom() const
{
	return hold_tetramino != nullptr;
}
