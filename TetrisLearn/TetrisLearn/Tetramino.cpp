#include "Tetramino.hpp"

#include "TetraminoMoves.hpp"

#include "TetraminosBag.hpp"
#include "IBoard.hpp"


//std::vector<TetraminosBag> Tetramino::bags = std::vector<TetraminosBag>();

/*std::shared_ptr<Tetramino> Tetramino::newTetramino(IBoard & board)
{
	size_t id = board.getID();

	if (id + 1 > bags.size()) bags.resize(id + 1, TetraminosBag());

	if (!bags[id].isInit()) bags[id].init(&board);

	return bags[id].getNext();
}*/

/*void Tetramino::clearBags()
{
	for (auto it = bags.begin(); it != bags.end(); ++it)
		it->clear();
}*/

Tetramino::~Tetramino()
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		if (blocks[i] != nullptr) pool.storeBlock(blocks[i]);
	}
}

// TODO different Tetra have different block color
Tetramino::Tetramino(IBoard& board, IBlocksPool& pool, bool isGhost, TetraminoTypes type) :
	board(board),
	pool(pool),
	type(type),
	isActive(false),
	phase(TetraminoPhases::ORIGIN)
{
	// Set up Ghost.
	if (isGhost) ghost = nullptr;
	else ghost = std::unique_ptr<Tetramino>(new Tetramino(board, pool, true, type));

	// Link Blocks
	// Anchor Block is always block index 0.
	// Block numbering is counterclowkwise starting from the bottom (i.e. a clock's 6)
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		blocks[i] = pool.getBlock();
		blocks[i]->setStatus(BlockStatus::Inactive);
		blocks[i]->setParentBoard(&board);
	}
	setBoardPosition(SPAWN_POINT);
}

void Tetramino::notifyGraphicUpdate_custom(sf::Time deltaTime)
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i) blocks[i]->notifyGraphicUpdate(deltaTime);
}

void Tetramino::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		// Only draw blocks in the visible zone
		if (blocks[i]->getBoardPosition().y < BOARD_VISIBLE_HEIGHT)  target.draw(*blocks[i], states.transform);
	}

	if (!isGhost()) target.draw(*ghost, states.transform);
}

bool Tetramino::canStepDown()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::DOWN], true);
}

bool Tetramino::canStepLeft()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::LEFT], true);
}

bool Tetramino::canStepRight()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::RIGHT], true);
}

bool Tetramino::canFlipR()
{
	return flip(false, true);
}

bool Tetramino::canFlipL()
{
	return flip(true, true);
}

bool Tetramino::stepDown()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::DOWN], false);
}

bool Tetramino::stepLeft()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::LEFT], false);
}

bool Tetramino::stepRight()
{
	return move(TetraminoMoves::Move_offsets[MoveDirections::RIGHT], false);
}

bool Tetramino::flipR()
{
	return flip(false, false);
}

bool Tetramino::flipL()
{
	return flip(true, false);
}

void Tetramino::setBoardPosition(sf::Vector2i pos)
{
	this->pos = pos;

	for (int i = 0; i < TETRAMINO_SIZE; ++i)
		blocks[i]->setBoardPosition(pos + TetraminoMoves::Spawn_offsets[type][i]);

	phase = TetraminoPhases::ORIGIN;
}

void Tetramino::changePhase(TetraminoPhases newPhase)
{
	sf::Transform combined = sf::Transform::Identity;

	/*
	switch (phase)
	{
	case TetraminoPhases::LEFT:
		combined *= TetraminoMoves::RotationMatrix[TetraminoPhases::RIGHT];
		break;

	case TetraminoPhases::INVERSE:
		combined *= TetraminoMoves::RotationMatrix[TetraminoPhases::INVERSE];
		break;

	case TetraminoPhases::RIGHT:
		combined *= TetraminoMoves::RotationMatrix[TetraminoPhases::LEFT];
		break;

	case TetraminoPhases::ORIGIN:
	default:
		break;
	}
	*/

	combined *= TetraminoMoves::RotationMatrix[newPhase];

	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		sf::Vector2f newOffset = combined.transformPoint( static_cast<sf::Vector2f>(TetraminoMoves::Spawn_offsets[type][i]) );
		blocks[i]->setBoardPosition(pos + sf::Vector2i((int)newOffset.x, (int)newOffset.y));
	}

	phase = newPhase;
}

bool Tetramino::move(sf::Vector2i offset, bool simulate)
{
	bool valid = true;
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		if (board.isFree(blocks[i]->getBoardPosition() + offset)) continue;
		else
		{
			valid = false;
			break;
		}
	}

	if (simulate || !valid) return valid;

	// Executed only if move is valid and not simulated
	pos += offset;
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		blocks[i]->setBoardPosition(blocks[i]->getBoardPosition() + offset);
	}

	resetGhost();

	return true;
}

bool Tetramino::flip(bool turnleft, bool simulate)
{
	// Get old and new Phases
	TetraminoPhases oldPhase = phase;
	TetraminoPhases newPhase;
	switch (phase)
	{
	case TetraminoPhases::LEFT:
		if (turnleft) newPhase = TetraminoPhases::INVERSE;
		else newPhase = TetraminoPhases::ORIGIN;
		break;

	case TetraminoPhases::INVERSE:
		if (turnleft) newPhase = TetraminoPhases::RIGHT;
		else newPhase = TetraminoPhases::LEFT;
		break;

	case TetraminoPhases::RIGHT:
		if (turnleft) newPhase = TetraminoPhases::ORIGIN;
		else newPhase = TetraminoPhases::INVERSE;
		break;

	case TetraminoPhases::ORIGIN:
	default:
		if (turnleft) newPhase = TetraminoPhases::LEFT;
		else newPhase = TetraminoPhases::RIGHT;
		break;
	}


	// Compute offsets
	sf::Vector2i offsets[SRS_OFFSET_VARIANTS];

	const auto& SRSOffsets = [&]()->auto&
	{
		if (type == TetraminoTypes::I) return TetraminoMoves::SRS_offset_I;
		else if (type == TetraminoTypes::O) return TetraminoMoves::SRS_offset_O;
		else return TetraminoMoves::SRS_offset_base;
	}(); // Declare and call lambda (hence the () at the end).

	for (int i = 0; i < SRS_OFFSET_VARIANTS; ++i)
		offsets[i] = SRSOffsets[oldPhase][i] - SRSOffsets[newPhase][i];


	// Flip.
	changePhase(newPhase);


	// Try offsets return if one is valid
	bool valid = false;
	for (int i = 0; i < SRS_OFFSET_VARIANTS; ++i)
	{
		if (move(offsets[i], simulate))
		{
			valid = true;
			break;
		}
	}


	// If simulated, reset phase
	// If not simulated, reset phase only if no valid move was found
	if (simulate || !valid) changePhase(oldPhase);
	else resetGhost();


	return valid;
}

void Tetramino::resetGhost()
{
	if (isGhost()) return;

	ghost->setBoardPosition(pos);
	ghost->changePhase(phase);

	while (ghost->stepDown()) {}
}

void Tetramino::release()
{
	if (isGhost())
	{
		for (int i = 0; i < 4; ++i)
		{
			pool.storeBlock(blocks[i]);
			blocks[i] = nullptr;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			blocks[i]->setStatus(BlockStatus::Locked);
			board.setBlock(blocks[i], blocks[i]->getBoardPosition());
			blocks[i] = nullptr;
		}
		ghost->release();
	}

	isActive = false;
}

void Tetramino::activate()
{
	isActive = true;

	BlockStatus newStatus;

	if (isGhost())
		newStatus = BlockStatus::Ghost;
	else
	{
		newStatus = BlockStatus::Active;
		ghost->activate();
	}

	for (int i = 0; i < 4; ++i) blocks[i]->setStatus(newStatus);
}

void Tetramino::resetPosition()
{
	setBoardPosition(SPAWN_POINT);
	resetGhost();
}

void Tetramino::clear()
{
	for (int i = 0; i < TETRAMINO_SIZE; ++i)
	{
		if (blocks[i] != nullptr) pool.storeBlock(blocks[i]);
		blocks[i] = nullptr;
	}
}

bool Tetramino::isGhost() const
{
	return (ghost == nullptr);
}

TetraminoTypes Tetramino::getType() const
{
	return type;
}

TetraminoPhases Tetramino::getPhase() const
{
	return phase;
}

sf::Vector2i Tetramino::getBoardPosition() const
{
	return pos;
}

sf::Vector2i Tetramino::getBlockBoardPosition(size_t index) const
{
	return blocks[index]->getBoardPosition();
}


