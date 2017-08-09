#pragma once

#include <memory>

#include "ITetraminoManager.hpp"

#include "PeekQueue.hpp"
#include "TetraminosBag.hpp"


class IBoard;
class Tetramino;
class IBlocksPool;

class TetraminoManager : public ITetraminoManager
{
public:
	TetraminoManager(IBoard& parent, IBlocksPool& pool);

private:
	std::shared_ptr<Tetramino> current_tetramino = nullptr;
	std::shared_ptr<Tetramino> hold_tetramino = nullptr;
	bool holdAvailable = false;

	PeekQueue<Tetramino, 3> nexts;
	TetraminosBag bag;

	IBoard& parent;


	// Inherited via ITetraminoManager
	virtual Tetramino * current_custom() const override;
	virtual void activate_next_custom() override;
	virtual void holdTetramino_custom() override;
	virtual TetraminoTypes getNextType_custom(size_t index) const override;
	virtual TetraminoTypes getHoldType_custom() const override;
	virtual bool isHolding_custom() const override;


	// Ad-hoc
	void updateThumbnails();

};