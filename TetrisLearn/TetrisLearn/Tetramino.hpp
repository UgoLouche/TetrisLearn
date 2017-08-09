#pragma once

#include <memory>

#include "const.hpp"
#include "enum.hpp"

#include "IGraphicObject.hpp"

class IBoard;
class IBlocksPool;
class Block;

class Tetramino : public IGraphicObject
{
	friend class TetraminosBag;

public:
	//Static "factory method"
	//static std::shared_ptr<Tetramino> newTetramino(Board& board);
	//static void clearBags();

	~Tetramino();
	
	// Only test if move is legal
	bool canStepDown();
	bool canStepLeft();
	bool canStepRight();
	bool canFlipR();
	bool canFlipL();

	// Try to move, return true if the tetramino has moved (ie move was legal)
	bool stepDown();
	bool stepLeft();
	bool stepRight();
	bool flipR();
	bool flipL();

	void release();  //Release currently held blocks.
	void activate(); //Activate the tetramino and all its blocks.

	void resetPosition();

	void clear();


	bool isGhost() const;

	TetraminoTypes getType() const;
	TetraminoPhases getPhase() const;

	sf::Vector2i getBoardPosition() const;
	sf::Vector2i getBlockBoardPosition(size_t index) const;

private:
	// Private constructor, should be created through bags only
	Tetramino(IBoard& board, IBlocksPool& pool, bool isGhost, TetraminoTypes type);

	//Override
	void notifyGraphicUpdate_custom(sf::Time deltaTime) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Positioning
	// Ineficient, absolute coordinate position setting. Use for initialization, do not check for position validity.
	void setBoardPosition(sf::Vector2i pos);

	//Change the phase (ie rotate), do not check for position validty nor wall-kick. subroutine for flip().
	void changePhase(TetraminoPhases newPhase);

	bool move(sf::Vector2i offset, bool simulate);
	bool flip(bool turnleft, bool simulate);

	void resetGhost();


	// Fields
	IBoard& board;
	IBlocksPool& pool;

	std::unique_ptr<Tetramino> ghost;
	Block* blocks[TETRAMINO_SIZE];
	sf::Vector2i pos;
	TetraminoTypes type;
	TetraminoPhases phase;

	// Status
	bool isActive;
};